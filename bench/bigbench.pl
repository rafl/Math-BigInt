#!/usr/bin/perl -w

use strict;
use Time::HiRes qw/gettimeofday tv_interval/;

my $VERSION = 0.01;

$| = 1;
my $digits = shift || 10;
my $class = 'Math::BigInt';
my $timeout = 5;	# not less then 5 for "precise" results
my $factor = 4;		# for O() approximation

use lib '../lib';
#use Math::BigInt calc => 'BitVect';
use Math::BigInt;

my ($done,$alarm,$i);
my (%variables,%variables_str);
$SIG{ALRM} = sub { $alarm = 1; };
my @sum;

print "BigBen $VERSION for Math::BigInt";
print " version=",$Math::BigInt::VERSION," core=";
my $c = Math::BigInt::_core_lib(); $c =~ s/Math::BigInt:://;
print "$c digits=$digits timeout=$timeout o_factor=$factor\n";

while (<DATA>)
  {
  chomp;
  $_ =~ s/#.*$//;
  next if /^\s*$/;		# skip empty (and commented out)
  my @args = split ':';
  my (@time,@done,@per);
  # first simple benchmark
  ($time[0],$done[0],$per[0]) = bench ( $digits, 0, @args );
  print " $digits: "; $per[0] = int(100*$per[0])/100; print "$per[0] ";
  push @sum,[$time[0],$done[0],$per[0]]; 	# add to table for summary
  # now a row, increasing digtis by factor 10
  my $d = $digits*$factor;
  for (my $j = 1; $j < 3; $j ++)
    {
    print "$d: ";
    ($time[$j],$done[$j],$per[$j]) = bench ( $d, 1, @args );
    $per[$j] = int(100*$per[$j])/100;
    print "$per[$j] ";
    $d *= $factor;
    }
  my $O = '1';
  $O = 'N' if (($per[1] < $per[0] * 0.8) && ($per[2] < $per[1] * 0.8));
  $O = 'N*N' if (($per[0] > 0.8*$per[1] * $per[1]) &&
                ($per[1] > 0.8*$per[2]*$per[2]));
  print "=> roughly O($O)\n"; 
  }
summary (\@sum);

print "Done\n";

1;

###############################################################################

sub summary
  {
  my $s = shift;
  my ($cnt,$time,$per,$done);
  foreach (@$s)
    {
    $per += $_->[2];
    $time += $_->[0];
    $done += $_->[1];
    $cnt++;
    }
  $per = $per / $cnt if $cnt > 1;
  $per = int(100*$per)/100;
 
  print "Running time in seconds total: ",int(100*$time)/100,"\n";
  print "Operations total             : $done\n";
  print "Average operations per second: $per";
  print " (",int(100*$done/$time)/100,")\n" if $cnt > 1;
  }

sub bench
  { 
  my $digits = shift || 10;
  my $silence = shift || 0;
  my @args = @_;
  my $des = shift @args;
  my $code = shift @args;
  $des = "$des:"; while (length($des) < 45) { $des .= ' '; }
  substr($des,-length($code),length($code)) = $code; 
  my $name = 'c';
  my $pre = "";
  foreach (@args)
    {
    $pre .= "\$variables{$name} = \$class->new($_); ";
    $pre .= "\$variables_str{$name} = \"$_\"; "; $name++;
    }
  my $count = 5;	# start with this
  my $time = 0; my $time2 = 0;
  while ($time < $timeout)
    {
    print "\r$des $count   " if !$silence;
    eval ($pre);
    $code =~ s/\$([a-z])_str/\$variables_str\{$1\}/g;
    $code =~ s/\$([a-z])([^a-z])/\$variables\{$1\}$2/g;
    my $start = gettimeofday;
    alarm ($timeout*1.2); $alarm = 0;
    #print "\nfor (\$i = 0; \$i < \$count; \$i++) { $code; last if \$alarm; }";
    eval "for (\$i = 0; \$i < \$count; \$i++) { $code; last if \$alarm; }";
    die "Error in code to be benchmarked: $@\n" if $@;
    $done = $i; $done = 1 if $i < 1;
    #print "\neval: $pre & $code\n";
    $time = gettimeofday - $start;
    $start = gettimeofday; $alarm = 0;
    # time empty loop
    eval 'for ($i = 0; $i < $done; $i++) { last if $alarm }';
    $time2 = (gettimeofday - $start); $time = $time - $time2 if $time2 < $time;
    # print "    (",$time," ",$time2,")";
    last if $time > $timeout; 
    my $fac = 1000;					# for time = 0
    $fac = $timeout / $time if $time != 0;
    #print "$fac $done $count\n";
    $count = int($done*$fac*1.05);			# 10% more for sure
    } 
  my $per = "n/a";
  $per = int(1000*$done / $time)/1000 if $time != 0;
  print "\r$des $done ",int(1000*$time)/1000," $per/s\n" if !$silence;
  return ($time,$done,$per,$time2,$des,$pre,$code);
  }

1;

# format:
# code to test:param1:param2:param2..
__END__
mul big number by small:$e = $c * $d;:'1'.('0' x $digits):1234567890
mul big number by big:$e = $c * $d;:'1'.('0' x $digits):'123'.('0' x $digits)
power of 2:$e = $c ** $d;:2:'1'.('0' x ($digits/2))
power of 10:$e = $c ** $d;:10:'1'.('0' x ($digits/2))
small power of small number:$e = $c ** $d;:13:9
big power of small number:$e = $c ** $d;:13:123
small power of big number:$e = $c ** $d;:'1'.('0' x $digits):4
new:$d = $class->new($c_str);:'1'.('0' x $digits)
to decimal string:$d = $c->bstr();:'1'.('0' x $digits)
str and new:$d = $class->new($c->bstr());:'1'.('0' x $digits)
length:$d = $c->length();:'1'.('0' x $digits)
absolute big number:$c = abs($c);:'1'.('0' x $digits)
absolute big number:$c->babs();:'1'.('0' x $digits)
negate big number:$c = -$c;:'1'.('0' x $digits)
negate big number:$c->bneg();:'1'.('0' x $digits)
add small number to big:$c = $c + $d;:'1'.('0' x $digits):123
add small number to big:$c += $d;:'1'.('0' x $digits):123
add big number to big:$c += $d;:'1'.('0' x $digits):'1'.('0' x $digits)
add big number to big:$c = $c + $d;:'1'.('0' x $digits):'1'.('0' x $digits)
sub big number from big:$c = $c - $d;:'1'.('0' x $digits):'1'.('0' x $digits)
sub big number from big:$c -= $d;:'1'.('0' x $digits):'1'.('0' x $digits)
sub small number from big:$c = $c - $d;:'1'.('0' x $digits):123
sub small number from big:$c -= $d;:'1'.('0' x $digits):123
div big number by small:$e = $c / $d;:'1'.('0' x $digits):59)
div big number by small:$c /= $d;:'1'.('0' x $digits):59)
