#!/usr/bin/perl -w

use strict;
use Test;

BEGIN 
  {
  $| = 1;
  # chdir 't' if -d 't';
  unshift @INC, '../lib'; # for running manually
  plan tests => 31;
  }

# testing of Math::BigInt::Scalar, primarily for interface/api and not for the
# math functionality

use Math::BigInt::Scalar;

# _new and _str
my $x = _new(\"123");  ok (${_str($x)},123); ok ($$x,123);
my $y = _new(\"321");

# _add, _sub, _mul, _div

ok (${_str(_add($x,$y))},444);
ok (${_str(_sub($x,$y))},123);
ok (${_str(_mul($x,$y))},39483);
ok (${_str(_div($x,$y))},123);
ok (${_str($x)},123);

my $z = _new(\"111");
 _mul($x,$y); 
ok (${_str($x)},39483);
_add($x,$z);
ok (${_str($x)},39594);
my ($re,$rr) = _div($x,$y);

ok (${_str($re)},123); ok (${_str($rr)},111);

# _copy
$x = _new(\"123456789");
ok (${_str(_copy($x))},123456789);

# _digit
$x = _new(\"123456789");
ok (_digit($x,0),9);
ok (_digit($x,1),8);
ok (_digit($x,2),7);
ok (_digit($x,-1),1);
ok (_digit($x,-2),2);
ok (_digit($x,-3),3); 

# is_zero, _is_one, _one, _zero
ok (_is_zero($x),0);
ok (_is_one($x),0);

# _pow
$x = _new(\"7"); $y = _new(\"7"); ok (${_str(_pow($x,$y))},823543);
$x = _new(\"31"); $y = _new(\"7"); ok (${_str(_pow($x,$y))},27512614111);
$x = _new(\"2"); $y = _new(\"10"); ok (${_str(_pow($x,$y))},1024);

# _and, _xor, _or
$x = _new(\"7"); $y = _new(\"5"); ok (${_str(_and($x,$y))},5);
$x = _new(\"6"); $y = _new(\"1"); ok (${_str(_or($x,$y))},7);
$x = _new(\"9"); $y = _new(\"6"); ok (${_str(_xor($x,$y))},15);

ok (_is_one(_one()),1); ok (_is_one(_zero()),0);
ok (_is_zero(_zero()),1); ok (_is_zero(_one()),0);

ok (_check($x),0);

# done

1;

