#ifndef __TOKENIZER_H__
#define __TOKENIZER_H__
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <cstring>
using namespace std;
struct token {
	string content;
	int start, end;
	token(string a, int start, int end) {
		this->start = start;
		this->end = end;
		content = a;
	}
};
bool is_digit_or_letter(char a);
vector<token> tokenizer(const char* input) {
	int start = 0;
	int end = 0;
	vector<token> tokens;
	FILE* article =  fopen(input, "r");
	if (!article) {
		cout << "The file does not exsit" << endl;
		return tokens;
	}
	string temp = "";
	char oneword;
	while ((oneword = fgetc(article)) != EOF) {
			if (is_digit_or_letter(oneword)) {
				temp += oneword;
				end++;
			} else {
				if (temp != "") {
					token t(temp, start, end);
					tokens.push_back(t);
					start = end;
					temp = "";
				}
				end++;
				if (oneword != ' '&&oneword != '\r' && oneword != '\t' && oneword != '\n') {
					temp += oneword;
					token t(temp, start, end);
					tokens.push_back(t);
				}
					start = end;
					temp = "";
				}
	}

	if (temp != "") {
		token t(temp, start, end);
		tokens.push_back(t);
	}
	fclose(article);
	return tokens;

}

bool is_digit_or_letter(char a) {
	if ((a >= '0' && a <= '9') || (a >= 'a' && a <= 'z') || (a > 'A' && a < 'Z')) {
		return true;
	}

	return false;
}
#endif
