#!/usr/local/bin/perl
# $Id: xmd5.pl,v 1.3 2012/10/11 18:53:33 gonter Exp $

=head1 NAME

  xmd5.pl     MD5 + inode comparision

=head1 USAGE

  chkmd5.pl
  xmd5.pl > tmp_dedup.sh

=head1 NOTES

This script checks the catalog produced by chkmd5.pl and tries to
identify identical files which can be replaced by hardlinks.  The
script's output is a shell script which does the hardlinking,
check it carefully before exectuing it!

=head1 CAVEAT

Keep in mind that hardlinked files can be modified in any place,
changes will then be present in the other directories as well.

=head1 BUGS

Right now, the script does not check for device boundaries which do not
allow hardlinking, nor does it check for permissions etc.

Also, single quotes in filenames are a problem.

=cut

use strict;

# use lib '/root/work/perl';

use Data::Dumper;
$Data::Dumper::Indent= 1;

my %MSIZE= {};

my $FC= new FCAT;

# collect catalog information
$FC->read_inode_catalog ('_catalog.inodes');
$FC->read_md5_catalog ('_catalog');

## print 'FC: ', Dumper ($FC), "\n";
## exit (0);

my $tsz= $FC->show_dups ();
printf ("echo %1.2f mbytes dups\n", $tsz/1048576);

exit (0);

package FCAT::inode;

sub new
{
  my $class= shift;
  my $inode= shift;
  my @paths= @_;

  my $i= { 'inode' => $inode, 'paths' => \@paths };
  bless $i;
}

package FCAT;

sub new
{
  my $class= shift;
  my $FC= { 'inodes' => {}, 'md5' => {}, 'paths' => {} };
  bless $FC;
}

sub read_md5_catalog
{
  my $FC= shift;
  my $fnm= shift;

  open (FI, $fnm) || die;
  print "# reading $fnm\n";

  my $FCm= $FC->{'md5'};
  my $FCp= $FC->{'paths'};

  while (<FI>)
  {
    chop;
    my ($md5, $str, $size, $path)= split (' ', $_, 4);
    my $FCpp= $FC->{'paths'}->{$path};

    $FCpp->{'size'}= $size;
    $FCpp->{'md5'}= $md5;
    $FCm->{$md5}->{$FCpp->{'inode'}}++;

    $MSIZE{$md5}= $size;
  }
  close (FI);
}

sub read_inode_catalog
{
  my $FC= shift;
  my $fnm= shift;

  open (FI, $fnm) || die;

  print "# reading $fnm\n";
  my $FCi= $FC->{'inodes'};
  my $FCp= $FC->{'paths'};

  while (<FI>)
  {
    chop;
    my ($inode, @paths)= split (/\|/, $_);
    my $io= new FCAT::inode ($inode, @paths);
    $FCi->{$inode}= $io;
    map { $FCp->{$_}= $io; } @paths;
  }
  close (FI);
}

sub show_dups
{
  my $FC= shift;

  my $FCm= $FC->{'md5'};
  my $FCi= $FC->{'inodes'};

  print "# searching duplicates\n";

  my $tsize= 0;
  my $md5;
  ## print __LINE__, " FCm=", main::Dumper ($FCm), "\n";
  foreach $md5 (keys %$FCm)
  {
    next if ($md5 eq 'd41d8cd98f00b204e9800998ecf8427e'); # 0 byte file

    my $fc_m= $FCm->{$md5};
    my @inodes= keys %$fc_m;
    my $n_inodes= @inodes;

    next if ($n_inodes == 1);
print join (' ', '#', __LINE__, %$fc_m), "\n";
    
    # @inodes is a list of inodes, but each inode may have a different
    # number of references, pick the inode with the largest number of
    # references
    my $c1= 0;
    my $i1= undef;
    foreach my $inode (@inodes)
    {
      if ($fc_m->{$inode} > $c1) { $i1= $inode; $c1= $fc_m->{$inode}; }
    }
    next unless ($i1);

    my $size= $MSIZE{$md5};
    $tsize += ($n_inodes-1) * $size;

    my $p1= $FCi->{$i1}->{'paths'}->[0];
    next unless ($p1); # catalog files do no match yet, skip file if no path known now

    ## print "## i1=$i1, p1=", main::Dumper ($p1), "\n";
    print "## selected inode $i1, refs=$c1, path=[$p1]\n";

    printf ("# %s %9ld :: %9d : ", $md5, $size, $i1);
    print join (' ', @inodes), "\n";

    my $i;
    my @x_paths;
    foreach $i (@inodes)
    {
      next if ($i == $i1);
      my $io= $FCi->{$i};
      ## &main::print_refs (*STDOUT, 'io', $io);
      push (@x_paths, @{$io->{'paths'}});
    }

    my $p;
    foreach $p (@x_paths)
    {
      print "rm '$p'\n";
      print "ln '$p1' '$p'\n";
    }
  }
  $tsize;
}

__END__

# POD Section

=head1 Copyright

Copyright (c) 2006..2012 Gerhard Gonter.  All rights reserved.  This
is free software; you can redistribute it and/or modify it under
the same terms as Perl itself.

=head1 AUTHOR

Gerhard Gonter <ggonter@cpan.org>

=head1 SEE ALSO

For more information, see http://sourceforge.net/projects/md5cat/
and possibly http://aix-pm.sourceforge.net/

=cut

