#!/usr/bin/perl -w

#use lib '../../Math-BigInt-1.36/lib'; use Math::BigInt;	# 1.36
#use lib '../../Math-BigInt-1.36/lib'; use Math::BigInt lib => 'BitVect';
#use lib '../lib'; use Math::BigInt;				# cur
#use lib '../lib'; use Math::BigInt lib => 'BitVect';		# cur BitVect 1.01
use lib '../lib'; use lib '../../Math-BigInt-BitVect-1.02'; use Math::BigInt lib => 'BitVect';		# cur BitVect 1.02
#use lib '../lib'; use Math::BigInt lib => 'Pari';		# cur Pari
#use lib '../../Math-BigInt-1.33/lib'; use Math::BigInt;		# 1.33
#use lib '../../Math-BigInt-0.01'; use Math::BigInt;		# 0.0.1 v5.7.1
#use lib '../../Math-BigInt-0.49/lib'; use Math::BigInt;		# 0.49 Daniel

my $a = shift || 2;
my $b = shift || 2;

Math::BigInt::Calc::_base_len(shift) if @ARGV;

my $d = "none (not supported)";
$d = Math::BigInt::_core_lib() if Math::BigInt->can('_core_lib');
$m =
 eval { Math::BigInt::Calc::_base_len(); } ||
 eval { Math::BigInt::BASEDIGITS(); } || 'unknown (likely 1e5)';
$d .= " " . $d->VERSION if $d ne '';

print "$Math::BigInt::VERSION ($d) base 1e$m\n"; 

for ($i=0; $i<10; $i++)
  {
  $x = Math::BigInt->new($a); $x **= $b;
  }

