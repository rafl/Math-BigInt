#!/usr/bin/perl -w

# measure copy overhead - comment out one of the lines below, and profile

# with:
# perl copy.pl -d:Dprof;dprofpp -O30
# You might adjust $cnt to 10 for the multiplication test, otherwise it takes
# too long.
# The numbers are more or less guesses, since DProf did not produce results
# that add up too 100%, e.g. Copy takes 52%, the rest takes summed up 2%. In
# these case I assummed that copy really takes 98% and DProf barfed.

use lib '../lib';

use strict;
use Math::BigInt;

my $y = 100000;				# 100, 1000, 10000 etc

my $a = Math::BigInt->new('1'.'0'x$y);
my $b1 = Math::BigInt->new(2);
my $c1 = Math::BigInt->new(3);
my $d1 = Math::BigInt->new(4);
my $e1 = Math::BigInt->new(5);
my $b = Math::BigInt->new('2'.'0'x$y);
my $c = Math::BigInt->new('3'.'0'x$y);
my $d = Math::BigInt->new('4'.'0'x$y);
my $e = Math::BigInt->new('5'.'0'x$y);


my $z; my $cnt = 10;
for (my $i = 0; $i < $cnt; $i++)
  {					# c = 1000   10_000  100_000	 
  $z = $a + $b1 + $c1 + $d1 + $e1;	#      45%     93%     99%
  #$a = -$a;				#      50%     95%
  #$z = $a + $b + $c + $d + $e;		#      12%     14%     16%
  #$z = $a - $b - $c - $d - $e;		#     8.8%     15%
  #$z = $a * $b * $c * $d * $e;		#     0.2%
  }

