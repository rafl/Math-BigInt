#!/usr/bin/perl -w

use Test::More;
use strict;

my $tests;

BEGIN
   {
   $tests = 2;
   plan tests => $tests;
   chdir 't' if -d 't';
   use lib '../lib';
   };

SKIP:
  {
  skip("Test::Pod::Coverage 1.00 required for testing POD coverage", $tests)
    unless do {
    eval "use Test::Pod::Coverage 1.00";
    $@ ? 0 : 1;
    };

  my $trustme = { 
    trustme => [ 'fround', 'objectify' ], 
    };
  pod_coverage_ok( 'Math::BigInt', $trustme, "All our Math::BigInts are covered" );

  $trustme = { 
    trustme => [ 'DEBUG', 'isa' ], 
    coverage_class => 'Pod::Coverage::CountParents',
    };
  pod_coverage_ok( 'Math::BigFloat', $trustme, "All our Math::BigFloats are covered" );

  }

