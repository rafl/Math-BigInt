#!/usr/bin/perl -w

$| = 1;

use lib '../old/Math-BigInt-GMP-1.03/lib';
#use lib '../old/Math-BigInt-Pari-1.04/lib';
#use lib '../Math-BigInt-BitVect-1.06/lib';
#use lib '../old/Math-BigInt-BitVect-1.05/lib';
#use lib '../old/Math-BigInt-1.45/lib';
#use lib '../old/Math-BigInt-0.01';
use lib 'lib';

use Math::BigInt lib => 'GMP';
#use Math::BigInt lib => 'BitVect';
#use Math::BigInt lib => 'Pari';
#use Math::BigInt;
use Math::BigFloat;

print "Math::BigInt v$Math::BigInt::VERSION ";
if (Math::BigInt->can('_core_lib'))
  {
  print "lib => ",Math::BigInt->_core_lib();
  my $v = '$'.Math::BigInt->_core_lib().'::VERSION'; $v = eval "$v";
  print " v$v";
  }
print "\n";

use Benchmark;

my $x_10000 = Math::BigInt->new('1'.'0' x 10000);
my $x_1000 = Math::BigInt->new('1'.'0' x 1000);
my $x_100 = Math::BigInt->new('1'.'0' x 100);
my $x_10 = Math::BigInt->new('1'.'0' x 10);
my $x1_10000 = Math::BigInt->new('1'.'0' x 10000);
my $x1_1000 = Math::BigInt->new('1'.'0' x 1000);
my $x1_100 = Math::BigInt->new('1'.'0' x 100);
my $x1_10 = Math::BigInt->new('1'.'0' x 10);
my $y = Math::BigInt->new('1');
my $u = Math::BigInt->new('9'.'9' x 1000);
my $uf = Math::BigInt->new('9'.'9' x 1000);
my $x2_1000 = Math::BigInt->new('1'.'0' x 1000);
my $x3_1000 = Math::BigInt->new('1'.'0' x 1000);

my $xf_10000 = Math::BigFloat->new('1'.('0' x 10000).'1');
my $xf_1000 = Math::BigFloat->new('1'.('0' x 1000).'1');
my $xf_100 = Math::BigFloat->new('1'.('0' x 100).'1');
my $xf_10 = Math::BigFloat->new('1'.('0' x 10).'1');
my $xf2_10000 = Math::BigFloat->new('1'.('0' x 10000).'1');
my $xf2_1000 = Math::BigFloat->new('1'.('0' x 1000).'1');
my $xf2_100 = Math::BigFloat->new('1'.('0' x 100).'1');
my $xf2_10 = Math::BigFloat->new('1'.('0' x 10).'1');

timethese ( -5,
  {
  # comment out the inc tests for v0.01 - it had no binc()/finc() methods  
  binc_10000 => sub { $x1_10000->binc(); },
  binc_1000 => sub { $x1_1000->binc(); },
  binc_100 => sub { $x1_100->binc(); },
  binc_10 => sub { $x1_10->binc(); },
  #finc_10000 => sub { $xf_10000->finc(); },
  finc_1000 => sub { $xf_1000->finc(); },
  finc_100 => sub { $xf_100->finc(); },
  finc_10 => sub { $xf_10->finc(); },
  '++_10000' => sub { ++$x_10000; },
  '++_1000' => sub { ++$x_1000; },
  '++_100' => sub { ++$x_100; },
  '++_10' => sub { ++$x_10; },
  #'f++_10000' => sub { ++$xf2_10000; },
  'f++_1000' => sub { ++$xf2_1000; },
  'f++_100' => sub { ++$xf2_100; },
  'f++_10' => sub { ++$xf2_10; },
  '++_999' => sub { ++$u; },
  'f++_999' => sub { ++$uf; },
  add_1000 => sub { $x2_1000 += $y; },
  sub_1000 => sub { $x3_1000 -= $y; },
  } );

