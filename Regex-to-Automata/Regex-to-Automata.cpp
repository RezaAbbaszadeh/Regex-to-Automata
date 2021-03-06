#include "stdafx.h"
#include "stdc++.h"
using namespace std;

int nfa_init_states[20], nfa_final_states[20], nfa_init_count = 0, nfa_final_count = 0, nfa_states_count;
int nfa_table[1000][5] = { 0 };
string init_dfa[20], fin_dfa[20];
int dfa_final_count = 0, dfa_states_count;
int v = 0;
int with_a[40][40];
int with_b[40][40];
string dfa_tab[50][3];
string dfa_states[50];

string add_concat(string s) {
	char news[200];
	int j = 1;
	news[0] = '(';
	for (int i = 0; i<s.length(); i++) {
		news[j] = s[i];
		j++;
		if ((s[i] == 'a' || s[i] == 'b') && (s[i + 1] == 'a' || s[i + 1] == 'b')) {
			news[j] = '.';
			j++;
		}
		else if ((s[i] == 'a' || s[i] == 'b') && s[i + 1] == '(') {
			news[j] = '.';
			j++;
		}
		else if (s[i] == ')' && (s[i + 1] == 'a' || s[i + 1] == 'b')) {
			news[j] = '.';
			j++;
		}
		else if (s[i] == ')'&&s[i + 1] == '(') {
			news[j] = '.';
			j++;
		}
		else if (s[i] == '*' && ((s[i + 1] == 'a' || s[i + 1] == 'b') || s[i + 1] == '(')) {
			news[j] = '.';
			j++;
		}
	}
	news[j] = ')';
	j++;
	string finals;
	for (int i = 0; i<j; i++)
		finals += news[i];
	return finals;
}


string postfix(string s) {
	char news[200];
	stack <char> ch;
	int j = 0;
	for (int i = 0; i<s.length(); i++) {
		char x = s[i];
		switch (x) {
		case 'a':
			news[j] = 'a';
			j++;
			break;
		case 'b':
			news[j] = 'b';
			j++;
			break;
		case '(':
			ch.push('(');
			break;
		case ')':
			while (!ch.empty()) {
				if (ch.top() == '(') {
					ch.pop();
					break;
				}
				else {
					news[j] = ch.top();
					ch.pop();
					j++;
				}
			}
			break;

			/*case '~':
			if (ch.empty()) {
			ch.push('~');
			}
			else {
			char temp = ch.top();
			if (temp == '(')
			ch.push('~');
			/*else if (temp == '*') {
			news[j] = ch.top();
			ch.pop();
			j++;
			if (ch.top() == '~') {
			news[j] = '.';
			j++;
			}
			else { //if '(' or '|'
			ch.push('.');
			}
			}
			else if (temp == '.') {
			ch.push('~');
			}
			else if (temp == '|') {
			ch.push('~');
			}
			}
			break;*/
		case '.':
			if (ch.empty()) {
				ch.push('.');
			}
			else {
				char temp = ch.top();
				if (temp == '(')
					ch.push('.');
				else if (temp == '*') {
					news[j] = ch.top();
					ch.pop();
					j++;
					if (ch.top() == '.') {
						news[j] = '.';
						j++;
					}
					else { //if '(' or '|'
						ch.push('.');
					}
				}
				else if (temp == '.') {
					news[j] = ch.top();
					ch.pop();
					j++;
					ch.push('.');
				}
				else if (temp == '|') {
					ch.push('.');
				}
			}
			break;

		case '|':
			if (ch.empty()) {
				ch.push('|');
			}
			else {
				char temp = ch.top();
				if (temp == '(')
					ch.push('|');
				else if (temp == '*') {
					news[j] = ch.top();
					ch.pop();
					j++;
					ch.push('|');
				}
				else if (temp == '.') {
					news[j] = ch.top();
					j++;
					ch.pop();
					ch.push('|');
				}
			}
			break;
		case '*':
			ch.push('*');
			break;
		}
	}
	string finals;
	for (int i = 0; i<j; i++) {
		finals += news[i];
	}
	return finals;
}

void print_nfa() {
	cout << "NFA:\nStates: ";

	for (int i = 1; i < nfa_states_count; i++) {
		cout << "q" << nfa_table[i][0];
		for (int j = 0; j < nfa_final_count; j++) {
			if (nfa_table[i][0] == nfa_final_states[j]) {
				cout << "(final)";
			}
		}

		if (nfa_table[i][0] == nfa_init_states[0])
			cout << "(first)";

		cout << ", ";
	}

	cout << endl << "Transitions:" << endl;

	for (int i = 1; i < nfa_states_count; i++) {
		if (nfa_table[i][1] != 0)
			cout << "transition(q" << i << "," << "a" << ") = q" << nfa_table[i][1] << endl;
		if (nfa_table[i][2] != 0)
			cout << "transition(q" << i << "," << "b" << ") = q" << nfa_table[i][2] << endl;
		if (nfa_table[i][3] != 0)
			cout << "transition(q" << i << "," << "Epsilon" << ") = q" << nfa_table[i][3] << endl;
		if (nfa_table[i][4] != 0)
			cout << "transition(q" << i << "," << "Epsilon" << ") = q" << nfa_table[i][4] << endl;
	}

}



void print_dfa(bool has_trap) {
	cout << endl << endl << "DFA:\nStates: " << endl;

	cout << "q1(first)";
	for (int j = 0; j < dfa_final_count; j++) {
		if (dfa_tab[1][0] == fin_dfa[j]) {
			cout << "(final)";
		}
	}

	for (int i = 2; i < dfa_states_count + has_trap; i++) {
		cout << ", ";

		cout << "q" << dfa_tab[i][0];
		for (int j = 0; j < dfa_final_count; j++) {
			if (dfa_tab[i][0] == fin_dfa[j]) {
				cout << "(final)";
			}
		}
	}

	cout << endl << "Transitions:" << endl;

	for (int i = 1; i < dfa_states_count + has_trap; i++) {
		cout << "transition(q" << i << "," << "a" << ") = q" << dfa_tab[i][1] << endl;
		cout << "transition(q" << i << "," << "b" << ") = q" << dfa_tab[i][2] << endl;
	}

}


void print_minimized(bool has_trap) {
	/*bool* flags;
	flags = new bool[dfa_states_count];*/
	bool flags[50] = { false };

	cout << endl << endl << "Minimized DFA:\nStates: " << endl;

	cout << "q1(first)";
	for (int j = 0; j < dfa_final_count; j++) {
		if (dfa_tab[1][0] == fin_dfa[j]) {
			cout << "(final)";
		}
	}
	flags[1] = true;
	int h = has_trap;
	for (int i = 2; i < dfa_states_count + has_trap; i++) {
		if (!flags[stoi(dfa_tab[i][0])]) {
			cout << ", ";

			cout << "q" << dfa_tab[i][0];
			for (int j = 0; j < dfa_final_count; j++) {
				if (dfa_tab[i][0] == fin_dfa[j]) {
					cout << "(final)";
				}
			}
			flags[stoi(dfa_tab[i][0])] = true;
		}
	}

	cout << endl << "Transitions:" << endl;

	for (int i = 1; i < dfa_states_count + has_trap; i++) {
		if (flags[i]) {
			cout << "transition(q" << dfa_tab[i][0] << "," << "a" << ") = q" << dfa_tab[i][1] << endl;
			cout << "transition(q" << dfa_tab[i][0] << "," << "b" << ") = q" << dfa_tab[i][2] << endl;
		}
	}

}



int reg_to_nfa(string s, int nfa_table[][5]) {
	nfa_states_count = 1;
	int m, n, j, counter;
	for (int i = 0; i < s.length(); i++) {
		char x = s[i];
		switch (x) {
		case 'a': nfa_table[nfa_states_count][0] = nfa_states_count;
			nfa_init_states[nfa_init_count] = nfa_states_count;
			nfa_states_count++;
			nfa_init_count++;
			nfa_table[nfa_states_count - 1][1] = nfa_states_count;
			nfa_final_states[nfa_final_count] = nfa_states_count;
			nfa_final_count++;
			nfa_table[nfa_states_count][0] = nfa_states_count;
			nfa_states_count++;
			break;
		case 'b': nfa_table[nfa_states_count][0] = nfa_states_count;
			nfa_init_states[nfa_init_count] = nfa_states_count;
			nfa_init_count++;
			nfa_states_count++;
			nfa_table[nfa_states_count - 1][2] = nfa_states_count;
			nfa_final_states[nfa_final_count] = nfa_states_count;
			nfa_final_count++;
			nfa_table[nfa_states_count][0] = nfa_states_count;
			nfa_states_count++;
			break;
		case '.':
			m = nfa_final_states[nfa_final_count - 2];
			n = nfa_init_states[nfa_init_count - 1];
			nfa_table[m][3] = n;
			nfa_final_states[nfa_final_count - 2] = nfa_final_states[nfa_final_count - 1];
			nfa_final_count--;
			nfa_init_count--;
			break;
		case '|':

			nfa_table[nfa_states_count][3] = nfa_init_states[nfa_init_count - 1];
			nfa_table[nfa_states_count][4] = nfa_init_states[nfa_init_count - 2];

			nfa_init_count -= 2;
			nfa_init_states[nfa_init_count] = nfa_states_count;
			nfa_init_count++;
			nfa_table[nfa_states_count][0] = nfa_states_count;
			nfa_states_count++;

			nfa_table[nfa_final_states[nfa_final_count - 1]][3] = nfa_states_count;
			nfa_table[nfa_final_states[nfa_final_count - 2]][3] = nfa_states_count;


			nfa_final_count -= 2;
			nfa_final_states[nfa_final_count] = nfa_states_count;
			nfa_final_count++;
			nfa_table[nfa_states_count][0] = nfa_states_count;
			nfa_states_count++;
			break;

			/*case '~':


			nfa_init[nfa_init_count] = nfa_states_count;


			nfa_table[nfa_states_count][3] = nfa_init[nfa_init_count - 1];
			nfa_table[nfa_states_count][4] = nfa_init[nfa_init_count - 2];

			nfa_init_count -= 2;
			nfa_init[nfa_init_count] = nfa_states_count;
			nfa_init_count++;
			nfa_table[nfa_states_count][0] = nfa_states_count;
			nfa_states_count++;

			nfa_table[nfa_fin[nfa_final_count - 1]][3] = nfa_states_count;
			nfa_table[nfa_fin[nfa_final_count - 2]][3] = nfa_states_count;


			nfa_final_count -= 2;
			nfa_fin[nfa_final_count] = nfa_states_count;
			nfa_final_count++;
			nfa_table[nfa_states_count][0] = nfa_states_count;
			nfa_states_count++;
			break;*/

		case '*': m = nfa_init_states[nfa_init_count - 1];
			nfa_table[nfa_states_count][0] = nfa_states_count;
			nfa_table[nfa_states_count][3] = m;
			nfa_init_states[nfa_init_count - 1] = nfa_states_count;

			nfa_states_count++;
			n = nfa_final_states[nfa_final_count - 1];
			nfa_table[n][3] = m;
			nfa_table[n][4] = nfa_states_count;
			nfa_table[nfa_states_count - 1][4] = nfa_states_count;
			nfa_final_states[nfa_final_count - 1] = nfa_states_count;
			nfa_table[nfa_states_count][0] = nfa_states_count;
			nfa_states_count++;
			break;
		}
	}

	print_nfa();

	return nfa_states_count;
}



//find where we can go with just one 'a' from state 'i'
//'x' is 'i' in first call
string findA(int nfa_table[][5], int i, bool is_find, int x) {
	string s = "";
	if (!is_find) {
		if (nfa_table[i][1] != 0) {
			s += findA(nfa_table, nfa_table[i][1], true, x);
		}
		else {
			if (nfa_table[i][3] != 0) {
				s += findA(nfa_table, nfa_table[i][3], false, x);
			}
			if (nfa_table[i][4] != 0) {
				s += findA(nfa_table, nfa_table[i][4], false, x);
			}

		}
	}
	else {
		with_a[x][v] = i;
		v++;
		s += "," + to_string(i);
		if (nfa_table[i][3] != 0) {
			s += findA(nfa_table, nfa_table[i][3], true, x);
		}
		if (nfa_table[i][4] != 0) {
			s += findA(nfa_table, nfa_table[i][4], true, x);
		}
	}

	return s;
}



string findB(int nfa_table[][5], int i, bool is_find, int x) {
	string s = "";
	if (!is_find) {
		if (nfa_table[i][2] != 0) {
			s += findB(nfa_table, nfa_table[i][2], true, x);
		}
		else {
			if (nfa_table[i][3] != 0) {
				s += findB(nfa_table, nfa_table[i][3], false, x);
			}
			if (nfa_table[i][4] != 0) {
				s += findB(nfa_table, nfa_table[i][4], false, x);
			}

		}
	}
	else {
		with_b[x][v] = i;
		v++;
		s += "," + to_string(i);
		if (nfa_table[i][3] != 0) {
			s += findB(nfa_table, nfa_table[i][3], true, x);
		}
		if (nfa_table[i][4] != 0) {
			s += findB(nfa_table, nfa_table[i][4], true, x);
		}
	}

	return s;
}

//where we can go with landa from start state
string find_start_states(int nfa_table[][5], int i) {
	string s = to_string(i);
	if (nfa_table[i][3] != 0) {
		s += "," + find_start_states(nfa_table, nfa_table[i][3]);
	}
	if (nfa_table[i][4] != 0) {
		s += "," + find_start_states(nfa_table, nfa_table[i][4]);
	}
	return s;
}

//complete with_a and with_b transition lists
string fillTransitionLists(int nfa_table[][5], int states) {
	for (int i = 1; i < states; i++) {
		v = 0;
		findB(nfa_table, i, false, i);
	}
	for (int i = 1; i < states; i++) {
		v = 0;
		findA(nfa_table, i, false, i);
	}

	return find_start_states(nfa_table, nfa_init_states[0]);
}

vector<std::string> Split(std::string str, std::string sep) {
	char* cstr = const_cast<char*>(str.c_str());
	char* current;
	std::vector<std::string> arr;
	char* token = NULL;
	current = strtok_s(cstr, sep.c_str(), &token);
	while (current != NULL) {
		arr.push_back(current);
		current = strtok_s(NULL, sep.c_str(), &token);
	}
	return arr;
}


//removes same states in s= "1,2,3,3,4,3"-> s="1,2,3,4"
string Common(string s) {
	vector<std::string> a = Split(s, ",");
	int *b;
	b = new int[a.size()];
	for (int i = 0; i < a.size(); i++) {
		std::istringstream iss(a[i]);
		iss >> b[i];
	}
	sort(b, b + a.size(), greater<int>());
	int h = a.size();
	for (int i = 0; i < h - 1; i++) {
		if (b[i] == b[i + 1]) {
			int n = 1;
			for (int k = i + 1; k < h - 1; k++) {
				if (b[k] == b[k + 1])
					n++;
				else
					break;
			}
			for (int j = i; j < h - n; j++) {
				b[j] = b[j + n];
			}
			h -= n;
		}
	}
	s = "";
	for (int i = 0; i < h; i++) {
		s += "," + to_string(b[i]);
	}

	return s;
}



bool nfa_to_dfa(string s) {
	//s = list of states in a group
	bool has_trap = false;
	dfa_states_count = 1;
	dfa_tab[dfa_states_count][0] = s;
	dfa_states[0] = "trap";
	dfa_states_count++;
	int(*ab);
	dfa_states[1] = s;
	bool endWhile = false;
	while (!endWhile) {
		vector<std::string> a = Split(s, ",");
		ab = new int[a.size()];
		for (int i = 0; i < a.size(); i++) {
			std::istringstream iss(a[i]);
			iss >> ab[i];
		}

		s = "";
		for (int i = 0; ab[i] > 0 && ab[i] < 1000; i++) {
			for (int j = 0; with_a[ab[i]][j] > 0; j++) {
				s += "," + to_string(with_a[ab[i]][j]);
			}
		}
		if (s.compare("") == 0) {
			dfa_tab[dfa_states_count - 1][1] = dfa_states[0];
			has_trap = true;
			endWhile = true;
		}
		else {
			s = Common(s);
			int i;
			for (i = 0; i < 50; i++) {
				if (s.compare(dfa_states[i]) == 0) {// states already exist
					dfa_tab[dfa_states_count - 1][1] = dfa_tab[i][0];
					endWhile = true;
					break;
				}
			}
			if (i == 50) {
				dfa_tab[dfa_states_count][0] = s;
				dfa_tab[dfa_states_count - 1][1] = dfa_tab[dfa_states_count][0];
				dfa_states[dfa_states_count] = s;
				dfa_states_count++;
			}
		}
	}

	int g;
	int oldStates = 1;
	while (true) {

		g = oldStates;
		oldStates = dfa_states_count;
		endWhile = false;
		if (g == dfa_states_count)
			break;
		while (!endWhile) {
			s = dfa_states[g];
			g++;

			vector<std::string> a = Split(s, ",");
			ab = new int[a.size()];
			for (int i = 0; i < a.size(); i++) {
				std::istringstream iss(a[i]);
				iss >> ab[i];
			}

			s = "";
			for (int i = 0; ab[i] > 0 && ab[i] < 1000; i++) {
				for (int j = 0; with_b[ab[i]][j] > 0; j++) {
					s += "," + to_string(with_b[ab[i]][j]);
				}
			}
			if (s.compare("") == 0) {
				dfa_tab[g - 1][2] = dfa_states[0];
				has_trap = true;
				//endWhile = true;
			}
			else {
				s = Common(s);
				int i;
				for (i = 0; i < 50; i++) {
					if (s.compare(dfa_states[i]) == 0) {
						dfa_tab[g - 1][2] = dfa_tab[i][0];
						//endWhile = true;
						break;
					}
				}
				if (i == 50) {
					dfa_tab[dfa_states_count][0] = s;
					dfa_tab[g - 1][2] = dfa_tab[dfa_states_count][0];
					dfa_states[dfa_states_count] = s;
					dfa_states_count++;
				}
			}
			if (g == dfa_states_count) {
				endWhile = true;
			}
		}



		g = oldStates;
		oldStates = dfa_states_count;
		endWhile = false;
		if (g == dfa_states_count) {
			break;
		}
		while (!endWhile) {
			s = dfa_states[g];
			g++;

			vector<std::string> a = Split(s, ",");
			ab = new int[a.size()];
			for (int i = 0; i < a.size(); i++) {
				std::istringstream iss(a[i]);
				iss >> ab[i];
			}

			s = "";
			for (int i = 0; ab[i] > 0 && ab[i] < 1000; i++) {
				for (int j = 0; with_a[ab[i]][j] > 0; j++) {
					s += "," + to_string(with_a[ab[i]][j]);
				}
			}
			if (s.compare("") == 0) {
				dfa_tab[g - 1][1] = dfa_states[0];
				has_trap = true;
				//endWhile = true;
			}
			else {
				s = Common(s);
				int i;
				for (i = 0; i < 50; i++) {
					if (s.compare(dfa_states[i]) == 0) {
						dfa_tab[g - 1][1] = dfa_tab[i][0];
						//endWhile = true;
						break;
					}
				}
				if (i == 50) {
					dfa_tab[dfa_states_count][0] = s;
					dfa_tab[g - 1][1] = dfa_tab[dfa_states_count][0];
					dfa_states[dfa_states_count] = s;
					dfa_states_count++;
				}
			}
			if (g == dfa_states_count) {
				endWhile = true;
			}
		}


	}

	//find final states in dfa
	for (int i = 1; i < dfa_states_count; i++) {
		vector<std::string> a = Split(dfa_states[i], ",");
		int *ba;
		ba = new int[a.size()];
		for (int i = 0; i < a.size(); i++) {
			std::istringstream iss(a[i]);
			iss >> ba[i];
		}

		for (int j = 0; j < nfa_final_count; j++) {
			for (int k = 0; k < a.size(); k++) {
				if (nfa_final_states[j] == ba[k]) {
					fin_dfa[dfa_final_count] = dfa_states[i];
					dfa_final_count++;
					j = nfa_final_count + 1;
					break;
				}
			}
		}
	}


	//rename states
	for (int i = 1; i < dfa_states_count; i++) {
		string tmp = dfa_states[i];
		dfa_states[i] = to_string(i);
		for (int j = 0; j < dfa_final_count; j++) {
			if (fin_dfa[j] == tmp) {
				fin_dfa[j] = to_string(i);
			}
		}
		dfa_tab[i][0] = to_string(i);

		for (int k = 0; k < dfa_states_count; k++) {
			if (dfa_tab[k][1] == tmp) {
				dfa_tab[k][1] = to_string(i);
			}
			if (dfa_tab[k][2] == tmp) {
				dfa_tab[k][2] = to_string(i);
			}
			if (dfa_tab[k][2] == "trap") {
				dfa_tab[k][2] = to_string(dfa_states_count);
			}
			if (dfa_tab[k][1] == "trap") {
				dfa_tab[k][1] = to_string(dfa_states_count);
			}
		}

	}


	if (has_trap) {
		dfa_states[0] = to_string(dfa_states_count);
		dfa_tab[dfa_states_count][0] = to_string(dfa_states_count);
		dfa_tab[dfa_states_count][1] = to_string(dfa_states_count);
		dfa_tab[dfa_states_count][2] = to_string(dfa_states_count);
	}
	else
		dfa_states[0] = "";

	print_dfa(has_trap);


	return has_trap;
}

void minimize_dfa(bool has_trap) {

	int minimized[50][50];
	int set1[50][50] = { 0 };
	int set2[50][50] = { 0 };
	int counter1[50] = { 0 };
	int counter2[50] = { 0 };
	int fi = 0, in = 0;
	int groups1 = 2;
	int groups2 = 0;
	bool first = true;

	int i = 0;
	if (!has_trap)
		i = 1;
	for (; i < dfa_states_count; i++) {
		bool final = false;
		for (int j = 0; j < dfa_final_count; j++) {
			if (dfa_states[i] == fin_dfa[j]) {
				set1[0][fi + 2] = stoi(dfa_states[i]);
				fi++;
				final = true;
			}
		}
		if (!final) {
			set1[1][in + 2] = stoi(dfa_states[i]);
			in++;
		}
		set1[0][49] = 1;
	}

	bool endWhile = false;
	while (!endWhile) {
		endWhile = true;
		//groups = 0;
		for (int i = 0; set1[i][2]>0; i++) {
			for (int j = 2; set1[i][j]>0; j++) {
				int a = stoi(dfa_tab[set1[i][j]][1]);
				int b = stoi(dfa_tab[set1[i][j]][2]);

				bool endfor = true;
				for (int d = 0; d < groups1 && endfor; d++) {
					for (int c = 2; set1[d][c] > 0; c++) {
						if (a == set1[d][c]) {
							a = d;
							endfor = false;
							break;
						}
					}
				}

				endfor = true;
				for (int d = 0; d < groups1 && endfor; d++) {
					for (int c = 2; set1[d][c] > 0; c++) {
						if (b == set1[d][c]) {
							b = d;
							endfor = false;
							break;
						}
					}
				}


				bool group_find = false;
				int k = 0;
				for (k = 0; k < groups2; k++) {
					if (set2[k][0] == a && set2[k][1] == b && set2[k][49] == set1[i][49]) {
						set2[k][counter2[k] + 2] = set1[i][j];
						group_find = true;
						counter2[k]++;
					}
				}
				if (!group_find) {
					groups2++;
					set2[k][0] = a;
					set2[k][1] = b;
					set2[k][2] = set1[i][j];
					set2[k][49] = set1[i][49];
					counter2[k]++;
					first = true;
				}
			}
		}
		for (int i = 0; i < groups1; i++) {
			for (int j = 0; j < 50; j++) {
				minimized[i][j] = set1[i][j];
				set1[i][j] = 0;
			}
		}
		for (int i = 0; i < 50; i++) {
			counter1[i] = 0;
		}
		if (groups1 == groups2) {
			endWhile = true;
		}
		else {
			groups1 = 0;
			endWhile = false;
		}

		if (!endWhile) {
			endWhile = true;
			for (int i = 0; set2[i][2] > 0; i++) {
				for (int j = 2; set2[i][j] > 0; j++) {
					int a = stoi(dfa_tab[set2[i][j]][1]);
					int b = stoi(dfa_tab[set2[i][j]][2]);

					bool endfor = true;
					for (int d = 0; d < groups2 && endfor; d++) {
						for (int c = 2; set2[d][c] > 0; c++) {
							if (a == set2[d][c]) {
								a = d;
								endfor = false;
								break;
							}
						}
					}

					endfor = true;
					for (int d = 0; d < groups2 && endfor; d++) {
						for (int c = 2; set2[d][c] > 0; c++) {
							if (b == set2[d][c]) {
								b = d;
								endfor = false;
								break;
							}
						}
					}


					bool group_find = false;
					int k = 0;
					for (k = 0; k < groups1; k++) {
						if (set1[k][0] == a && set1[k][1] == b && set1[k][49] == set2[i][49]) {
							set1[k][counter1[k] + 2] = set2[i][j];
							group_find = true;
							counter1[k]++;
						}
					}
					if (!group_find) {
						groups1++;
						set1[k][0] = a;
						set1[k][1] = b;
						set1[k][2] = set2[i][j];
						set1[k][49] = set2[i][49];
						counter1[k]++;
						first = true;
					}
				}
			}

			for (int i = 0; i < groups2; i++) {
				for (int j = 0; j < 50; j++) {
					minimized[i][j] = set2[i][j];
					set2[i][j] = 0;
				}
			}
			for (int i = 0; i < 50; i++) {
				counter2[i] = 0;
			}

			if (groups1 == groups2) {
				endWhile = true;
			}
			else {
				groups2 = 0;
				endWhile = false;
			}

		}

	}

	//find final states in minimized dfa
	dfa_final_count = 0;
	for (int k = 0; k < dfa_states_count; k++) {
		if (minimized[k][49] == 1) {
			fin_dfa[dfa_final_count] = to_string(minimized[k][2]);
			dfa_final_count++;
		}
		for (int l = 3; minimized[k][l] > 0; l++) {
			for (int i = 0; i < dfa_states_count; i++) {
				for (int j = 0; dfa_tab[i][j] != ""; j++) {
					if (dfa_tab[i][j] == to_string(minimized[k][l])) {
						dfa_tab[i][j] = to_string(minimized[k][2]);
					}
				}
			}
		}
	}


	print_minimized(has_trap);


}


string handle_tilde(string s) {
	char news[200];
	int j = 0;
	char e[20];
	int counter = 19;

	for (int i = 0; i < s.length(); i++) {
		if (s[i] == '~') {
			if (s[i - 1] == ')') {
				int openPar = 0;
				bool x = true;
				for (int g = i - 1; x; g--) {
					if (s[g] == ')') {
						openPar++;
						x = true;
					}
					if (s[g] == '(') {
						openPar--;
						if (openPar == 0)
							x = false;
					}
					e[counter] = s[g];
					counter--;
					j--;
				}
				int k;
				char f[20];
				for (k = 0; k < 20 - counter; k++) {
					f[k] = e[k + counter + 1];
				}

				news[j++] = '(';
				for (int h = 0; h < k; h++) {
					news[j++] = f[h];
				}
				j--;
				news[j++] = '|';
				for (int p = 0; p < 4; p++) {
					for (int h = 0; h < k; h++) {
						news[j++] = f[h];
					}
					news[j - 1] = '.';
				}
				j--;
				news[j++] = ')';

			}
			else if (s[i - 1] == 'a' || s[i - 1] == 'b') {
				j--;
				news[j++] = '(';
				news[j++] = s[i - 1];
				news[j++] = '|';
				news[j++] = s[i - 1];
				news[j++] = '.';
				news[j++] = s[i - 1];
				news[j++] = '.';
				news[j++] = s[i - 1];
				news[j++] = '.';
				news[j++] = s[i - 1];
				news[j++] = ')';
			}
		}
		else {
			news[j++] = s[i];
		}
	}
	news[j] = 0;
	return news;
}


int main() {

	int states = 0;
	cout << "Enter regex:" << endl;
	string s;
	cin >> s;

	s = add_concat(s);
	cout << "concatenation added: " << s << endl;

	s = handle_tilde(s);
	cout << "after tilde replacement: " << s << endl;

	s = postfix(s);
	cout << "postfix form: " << s << endl << endl << endl;

	states = reg_to_nfa(s, nfa_table);
	string start = fillTransitionLists(nfa_table, states);
	bool has_trap = nfa_to_dfa(start);
	minimize_dfa(has_trap);

	cin >> s;

	return 0;
}