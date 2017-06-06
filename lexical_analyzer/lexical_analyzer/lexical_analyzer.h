#pragma once
#include <string>
#include <vector>
#include <map>
#include <set>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <exception>
#include <iterator>
#define SPACE		0
#define CONSTANT	1
#define IDENTIFIER	2
#define SEPARATOR	3
#define COMMENT		4
#define ERR			5

using namespace std;

struct token
{
	int index;
	int row;
	int column;
};

static map<string, int> key_word;
static map<string, int> identifier;
static map<string, int> constant;
static map<char, int> ascii;

static int ind_key_word = 400;
static int ind_ident = 1000;
static int ind_const = 500;
static int row = 1;
static int column = 1;

static set<char> letters;
static set<char> digits;
static set<char> separators;

static vector<token> lexical_table;

void lexical_analyzer(const char *file_name);
void init_key_words();
void init_table_ascii();
void ident_token(char &attr, ifstream &fin, ofstream &fout);
token fill_in_lexema(int index, int column);
void print_table();
void print_table_in_file(ofstream &fout);
void print_ascii_table();
void print_err();
void print_err_in_file(ofstream &fout);
void print_key_words_table();
void print_ident_table();
void print_const_table();