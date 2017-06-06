#include "parser.h"

parse_tree *add_node(parse_tree *tree, token lexema, string node_name, bool is_term)
{
	if (tree == NULL)
	{
		tree = new parse_tree;
		tree->lexema = lexema;
		tree->node_name = node_name;
		tree->is_term = is_term;
	}
	else
	{
		parse_tree *node = new parse_tree;
		node->lexema = lexema;
		node->node_name = node_name;
		node->is_term = is_term;
		tree->children.push_back(node);
	}

}

void print_tree(parse_tree *tree, string str)
{
	if (tree != NULL)
	{
		cout << str << "terminal" << endl;
		for (int i = 0; i < tree->children.size(); i++)
			print_tree(tree->children.at(i), str + "/t");
	}
}

void delete_tree(parse_tree *tree)
{
	if (tree != NULL)
	{
		for (int i = 0; i < tree->children.size(); i++)
			delete_tree(tree->children.at(i));
		delete tree;
	}
}

token make_zero(token zero_token)
{
	zero_token.column = 0;
	zero_token.index = 0;
	zero_token.row = 0;
	return zero_token;
}

void signal_program()
{
	parse_tree tree;
	token signal_program = make_zero(signal_program);

	add_node(&tree, signal_program, "<signal program>", false);
	program(&tree);
}

void program(parse_tree *tree)//щось не так з дітьми...подумати
{
	token program = make_zero(program);
	add_node(tree, program, "<program>", false);

	if (lexical_table.at(pos_in_lex_table).index != key_word["PROGRAM"])
		print_err();
	else
	{
		add_node(tree->children[0], lexical_table.at(pos_in_lex_table), "PROGRAM", true);
		++pos_in_lex_table;
		procedure_identifier(tree);

		if (lexical_table.at(pos_in_lex_table).index != ascii[';'])
			print_err();
		else
		{
			add_node(tree, lexical_table.at(pos_in_lex_table), ";", true);
			++pos_in_lex_table;
			block(tree);

			if (lexical_table.at(pos_in_lex_table).index != ascii['.'])
				print_err();
			else
			{
				add_node(tree, lexical_table.at(pos_in_lex_table), ".", true);
				++pos_in_lex_table;
			}
		}
	}
}

void block(parse_tree *tree)
{
	token block = make_zero(block);
	add_node(tree, block, "<block>", false);

	if (lexical_table.at(pos_in_lex_table).index != key_word["BEGIN"])
		print_err();
	else
	{
		add_node(tree, lexical_table.at(pos_in_lex_table), "BEGIN", true);
		++pos_in_lex_table;
		statements_list(tree);

		if (lexical_table.at(pos_in_lex_table).index != key_word["END"])
			print_err();
		else
		{
			add_node(tree, lexical_table.at(pos_in_lex_table), "END", true);
			++pos_in_lex_table;
		}
	}
}

void statements_list(parse_tree *tree)
{
	token statements_list = make_zero(statements_list);
	add_node(tree, statements_list, "<statements list>", false);
}

void statement(parse_tree *tree)
{
	token statement = make_zero(statement);
	add_node(tree, statement, "<statement>", false);

	if (lexical_table.at(pos_in_lex_table).index == key_word["LOOP"])
	{
		add_node(tree, lexical_table.at(pos_in_lex_table), "LOOP", true);
		++pos_in_lex_table;
		statements_list(tree);

		if (lexical_table.at(pos_in_lex_table).index != key_word["ENDLOOP"])
			print_err();
		else
		{
			add_node(tree, lexical_table.at(pos_in_lex_table), "ENDLOOP", true);
			++pos_in_lex_table;

			if (lexical_table.at(pos_in_lex_table).index != ascii[';'])
				print_err();
			else
			{
				add_node(tree, lexical_table.at(pos_in_lex_table), ";", true);
				++pos_in_lex_table;
			}
		}
	}

	else if (lexical_table.at(pos_in_lex_table).index == key_word["CASE"])
	{
		add_node(tree, lexical_table.at(pos_in_lex_table), "CASE", true);
		++pos_in_lex_table;
		expression(tree);

		if (lexical_table.at(pos_in_lex_table).index != key_word["OF"])
			print_err();
		else
		{
			add_node(tree, lexical_table.at(pos_in_lex_table), "OF", true);
			++pos_in_lex_table;
			alternatives_list(tree);


			if (lexical_table.at(pos_in_lex_table).index != key_word["ENDCASE"])
				print_err();
			else
			{
				add_node(tree, lexical_table.at(pos_in_lex_table), "ENDCASE", true);
				++pos_in_lex_table;

				if (lexical_table.at(pos_in_lex_table).index != ascii[';'])
					print_err();
				else
				{
					add_node(tree, lexical_table.at(pos_in_lex_table), ";", true);
					++pos_in_lex_table;
				}
			}
		}
	}
	else
		print_err();	
}

void alternatives_list(parse_tree *tree)
{
	token alternative_list = make_zero(alternative_list);
	add_node(tree, alternative_list, "<alternatives list>", false);


}

void alternative(parse_tree *tree)
{
	token alternative = make_zero(alternative);
	add_node(tree, alternative, "<alternative>", false);

	expression(tree);
	if (lexical_table.at(pos_in_lex_table).index != ascii[':'])
		print_err();
	else
	{
		add_node(tree, lexical_table.at(pos_in_lex_table), ":", true);
		++pos_in_lex_table;

		if (lexical_table.at(pos_in_lex_table).index != ascii['/'])
			print_err();
		else
		{
			add_node(tree, lexical_table.at(pos_in_lex_table), "/", true);
			++pos_in_lex_table;
			statements_list(tree);

			if (lexical_table.at(pos_in_lex_table).index != ascii['\\'])
				print_err();
			else
			{
				add_node(tree, lexical_table.at(pos_in_lex_table), "\\", true);
				++pos_in_lex_table;
			}
		}
	}
}

void expression(parse_tree *tree)
{
	token expression = make_zero(expression);
	add_node(tree, expression, "<expression>", false);

	multiplier(tree);
	multipliers_list(tree);
}

void multipliers_list(parse_tree *tree)
{
	token multipliers_list = make_zero(multipliers_list);
	add_node(tree, multipliers_list, "<alternative>", false);


}

void multiplication_instruction(parse_tree *tree)
{
	token multiplication_inst = make_zero(multiplication_inst);
	add_node(tree, multiplication_inst, "<multiplication list>", false);

	if (lexical_table.at(pos_in_lex_table).index == ascii['*'])
	{
		add_node(tree, lexical_table.at(pos_in_lex_table), "*", true);
		++pos_in_lex_table;
	}
	else if (lexical_table.at(pos_in_lex_table).index == ascii['/'])
	{
		add_node(tree, lexical_table.at(pos_in_lex_table), "*", true);
		++pos_in_lex_table;
	}
	else if (lexical_table.at(pos_in_lex_table).index == key_word["MOD"])
	{
		add_node(tree, lexical_table.at(pos_in_lex_table), "MOD", true);
		++pos_in_lex_table;
	}
	else
		print_err();
}

void multiplier(parse_tree *tree)
{
	token multiplier = make_zero(multiplier);
	add_node(tree, multiplier, "<multiplier>", false);

}

void variable_identifier(parse_tree *tree)
{
	token variable_identifier = make_zero(variable_identifier);
	add_node(tree, variable_identifier, "<variable identifier>", false);

	ident(tree);
}

void procedure_identifier(parse_tree *tree)
{
	token procedure_identifier = make_zero(procedure_identifier);
	add_node(tree, procedure_identifier, "<procedure identifier>", false); 

	ident(tree);
}

void ident(parse_tree *tree)
{
	token ident = make_zero(ident);
	add_node(tree, ident, "<identifier>", false);

	letter(tree);
	line(tree);
}

void line(parse_tree *tree)
{
	token line = make_zero(line);
	add_node(tree, line, "<string>", false);


}

void unsigned_integer(parse_tree *tree)
{
	token unsigned_integer = make_zero(unsigned_integer);
	add_node(tree, unsigned_integer, "<unsigned integer>", false);

	digit(tree);
	digits_string(tree);
}

void digits_string(parse_tree *tree)
{
	token digits_string = make_zero(digits_string);
	add_node(tree, digits_string, "<digits string>", false);


}

void digit(parse_tree *tree)
{
	token digit = make_zero(digit);
	add_node(tree, digit, "<digit>", false);


}

void letter(parse_tree *tree)
{
	token letter = make_zero(letter);
	add_node(tree, letter, "<letter>", false);
}

void parser(string file_name)
{
	lexical_analyzer(file_name.c_str());
	
}