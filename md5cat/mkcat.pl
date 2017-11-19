#!/usr/bin/perl

use strict;

my $chkmd5= '/usr/local/bin/chkmd5.pl';

my $dir= undef;
my $cat= undef;

while (my $arg= shift (@ARGV))
{
  if ($arg =~ /^-/)
  {
    if ($arg eq '-cat') { $cat= shift (@ARGV); }
    elsif ($arg eq '-dir') { $dir= shift (@ARGV); }
    elsif ($arg eq '-vol')
    {
      my $vol= shift (@ARGV);
      # TBD!
      &usage ();
    }
  }

}

unless ($dir && $cat)
{
  &usage ();
  exit -1;
}

&mk_cat ($dir, $cat);

exit;

sub mk_cat
{
  my $dir= shift;
  my $cat= shift;

  my $pwd= `pwd`;
  chop ($pwd);
  my $x_cat= join ('/', $pwd, $cat);

  my $cmd= "(cd '$dir' && $chkmd5 -catalog '$x_cat')";
  print ">>> [$cmd]\n";
  system ($cmd);
}



