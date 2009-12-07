use ExtUtils::MakeMaker;

sub MY::metafile {
  package MY;
  my $self = shift;
  if (!eval { require YAML; 1 })
  {
    warn ("YAML not installed, will not override META.yml");
    return $self->SUPER::metafile_target(@_);
  };

  require YAML::Node;
  my $node = new YAML::Node {};

  $node->{name}			= $self->{DISTNAME};
  $node->{version}		= $self->{VERSION};
  $node->{author}		= 'Tels',
  $node->{abstract}		= 'Arbitrary size integer/float math package';
  $node->{version_from}		= $self->{VERSION_FROM};
  $node->{license}		= 'perl';
  $node->{distribution_type}	= 'module';
  $node->{generated_by}		= "$self->{DISTNAME} version $self->{VERSION}";
  $node->{installdirs}		= 'perl';
  $node->{requires}		= $self->{PREREQ_PM};

  my $dump = YAML::Dump( $node );

  $dump =~ s/^(.*)$/\t\$(NOECHO) \$(ECHO) "$1" >>META.yml/gm;
  $dump =~ s/>>META\.yml/>META.yml/;

  "metafile:\n$dump";
  }

WriteMakefile(
    'NAME'		=> 'Math-BigInt',
    'VERSION_FROM'	=> 'lib/Math/BigInt.pm',
    'PREREQ_PM'		=> { },
    'INSTALLDIRS'	=> 'perl',
);
