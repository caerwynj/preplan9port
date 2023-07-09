#!/usr/bin/perl -w

use strict;

package TestTool;
use vars qw(@ISA @EXPORT @EXPORT_OK %EXPORT_TAGS $VERSION);
use Exporter ();
@ISA = qw(Exporter);

@EXPORT = qw(tlogin tlogoff savepoint rollback sql sqllst del ini dtr etr dcnst ecnst colnames);
@EXPORT_OK = qw();

use File::Basename;
use DBI;

# Default values for options
my ( $trace, $inst, $cache, $delim, $headers, $null_str ) =
   ( 0, '', '', ",", 1, 'NULL' );

my $dbh;

sub tlogin {
	my ($user, $pass) = @_;

	if (defined($dbh)) {
		return;
	}
	$ENV{USER} = $user;
	$ENV{DBPASSWD} = $pass;
	DBI->trace( $trace );

	if ($ENV{DBMS} eq "oracle") {
		if (! $inst) {
			$inst = $ENV{TWO_TASK} || $ENV{ORACLE_SID} || '';
		}
		$dbh = DBI->connect( "dbi:Oracle:$inst", $user, $pass,
			{ AutoCommit => 1, RaiseError => 0, PrintError => 1 } )
			or die $DBI::errstr;
		$dbh->do("alter session set NLS_DATE_FORMAT='Mon fmDD YYYY HH:MIAM'");
	} else {
		if (! $inst) {
			 $inst = $ENV{DS_DATABASE} || '';
		}
		$dbh = DBI->connect( "dbi:Sybase:database=$inst", $user, $pass,
			{ AutoCommit => 1, RaiseError => 0, PrintError => 1 } )
			or die $DBI::errstr;
	}

	$dbh->{RowCacheSize} = $cache if $cache;
	$dbh->{ChopBlanks} = 1;

}

sub tlogoff {
	$dbh->disconnect;
	$dbh = undef;
}

sub sqllst {
	my @sql = @_;

	exec_sqllst(\@sql, 0, 0, 0);
}

sub exec_sqllst {
	my ($stsref, $issp, $spname, $tablesref) = @_;
	my $l = "";
	my $stmt = "";

#	print "exec_sqllst: @$stsref";
	foreach $l (@$stsref) {
		if ($l eq "/\n") {
			my $table = 0;
			if ($tablesref) {
				$table = shift(@$tablesref);
			}
			exec_sql($stmt, $issp, $spname, $table);
			$stmt = "";
		} else {
			$stmt = join "\n", ($stmt, $l);
		}
	}
	if ($stmt) {
		print STDERR "stmt not empty: $stmt\n";
	}
}

sub sql {
	exec_sql($_[0], 0, 0, 0);
}

sub exec_sql {
	my ($stmt, $issp, $spname, $table) = @_;

	#print "prepare $stmt";
	my $sth = $dbh->prepare($stmt);
	$sth->execute;
	if ($sth->state) {
		print STDERR "state: ",  $sth->state;
		return $sth->state;
	}
	my $nfields = $sth->{NUM_OF_FIELDS};
	
	if ($nfields) {
		# the statement has output columns
		my ( @col, $col );
		my $row;
		my @name = @{$sth->{NAME_lc}};
		my @types = @{$sth->{TYPE}};
		$row = join ",",  map { s/\s+$//; $_ } @name;
		if ($issp == 1) {
			open DUMP, ">" . $table . ".$spname";
			print DUMP $row . "\n"; 
		} else { 
			print $row . "\n"; 
		}	
		# Associate @col with output columns and fetch the rows
		$sth->bind_columns( {}, \( @col[0 .. $#name] ));
		while ($sth->fetch) {
			for (my $i = 0; $i <= $#name; $i++) {
				if (defined($col[$i])) {
					$col[$i] = $dbh->quote($col[$i], $types[$i]);
					# col might contain newlines, but we 
					# can not allow that in CSV file; 
					# for now we will just delete
					$col[$i] =~ s/\n/ /gs
				} else { 
					$col[$i] = $null_str;
				}
			}
			$row = join ",", @col;
			if ($issp) {
				print DUMP $row . "\n";
			} else {
				print $row . "\n";
			}
		}
		if ($issp) {
			close DUMP;
		}
	}
	$sth->finish;
	0;
}

sub savepoint {
	my ($spname, @tables) = @_;
	my @sql = ();
	
	if ($tables[0] eq "all") {
		@tables = $dbh->tables;
	}
	foreach my $t (@tables) {
		push @sql, ("select * from $t\n");
		push @sql, ("/\n");
	}
	exec_sqllst(\@sql, 1, $spname, \@tables);
}

sub rollback {
	my $sp = $_[0];
	my @sql = ();
	my @files = ();
	
	@files = glob("*.$sp");

	foreach my $f (@files) {
		my ($name, $path, $suffix) = fileparse($f, ("." . $sp));
		del($name);
		ini($name, $f);
	}
}

sub ini {
	my ($table, $file) = @_;
	my @sql = ();

	dtr($table);
	open INI, "<" . $file  or warn "Can't open $file $!\n";
	my $header = <INI>;
	chomp $header;
	while (<INI>) {
		push @sql, ("insert into $table ($header) values ($_)\n");
		push @sql, ("/\n");
	}
	close INI;
	exec_sqllst(\@sql, 0,0,0);
	etr($table);
}

sub del {
	my (@tables) = @_;
	my @sql = ();

	dtr(@tables);
	foreach my $t (@tables) {
		if ($ENV{DBMS} eq "oracle") {
			push @sql, ("truncate table $t\n");
		} else {
			push @sql, ("delete from $t\n");
		}
		push @sql, ("/\n");
	}
	exec_sqllst(\@sql, 0, 0, 0);
	etr(@tables);
}

sub dtr {
	my (@tables) = @_;
	my @sql = ();

	if ($ENV{DBMS} eq "oracle") {
		foreach my $t (@tables) {
			push @sql, ("alter table $t disable all triggers\n");
			push @sql, ("/\n");
		}
	}
	exec_sqllst(\@sql, 0, 0, 0);
}

sub etr {
	my (@tables) = @_;
	my @sql = ();

	if ($ENV{DBMS} eq "oracle") {
		foreach my $t (@tables) {
			push @sql, ("alter table $t enable all triggers\n");
			push @sql, ("/\n");
		}
	}
	exec_sqllst(\@sql,0,0,0);
}

sub dcnst {
	if ($ENV{DBMS} eq "sybase") {
		return;
	}
	my @sql = ();
	my $stmt = q/select constraint_name, table_name 
				 from user_constraints
				 where constraint_type in ('R', 'P')/;
	my $sth = $dbh->prepare($stmt);
	$sth->execute;
	if ($sth->state) {
		print STDERR "state: ",  $sth->state;
		return $sth->state;
	}
	my $nfields = $sth->{NUM_OF_FIELDS};
	
	if ($nfields) {
		# the statement has output columns
		my ( @col, $col );
		my @name = @{$sth->{NAME_lc}};
		my @types = @{$sth->{TYPE}};

		# Associate @col with output columns and fetch the rows
		$sth->bind_columns( {}, \( @col[0 .. $#name] ));
		while ($sth->fetch) {
			push @sql, "alter table $col[1] disable constraint $col[0] cascade\n";
			push @sql, "/\n";	
		}
	}
	$sth->finish;
	exec_sqllst(\@sql,0,0,0);
}


sub ecnst {
	if ($ENV{DBMS} eq "sybase") {
		return;
	}
	my @sql = ();
	my $stmt = q/select constraint_name, table_name 
				 from user_constraints
				 where constraint_type in ('R', 'P')/;
	my $sth = $dbh->prepare($stmt);
	$sth->execute;
	if ($sth->state) {
		print STDERR "state: ",  $sth->state;
		return $sth->state;
	}
	my $nfields = $sth->{NUM_OF_FIELDS};
	
	if ($nfields) {
		# the statement has output columns
		my ( @col, $col );
		my @name = @{$sth->{NAME_lc}};
		my @types = @{$sth->{TYPE}};

		# Associate @col with output columns and fetch the rows
		$sth->bind_columns( {}, \( @col[0 .. $#name] ));
		while ($sth->fetch) {
			push @sql, "alter table $col[1] enable constraint $col[0] \n";
			push @sql, "/\n";	
		}
	}
	$sth->finish;
	exec_sqllst(\@sql,0,0,0);
}

sub colnames {
    my $table   = shift;
    my @columns = ();
    my @row = ();
    my $sql = "";
    my $sth = 0;

    if( $ENV{DBMS} eq "oracle")
    {
	$sql = "select column_name from user_tab_columns
	where table_name = '$table'";
    }
    elsif( $ENV{DBMS} eq "sybase")
    {
	$sql = "select syscolumns.name from sysobjects, syscolumns
                where  sysobjects.name = '$table' and
                       sysobjects.id = syscolumns.id";
    }

    $sth = $dbh->prepare( $sql);
    $sth->execute;
    if ($sth->state) {
	print STDERR "state: ",  $sth->state;
	return $sth->state;
    }

    while( @row = $sth->fetchrow_array)
    {
	 push @columns, $row[0];
    }

    $sth->finish;
    return @columns;
}

1;
