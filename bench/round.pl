#!/usr/bin/perl -w
# benchmark to measure rounding speed

# comment one out
#use lib '../old'; my $class = 'Math::BigFloat';
use lib '../lib'; my $class = 'Math::BigInt';

use Math::BigInt calc => 'BitVect';
#use Math::BigInt;
use Math::BigFloat;

$| = 1;
# for some determined randomness
srand(3);
my $digits = shift || 2000;	# test numbers up to this length
my $loops = 25;			# so much different numbers (more, better)

print "Rounding benchmark using $digits digits...\n";
for ($i = 0; $i < $loops; $i ++)
  {
  $x = int(rand($digits)+1)+4;                  # length
  $y = "";
  while (length($y) < $x)
    {
    $y .= int(rand(10000));
    }
  $z = length($y);
  $y = $class->new($y);
  print "\r to go ",$loops-$i,"        ";
  # now round some amount to measure it instead of the setup time
  $x = $z-2;                                    # preserve so many digits
  while ($x > 3)
    {
    #$y->fround($x);                            # now round to somewhere
    $y = Math::BigFloat->new($y->fround($x));   # for old lib
    $x -= 3;
    }
  }

print "done\n";
