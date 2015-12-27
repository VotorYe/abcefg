/*
	patternItem is a class to store the pattern items.
*/
#pragma once

#include <string>
#include "View.h"

using namespace std;

struct  TYPE {
		const int COLUMN = 0;
		const int TOKEN = 1;
		const int REGEX = 2;
} pattern_type;

class patternItem {
	/*
		type is a type of the pattern, 0:the column of view; 1:token; 2:regex
		view_nick_name is the view_nick_name to be searched, current_column is the view;
		start and end is the range of the number of tokens
	*/
	private:
		int type;
		int start;
		int end;
		string view_nick_name;
		string current_column;
		string regex;
	public:
		patternItem(int type, string current_column, string view_nick_name) {
			this.type = type;
			this.current_column = current_column;
			this.view_nick_name = view_nick_name;
		}

		patternItem(int type, int start, int end) {
			this.type = type;
			this.start = start;
			this.end = end;
		}

		patternItem(int type, string regex) {
			this.type = type;
			this.regex = regex;
		}
		/*
			get the items from the class;
		*/
		int getType() {return type;}
		int getStart() {return start;}
		int getEnd() {return end;}
		string getView() {return view_nick_name;}
		string getColumn() {return current_column;}
		string getRegex() {return regex;}


};