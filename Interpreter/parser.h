#pragma once
#include "lexical_analyzer.h"

struct parse_tree
{
	token lexema;
	vector<parse_tree *> chirdren;
};

parse_tree *add_node(parse_tree *tree, token lexema);
void print_tree(parse_tree *tree, string str);
void delete_tree(parse_tree *tree);
