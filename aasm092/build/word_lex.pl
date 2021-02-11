#!/usr/bin/perl

#    This file is part of AASM.
#
#    AASM is free software; you can redistribute it and/or modify
#    it under the terms of the GNU General Public License as published by
#    the Free Software Foundation; either version 3 of the License, or
#    (at your option) any later version.
#
#    AASM is distributed in the hope that it will be useful,
#    but WITHOUT ANY WARRANTY; without even the implied warranty of
#    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#    GNU General Public License for more details.
#
#    You should have received a copy of the GNU General Public License
#    along with AASM; if not, write to the Free Software
#    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
#
#    Copyright Alexandre Becoulet, 2003
#    contact : alexandre.becoulet@free.fr

use strict;

my $line_num = 0;

##################################################
# Word tree functions

sub tree_new_node
{
    my $node =
    {
	"link" => {},
	"code" => undef,
    };

    return $node;
}

sub tree_word_add
{
    (my $tree, my $word) = @_;

    my @chars = split (//, $word);
    my $node = $tree;

    foreach my $char (@chars)
    {
	${$$node{link}}{$char} = tree_new_node ()
	    if (! defined ${$$node{link}}{$char});

	$node = ${$$node{link}}{$char}
    }

    die "line $line_num: word already defined\n"
	if (defined $$node{code});

    $$node{code} = [];

    return $node;
}

##################################################
# Command functions

sub cmd_space
{
    (my $str, my $state_p) = @_;

    my @args = split(/[\n\t ]+/, $str);

    die "line $line_num: at least 1 argument expected for %space\n"
	if ($#args < 1);

    $$state_p{sep} = [@args[1..$#args], "\\0"]; 
}

sub cmd_global
{
    (my $str, my $state_p) = @_;

    $$state_p{word_tree} = undef;

    push (@{$$state_p{blocks}}, $$state_p{block_cur} = []);
}

sub cmd_select
{
    (my $str, my $state_p) = @_;

    my @args = split(/[\n\t ]+/, $str);

    die "line $line_num: 1 argument expected for %select\n"
	if ($#args != 1);

    my $node = tree_new_node ();
    $$node{var} = @args[1];
    $$node{nomatch} = [];
    $$state_p{word_tree} = $node; 
    push (@{$$state_p{blocks}}, $node);
}

sub cmd_nomatch
{
    (my $str, my $state_p) = @_;

    die "line $line_num: no %select found before %nomatch\n"
	if (! defined $$state_p{word_tree});

    $$state_p{block_cur} = ${$$state_p{word_tree}}{nomatch};
}

sub cmd_skipspace
{
    (my $str, my $state_p) = @_;

    die "line $line_num: no %select found before %skipspace\n"
	if (! defined $$state_p{word_tree});

    my $var = ${$$state_p{word_tree}}{var};
    my $sep = $$state_p{sep};
    push (@{$$state_p{block_cur}}, $var." += strspn(".$var.", \"".join("", @$sep)."\");");
}

sub cmd_word
{
    (my $str, my $state_p) = @_;

    my @args = split(/[\n\t ]+/, $str);

    die "line $line_num: 1 argument expected for %word\n"
	if ($#args != 1);

    die "line $line_num: no %select found before %word\n"
	if (! defined $$state_p{word_tree});

    my $node = tree_word_add ($$state_p{word_tree}, @args[1]);

    $$state_p{block_cur} = $$node{code};
}

my %wlex_cmd =
(
 "global" => \&cmd_global,
 "nomatch" => \&cmd_nomatch,
 "select" => \&cmd_select,
 "skipspace" => \&cmd_skipspace,
 "space" => \&cmd_space,
 "word" => \&cmd_word,
);

##################################################
# File parse functions

sub wlex_parse
{
    (local *IN_FILE, my $state_p) = @_;

    push (@{$$state_p{blocks}}, $$state_p{block_cur} = []);

    foreach my $line (<IN_FILE>)
    {
	$line_num++;

	next if ($line =~ /^[ \t]*$/);

	if ($line =~ /^%(\S*)/)
	{
	    my $cmd_name = $1;

	    if (my $func_ptr = $wlex_cmd{$cmd_name})
	    {
		$func_ptr -> ($line, $state_p);
	    }
	    else
	    {
		die "line $line_num: unknow command `$cmd_name'\n";
	    }
	}
	else
	{
	    push (@{$$state_p{block_cur}}, $line);
	}
    }
}

##################################################
# File write function

sub wlex_write_code
{
    (local *OUT_FILE, my $code_p, my $depth) = @_;

    foreach my $line (@$code_p)
    {
	print OUT_FILE "  " x $depth . $line;
    }
}

sub wlex_write_node
{
    (local *OUT_FILE, my $state_p, my $node, my $depth, my $root) = @_;

    my @list = keys(%{$$node{link}});
    my $var = $$root{var};

    if ($$node{code})
    {
	print OUT_FILE "  " x $depth . "switch (".$var."[".$depth."])\n";
	print OUT_FILE "  " x $depth . "{\n";
	print OUT_FILE "  " x $depth . " case ('".$_."'):\n" foreach (@{$$state_p{sep}});
	print OUT_FILE "  " x $depth . "  ".$var." += ".$depth.";\n";
	print OUT_FILE "  " x $depth . "  {\n";
	wlex_write_code (*OUT_FILE, $$node{code}, $depth + 1);
	print OUT_FILE "  " x $depth . "    goto ".$var."_lex_end;\n";
	print OUT_FILE "  " x $depth . "  }\n";
    }
    else
    {
	if (@list eq 1)
	{
	    my $str = "";
	    my $count = 0;
	    my $child = $node;

	    do
	    {
		my $char = @list[0];
		$count++;

		$str .= $char;

		$child = ${$$child{link}}{$char};
		@list = keys(%{$$child{link}});
	    } while ((@list eq 1) && !$$child{code});

	    if ($count > 1)
	    {
		print OUT_FILE "  " x $depth . " if (!strncmp(\"".$str."\", "
		    .$var." + ".$depth.", ".$count."))\n";
	    }
	    else
	    {
		print OUT_FILE "  " x $depth . " if (".$var."[".$depth."] == '".$str."')\n";
	    }
	    print OUT_FILE "  " x $depth . " {\n";
	    wlex_write_node (*OUT_FILE, $state_p, $child, $depth + $count, $root);
	    print OUT_FILE "  " x $depth . " }\n";
	    goto wlex_write_node_end;
	}
	print OUT_FILE "  " x $depth . "switch (".$var."[".$depth."])\n";
	print OUT_FILE "  " x $depth . "{\n";
    }

    foreach my $char (@list)
    {
	my $child = ${$$node{link}}{$char};

	print OUT_FILE "  " x $depth . " case ('$char'):\n";
	    
	wlex_write_node (*OUT_FILE, $state_p, $child, $depth + 1, $root);
	print OUT_FILE "  " x $depth . "  break;\n\n";
    }

    print OUT_FILE "  " x $depth . "}\n";

  wlex_write_node_end:

    if (! $depth)
    {
	wlex_write_code (*OUT_FILE, $$node{nomatch}, 0);
	print OUT_FILE $var."_lex_end:\n";
    }
}

sub wlex_write
{
    (local *OUT_FILE, my $state_p) = @_;

    foreach my $block (@{$$state_p{blocks}})
    {
	if (ref $block eq "ARRAY")
	{
	    wlex_write_code (*OUT_FILE, $block);
	}
	else
	{
	    print OUT_FILE "\n  /** WLEX begin **/\n\n";
	    wlex_write_node (*OUT_FILE, $state_p, $block, 0, $block);
	    print OUT_FILE "\n  /** WLEX end **/\n\n";
	}
    }
}

##################################################
# main

sub main
{
    die
	"Simple C word lexer generator\n",
	"Copyright Alexandre Becoulet 2003\n\n",
	"usage: word_lex.pl file.wlex\n"
	if (! @ARGV);

    my $in_fname = @ARGV[0];

    my $out_fname = $in_fname;
    $out_fname .= ".c" if (! ($out_fname =~ s/.wlex/.c/));

    local *IN_FILE, *OUT_FILE;

    die "can't open `$in_fname' for reading\n"
	if (! open (IN_FILE, "<$in_fname"));

    die "can't open `$out_fname' for writting\n"
	if (! open (OUT_FILE, ">$out_fname"));

    my %state = 
	(
	 "blocks" => [],
	 "sep" => ["\\0"]
	 );

    print "reading $in_fname\n";
    wlex_parse (*IN_FILE, \%state);

    print "writing $out_fname\n";
    wlex_write (*OUT_FILE, \%state);

    close (IN_FILE);
    close (OUT_FILE);
}

main;
exit 0;

