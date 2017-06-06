#include "lexical_analyzer.h"	

void init_key_words()
{
    key_word["PROGRAM"] = ++ind_key_word;
    key_word["BEGIN"] = ++ind_key_word;
    key_word["END"] = ++ind_key_word;
    key_word["LOOP"] = ++ind_key_word;
    key_word["ENDLOOP"] = ++ind_key_word;
    key_word["CASE"] = ++ind_key_word;
    key_word["OF"] = ++ind_key_word;
    key_word["ENDCASE"] = ++ind_key_word;
    key_word["MOD"] = ++ind_key_word;
}

void init_table_ascii()
{
    for (int i = 'A'; i <= 'Z'; ++i)
        letters.insert(i);

    for (int i = '0'; i <= '9'; ++i)
	digits.insert(i);
	
    separators.insert(';');
    separators.insert(':');
    separators.insert('/');
    separators.insert('.');
    separators.insert('\\');
    separators.insert('*');

    for (int i = 1; i <= 127; ++i)
    {
    	if (i == 8 || i == 9 || i == 10 || i == 13 || i == 32)
            ascii[char(i)] = SPACE;
	else if (letters.find(char(i)) != letters.end())
            ascii[char(i)] = IDENTIFIER;
	else if (digits.find(char(i)) != digits.end())
            ascii[char(i)] = CONSTANT;
	else if (separators.find(char(i)) != separators.end())
            ascii[char(i)] = SEPARATOR;
	else if (char(i) == '(')
            ascii[char(i)] = COMMENT;
	else
            ascii[char(i)] = ERR;
    }
}

void lexical_analyzer(const char *file_name)
{
    char attr;
    ifstream fin(file_name);
    ofstream fout1("line_of_tokens.txt");
    ofstream fout2("list_of_errors.txt");
    init_table_ascii();
    init_key_words();
    try
    {
        if (!fin)
            throw "File isn't found";
        else
        {
            fin.get(attr);
            while (!fin.eof())
                ident_token(attr, fin, fout2);            
        }
        print_table();
        print_table_in_file(fout1);
        print_key_words_table();
        print_ident_table();  
        print_const_table();
        cout << "You can watch your errors in file list_of_errors.txt" << endl;
    }
    catch(const char *str)
    {
        cout << str << endl;
    }
}

void ident_token(char &attr, ifstream &fin, ofstream &fout)
{
    token lexema;
    int len = 0;
    string ident = "";
    switch (ascii[attr])
    {
        case SPACE: // spaces
            if (int(attr) == 8 || int(attr) == 13)
                break;
            else if (int(attr) == 9)
                column += 4;
            else if (int(attr) == 10) 
            {
                ++row;
                column = 1;
            }
            else
                ++column;
            fin.get(attr);
            break;
        case CONSTANT:	// constants
            while (!fin.eof() && ascii[attr] == 1)
            {
                ident += attr;
                fin.get(attr);
                ++column;
                ++len;
            }
            constant[ident] = ++ind_const;
            lexical_table.push_back(fill_in_lexema(constant[ident], column - len));
            break;
        case IDENTIFIER:	// identifieres and key words
            while (!fin.eof() && (ascii[attr] == 2 || ascii[attr] == 1))
            {
                ident += attr;
                fin.get(attr);
                ++column;
                ++len;
            }
            if (key_word.find(ident) == key_word.end())
            {
                identifier[ident] = ++ind_ident;
                lexical_table.push_back(fill_in_lexema(identifier[ident], column - len));
            }
            else
                lexical_table.push_back(fill_in_lexema(key_word[ident], column - len));
            break;
        case SEPARATOR:	// one symbol separators
            lexical_table.push_back(fill_in_lexema(int(attr), column));
            ++column;
            fin.get(attr);
            break;
        case COMMENT: // comments
            fin.get(attr);//змінити цикли...бо завжди незавершений коментар
            ++column;            
            if (attr == '*')
            {
                int beg_c = column - 1;
                int beg_r = row;
                while (!fin.eof())
                {
                    fin.get(attr);
                    
                    if (int(attr) == 10) 
                    {
                        ++row;
                        column = 1;
                    }
                    
                        
                    if (attr == '*' && !fin.eof())
                    {
                        fin.get(attr);
                        ++column;
                        if (attr == ')')
                        {
                            fin.get(attr);
                            ++column;
                            break;
                        } 
                    }
                }
                                    
                if (fin.eof())
                    fout << "Your comment isn't closed. Started in column = " << beg_c << " row = " << beg_r << endl;
            }
            else
                print_err_in_file(fout);
            break;
        default: // invalid characters
            print_err_in_file(fout);
            fin.get(attr);
            ++column;
            break;
    }
}

token fill_in_lexema(int index, int column)
{
    token lexema;
    lexema.index = index;
    lexema.row = row;
    lexema.column = column;
    return lexema;
}

void print_table()
{
    cout << "Index| Row |Column|" << endl;
    for (int i = 0; i < lexical_table.size(); i++)
    {
        cout << left << setw(5) << lexical_table[i].index << "|" << setw(5) << lexical_table[i].row << "|" 
                << setw(6) << lexical_table[i].column << "|" << endl;
    }
}

void print_table_in_file(ofstream &fout)
{
    fout << "Index| Row |Column|" << endl;
    for (int i = 0; i < lexical_table.size(); i++)
    {
        fout << left << setw(5) << lexical_table[i].index << "|" << setw(5) << lexical_table[i].row << "|" 
                << setw(6) << lexical_table[i].column << "|" << endl;
    }
}

void print_ascii_table()
{
    cout << "------------------------" << endl;
    cout << "Number|Symbol|Attribute|" << endl;
    for (int i = 0; i < ascii.size(); i++)
    {
        cout << i << "|" << char(i) << "|" << ascii[char(i)] << "|" << endl;
    }
}

void print_key_words_table()
{
    cout << "------------------------" << endl;
    cout << "Key words|Index|" << endl;
       
    map<string, int>::iterator it;   
    for (it = key_word.begin(); it != key_word.end(); it++)
        cout << setw(9) << it->first << "|" << setw(5) << it->second << "|" << endl;
}

void print_ident_table()
{
    cout << "------------------------" << endl;
    cout << "Identifier|Index|" << endl;
    map<string, int>::iterator it;
    for (it = identifier.begin(); it != identifier.end(); it++)
        cout << setw(10) << it->first << "|" << setw(5) << it->second << "|" << endl;
}

void print_const_table()
{
    cout << "------------------------" << endl;
    cout << "Constant|Index|" << endl;
    map<string, int>::iterator it;
    for (it = constant.begin(); it != constant.end(); it++)
        cout << setw(8) << it->first << "|" << setw(5) << it->second << "|" << endl;
}

void print_err()
{
    cout << "There is error in row = " << row << " column = " << column << endl;
}

void print_err_in_file(ofstream &fout)
{
    fout << "There is error in row = " << row << " column = " << column << endl;
}