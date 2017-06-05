#include "parser.h"

parse_tree *add_node(parse_tree *tree, token lexema)
{
	if (tree == NULL)
	{
		tree = new parse_tree;
		tree->lexema = lexema;
	}
	else
	{
		parse_tree *node = new parse_tree;
		node->lexema = lexema;
		tree->chirdren.push_back(node);
	}

}

void print_tree(parse_tree *tree, string str)
{
	if (tree != NULL)
	{
		cout << str << "terminal" << endl;
		for (int i = 0; i < tree->chirdren.size(); i++)
			print_tree(tree->chirdren.at(i), str + "/t");
	}
}

void delete_tree(parse_tree *tree)
{
	if (tree != NULL)
	{
		for (int i = 0; i < tree->chirdren.size(); i++)
			delete_tree(tree->chirdren.at(i));
		delete tree;
	}
}