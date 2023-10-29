#!/usr/bin/perl
# FILE %gg/bin/cmp2md5.pl
#
# check if all files in listing 1 also appear in listing 2
# see "sub usage" for details
#
# written:       1996-01-04
# latest update: 2000-01-23 15:00:00
my $ID= '$Id: cmp2md5.pl,v 1.6 2009/05/24 22:13:36 gonter Exp $';
#
# ----------------------------------------------------------------------------
# Contents:
# sub usage                     print usage
# sub read_table                read a MD5 checksum table
#
# ----------------------------------------------------------------------------

use strict;

# reports
my $show_missing= 0;            # list items that do not appear in the
                                # target table
my $show_doubles= 1;            # list items from the target that were
                                # already listed in the reference table
                                # or earlier in the target table
my $show_double_ref= 0;         # list items that were listed more than
                                # once in the reference tables

# actions
my $remove_doubles= 0;          # remove files referenced in the target
                                # table if they appear more than once
my $make_symlink= 0;            # replace double files by symlink

# initialization
my $is_ref= 1;
my $cnt_removed= 0;
my $cnt_moved= 0;
my $min_size= -1;               # for space optimization
my $is_compact= 0;
my $silent= 0;
my $verify= 0;

my %MD5= ();            # MD5 catalog
my %table_files= ();    # list of MD5 cats already processed
my $move_dest= undef;   # destination directory for moves

my ($arg);
ARGUMENT: while (defined ($arg= shift (@ARGV)))
{
  if ($arg =~ /^--(.+)/)
  {
    my ($opt, $val)= split ('=', $1, 2);
       if ($opt eq 'compact')  { $is_compact= 1;       }
    elsif ($opt eq 'discard')  { $remove_doubles= 1; $move_dest= undef; }
    elsif ($opt eq 'move')     { $move_dest= shift (@ARGV); $remove_doubles= 0; }
    elsif ($opt eq 'double')   { $show_double_ref= 1;  }
    elsif ($opt eq 'missing')  { $show_missing= 1;     }
    elsif ($arg eq 'silent')   { $show_missing= $silent= 1; $show_doubles= 0; }
    elsif ($arg eq 'verify')   { $verify= $show_missing= $silent= 1; $show_doubles= 0; }
    else { usage(); }
  }
  elsif ($arg =~ /^-/)
  {
    if ($arg =~ /^-listref/ || $arg =~ /^-reflist/)
    {
      $is_ref= 1;
      read_reference_list (shift (@ARGV));
    }
    elsif ($arg =~ /^-ref/)     { $is_ref= 1;           }
    elsif ($arg =~ /^-targ/)    { $is_ref= 0;           }
    elsif ($arg eq '-compact')  { $is_compact= 1;       }
    elsif ($arg eq '-discard')  { $remove_doubles= 1; $move_dest= undef; }
    elsif ($arg eq '-move')     { $move_dest= shift (@ARGV); $remove_doubles= 0; }
    elsif ($arg eq '-double')   { $show_double_ref= 1;  }
    elsif ($arg eq '-missing')  { $show_missing= 1;     }
    elsif ($arg eq '-silent')   { $show_missing= $silent= 1; $show_doubles= 0; }
    elsif ($arg eq '-verify')   { $verify= $show_missing= $silent= 1; $show_doubles= 0; }
    elsif ($arg =~ /^-mins/)    { $min_size= shift (@ARGV); }
    elsif ($arg =~ /^-mks(l|ym)/)     { $make_symlink= 1;     }
    else
    {
      usage();
      exit (0);
    }
    next ARGUMENT;
  }

  read_table ($arg, $is_ref);
}

my $md5;
foreach $md5 (sort keys %MD5)
{
  next unless ($MD5{$md5});
  my $fnm= ($is_compact) ? $MD5{$md5} : $MD5{$md5}->{store}->[0]->{path};
  next if ($verify && $fnm =~ /\/TRANS.TBL$/);
  print "missing: $fnm\n" if ($show_missing);
}

print "removed: $cnt_removed\n";

exit (0);

# ----------------------------------------------------------------------------
sub usage
{
  my @base= split ('/', $0);
  my $base= pop (@base);

  print <<END_OF_USAGE
usage: $base [-options] [filenanme]
Example:
1. $base -discard -ref /tmp/checked-cdrom.md5 -target _catalog
   verifies the files that were writting to a CDROM and removed
   those files from the original tree that were found again...

Options:
-ref            ... subsequent files contain reference catalog
-target         ... subsequent files contain target catalogs with
                    items to check
-reflist        ... the following file contains a list of refrence file

-discard        ... remove files named in target catalog whose MD5 finger
                    print appear also in the reference catalog
-move <dest>    ... move duplicate files into <dest> directory
-verify         ... [under construction]
                    verify if everything from the reference is in the target
-missing        ... show items missing in target catalog
-double         ... show items listed more than once in reference catalog
-silent         ... dont show the names of files that appear in both lists
-help           ... print help

$ID
END_OF_USAGE
}

# ----------------------------------------------------------------------------
sub read_table
{
  my ($tfnm, $is_ref, $comment)= @_;

  my $tbl;
  if (defined ($tbl= $table_files{$tfnm}))
  {
    print "WARNING: file $tfnm already processed; is_ref=$tbl->{is_ref} $tbl->{comment}\n";
    return 0;
  }

  local *FI;
  print "# processing table $tfnm; is_ref=$is_ref; $comment\n";
  unless (open (FI, $tfnm))
  {
    print "can't open $tfnm: $?\n";
    return 0;
  }

  $tbl=
  {
    'fnm'       => $tfnm,
    'is_ref'    => $is_ref,
    'comment'   => $comment,
  } ;
  $table_files{$tfnm}= $tbl;

  my $cat;      # catalog entry
  my $store;    # storage entry
  my $is_aggr;  # type of aggregation

  ENTRY: while (<FI>)
  {
    chop;
    next if (/^#/);

    my ($md5, $type, $size, $fnm)= split (' ', $_, 4);

    $is_aggr= '';
    $is_aggr= 'xv' if ($fnm =~ /^\.xvpics\// || $fnm =~ /^_xvpics\//
                       || $fnm =~ /\/\.xvpics\// || $fnm =~ /\/_xvpics\//);

    if ($is_ref)
    { # processing reference list
      if (defined ($cat= $MD5{$md5}) && $size > $min_size)
      {
        my $double_ref_path= $cat->{store}->[0]->{path} unless ($is_compact);
        if ($show_double_ref && !$is_compact)
        {
          print "double: ", " ", $_;
          print " -> ", $double_ref_path if ($make_symlink);
          print "\n";
        }

        if ($remove_doubles && $show_double_ref)
        {
          if ($show_double_ref)
          {
            if ($fnm =~ /catalog/)  # debugging
            {
              print "is_ref= $is_ref cat=$cat\n";
              print ">> rm 1b $cat $fnm\n";
            }
            print ">> rm 1 $cat $fnm\n";
            unlink $fnm;
            $cnt_removed++;
          }
          symlink ($double_ref_path, $fnm) if ($make_symlink && !$is_compact);
          next ENTRY;   # don't store additional path information
        }
      }
      else
      {
        $cat= ($is_compact)
              ? $fnm
              : {
                  'aggr' => $is_aggr,
                  'store' => [],
                } ;

        $MD5{$md5}= $cat;
      }

      unless ($is_compact)
      {
        my $store=
        {
          'tbl' => $tbl,
          'path' => $fnm,
        } ;

        push (@{$cat->{store}}, $store);
      }
    }
    else
    { # processing target list
      if (defined ($MD5{$md5}))
      {
        $MD5{$md5}= ''            if ($show_missing); # T2D
        print $fnm, "\n"          if ($show_doubles);

        if ($remove_doubles)
        {
          # print ">> rm 2 $fnm\n";
          unlink $fnm;
          $cnt_removed++;
        }
        elsif (defined ($move_dest))
        {
          &move_file ($move_dest, $fnm);
          $cnt_moved++;
        }
      }
    }
  }
  close (FI);
  # print "# processed table $tfnm; ref=$is_ref; size=$is_there $comment\n";
  1;
}

# ----------------------------------------------------------------------------
sub move_file
{
  my $dest_path= shift;
  my $fnm= shift;

  print "dest_path='$dest_path' fnm='$fnm'\n";
  my $full_dest= join ('/', $dest_path, $fnm);
  my @full_dest= split ('/', $full_dest);
  my $targ_fnm= pop (@full_dest);
  &mkdir_path (@full_dest);
  rename $fnm, $full_dest;
}

# ----------------------------------------------------------------------------
sub mkdir_path
{
  my @path= @_;
  my @path2= ();

  my $p;

  # cleanup path
  while ($p= shift (@path))
  {
    if ($p eq '.') { next; }
    elsif ($p eq '..' && @path2)
    {
      pop (@path2);
      next;
    }
    else { push (@path2, $p); }
  }

  @path= (); # should be empty anyway
  while ($p= shift (@path2))
  {
    push (@path, $p);
    my $path= join ('/', @path);
    unless (-d $path)
    {
      print ">>> mkdir $path\n";
      mkdir $path, 0755;
    }
  }
}

# ----------------------------------------------------------------------------
sub read_reference_list
{
  my $ref_list= shift;
  my $base= '';

  local *REF_FI;
  unless (open (REF_FI, $ref_list))
  {
    print "# could not open reference list: $ref_list\n";
    return 0;
  }
  print "# processing reference list: $ref_list\n";

  while (<REF_FI>)
  {
    chop;
    next if (/^#/ || /^[ \t]*$/);
    my ($ref_fnm, $comment)= split (' ', $_, 2);

    if ($ref_fnm eq '@base')
    {
      $base= $comment;
      next;
    }

    &read_table ($base.$ref_fnm, 1, $comment);
  }
  close (REF_FI);

  1;
}
