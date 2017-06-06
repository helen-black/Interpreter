#pragma once
#include "lexical_analyzer.h"

struct parse_tree
{
	token lexema;
	string node_name;
	bool is_term;
	vector<parse_tree *> children;
};

static int pos_in_lex_table = 0;

parse_tree *add_node(parse_tree *tree, token lexema, string node_name, bool is_term);
void print_tree(parse_tree *tree, string str);
void delete_tree(parse_tree *tree);

void parser(string file_name);
void signal_program();
void program(parse_tree *tree);
void block(parse_tree *tree);
void statements_list(parse_tree *tree);
void statement(parse_tree *tree);
void alternatives_list(parse_tree *tree);
void alternative(parse_tree *tree);
void expression(parse_tree *tree);
void multipliers_list(parse_tree *tree);
void multiplication_instruction(parse_tree *tree);
void multiplier(parse_tree *tree);
void variable_identifier(parse_tree *tree);
void procedure_identifier(parse_tree *tree);
void ident(parse_tree *tree);
void line(parse_tree *tree);
void unsigned_integer(parse_tree *tree);
void digits_string(parse_tree *tree);
void digit(parse_tree *tree);
void letter(parse_tree *tree);

void print_err();
token make_zero(token zero_token);