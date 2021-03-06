#!/usr/bin/perl

use strict;

my @struct_field_names = ("name", "desc", "id", "i_size", "i_type", "i_flag", "fix", "instr_asm");

my %struct_default_values = 
(
 "name" => "\"\"",
 "desc" => "\"no description\"",
 "id" => "0",
 "i_size" => "{ 0 }",
 "i_type" => "{ 0 }",
 "i_flag" => "0",
 "fix" => "{ 0 }",
 "instr_asm" => "NULL"
);

my $copyright_comment =
	"/*\n".
	"** This file is genrated by gen_opcodes.pl script\n".
	"** This file is part of the GPL AASM project\n".
	"** Do not edit\n".
	"*/\n\n";

sub parse_lists
{
    my ($instrs_p, $file_list_p) = @_;

    foreach my $file (@$file_list_p)
    {
	my $name = "";
	my $info = "";

	print "reading $file ... ";

	open (LIST, "<$file") || die "unable to open $file\n";

	my $line_num = 0;

	foreach my $line (<LIST>)
	{
	    $line_num++;

	    # skip empty lines and comment lines
	    next if ($line =~ /^[ \t]*(\#.*)?$/);

	    # remove leading spaces
	    $line =~ s/^[ \t]*//;
	    $line =~ s/[ \t]*$//;

	    my @token = split(/[\t\n]+/, $line);

	    SWITCH: for (@token[0])
	    {
		/^name$/ && do
		{
		    die "$file:$line_num: empty instruction name" 
			if (!($name = @token[1]));

		    $$instrs_p{$name} = [] if (! defined $$instrs_p{$name});

		    last SWITCH;
		};

		/^opcode$/ && do
		{
		    die "$file:$line_num: no instruction name before `@token[0]'\n"
			if (! defined $$instrs_p{$name});

		    push (@{$$instrs_p{$name}}, $info = {%struct_default_values});

		    $$info{"name"} = "\"$name\"";
		    $$info{"id"} = "OPC_" . uc $name . "_" . (@{$$instrs_p{$name}} - 1);

		    last SWITCH;
		};

		die "$file:$line_num: no opcode started before `@token[0]'\n"
		    if (!$info);

		$$info{@token[0]} = @token[1];
	    }
	}

	close (LIST);
	print "done\n";
    }
}

sub write_opcodes_h
{
    my ($instrs_p, $filename_p) = @_;

    open (OUT_FILE, ">$filename_p") || die "unable to open `$filename_p' for writing\n";

    print "writing $filename_p ... ";

    print OUT_FILE $copyright_comment;

    print OUT_FILE "#ifndef OPCODES_H_\n";
    print OUT_FILE "# define OPCODES_H_\n\n";

    foreach my $name (keys(%$instrs_p))
    {
	my $opcodes = $$instrs_p{$name};

	print OUT_FILE "static const struct instr_info_s	opcodes_" . $name . "[" . (@$opcodes + 1). "] =\n";
	print OUT_FILE "{\n";

	foreach my $opcode (@$opcodes, \%struct_default_values)
	{
	    print OUT_FILE "  {\n";

	    foreach my $fields (@struct_field_names)
	    {
		print OUT_FILE "    $$opcode{$fields},\n"
	    }
	    print OUT_FILE "  },\n";
	}

	print OUT_FILE "};\n\n";
    }

    print OUT_FILE "\n#endif\n";

    close (OUT_FILE);
    print "done\n";
}

sub write_opcodes_id_h
{
    my ($instrs_p, $filename_p) = @_;

    open (OUT_FILE, ">$filename_p") || die "unable to open `$filename_p' for writing\n";

    print "writing $filename_p ... ";

    print OUT_FILE $copyright_comment;

    print OUT_FILE "#ifndef OPCODES_ID_H_\n";
    print OUT_FILE "# define OPCODES_ID_H_\n\n";

    my $index = 1;

    foreach my $name (keys(%$instrs_p))
    {
	my $opcodes = $$instrs_p{$name};

	foreach my $opcode (@$opcodes)
	{
	    print OUT_FILE "#define " . $$opcode{"id"} . "		$index\n";
	    $index++;
	}
    }

    print OUT_FILE "\n#endif\n";

    close (OUT_FILE);
    print "done\n";
}

sub write_instr_perf
{
    my ($instrs_p, $filename_p, $filename_skel_p) = @_;

    open (OUT_FILE, ">$filename_p") || die "unable to open `$filename_p' for writing\n";

    open (SKEL, "<$filename_skel_p") || die "unable to open `$filename_skel_p'\n";
    print "reading $filename_skel_p ... ";
    print OUT_FILE $_ foreach (<SKEL>);
    close (SKEL);
    print "done\n";

    print "writing $filename_p ... ";

    print OUT_FILE "%%\n";
    foreach my $name (keys(%$instrs_p))
    {
	my $opcodes = $$instrs_p{$name};

	print OUT_FILE "$name, opcodes_$name\n";
    }
    print OUT_FILE "%%\n";

    close (OUT_FILE);
    print "done\n";
}

sub main
{
    my %instrs;

    die "usage: gen_opcodes.pl [file.list ...]\n"
	if (! @ARGV);

    parse_lists (\%instrs, \@ARGV);

    write_opcodes_h (\%instrs, "opcodes.h");
    write_opcodes_id_h (\%instrs, "opcodes_id.h");
    write_instr_perf (\%instrs, "instr.perf", "instr.skel");
}

main;
exit 0;


