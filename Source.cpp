#include <iostream>
#include <conio.h>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include<stack>
#include<queue>
#include<sstream>
#include <iomanip>
using namespace std;

void load_grammar(vector<vector<string>>& A, vector<string>& terminal, vector<string>& non_terminal, string fn)
{
	ifstream reader(fn);
	A.resize(100); // temp size
	int idx = 0;
	string temp;
	vector<string> str;
	while (getline(reader, temp))
	{
		bool firsttime = true;
		for (int i = 0; i < temp.size(); i++)
		{
			string b;

			if (temp[i] == '\t')
				continue;
			else if (temp[i + 1] == '\t')
			{
				b.append(1, temp[i]);
			}
			else if (temp[i + 1] != '\t' || temp[i + 1] != '\0')
			{
				while (temp[i] != '\t' && temp[i] != '\0')
				{
					b.append(1, temp[i++]);
				}
			}
			A[idx].push_back(b);
			if (firsttime)
				non_terminal.push_back(b);
			else
				terminal.push_back(b);
			firsttime = false;
		}
		idx++;
	}
	A.resize(idx);
}

void print_2d_string(vector<vector<string>> A) {

	cout << "Grammer Lines Size : " << A.size() << endl;
	for (int i = 0; i < A.size(); i++) // number of terminals
	{

		for (int j = 0; j < A[i].size(); j++) // number of string vector against a terminal
		{
			cout << setw(3) << A[i][j] << " ";
		}
		cout << endl;
	}

}

bool is_in_vec(vector<string> vec, string str) {

	if (std::find(vec.begin(), vec.end(), str) != vec.end()) {
		return true;
	}
	return false;
}

void remove_duplicate(vector<string> original, vector<string>& terminals) {
	for (int i = 0; i < original.size(); i++) {
		if (!is_in_vec(terminals, original[i])) {
			terminals.push_back(original[i]);
		}
	}
}

void remove_terminal(vector<string> T, vector<string>& NT) {
	for (int i = 0; i < NT.size(); i++)
	{
		string str = NT[i];
		if (is_in_vec(T, str) || str == "~")
		{
			NT.erase(NT.begin() + i);
			i--;
		}
	}
}

void print_map_string(map< string, vector<string>> A, vector<string> t) {

	cout << "Grammer Lines Size : " << t.size() << endl;
	for (int i = 0; i < t.size(); i++) // number of terminals
	{
		cout << t[i] << " : ";
		for (int j = 0; j < A[t[i]].size(); j++) // number of string vector against a terminal
		{
			cout << A[t[i]][j] << " ";
		}
		cout << endl;
	}

}

void write_map_string(map< string, vector<string>> A, vector<string> t, ofstream& writer) {

	for (int i = 0; i < t.size(); i++) // number of terminals
	{
		writer << t[i] << "\t";
		for (int j = 0; j < A[t[i]].size(); j++) // number of string vector against a terminal
		{
			writer << A[t[i]][j] << "\t";
		}
		writer << endl;
	}

}

void write_2d_string(vector<vector<string>> A, ofstream& writer) {

	for (int i = 0; i < A.size(); i++) {

		for (int j = 0; j < A[i].size(); j++)
			writer << A[i][j] << " ";
		writer << endl;
	}

}

// returns indexes of the
void get_indexes(vector<vector<string>> A, string str, stack<int>& indexes) {
	for (int i = 0; i < A.size(); i++)
	{
		if (A[i][0] == str)
		{
			indexes.push(i);
		}
	}
}

int get_first_indexes(vector<vector<string>> A, string str) {
	for (int i = 0; i < A.size(); i++)
	{
		if (A[i][0] == str)
		{
			return i;
		}
	}
}

bool is_in_terminal(vector<string>terminals, string prod)
{
	for (int i = 0; i < terminals.size(); i++) {
		if (terminals[i] == prod) {
			return true;
		}
	}
	return false;
}

bool is_epsilon(string prod, string epsilon) {
	if (prod == epsilon)
		return true;
	return false;
}

void compute_first_set(vector<string>& first, vector<vector<string>> grammar, string current, vector<string> non_terminals, stack<string>& next_terminals, int& j, int NT_number) {
	string prod = grammar[NT_number][j];
	if (is_in_terminal(non_terminals, prod)) // if it is a non_terminal // we have to compute its first so push it
	{
		next_terminals.push(prod);
	}
	else if (is_epsilon(prod, "~")) // if epsilon
	{
		next_terminals.push("~");
	}
	else // it is a terminal
	{
		first.push_back(prod);
	}
}

bool is_already_computed(vector<vector<string>> first_set, string non_terminal) {
	for (int i = 0; i < first_set.size(); i++)
		if (first_set[i][0] == non_terminal)
			return true;
	return false;
}

void remove_from_vector(vector<vector<string>>& A, int idx) {
	A.erase(A.begin() + idx);
}

map<string, vector<string>> first_set(vector<vector<string>> A, vector<string> non_terminals)
{
	map<string, vector<string>> first_set;
	stack<int> indexes;
	stack<string> next;
	for (int idx = 0; idx < A.size(); idx++)
	{
		vector<string> first;
		string non_terminal = A[idx][0]; // get Non-Terminal
		int current_ind;
		int  main = 1;
		indexes.push(idx); // index jiska first nikalna
		next.push(non_terminal);// we compute first which is in the next stack
		string current; //
		int secind = 1;
		while (!next.empty() || !indexes.empty()) // next: next in production
		{
			if (!next.empty()) 
			{
				current = next.top(); next.pop(); // get symbol from the next
			}
			if (A[idx][0] != current && indexes.empty())
				get_indexes(A, current, indexes); // index of that symbols 
			if (!indexes.empty()) {
				current_ind = indexes.top(); indexes.pop(); // we can get multiple indexes for a symbol
			}
			if (current != "~") //if not epsilon
			{
				compute_first_set(first, A, current, non_terminals, next, main, current_ind); // compute first of that 
				while (!indexes.empty()) // check all indexes of the symbol and compute its first
				{
					current_ind = indexes.top();
					indexes.pop();
					compute_first_set(first, A, current, non_terminals, next, main, current_ind);
				}
			}
			else if (next.size() == 0 && current == "~" && secind == A[idx].size() - 1) // if there is nothing in next so we have to push ~
			{
				first.push_back("~");
			}
			else if (next.size() == 0 && current == "~" && secind < A[idx].size() - 1) // if there are more symbols in front of ~ , we will compute there first
			{
				secind++;
				current = A[idx][secind];
				if (current == "~") {
					first.push_back("~");
					break;
				}
				get_indexes(A, current, indexes);
				while (!indexes.empty()) {
					current_ind = indexes.top();
					indexes.pop();
					compute_first_set(first, A, current, non_terminals, next, main, current_ind);
				}
			}
			else if (next.size() == 0 && indexes.size() == 0 && current == "~")
			{
				first.push_back("~");
			}
		}
		for (int i = 0; i < first.size(); i++) {
			first_set[non_terminal].push_back(first[i]);
		}
		while (!indexes.empty()) {
			indexes.pop();
		}
	}
	return first_set;
}

void findallindexes(vector<vector<string>> grammar, string terminal, queue<int>& indexes) {
	for (int i = 0; i < grammar.size(); i++) {
		for (int j = 1; j < grammar[i].size(); j++)// we are watching right hand side 
		{
			string str = grammar[i][j];
			if (str == terminal)
			{
				indexes.push(i);
				indexes.push(j);
			}
		}
	}
}

vector<string> get_first(map<string, vector<string>> first_set, string str) {
	return first_set[str];
}

bool is_in_map(map<string, vector<string>>follow_set, string current_terminal, string follow) {
	for (int i = 0; i < follow_set[current_terminal].size(); i++) {
		if (follow_set[current_terminal][i] == follow) {
			return true;
		}
	}
	return false;
}

map<string, vector<string>> follow_set(vector<vector<string>> grammar, vector<string> non_terminals, map<string, vector<string>> first_set)
{
	map<string, vector<string>> follow_set;
	queue<int> indexes;
	for (int idx = 0; idx < non_terminals.size(); idx++)
	{
		vector<string> follow;
		string current_terminal = non_terminals[idx]; // nonterminal, terminal written by mistake
		if (idx == 0)  // push $ for the starting symbol
		{
			follow.push_back("$");
		}
		findallindexes(grammar, current_terminal, indexes); // find all indexes
		for (int j = 0; j < indexes.size() / 2; j++)
		{
			int ind = 1;
			while (!indexes.empty()) // while all indexes are not discovered
			{
				// find coordinates of the non_terminal
				int r = indexes.front();
				indexes.pop();
				int c = indexes.front();
				indexes.pop();
				if (c + ind < grammar[r].size()) // is not the last character
				{
					if (grammar[r][c + ind] == "~")
					{ //if there is ~ then check next symbol in the production
						ind++;
						indexes.push(r);
						indexes.push(c);
					}
					else if (!is_in_terminal(non_terminals, grammar[r][c + ind]))// if it is a non-terminal add it to the follow 
					{
						follow.push_back(grammar[r][c + ind]);
					}
					else // if its a terminal then get its first set
					{
						vector<string> f = get_first(first_set, grammar[r][c + ind]);
						for (int z = 0; z < f.size(); z++) {
							if (f[z] == "~") { // if ~ comes then check next symbol
								ind++;
								indexes.push(r);
								indexes.push(c);
							}
							else {
								follow.push_back(f[z]);
							}
						}
					}
				}
				else // string is ending // then find follow of the nonterminal that is producing
				{
					if (follow_set[grammar[r][0]].empty() && grammar[r][0] != current_terminal && grammar[r][c] != current_terminal)
					{
						findallindexes(grammar, grammar[r][0], indexes);
					}
					else 
					{
						for (int i = 0; i < follow_set[grammar[r][0]].size(); i++)
							follow.push_back(follow_set[grammar[r][0]][i]);
					}
				}
			}
		}
		for (int i = 0; i < follow.size(); i++) {
			if (!is_in_map(follow_set, current_terminal, follow[i]))
				follow_set[current_terminal].push_back(follow[i]);
		}
	}
	return follow_set;
}

int find_terminal_row(vector<vector<string>>& table, string str)
{
	for (int i = 0; i < table.size(); i++) {
		if (table[i][0] == str) {
			return i;
		}
	}
}

int find_terminal_col(vector<vector<string>>& table, string str)
{
	for (int i = 0; i < table[0].size(); i++) {
		if (table[0][i] == str) {
			return i;
		}
	}
}

void parse_table(vector<vector<string>>& table, map<string, vector<string>>first_set1, map<string, vector<string>>follow_set1, vector<string> terminals, vector<string> nonterminals, vector<vector<string>> grammar) 
{
	vector<string>temp;
	temp.push_back("\t");
	table.push_back(temp);
	temp.clear();
	for (int i = 0; i < terminals.size(); i++) // add terminals
	{
		temp.push_back(terminals[i]);
		table.push_back(temp);
		temp.clear();
	}
	for (int i = 0; i < nonterminals.size(); i++)// add non-terminals
		table[0].push_back(nonterminals[i]);
	table[0].push_back("$"); //
	for (int i = 0; i < table.size(); i++) // resize all rows with max columns
	{
		table[i].resize(table[0].size());
	}
	int r, c;
	for (int i = 0; i < grammar.size(); i++)
	{
		int j = 1;
		queue<int> indexes;
		stringstream ss;
		ss << i + 1;
		string prodnum;
		ss >> prodnum;
		r = find_terminal_row(table, grammar[i][0]);
		if (is_epsilon(grammar[i][j], "~")) 
		{
			vector<string> follow = follow_set1[grammar[i][0]];// then get follow
			for (int fo = 0; fo < follow.size(); fo++) //get indexes
			{
				string str2 = follow_set1[grammar[i][0]][fo];
				c = find_terminal_col(table, str2);
				indexes.push(c);
			}
		}
		else if (is_in_vec(nonterminals, grammar[i][j]))
		{
			c = find_terminal_col(table, grammar[i][j]);
			indexes.push(c);
		}
		else if (!is_in_vec(nonterminals, grammar[i][j]))
		{
			vector<string> first = first_set1[grammar[i][j]]; // get its first set
			for (int f = 0; f < first.size(); f++) // go
			{
				string str = first_set1[grammar[i][j]][f];
				if (str == "~") // if epsilon
				{
					vector<string> follow = follow_set1[grammar[i][0]];// then get follow
					for (int fo = 0; fo < follow.size(); fo++) //get indexes
					{
						string str2 = follow_set1[grammar[i][j]][fo];
						c = find_terminal_col(table, str2);
						indexes.push(c);
					}
				}
				else
				{

					c = find_terminal_col(table, str);
					indexes.push(c);
				}
			}
		}
		while (!indexes.empty()) 
		{
			int c = indexes.front();
			indexes.pop();
			table[r][c] = prodnum;
		}
	}
}

void print_string(vector<string> a) {
	for (int i = 0; i < a.size(); i++) {
		cout << a[i] << " ";
	}
	cout << endl;
}


void write_in_excel(vector<vector<string>> table, ofstream& writer_parse)
{
	for (int i = 0; i < table.size(); i++) {
		for (int j = 0; j < table[i].size(); j++) {
			if (!table[i][j].empty() && table[i][j] != "\t") {
				writer_parse << table[i][j] << "\t";
			}
			else {
				writer_parse << "\t";
			}
		}
		writer_parse << endl;
	}
}

void print_2d_table(vector<vector<string>> table) {
	for (int i = 0; i < table.size(); i++) {
		for (int j = 0; j < table[i].size(); j++) {
			if (!table[i][j].empty() && table[i][j] != "\t") {
				cout << table[i][j] << "\t";
			}
			else {
				cout << "\t";
			}
		}
		cout << endl;
	}
}


int main()
{
	vector<vector<string>> grammar;
	vector<vector<string>> table;
	vector<string> allterminal;
	vector<string> non_terminal;
	vector<string> terminal_wo_duplicates;
	vector<string> nonterminal_wo_duplicates;

	string file = "grammar2.txt";

	ofstream writer_first("First.txt");
	ofstream writer_follow("Follow.txt");
	ofstream writer_parse("LL1ParsingTable.xls");

	load_grammar(grammar,  non_terminal, allterminal, file);
	remove_duplicate(allterminal, terminal_wo_duplicates);
	remove_duplicate(non_terminal, nonterminal_wo_duplicates);
	remove_terminal(terminal_wo_duplicates, nonterminal_wo_duplicates);

    print_2d_string(grammar);

	cout << "\n----------------FIRST SET------------------\n";
	map<string, vector<string>> first_set1 = first_set(grammar, allterminal);
	print_map_string(first_set1, terminal_wo_duplicates);
	write_map_string(first_set1, terminal_wo_duplicates, writer_first);

	cout << "\n----------------FOLLOW SET------------------\n";
	map<string, vector<string>> follow_set1 = follow_set(grammar, terminal_wo_duplicates, first_set1);
	print_map_string(follow_set1, terminal_wo_duplicates);
	write_map_string(follow_set1, terminal_wo_duplicates, writer_follow);
	cout << "\n----------------PARSING TABLE---------------\n";
	parse_table(table, first_set1, follow_set1, terminal_wo_duplicates, nonterminal_wo_duplicates, grammar);
	print_2d_table(table);
	write_in_excel(table, writer_parse);

	return _getch();
}

