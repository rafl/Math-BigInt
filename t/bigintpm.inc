#include this file into another for subclass testing

my $version = ${"$class\::VERSION"};

##############################################################################
# for testing inheritance of _swap

package Math::Foo;

use Math::BigInt;
#use Math::BigInt lib => 'BitVect';	# for testing
use vars qw/@ISA/;
@ISA = (qw/Math::BigInt/);

use overload
# customized overload for sub, since original does not use swap there
'-'     =>      sub { my @a = ref($_[0])->_swap(@_);
                   $a[0]->bsub($a[1])};

sub _swap
  {
  # a fake _swap, which reverses the params
  my $self = shift;                     # for override in subclass
  if ($_[2])
    {
    my $c = ref ($_[0] ) || 'Math::Foo';
    return ( $_[0]->copy(), $_[1] );
    }
  else
    {
    return ( Math::Foo->new($_[1]), $_[0] );
    }
  }

##############################################################################
package main;

my $CALC = $class->_core_lib(); ok ($CALC,$CL);

my ($f,$z,$a,$exp,@a,$m,$e,$round_mode);

while (<DATA>) 
  {
  chop;
  next if /^#/;	# skip comments
  if (s/^&//) 
    {
    $f = $_;
    }
  elsif (/^\$/) 
    {
    $round_mode = $_;
    $round_mode =~ s/^\$/$class\->/;
    # print "$round_mode\n";
    }
  else 
    {
    @args = split(/:/,$_,99);
    $ans = pop(@args);
    $try = "\$x = $class->new(\"$args[0]\");";
    if ($f eq "bnorm"){
      $try = "\$x = $class->bnorm(\"$args[0]\");";
    # some is_xxx tests
    } elsif ($f =~ /^is_(zero|one|odd|even|negative|positive|nan)$/) {
      $try .= "\$x->$f();";
    } elsif ($f eq "as_hex") {
      $try .= '$x->as_hex();';
    } elsif ($f eq "as_bin") {
      $try .= '$x->as_bin();';
    } elsif ($f eq "is_inf") {
      $try .= "\$x->is_inf('$args[1]');";
    } elsif ($f eq "binf") {
      $try .= "\$x->binf('$args[1]');";
    } elsif ($f eq "bone") {
      $try .= "\$x->bone('$args[1]');";
    # some unary ops
    } elsif ($f =~ /^b(nan|floor|ceil|sstr|neg|abs|inc|dec|not|sqrt)$/) {
      $try .= "\$x->$f();";
    }elsif ($f eq "length") {
      $try .= '$x->length();';
    }elsif ($f eq "exponent"){
      # ->bstr() to see if an object is returned
      $try .= '$x = $x->exponent()->bstr();';
    }elsif ($f eq "mantissa"){
      # ->bstr() to see if an object is returned
      $try .= '$x = $x->mantissa()->bstr();';
    }elsif ($f eq "parts"){
      $try .= '($m,$e) = $x->parts();'; 
      # ->bstr() to see if an object is returned
      $try .= '$m = $m->bstr(); $m = "NaN" if !defined $m;';
      $try .= '$e = $e->bstr(); $e = "NaN" if !defined $e;';
      $try .= '"$m,$e";';
    } else {
      $try .= "\$y = $class->new('$args[1]');";
      if ($f eq "bcmp"){
        $try .= '$x <=> $y;';
      }elsif ($f eq "bround") {
      $try .= "$round_mode; \$x->bround(\$y);";
      }elsif ($f eq "bacmp"){
        $try .= '$x->bacmp($y);';
      }elsif ($f eq "badd"){
        $try .= '$x + $y;';
      }elsif ($f eq "bsub"){
        $try .= '$x - $y;';
      }elsif ($f eq "bmul"){
        $try .= '$x * $y;';
      }elsif ($f eq "bdiv"){
        $try .= '$x / $y;';
      }elsif ($f eq "bdiv-list"){
        $try .= 'join (",",$x->bdiv($y));';
      # overload via x=
      }elsif ($f =~ /^.=$/){
        $try .= "\$x $f \$y;";
      # overload via x
      }elsif ($f =~ /^.$/){
        $try .= "\$x $f \$y;";
      }elsif ($f eq "bmod"){
        $try .= '$x % $y;';
      }elsif ($f eq "bgcd")
        {
        if (defined $args[2])
          {
          $try .= " \$z = $class->new(\"$args[2]\"); ";
          }
        $try .= "$class\::bgcd(\$x, \$y";
        $try .= ", \$z" if (defined $args[2]);
        $try .= " );";
        }
      elsif ($f eq "blcm")
        {
        if (defined $args[2])
          {
          $try .= " \$z = $class->new(\"$args[2]\"); ";
          }
        $try .= "$class\::blcm(\$x, \$y";
        $try .= ", \$z" if (defined $args[2]);
        $try .= " );";
      }elsif ($f eq "blsft"){
        if (defined $args[2])
          {
          $try .= "\$x->blsft(\$y,$args[2]);";
          }
        else
          {
          $try .= "\$x << \$y;";
          }
      }elsif ($f eq "brsft"){
        if (defined $args[2])
          {
          $try .= "\$x->brsft(\$y,$args[2]);";
          }
        else
          {
          $try .= "\$x >> \$y;";
          }
      }elsif ($f eq "band"){
        $try .= "\$x & \$y;";
      }elsif ($f eq "bior"){
        $try .= "\$x | \$y;";
      }elsif ($f eq "bxor"){
        $try .= "\$x ^ \$y;";
      }elsif ($f eq "bpow"){
        $try .= "\$x ** \$y;";
      }elsif ($f eq "digit"){
        $try = "\$x = $class->new(\"$args[0]\"); \$x->digit($args[1]);";
      } else { warn "Unknown op '$f'"; }
    }
   #  print "trying $try\n";
    $ans1 = eval $try;
    # remove leading '+' from target
    $ans =~ s/^[+]([0-9])/$1/; 		
    # convert hex/binary targets to decimal	
    if ($ans =~ /^(0x0x|0b0b)/)
      {
      $ans =~ s/^0[xb]//;
      $ans = Math::BigInt->new($ans)->bstr();
      }
    if ($ans eq "")
      {
      ok_undef ($ans1); 
      }
    else
      {
      # print "try: $try ans: $ans1 $ans\n";
      print "# Tried: '$try'\n" if !ok ($ans1, $ans);
      }
    # check internal state of number objects
    is_valid($ans1,$f) if ref $ans1; 
    }
  } # endwhile data tests
close DATA;

# test some more
@a = ();
for (my $i = 1; $i < 10; $i++) 
  {
  push @a, $i;
  }
ok "@a", "1 2 3 4 5 6 7 8 9";

# test whether self-multiplication works correctly (result is 2**64)
$try = "\$x = $class->new('4294967296');";
$try .= '$a = $x->bmul($x);';
$ans1 = eval $try;
print "# Tried: '$try'\n" if !ok ($ans1, $class->new(2) ** 64);
# test self-pow
$try = "\$x = $class->new(10);";
$try .= '$a = $x->bpow($x);';
$ans1 = eval $try;
print "# Tried: '$try'\n" if !ok ($ans1, $class->new(10) ** 10);

# test whether op destroys args or not (should better not)

$x = $class->new(3);
$y = $class->new(4);
$z = $x & $y;
ok ($x,3);
ok ($y,4);
ok ($z,0);
$z = $x | $y;
ok ($x,3);
ok ($y,4);
ok ($z,7);
$x = $class->new(1);
$y = $class->new(2);
$z = $x | $y;
ok ($x,1);
ok ($y,2);
ok ($z,3);

$x = $class->new(5);
$y = $class->new(4);
$z = $x ^ $y;
ok ($x,5);
ok ($y,4);
ok ($z,1);

$x = $class->new(-5); $y = -$x;
ok ($x, -5);

$x = $class->new(-5); $y = abs($x);
ok ($x, -5);

# check whether overloading cmp works
$try = "\$x = $class->new(0);";
$try .= "\$y = 10;";
$try .= "'false' if \$x ne \$y;";
$ans = eval $try;
print "# For '$try'\n" if (!ok "$ans" , "false" ); 

# we cant test for working cmpt with other objects here, we would need a dummy
# object with stringify overload for this. see Math::String tests as example

###############################################################################
# check reversed order of arguments

$try = "\$x = $class->new(10); \$x = 2 ** \$x;";
$try .= "'ok' if \$x == 1024;"; $ans = eval $try;
print "# For '$try'\n" if (!ok "$ans" , "ok" ); 

$try = "\$x = $class->new(10); \$x = 2 * \$x;";
$try .= "'ok' if \$x == 20;"; $ans = eval $try;
print "# For '$try'\n" if (!ok "$ans" , "ok" ); 

$try = "\$x = $class->new(10); \$x = 2 + \$x;";
$try .= "'ok' if \$x == 12;"; $ans = eval $try;
print "# For '$try'\n" if (!ok "$ans" , "ok" ); 

$try = "\$x = $class\->new(10); \$x = 2 - \$x;";
$try .= "'ok' if \$x == -8;"; $ans = eval $try;
print "# For '$try'\n" if (!ok "$ans" , "ok" ); 

$try = "\$x = $class\->new(10); \$x = 20 / \$x;";
$try .= "'ok' if \$x == 2;"; $ans = eval $try;
print "# For '$try'\n" if (!ok "$ans" , "ok" ); 

$try = "\$x = $class\->new(3); \$x = 20 % \$x;";
$try .= "'ok' if \$x == 2;"; $ans = eval $try;
print "# For '$try'\n" if (!ok "$ans" , "ok" ); 

$try = "\$x = $class\->new(7); \$x = 20 & \$x;";
$try .= "'ok' if \$x == 4;"; $ans = eval $try;
print "# For '$try'\n" if (!ok "$ans" , "ok" ); 

$try = "\$x = $class\->new(7); \$x = 0x20 | \$x;";
$try .= "'ok' if \$x == 0x27;"; $ans = eval $try;
print "# For '$try'\n" if (!ok "$ans" , "ok" ); 

$try = "\$x = $class\->new(7); \$x = 0x20 ^ \$x;";
$try .= "'ok' if \$x == 0x27;"; $ans = eval $try;
print "# For '$try'\n" if (!ok "$ans" , "ok" ); 

###############################################################################
# check badd(4,5) form

$try = "\$x = $class\->badd(4,5);";
$try .= "'ok' if \$x == 9;";
$ans = eval $try;
print "# For '$try'\n" if (!ok "$ans" , "ok" ); 

###############################################################################
# check undefs: NOT DONE YET

###############################################################################
# bool

$x = Math::BigInt->new(1); if ($x) { ok (1,1); } else { ok($x,'to be true') }
$x = Math::BigInt->new(0); if (!$x) { ok (1,1); } else { ok($x,'to be false') }

###############################################################################
# objectify()

@args = Math::BigInt::objectify(2,4,5);
ok (scalar @args,3);		# $class, 4, 5
ok ($args[0],$class);
ok ($args[1],4);
ok ($args[2],5);

@args = Math::BigInt::objectify(0,4,5);
ok (scalar @args,3);		# $class, 4, 5
ok ($args[0],$class);
ok ($args[1],4);
ok ($args[2],5);

@args = Math::BigInt::objectify(2,4,5);
ok (scalar @args,3);		# $class, 4, 5
ok ($args[0],$class);
ok ($args[1],4);
ok ($args[2],5);

@args = Math::BigInt::objectify(2,4,5,6,7);
ok (scalar @args,5);		# $class, 4, 5, 6, 7
ok ($args[0],$class);
ok ($args[1],4); ok (ref($args[1]),$args[0]);
ok ($args[2],5); ok (ref($args[2]),$args[0]);
ok ($args[3],6); ok (ref($args[3]),'');
ok ($args[4],7); ok (ref($args[4]),'');

@args = Math::BigInt::objectify(2,$class,4,5,6,7);
ok (scalar @args,5);		# $class, 4, 5, 6, 7
ok ($args[0],$class);
ok ($args[1],4); ok (ref($args[1]),$args[0]);
ok ($args[2],5); ok (ref($args[2]),$args[0]);
ok ($args[3],6); ok (ref($args[3]),'');
ok ($args[4],7); ok (ref($args[4]),'');

###############################################################################
# test for floating-point input (other tests in bnorm() below)

$z = 1050000000000000;          # may be int on systems with 64bit?
$x = $class->new($z); ok ($x->bsstr(),'105e+13');	# not 1.05e+15
$z = 1e+129;			# definitely a float (may fail on UTS)
# don't compare to $z, since some Perl versions stringify $z into something
# like '1.e+129' or something equally ugly
$x = $class->new($z); ok ($x->bsstr(),'1e+129');

###############################################################################
# prime number tests, also test for **= and length()
# found on: http://www.utm.edu/research/primes/notes/by_year.html

# ((2^148)-1)/17
$x = $class->new(2); $x **= 148; $x++; $x = $x / 17;
ok ($x,"20988936657440586486151264256610222593863921");
ok ($x->length(),length "20988936657440586486151264256610222593863921");

# MM7 = 2^127-1
$x = $class->new(2); $x **= 127; $x--;
ok ($x,"170141183460469231731687303715884105727");

$x = $class->new('215960156869840440586892398248');
($x,$y) = $x->length();
ok ($x,30); ok ($y,0);

$x = $class->new('1_000_000_000_000');
($x,$y) = $x->length();
ok ($x,13); ok ($y,0);

# I am afraid the following is not yet possible due to slowness
# Also, testing for 2 meg output is a bit hard ;)
#$x = $class->new(2); $x **= 6972593; $x--;

# 593573509*2^332162+1 has exactly 1,000,000 digits
# takes about 24 mins on 300 Mhz, so cannot be done yet ;)
#$x = $class->new(2); $x **= 332162; $x *= "593573509"; $x++;
#ok ($x->length(),1_000_000);

###############################################################################
# inheritance and overriding of _swap

$x = Math::Foo->new(5);
$x = $x - 8;		# 8 - 5 instead of 5-8
ok ($x,3);
ok (ref($x),'Math::Foo');

$x = Math::Foo->new(5);
$x = 8 - $x;		# 5 - 8 instead of 8 - 5
ok ($x,-3);
ok (ref($x),'Math::Foo');

###############################################################################
# Test whether +inf eq inf
# This tried to test whether BigInt inf equals Perl inf. Unfortunately, Perl
# hasn't (before 5.7.3 at least) a consistent way to say inf, and some things
# like 1e100000 crash on some platforms. So simple test for the string 'inf'
$x = $class->new('+inf'); ok ($x,'inf');

###############################################################################
###############################################################################
# the followin tests only make sense with Math::BigInt::Calc or BareCalc

exit if $CALC !~ /^Math::BigInt::(Calc|BareCalc)$/; # for Pari et al.

###############################################################################
# check proper length of internal arrays

my $bl = $CL->_base_len();
my $BASE = '9' x $bl;
my $MAX = $BASE;
$BASE++;

$x = $class->new($MAX); is_valid($x);	# f.i. 9999
$x += 1; ok ($x,$BASE); is_valid($x); 	# 10000
$x -= 1; ok ($x,$MAX); is_valid($x);	# 9999 again

###############################################################################
# check numify

$x = $class->new($BASE-1);     ok ($x->numify(),$BASE-1); 
$x = $class->new(-($BASE-1));  ok ($x->numify(),-($BASE-1)); 
$x = $class->new($BASE);       ok ($x->numify(),$BASE); 
$x = $class->new(-$BASE);      ok ($x->numify(),-$BASE);
$x = $class->new( -($BASE*$BASE*1+$BASE*1+1) ); 
ok($x->numify(),-($BASE*$BASE*1+$BASE*1+1)); 

###############################################################################
# test bug in _digits with length($c[-1]) where $c[-1] was "00001" instead of 1

$x = $class->new($BASE-2); $x++; $x++; $x++; $x++;
if ($x > $BASE) { ok (1,1) } else { ok ("$x < $BASE","$x > $BASE"); }

$x = $class->new($BASE+3); $x++;
if ($x > $BASE) { ok (1,1) } else { ok ("$x > $BASE","$x < $BASE"); }

# test for +0 instead of int(): 
$x = $class->new($MAX); ok ($x->length(), length($MAX));

###############################################################################
# bug in sub where number with at least 6 trailing zeros after any op failed

$x = $class->new(123456); $z = $class->new(10000); $z *= 10; $x -= $z;
ok ($z, 100000);
ok ($x, 23456);

###############################################################################
# bug in shortcut in mul()

# construct a number with a zero-hole of BASE_LEN
$x = '1' x $bl . '0' x $bl . '1' x $bl . '0' x $bl;
$y = '1' x (2*$bl);
$x = $class->new($x)->bmul($y);
# result is 123..$bl .  $bl x (3*bl-1) . $bl...321 . '0' x $bl
$y = ''; my $d = '';
for (my $i = 1; $i <= $bl; $i++)
  {
  $y .= $i; $d = $i.$d;
  }
$y .= $bl x (3*$bl-1) . $d . '0' x $bl;
ok ($x,$y);

###############################################################################
# see if mul shortcut for small numbers works

$x = '9' x $bl;
$x = $class->new($x); 
# 999 * 999 => 998 . 001, 9999*9999 => 9998 . 0001
ok ($x*$x, '9' x ($bl-1) . '8' . '0' x ($bl-1) . '1');

###############################################################################
# bug with rest "-0" in div, causing further div()s to fail

$x = $class->new('-322056000'); ($x,$y) = $x->bdiv('-12882240');

ok ($y,'0','not -0');	# not '-0'
is_valid($y);

###############################################################################
# test whether bone/bzero take additional A & P, or reset it etc

$x = $class->new(2); $x->bzero(); ok_undef ($x->{_a}); ok_undef ($x->{_p});
$x = $class->new(2); $x->binf();  ok_undef ($x->{_a}); ok_undef ($x->{_p});
$x = $class->new(2); $x->bone();  ok_undef ($x->{_a}); ok_undef ($x->{_p});
$x = $class->new(2); $x->bnan();  ok_undef ($x->{_a}); ok_undef ($x->{_p});

$x = $class->new(2); $x->{_a} = 1; $x->{_p} = 2; $x->bnan();
ok_undef ($x->{_a}); ok_undef ($x->{_p});
$x = $class->new(2); $x->{_a} = 1; $x->{_p} = 2; $x->binf();
ok_undef ($x->{_a}); ok_undef ($x->{_p});

### all tests done ############################################################

1;

###############################################################################
# Perl 5.005 does not like ok ($x,undef)

sub ok_undef
  {
  my $x = shift;

  ok (1,1) and return if !defined $x;
  ok ($x,'undef');
  }

###############################################################################
# sub to check validity of a BigInt internally, to ensure that no op leaves a
# number object in an invalid state (f.i. "-0")

sub is_valid
  {
  my ($x,$f) = @_;

  my $e = 0;			# error?
  # ok as reference? 
  $e = 'Not a reference to Math::BigInt' if !ref($x);

  # has ok sign?
  $e = "Illegal sign $x->{sign} (expected: '+', '-', '-inf', '+inf' or 'NaN'"
   if $e eq '0' && $x->{sign} !~ /^(\+|-|\+inf|-inf|NaN)$/;

  $e = "-0 is invalid!" if $e ne '0' && $x->{sign} eq '-' && $x == 0;
  $e = $CALC->_check($x->{value}) if $e eq '0';

  # test done, see if error did crop up
  ok (1,1), return if ($e eq '0');

  ok (1,$e." after op '$f'");
  }

__DATA__
&.=
1234:-345:1234-345
&+=
1:2:3
-1:-2:-3
&-=
1:2:-1
-1:-2:1
&*=
2:3:6
-1:5:-5
&%=
100:3:1
8:9:8
&/=
100:3:33
-8:2:-4
&|=
2:1:3
&&=
5:7:5
&^=
5:7:2
&is_negative
0:0
-1:1
1:0
+inf:0
-inf:1
NaNneg:0
&is_positive
0:1
-1:0
1:1
+inf:1
-inf:0
NaNneg:0
&is_odd
abc:0
0:0
1:1
3:1
-1:1
-3:1
10000001:1
10000002:0
2:0
&is_even
abc:0
0:1
1:0
3:0
-1:0
-3:0
10000001:0
10000002:1
2:1
&bacmp
+0:-0:0
+0:+1:-1
-1:+1:0
+1:-1:0
-1:+2:-1
+2:-1:1
-123456789:+987654321:-1
+123456789:-987654321:-1
+987654321:+123456789:1
-987654321:+123456789:1
-123:+4567889:-1
# NaNs
acmpNaN:123:
123:acmpNaN:
acmpNaN:acmpNaN:
# infinity
+inf:+inf:0
-inf:-inf:0
+inf:-inf:0
-inf:+inf:0
+inf:123:1
-inf:123:1
+inf:-123:1
-inf:-123:1
# return undef
+inf:NaN:
NaN:inf:
-inf:NaN:
NaN:-inf:
&bnorm
123:123
# binary input
0babc:NaN
0b123:NaN
0b0:0
-0b0:0
-0b1:-1
0b0001:1
0b001:1
0b011:3
0b101:5
0b1000000000000000000000000000000:1073741824
0b_101:NaN
0b1_0_1:5
0b0_0_0_1:1
# hex input
-0x0:0
0xabcdefgh:NaN
0x1234:4660
0xabcdef:11259375
-0xABCDEF:-11259375
-0x1234:-4660
0x12345678:305419896
0x1_2_3_4_56_78:305419896
0xa_b_c_d_e_f:11259375
0x_123:NaN
# inf input
inf:inf
+inf:inf
-inf:-inf
0inf:NaN
# abnormal input
:NaN
abc:NaN
   1 a:NaN
1bcd2:NaN
11111b:NaN
+1z:NaN
-1z:NaN
# only one underscore between two digits
_123:NaN
_123_:NaN
123_:NaN
1__23:NaN
1E1__2:NaN
1_E12:NaN
1E_12:NaN
1_E_12:NaN
+_1E12:NaN
+0_1E2:100
+0_0_1E2:100
-0_0_1E2:-100
-0_0_1E+0_0_2:-100
E1:NaN
E23:NaN
1.23E1:NaN
1.23E-1:NaN
# bug with two E's in number beeing valid
1e2e3:NaN
1e2r:NaN
1e2.0:NaN
# normal input
0:0
+0:0
+00:0
+000:0
000000000000000000:0
-0:0
-0000:0
+1:1
+01:1
+001:1
+00000100000:100000
123456789:123456789
-1:-1
-01:-1
-001:-1
-123456789:-123456789
-00000100000:-100000
1_2_3:123
10000000000E-1_0:1
1E2:100
1E1:10
1E0:1
1.23E2:123
100E-1:10
# floating point input
# .2e2:20
1.E3:1000
1.01E2:101
1010E-1:101
-1010E0:-1010
-1010E1:-10100
1234.00:1234
# non-integer numbers
-1010E-2:NaN
-1.01E+1:NaN
-1.01E-1:NaN
&bnan
1:NaN
2:NaN
abc:NaN
&bone
2:+:+1
2:-:-1
boneNaN:-:-1
boneNaN:+:+1
2:abc:+1
3::+1
&binf
1:+:inf
2:-:-inf
3:abc:inf
&is_nan
123:0
abc:1
NaN:1
-123:0
&is_inf
+inf::1
-inf::1
abc::0
1::0
NaN::0
-1::0
+inf:-:0
+inf:+:1
-inf:-:1
-inf:+:0
# it must be exactly /^[+-]inf$/
+infinity::0
-infinity::0
&blsft
abc:abc:NaN
+2:+2:+8
+1:+32:+4294967296
+1:+48:+281474976710656
+8:-2:NaN
# excercise base 10
+12345:4:10:123450000
-1234:0:10:-1234
+1234:0:10:+1234
+2:2:10:200
+12:2:10:1200
+1234:-3:10:NaN
1234567890123:12:10:1234567890123000000000000
&brsft
abc:abc:NaN
+8:+2:+2
+4294967296:+32:+1
+281474976710656:+48:+1
+2:-2:NaN
# excercise base 10
-1234:0:10:-1234
+1234:0:10:+1234
+200:2:10:2
+1234:3:10:1
+1234:2:10:12
+1234:-3:10:NaN
310000:4:10:31
12300000:5:10:123
1230000000000:10:10:123
09876123456789067890:12:10:9876123
1234561234567890123:13:10:123456
&bsstr
1e+34:1e+34
123.456E3:123456e+0
100:1e+2
abc:NaN
&bneg
bnegNaN:NaN
+inf:-inf
-inf:inf
abd:NaN
+0:+0
+1:-1
-1:+1
+123456789:-123456789
-123456789:+123456789
&babs
babsNaN:NaN
+inf:inf
-inf:inf
+0:+0
+1:+1
-1:+1
+123456789:+123456789
-123456789:+123456789
&bcmp
bcmpNaN:bcmpNaN:
bcmpNaN:+0:
+0:bcmpNaN:
+0:+0:0
-1:+0:-1
+0:-1:1
+1:+0:1
+0:+1:-1
-1:+1:-1
+1:-1:1
-1:-1:0
+1:+1:0
+123:+123:0
+123:+12:1
+12:+123:-1
-123:-123:0
-123:-12:-1
-12:-123:1
+123:+124:-1
+124:+123:1
-123:-124:1
-124:-123:-1
+100:+5:1
-123456789:+987654321:-1
+123456789:-987654321:1
-987654321:+123456789:-1
-inf:5432112345:-1
+inf:5432112345:1
-inf:-5432112345:-1
+inf:-5432112345:1
+inf:+inf:0
-inf:-inf:0
+inf:-inf:1
-inf:+inf:-1
5:inf:-1
5:inf:-1
-5:-inf:1
-5:-inf:1
# return undef
+inf:NaN:
NaN:inf:
-inf:NaN:
NaN:-inf:
&binc
abc:NaN
+inf:inf
-inf:-inf
+0:+1
+1:+2
-1:+0
&bdec
abc:NaN
+inf:inf
-inf:-inf
+0:-1
+1:+0
-1:-2
&badd
abc:abc:NaN
abc:+0:NaN
+0:abc:NaN
+inf:-inf:0
-inf:+inf:0
+inf:+inf:inf
-inf:-inf:-inf
baddNaN:+inf:NaN
baddNaN:+inf:NaN
+inf:baddNaN:NaN
-inf:baddNaN:NaN
+0:+0:+0
+1:+0:+1
+0:+1:+1
+1:+1:+2
-1:+0:-1
+0:-1:-1
-1:-1:-2
-1:+1:+0
+1:-1:+0
+9:+1:+10
+99:+1:+100
+999:+1:+1000
+9999:+1:+10000
+99999:+1:+100000
+999999:+1:+1000000
+9999999:+1:+10000000
+99999999:+1:+100000000
+999999999:+1:+1000000000
+9999999999:+1:+10000000000
+99999999999:+1:+100000000000
+10:-1:+9
+100:-1:+99
+1000:-1:+999
+10000:-1:+9999
+100000:-1:+99999
+1000000:-1:+999999
+10000000:-1:+9999999
+100000000:-1:+99999999
+1000000000:-1:+999999999
+10000000000:-1:+9999999999
+123456789:+987654321:+1111111110
-123456789:+987654321:+864197532
-123456789:-987654321:-1111111110
+123456789:-987654321:-864197532
&bsub
abc:abc:NaN
abc:+0:NaN
+0:abc:NaN
+inf:-inf:inf
-inf:+inf:-inf
+inf:+inf:0
-inf:-inf:0
+0:+0:+0
+1:+0:+1
+0:+1:-1
+1:+1:+0
-1:+0:-1
+0:-1:+1
-1:-1:+0
-1:+1:-2
+1:-1:+2
+9:+1:+8
+99:+1:+98
+999:+1:+998
+9999:+1:+9998
+99999:+1:+99998
+999999:+1:+999998
+9999999:+1:+9999998
+99999999:+1:+99999998
+999999999:+1:+999999998
+9999999999:+1:+9999999998
+99999999999:+1:+99999999998
+10:-1:+11
+100:-1:+101
+1000:-1:+1001
+10000:-1:+10001
+100000:-1:+100001
+1000000:-1:+1000001
+10000000:-1:+10000001
+100000000:-1:+100000001
+1000000000:-1:+1000000001
+10000000000:-1:+10000000001
+123456789:+987654321:-864197532
-123456789:+987654321:-1111111110
-123456789:-987654321:+864197532
+123456789:-987654321:+1111111110
&bmul
abc:abc:NaN
abc:+0:NaN
+0:abc:NaN
NaNmul:+inf:NaN
NaNmul:-inf:NaN
-inf:NaNmul:NaN
+inf:NaNmul:NaN
+inf:+inf:inf
+inf:-inf:-inf
-inf:+inf:-inf
-inf:-inf:inf
+0:+0:+0
+0:+1:+0
+1:+0:+0
+0:-1:+0
-1:+0:+0
+123456789123456789:+0:+0
+0:+123456789123456789:+0
-1:-1:+1
-1:+1:-1
+1:-1:-1
+1:+1:+1
+2:+3:+6
-2:+3:-6
+2:-3:-6
-2:-3:+6
+111:+111:+12321
+10101:+10101:+102030201
+1001001:+1001001:+1002003002001
+100010001:+100010001:+10002000300020001
+10000100001:+10000100001:+100002000030000200001
+11111111111:+9:+99999999999
+22222222222:+9:+199999999998
+33333333333:+9:+299999999997
+44444444444:+9:+399999999996
+55555555555:+9:+499999999995
+66666666666:+9:+599999999994
+77777777777:+9:+699999999993
+88888888888:+9:+799999999992
+99999999999:+9:+899999999991
+25:+25:+625
+12345:+12345:+152399025
+99999:+11111:+1111088889
9999:10000:99990000
99999:100000:9999900000
999999:1000000:999999000000
9999999:10000000:99999990000000
99999999:100000000:9999999900000000
999999999:1000000000:999999999000000000
9999999999:10000000000:99999999990000000000
99999999999:100000000000:9999999999900000000000
999999999999:1000000000000:999999999999000000000000
9999999999999:10000000000000:99999999999990000000000000
99999999999999:100000000000000:9999999999999900000000000000
999999999999999:1000000000000000:999999999999999000000000000000
9999999999999999:10000000000000000:99999999999999990000000000000000
99999999999999999:100000000000000000:9999999999999999900000000000000000
999999999999999999:1000000000000000000:999999999999999999000000000000000000
9999999999999999999:10000000000000000000:99999999999999999990000000000000000000
&bdiv-list
100:20:5,0
4095:4095:1,0
-4095:-4095:1,0
4095:-4095:-1,0
-4095:4095:-1,0
123:2:61,1
# inf handling and general remainder
5:8:0,5
0:8:0,0
11:2:5,1
11:-2:-5,-1
-11:2:-5,1
# see table in documentation in MBI
0:inf:0,0
0:-inf:0,0
5:inf:0,5
5:-inf:0,5
-5:inf:0,-5
-5:-inf:0,-5
inf:5:inf,0
-inf:5:-inf,0
inf:-5:-inf,0
-inf:-5:inf,0
5:5:1,0
-5:-5:1,0
inf:inf:1,0
-inf:-inf:1,0
-inf:inf:-1,0
inf:-inf:-1,0
8:0:inf,8
inf:0:inf,inf
# exceptions to reminder rule
-8:0:-inf,-8
-inf:0:-inf,-inf
0:0:NaN,NaN
&bdiv
abc:abc:NaN
abc:+1:NaN
+1:abc:NaN
+0:+0:NaN
# inf handling (see table in doc)
0:inf:0
0:-inf:0
5:inf:0
5:-inf:0
-5:inf:0
-5:-inf:0
inf:5:inf
-inf:5:-inf
inf:-5:-inf
-inf:-5:inf
5:5:1
-5:-5:1
inf:inf:1
-inf:-inf:1
-inf:inf:-1
inf:-inf:-1
8:0:inf
inf:0:inf
-8:0:-inf
-inf:0:-inf
0:0:NaN
11:2:5
-11:-2:5
-11:2:-5
11:-2:-5
+0:+1:+0
+0:-1:+0
+1:+1:+1
-1:-1:+1
+1:-1:-1
-1:+1:-1
+1:+2:+0
+2:+1:+2
+1:+26:+0
+1000000000:+9:+111111111
+2000000000:+9:+222222222
+3000000000:+9:+333333333
+4000000000:+9:+444444444
+5000000000:+9:+555555555
+6000000000:+9:+666666666
+7000000000:+9:+777777777
+8000000000:+9:+888888888
+9000000000:+9:+1000000000
+35500000:+113:+314159
+71000000:+226:+314159
+106500000:+339:+314159
+1000000000:+3:+333333333
+10:+5:+2
+100:+4:+25
+1000:+8:+125
+10000:+16:+625
+999999999999:+9:+111111111111
+999999999999:+99:+10101010101
+999999999999:+999:+1001001001
+999999999999:+9999:+100010001
+999999999999999:+99999:+10000100001
+1111088889:+99999:+11111
-5:-3:1
-5:3:-1
4:3:1
4:-3:-1
1:3:0
1:-3:0
-2:-3:0
-2:3:0
8:3:2
-8:3:-2
14:-3:-4
-14:3:-4
-14:-3:4
14:3:4
# bug in Calc with '99999' vs $BASE-1
10000000000000000000000000000000000000000000000000000000000000000000000000000000000:10000000375084540248994272022843165711074:999999962491547381984643365663244474111576
&bmod
# inf handling, see table in doc
0:inf:0
0:-inf:0
5:inf:5
5:-inf:5
-5:inf:-5
-5:-inf:-5
inf:5:0
-inf:5:0
inf:-5:0
-inf:-5:0
5:5:0
-5:-5:0
inf:inf:0
-inf:-inf:0
-inf:inf:0
inf:-inf:0
8:0:8
inf:0:inf
# exceptions to reminder rule
-inf:0:-inf
-8:0:-8
0:0:NaN
abc:abc:NaN
abc:+1:abc:NaN
+1:abc:NaN
+0:+0:NaN
+0:+1:+0
+1:+0:+1
+0:-1:+0
-1:+0:-1
+1:+1:+0
-1:-1:+0
+1:-1:+0
-1:+1:+0
+1:+2:+1
+2:+1:+0
+1000000000:+9:+1
+2000000000:+9:+2
+3000000000:+9:+3
+4000000000:+9:+4
+5000000000:+9:+5
+6000000000:+9:+6
+7000000000:+9:+7
+8000000000:+9:+8
+9000000000:+9:+0
+35500000:+113:+33
+71000000:+226:+66
+106500000:+339:+99
+1000000000:+3:+1
+10:+5:+0
+100:+4:+0
+1000:+8:+0
+10000:+16:+0
+999999999999:+9:+0
+999999999999:+99:+0
+999999999999:+999:+0
+999999999999:+9999:+0
+999999999999999:+99999:+0
-9:+5:+1
+9:-5:-1
-9:-5:-4
-5:3:1
-2:3:1
4:3:1
1:3:1
-5:-3:-2
-2:-3:-2
4:-3:-2
1:-3:-2
4095:4095:0
100041000510123:3:0
152403346:12345:4321
&bgcd
abc:abc:NaN
abc:+0:NaN
+0:abc:NaN
+0:+0:+0
+0:+1:+1
+1:+0:+1
+1:+1:+1
+2:+3:+1
+3:+2:+1
-3:+2:+1
+100:+625:+25
+4096:+81:+1
+1034:+804:+2
+27:+90:+56:+1
+27:+90:+54:+9
&blcm
abc:abc:NaN
abc:+0:NaN
+0:abc:NaN
+0:+0:NaN
+1:+0:+0
+0:+1:+0
+27:+90:+270
+1034:+804:+415668
&band
abc:abc:NaN
abc:0:NaN
0:abc:NaN
1:2:0
3:2:2
+8:+2:+0
+281474976710656:+0:+0
+281474976710656:+1:+0
+281474976710656:+281474976710656:+281474976710656
-2:-3:-4
-1:-1:-1
-6:-6:-6
-7:-4:-8
-7:4:0
-4:7:4
# equal arguments are treated special, so also do some test with unequal ones
0xFFFF:0xFFFF:0x0xFFFF
0xFFFFFF:0xFFFFFF:0x0xFFFFFF
0xFFFFFFFF:0xFFFFFFFF:0x0xFFFFFFFF
0xFFFFFFFFFF:0xFFFFFFFFFF:0x0xFFFFFFFFFF
0xFFFFFFFFFFFF:0xFFFFFFFFFFFF:0x0xFFFFFFFFFFFF
0xF0F0:0xF0F0:0x0xF0F0
0x0F0F:0x0F0F:0x0x0F0F
0xF0F0F0:0xF0F0F0:0x0xF0F0F0
0x0F0F0F:0x0F0F0F:0x0x0F0F0F
0xF0F0F0F0:0xF0F0F0F0:0x0xF0F0F0F0
0x0F0F0F0F:0x0F0F0F0F:0x0x0F0F0F0F
0xF0F0F0F0F0:0xF0F0F0F0F0:0x0xF0F0F0F0F0
0x0F0F0F0F0F:0x0F0F0F0F0F:0x0x0F0F0F0F0F
0xF0F0F0F0F0F0:0xF0F0F0F0F0F0:0x0xF0F0F0F0F0F0
0x0F0F0F0F0F0F:0x0F0F0F0F0F0F:0x0x0F0F0F0F0F0F
0x1F0F0F0F0F0F:0x3F0F0F0F0F0F:0x0x1F0F0F0F0F0F
&bior
abc:abc:NaN
abc:0:NaN
0:abc:NaN
1:2:3
+8:+2:+10
+281474976710656:+0:+281474976710656
+281474976710656:+1:+281474976710657
+281474976710656:+281474976710656:+281474976710656
-2:-3:-1
-1:-1:-1
-6:-6:-6
-7:4:-3
-4:7:-1
# equal arguments are treated special, so also do some test with unequal ones
0xFFFF:0xFFFF:0x0xFFFF
0xFFFFFF:0xFFFFFF:0x0xFFFFFF
0xFFFFFFFF:0xFFFFFFFF:0x0xFFFFFFFF
0xFFFFFFFFFF:0xFFFFFFFFFF:0x0xFFFFFFFFFF
0xFFFFFFFFFFFF:0xFFFFFFFFFFFF:0x0xFFFFFFFFFFFF
0:0xFFFF:0x0xFFFF
0:0xFFFFFF:0x0xFFFFFF
0:0xFFFFFFFF:0x0xFFFFFFFF
0:0xFFFFFFFFFF:0x0xFFFFFFFFFF
0:0xFFFFFFFFFFFF:0x0xFFFFFFFFFFFF
0xFFFF:0:0x0xFFFF
0xFFFFFF:0:0x0xFFFFFF
0xFFFFFFFF:0:0x0xFFFFFFFF
0xFFFFFFFFFF:0:0x0xFFFFFFFFFF
0xFFFFFFFFFFFF:0:0x0xFFFFFFFFFFFF
0xF0F0:0xF0F0:0x0xF0F0
0x0F0F:0x0F0F:0x0x0F0F
0xF0F0:0x0F0F:0x0xFFFF
0xF0F0F0:0xF0F0F0:0x0xF0F0F0
0x0F0F0F:0x0F0F0F:0x0x0F0F0F
0x0F0F0F:0xF0F0F0:0x0xFFFFFF
0xF0F0F0F0:0xF0F0F0F0:0x0xF0F0F0F0
0x0F0F0F0F:0x0F0F0F0F:0x0x0F0F0F0F
0x0F0F0F0F:0xF0F0F0F0:0x0xFFFFFFFF
0xF0F0F0F0F0:0xF0F0F0F0F0:0x0xF0F0F0F0F0
0x0F0F0F0F0F:0x0F0F0F0F0F:0x0x0F0F0F0F0F
0x0F0F0F0F0F:0xF0F0F0F0F0:0x0xFFFFFFFFFF
0xF0F0F0F0F0F0:0xF0F0F0F0F0F0:0x0xF0F0F0F0F0F0
0x0F0F0F0F0F0F:0x0F0F0F0F0F0F:0x0x0F0F0F0F0F0F
0x0F0F0F0F0F0F:0xF0F0F0F0F0F0:0x0xFFFFFFFFFFFF
0x1F0F0F0F0F0F:0xF0F0F0F0F0F0:0x0xFFFFFFFFFFFF
&bxor
abc:abc:NaN
abc:0:NaN
0:abc:NaN
1:2:3
+8:+2:+10
+281474976710656:+0:+281474976710656
+281474976710656:+1:+281474976710657
+281474976710656:+281474976710656:+0
-2:-3:3
-1:-1:0
-6:-6:0
-7:4:-3
-4:7:-5
4:-7:-3
-4:-7:5
# equal arguments are treated special, so also do some test with unequal ones
0xFFFF:0xFFFF:0
0xFFFFFF:0xFFFFFF:0
0xFFFFFFFF:0xFFFFFFFF:0
0xFFFFFFFFFF:0xFFFFFFFFFF:0
0xFFFFFFFFFFFF:0xFFFFFFFFFFFF:0
0:0xFFFF:0x0xFFFF
0:0xFFFFFF:0x0xFFFFFF
0:0xFFFFFFFF:0x0xFFFFFFFF
0:0xFFFFFFFFFF:0x0xFFFFFFFFFF
0:0xFFFFFFFFFFFF:0x0xFFFFFFFFFFFF
0xFFFF:0:0x0xFFFF
0xFFFFFF:0:0x0xFFFFFF
0xFFFFFFFF:0:0x0xFFFFFFFF
0xFFFFFFFFFF:0:0x0xFFFFFFFFFF
0xFFFFFFFFFFFF:0:0x0xFFFFFFFFFFFF
0xF0F0:0xF0F0:0
0x0F0F:0x0F0F:0
0xF0F0:0x0F0F:0x0xFFFF
0xF0F0F0:0xF0F0F0:0
0x0F0F0F:0x0F0F0F:0
0x0F0F0F:0xF0F0F0:0x0xFFFFFF
0xF0F0F0F0:0xF0F0F0F0:0
0x0F0F0F0F:0x0F0F0F0F:0
0x0F0F0F0F:0xF0F0F0F0:0x0xFFFFFFFF
0xF0F0F0F0F0:0xF0F0F0F0F0:0
0x0F0F0F0F0F:0x0F0F0F0F0F:0
0x0F0F0F0F0F:0xF0F0F0F0F0:0x0xFFFFFFFFFF
0xF0F0F0F0F0F0:0xF0F0F0F0F0F0:0
0x0F0F0F0F0F0F:0x0F0F0F0F0F0F:0
0x0F0F0F0F0F0F:0xF0F0F0F0F0F0:0x0xFFFFFFFFFFFF
&bnot
abc:NaN
+0:-1
+8:-9
+281474976710656:-281474976710657
-1:0
-2:1
-12:11
&digit
0:0:0
12:0:2
12:1:1
123:0:3
123:1:2
123:2:1
123:-1:1
123:-2:2
123:-3:3
123456:0:6
123456:1:5
123456:2:4
123456:3:3
123456:4:2
123456:5:1
123456:-1:1
123456:-2:2
123456:-3:3
100000:-3:0
100000:0:0
100000:1:0
&mantissa
abc:NaN
1e4:1
2e0:2
123:123
-1:-1
-2:-2
+inf:inf
-inf:-inf
&exponent
abc:NaN
1e4:4
2e0:0
123:0
-1:0
-2:0
0:1
+inf:inf
-inf:inf
&parts
abc:NaN,NaN
1e4:1,4
2e0:2,0
123:123,0
-1:-1,0
-2:-2,0
0:0,1
+inf:inf,inf
-inf:-inf,inf
&bpow
abc:12:NaN
12:abc:NaN
0:0:1
0:1:0
0:2:0
0:-1:NaN
0:-2:NaN
1:0:1
1:1:1
1:2:1
1:3:1
1:-1:1
1:-2:1
1:-3:1
2:0:1
2:1:2
2:2:4
2:3:8
3:3:27
2:-1:NaN
-2:-1:NaN
2:-2:NaN
-2:-2:NaN
+inf:1234500012:inf
-inf:1234500012:-inf
+inf:-12345000123:inf
-inf:-12345000123:-inf
# 1 ** -x => 1 / (1 ** x)
-1:0:1
-2:0:1
-1:1:-1
-1:2:1
-1:3:-1
-1:4:1
-1:5:-1
-1:-1:-1
-1:-2:1
-1:-3:-1
-1:-4:1
10:2:100
10:3:1000
10:4:10000
10:5:100000
10:6:1000000
10:7:10000000
10:8:100000000
10:9:1000000000
10:20:100000000000000000000
123456:2:15241383936
&length
100:3
10:2
1:1
0:1
12345:5
10000000000000000:17
-123:3
215960156869840440586892398248:30
&bsqrt
145:12
144:12
143:11
16:4
170:13
169:13
168:12
4:2
3:1
2:1
9:3
12:3
256:16
100000000:10000
4000000000000:2000000
152399026:12345
152399025:12345
152399024:12344
1:1
0:0
-2:NaN
-123:NaN
Nan:NaN
+inf:NaN
&bround
$round_mode('trunc')
0:12:0
NaNbround:12:NaN
+inf:12:inf
-inf:12:-inf
1234:0:1234
1234:2:1200
123456:4:123400
123456:5:123450
123456:6:123456
+10123456789:5:+10123000000
-10123456789:5:-10123000000
+10123456789:9:+10123456700
-10123456789:9:-10123456700
+101234500:6:+101234000
-101234500:6:-101234000
#+101234500:-4:+101234000
#-101234500:-4:-101234000
$round_mode('zero')
+20123456789:5:+20123000000
-20123456789:5:-20123000000
+20123456789:9:+20123456800
-20123456789:9:-20123456800
+201234500:6:+201234000
-201234500:6:-201234000
#+201234500:-4:+201234000
#-201234500:-4:-201234000
+12345000:4:12340000
-12345000:4:-12340000
$round_mode('+inf')
+30123456789:5:+30123000000
-30123456789:5:-30123000000
+30123456789:9:+30123456800
-30123456789:9:-30123456800
+301234500:6:+301235000
-301234500:6:-301234000
#+301234500:-4:+301235000
#-301234500:-4:-301234000
+12345000:4:12350000
-12345000:4:-12340000
$round_mode('-inf')
+40123456789:5:+40123000000
-40123456789:5:-40123000000
+40123456789:9:+40123456800
-40123456789:9:-40123456800
+401234500:6:+401234000
+401234500:6:+401234000
#-401234500:-4:-401235000
#-401234500:-4:-401235000
+12345000:4:12340000
-12345000:4:-12350000
$round_mode('odd')
+50123456789:5:+50123000000
-50123456789:5:-50123000000
+50123456789:9:+50123456800
-50123456789:9:-50123456800
+501234500:6:+501235000
-501234500:6:-501235000
#+501234500:-4:+501235000
#-501234500:-4:-501235000
+12345000:4:12350000
-12345000:4:-12350000
$round_mode('even')
+60123456789:5:+60123000000
-60123456789:5:-60123000000
+60123456789:9:+60123456800
-60123456789:9:-60123456800
+601234500:6:+601234000
-601234500:6:-601234000
#+601234500:-4:+601234000
#-601234500:-4:-601234000
#-601234500:-9:0
#-501234500:-9:0
#-601234500:-8:0
#-501234500:-8:0
+1234567:7:1234567
+1234567:6:1234570
+12345000:4:12340000
-12345000:4:-12340000
&is_zero
0:1
NaNzero:0
+inf:0
-inf:0
123:0
-1:0
1:0
&is_one
0:0
NaNone:0
+inf:0
-inf:0
1:1
2:0
-1:0
-2:0
# floor and ceil tests are pretty pointless in integer space...but play safe
&bfloor
0:0
NaNfloor:NaN
+inf:inf
-inf:-inf
-1:-1
-2:-2
2:2
3:3
abc:NaN
&bceil
NaNceil:NaN
+inf:inf
-inf:-inf
0:0
-1:-1
-2:-2
2:2
3:3
abc:NaN
&as_hex
128:0x80
-128:-0x80
0:0x0
-0:0x0
1:0x1
0x123456789123456789:0x123456789123456789
+inf:inf
-inf:-inf
NaNas_hex:NaN
&as_bin
128:0b10000000
-128:-0b10000000
0:0b0
-0:0b0
1:0b1
0b1010111101010101010110110110110110101:0b1010111101010101010110110110110110101
+inf:inf
-inf:-inf
NaNas_bin:NaN