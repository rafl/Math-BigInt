###############################################################################
# core math lib for BigInt, representing big numbers by normal int/float's
# for testing only, will fail any bignum test if range is exceeded

package Math::BigInt::Scalar;

use 5.005;
use strict;
use warnings;

require Exporter;

use vars qw/ @ISA @EXPORT $VERSION/;
@ISA = qw(Exporter);

@EXPORT = qw(
        _add _mul _div _mod _sub
        _new _from_hex _from_bin
        _str _num _acmp _len
        _digit
        _is_zero _is_one
        _is_even _is_odd
        _check _zero _one _copy
	_pow _and _or _xor
);
$VERSION = '0.03';

##############################################################################
# global constants, flags and accessory
 
# constants for easier life
my $nan = 'NaN';
my $class = 'Math::BigInt::Small';

##############################################################################
# create objects from various representations

sub _new
  {
  # (string) return ref to num
  shift @_ if $_[0] eq $class;
  my $d = shift;
  my $x = $$d;
  return \$x;
  }                                                                             

sub _zero
  {
  my $x = 0; return \$x;
  }

sub _one
  {
  my $x = 1; return \$x;
  }

sub _copy
  {
  shift @_ if $_[0] eq $class;
  my $x = shift;
  
  my $z = $$x;
  return \$z;
  }

##############################################################################
# convert back to string and number

sub _str
  {
  # make string
  shift @_ if $_[0] eq $class;
  my $ar = shift;
  return $ar;
  }                                                                             

sub _num
  {
  # make a number
  shift @_ if $_[0] eq $class;
  my $x = shift; return $$x;
  }


##############################################################################
# actual math code

sub _add
  {
  shift @_ if $_[0] eq $class;
  my ($x,$y) = @_;
  $$x += $$y;
  return $x;
  }                                                                             

sub _sub
  {
  shift @_ if $_[0] eq $class;
  my ($x,$y) = @_;
  $$x -= $$y;
  return $x;
  }                                                                             

sub _mul
  {
  shift @_ if $_[0] eq $class;
  my ($x,$y) = @_;
  $$x *= $$y;
  return $x;
  }                                                                             

sub _div
  {
  shift @_ if $_[0] eq $class;
  my ($x,$y) = @_;

  my $u = int($$x / $$y); my $r = $$x % $$y; $$x = $u;
  return ($x,\$r) if wantarray;
  return $x;
  }                                                                             

sub _pow
  {
  my ($x,$y) = @_;
  my $u = $$x ** $$y; $$x = $u;
  return $x;
  }

sub _and
  {
  my ($x,$y) = @_;
  my $u = int($$x) & int($$y); $$x = $u;
  return $x;
  }

sub _xor
  {
  my ($x,$y) = @_;
  my $u = int($$x) ^ int($$y); $$x = $u;
  return $x;
  }

sub _or
  {
  my ($x,$y) = @_;
  my $u = int($$x) | int($$y); $$x = $u;
  return $x;
  }

##############################################################################
# testing

sub _acmp
  {
  shift @_ if $_[0] eq $class;
  my ($x, $y) = @_;
  return ($$x <=> $$y);
  }

sub _len
  {
  shift @_ if $_[0] eq $class;
  my ($x) = @_;
  return length("$$x");
  }

sub _digit
  {
  # return the nth digit, negative values count backward
  # 0 is the rightmost digit
  shift @_ if $_[0] eq $class;
  my ($x,$n) = @_;
  
  $n ++;			# 0 => 1, 1 => 2
  return substr($$x,-$n,1);	# 1 => -1, -2 => 2 etc
  }

##############################################################################
# _is_* routines

sub _is_zero
  {
  # return true if arg is zero
  shift @_ if $_[0] eq $class;
  my ($x) = shift;
  return ($$x == 0) <=> 0;
  }

sub _is_even
  {
  # return true if arg is even
  shift @_ if $_[0] eq $class;
  my ($x) = shift;
  return (!($$x & 1)) <=> 0; 
  }

sub _is_odd
  {
  # return true if arg is odd
  shift @_ if $_[0] eq $class;
  my ($x) = shift;
  return ($$x & 1) <=> 0;
  }

sub _is_one
  {
  # return true if arg is one
  shift @_ if $_[0] eq $class;
  my ($x) = shift;
  return ($$x == 1) <=> 0;
  }

###############################################################################
# check routine to test internal state of corruptions

sub _check
  {
  # no checks yet, pull it out from the test suite
  shift @_ if $_[0] eq $class;

  my ($x) = shift;
  return "$x is not a reference" if !ref($x);
  return 0;
  }

1;
__END__

=head1 NAME

Math::BigInt::Scalar - Pure Perl module to test Math::BigInt with scalars

=head1 SYNOPSIS

Provides support for big integer calculations via means of 'small' int/floats.
Only for testing purposes, since it will fail at large values. But it is simple
enough not to introduce bugs on it's own and to serve as a testbed.

=head1 LICENSE
 
This program is free software; you may redistribute it and/or modify it under
the same terms as Perl itself. 

=head1 AUTHOR

Tels http://bloodgate.com in 2001.

=head1 SEE ALSO

L<Math::BigInt>, L<Math::BigInt::Calc>.

=cut
