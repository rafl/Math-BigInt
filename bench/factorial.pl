#!/usr/bin/perl -w

#use lib '../../Math-BigInt-1.36/lib'; use Math::BigInt;	# 1.36
#use lib '../../Math-BigInt-1.36/lib'; use Math::BigInt lib => 'BitVect';
#use lib '.'; use Math::BigInt lib => 'BitVect';		# 1.36 BitVect
use lib '../lib'; use Math::BigInt;				# 1.37
#use lib '../lib'; use Math::BigInt lib => 'BitVect';		# 1.37 BitVect
#use lib '../lib'; use Math::BigInt lib => 'Pari';		# 1.37 Pari
#use lib '../../Math-BigInt-1.33/lib'; use Math::BigInt;		# 1.33
#use lib '../../Math-BigInt-0.01'; use Math::BigInt;		# 0.0.1 v5.7.1
#use lib '../../Math-BigInt-0.49/lib'; use Math::BigInt;		# 0.49 Daniel

my $n = shift || 500;

Math::BigInt::Calc::_base_len(shift) if @ARGV;

my $d = "none (not supported)";
$d = Math::BigInt::_core_lib() if Math::BigInt->can('_core_lib');
$m = 
 eval { Math::BigInt::Calc::_base_len(); } ||
 eval { Math::BigInt::BASEDIGITS(); } || 'unknown (likely 1e5)';
print "$Math::BigInt::VERSION ($d) base 1e$m\n";

for ($k = 0; $k < 10; $k++)
  {
  factorial($n);
  }

sub factorial
  {
  my ($n,$i) = shift;
  my $res = Math::BigInt->new(1);
  return $res if $n < 1;
  for ($i = 2; $i <= $n; $i++)
    {
    $res *= $i;
    }
  return $res;
  }
