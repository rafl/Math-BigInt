#!/usr/bin/perl -w

use lib '../lib';
#use lib '../../Math-BigInt-1.39/lib';
#use lib '../../Math-BigInt-0.01/lib';
use Math::BigInt;
use Math::BigFloat;
use Benchmark;

my ($x,$y,$z,$xf,$yf,$zf);

my $digits = shift || 10;
$x = '1'.'0' x $digits;

print "digits: $digits\n";

$x = Math::BigInt->new($x);
$y = -$x;
$z = Math::BigInt->bzero();

$xf = Math::BigFloat->new($x);
$yf = -$xf;
$zf = Math::BigFloat->bzero();

timethese (40000, 
  {
  '1   0 <=>  0' => sub { $z->bcmp($z); }, 
  '2   0 <=> +x' => sub { $z->bcmp($x); }, 
  '3  +x <=>  0' => sub { $x->bcmp($z); }, 
  '4   0 <=> -x' => sub { $z->bcmp($y); }, 
  '5  -x <=>  0' => sub { $y->bcmp($z); }, 
  
  '6   0 <=>  0' => sub { $zf->bcmp($zf); }, 
  '7   0 <=> +x' => sub { $zf->bcmp($xf); }, 
  '8  +x <=>  0' => sub { $xf->bcmp($zf); }, 
  '9   0 <=> -x' => sub { $zf->bcmp($yf); }, 
  '10 -x <=>  0' => sub { $yf->bcmp($zf); }, 
  } );
