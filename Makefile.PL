
# We need at least Perl 5.6.2
require 5.6.2;
use strict;

# Load the Module::Install bundled in ./inc/
use inc::Module::Install;

# The name of your distribution
name		'Math-BigInt';

# Get most of the details from the primary module
all_from	'lib/Math/BigInt.pm';

requires	'perl'		=> 5.006002;

# see if old versions are installed, and if so, force
# and upgrade:

my @checks;

check_minimum_version ('Math::BigInt::FastCalc' => 0.12);
check_minimum_version ('Math::BigInt::GMP'	=> 1.19);
check_minimum_version ('Math::BigRat'		=> 0.17);
check_minimum_version ('bignum' 		=> 0.19);

if (@checks > 0)
  {
  print ("\n" . '*' x 70 . "\n Attention: After installing this package, you also need to update:\n\n");
  for my $need (@checks)
    {
    print "  $need->[0]  to at least  v$need->[1]!\n";
    }
  print "\n";
  sleep(5);
  };

recommends	'Math::BigRat'		=> 0.17;
recommends	'Math::BigInt::GMP'	=> 1.19;
recommends	'bignum'		=> 0.19;

build_requires	'Test::More'	=> 0.42;

install_as_core();

license		'perl';

author		'Tels <nospam-abuse@bloodgate.com>';

# Do not index these
no_index	directory	=> 'examples';

# Generate the Makefile
WriteAll;

1;

sub check_minimum_version
  {
  my ($module, $version) = @_;

  my $v; eval " require $module; \$v = \$${module}::VERSION;";

  # if the module is installed, but outdated, add it to the requirements
  if (defined $v && $v < $version)
    {
    push @checks, [ $module, $version];
    } 
  }

