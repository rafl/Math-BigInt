#!/usr/bin/perl -w

# run with 
# time perl factor.pl [digits]

#use lib '../old';
#use lib '../lib';
#use lib '../../Math-Big-1.04/lib';

#use Math::BigInt calc => 'BitVect';
use Math::BigInt;
use Math::Big;

my $y = shift || 1000;

my $x = Math::Big::factorial(Math::BigInt->new($y));
#print $x->length(),"\n";
#
#print "$x";
