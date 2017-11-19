#!/usr/local/bin/perl
# FILE md5cat/chkmd5.pl
#
# update a md5 catalog
# see sub usage for details
#
# written:       1996-06-15
# latest update: 2000-01-23 14:45:00
my $ID= '$Id: chkmd5.pl,v 1.20 2013/10/20 13:26:23 gonter Exp $';
#
# ----------------------------------------------------------------------------
# Contents:
# sub save_catalog              save the (new) MD5 catalog
# sub check_new_files           generate a list of new files and MD5 them
# sub usage                     print usage
# sub read_flist                get a reference list from a find -print output
# sub read_dir_listing          get a reference list from an ls -XX output
# sub check_md5_entries         read a MD5 listing
#
# VARS
# ----------------------------------------------------------------------------

use strict;

use Data::Dumper;
$Data::Dumper::Indent= 1;

BEGIN {
  my @p= split ('/', $0);
  pop (@p);
  unshift (@INC, join ('/', @p));
}

use md5cat;

# configuration
my $chk_new_files= 0;
my $checksum;

use Digest::MD5::File;

=begin comment

{
  print "no Digest::MD5::File\n";

  $checksum= '/usr/bin/checksum';  # if its 'checksum', where is it?
  $checksum= '/usr/local/bin/checksum' unless (-x $checksum);

unless (-x $checksum && -x '/usr/bin/md5sum')
{
  $checksum= '/usr/bin/md5sum';
  $chksum_pgm= 'md5sum';
}
}

=end comment
=cut

# temp files
my $tmp_dir;
my $xsep= 0;
my $input_fmt= 'list';
$"= '/';
$|= 1;
my $ref_cnt= 0;
my @REPORT= ();
my $Catalog_Date= time;
# my $Dir_Pattern= '[0-9_a-zA-Z]*';
my $Dir_Pattern= '.';
my $DEFAULT_file_list= "find $Dir_Pattern -xdev -type f -print|";

my %md5cat_opts=
(
  'catalog' => '_catalog',
  'to_lower' => 0,
  'skip_vcs' => 1, # skip directories that belong to version control systems
  'do_unlink' => 0,
  'verbose_level' => 0,
  'MAX_CSIZE' => 2147483640,
  # 'chksum_pgm' => 'checksum',         # which checksum program to use
  'chksum_pgm' => 'Digest::MD5::File',
  # 'checksum'
);

# initializiation
my %diag= ();
my $file_list;
ARGUMENT: while (defined (my $arg= shift (@ARGV)))
{
  if ($arg =~ /^-/)
  {
    if ($arg eq '-xsep')
    {
      $xsep= 1;
    }
    elsif ($arg eq '-dot')
    {
      $Dir_Pattern= '.';
      $DEFAULT_file_list= "find $Dir_Pattern -xdev -type f -print|";
    }
    elsif ($arg eq '-W')
    {
      $Dir_Pattern= '[0-9_a-zA-Z]*';
      $DEFAULT_file_list= "find $Dir_Pattern -xdev -type f -print|";
    }
    elsif ($arg eq '-file')
    {
      $file_list= shift (@ARGV);
      $input_fmt= 'list';
    }
    elsif ($arg eq '-lR')
    {
      $file_list= shift (@ARGV);
      $input_fmt= '-lR';
    }
    elsif ($arg eq '-to_lower') { $md5cat_opts{'to_lower'}= 1; }
    elsif ($arg =~ /^-cat/)
    {
      $md5cat_opts{'catalog'}= shift (@ARGV);
      $tmp_dir= '/tmp/' unless (defined ($tmp_dir));
    }
    elsif ($arg eq '-CRC32') { $md5cat_opts{'chksum_pgm'}= 'CRC32'; }
    elsif ($arg eq '-tmp')
    {
      $tmp_dir= shift (@ARGV);
      $tmp_dir .= '/' unless ($tmp_dir =~ /\/$/);
    }
    elsif ($arg eq '-base')
    {
      $tmp_dir= shift (@ARGV);
      $tmp_dir .= '/' unless ($tmp_dir =~ /\/$/);
      $md5cat_opts{'catalog'}= $tmp_dir . '_catalog';
    }
    elsif ($arg eq '-vcs') { $md5cat_opts{'skip_vcs'}= 0; }
    elsif ($arg eq '-X') { $diag{'DUMP'}= 1; }
    else
    {
      &usage;
      exit (0);
    }
    next ARGUMENT;
  }
}

# die "can't find checksum program" unless (-x $checksum);
print "chksum_pgm='$md5cat_opts{'chksum_pgm'}'\n";

my $md5cat= new md5cat (%md5cat_opts);
$md5cat->set_tmpdir ($tmp_dir);

# print __LINE__, " md5cat: ", Dumper ($md5cat);

# Step 1: read list of files that are currently present
if ($input_fmt eq 'list')
{ # input is a stream with individual filenames on each line
  $file_list= $DEFAULT_file_list unless ($file_list);
  $ref_cnt= $md5cat->read_flist ($file_list);
}
elsif ($input_fmt eq '-lR')
{
  $ref_cnt= $md5cat->read_dir_listing ($file_list);
}
else
{
  &usage;
  exit (0);
}

# print __LINE__, " md5cat: ", Dumper ($md5cat);
print "ref_cnt=$ref_cnt\n";

my $to_save= $md5cat->update_catalog ();

foreach (@REPORT) { print $_, "\n"; }

# print __LINE__, " md5cat: ", Dumper ($md5cat);
# print __LINE__, " diag: finalizing\n"; $md5cat->diag ();

print "no changes!\n" unless ($to_save);

print Dumper ($md5cat) if ($diag{'DUMP'});

exit (0);

# ----------------------------------------------------------------------------
sub usage
{
  print <<END_OF_USAGE;
usage: $0 [-options] [filename]

Options:
-help           ... print help
-catalog <fnm>  ... specify filename for the catalog
-file <fnm>     ... specify filename for file listing
                    default: generate file list on the fly, using find:
                    $DEFAULT_file_list
-lR <fnm>       ... this file contains a directory listing in "ls -lR" format,
                    use it instead of -file reference
-to_lower       ... convert filenames from reference list to lower case (MSDOS)
-tmp <dir>      ... specify directory for temporary files
-dot            ... scan . recursively

Experimental Options:
-xsep           ... cataloged files were xsep'ed in the meantime; adjust paths
                    [test]

Notes:
1. Only uses files on the same device only; does not cross mount points.

$ID
END_OF_USAGE
}

__END__

=pod

=head1 TODO

 * factor catalog reading into library which should also take code from cmp2md5.pl
 * option for new catalog format

=head1 PACKAGES

