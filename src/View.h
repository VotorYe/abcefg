#pragma once

#include <string>
#include <map>
#include <vector>
#include <set>
#include "patternItem.h"
#include "regex.cpp"
using namespace std;
struct group {
    int start;
    int end;
    group(){start = 0; end = 0;}
};
struct Data {
    string str;
    int st, ed;
    Data() {};
    Data(string str, int st, int ed) {
        this->str = str;
        this->st = st;
        this->ed = ed;
    }
};

struct ColumnNamePair {
    string from;
    string to;
    ColumnNamePair() {};
    ColumnNamePair(string from, string to) {
        this->from = from;
        this->to = to;
    }
};
const static vector<token> text_tokens = get_tokens();
class View {
private:
    map<string, vector<Data>> columns;
    string viewId;
    set<string> aliases;

    string fromViewId;
    string fromViewNickName;
    vector<ColumnNamePair> namePairs;
    string reg;
    string fileName;
    map<int, string> groupNames;
    vector<patternItem> patterns;
    map<string, string> nick_to_real;
public:
    // stores all the views with the viewId(viewName) as the key
    static map<string, View> viewManager;
    
    // print the view in a special format
    static void print_view(string viewId, string nickName) {
        
    }

    // get the view from the given view name
    // return NULL if there is no the view
    static View * getView(string viewId) {
        map<string, View>::iterator iter = viewManager.find(viewId);
        if (iter == viewManager.end()) return NULL;
        else return &(iter->second);
    }

    View() {
    }

    // return all the columns
    map<string, vector<Data> > getAllColumns(//////////////////////////) {
        return columns;
    }

    // set all the columns
    void setAllColumns(map<string, vector<Data> > columns) {
        this->columns = columns;
    }

    // get a column with a given column name
    // notice that before calling the method, we should use @hasColumn method to judge
    // whether this view has the column name with the given name
    // if there is no such a column with the given name, return an empty column,
    // but notice that this situation should not happen(because we have judged whether there is such column)
    vector<Data> getColumn(string columnName) {
        map<string, vector<Data> >::iterator iter = columns.find(columnName);
        if (iter != columns.end()) {
            return iter->second;
        }
        else {
            return vector<Data>();
        }
    }

    // set a column with a given column name and a given column
    void setColumn(string columnName, vector<Data> column) {
        columns[columnName] = column;
    }

    // whether this view has the column with the given name
    bool hasColumn(string columnName) {
        return columns.find(columnName) != columns.end();
    }

    // set the view id
    void setViewId(string viewId) {
        this->viewId = viewId;
    }

    // get the view id
    string getViewId() {
        return viewId;
    }
 
    // set the alias
    void setAliases(set<string> aliases) {
        this->aliases = aliases;
    }

    // get the alias
    set<string> getAliases() {
        return aliases;
    }

    // judge whether this view has the given alias
    bool hasAlias(string alias) {
        return aliases.find(alias) != aliases.end();
    }

    // get the columns' name in source view and the new names in this view
    void add_select_item(string fromViewName, string header, string newHeaderName) {
        fromViewId = fromViewId;
        namePairs.push_back(ColumnNamePair(header, newHeaderName));
    }

    // get the full name and nick name of the source view
    void set_selectFrom_viewID(string fromFullViewName, string fromNickViewName) {
        this->fromViewId = fromFullViewName;
        this->fromViewNickName = fromNickViewName;
    }

    // 1. the name of the new view is set(this is done in @setViewId)
    // 2. create the columns from the namePairs and the fromViewId
    void select_action() {
        columns.clear();
        View * sourceView = View::getView(fromViewId);
        if (sourceView == NULL) {
            // there is no source view
        }
        else {
            for (int i = namePairs.size() - 1; i >= 0; i--) {
                columns[namePairs[i].to] = sourceView->getColumn(namePairs[i].from);
            }
        }
    }

    // 
    void add_group(int groupId, string groupName) {
        groupNames[groupId] = groupName;
    }
        
    // set the reg
    void set_reg(string reg) {
        this->reg = reg;
    }

    // get the reg
    string get_reg() {
        return reg;
    }

    // construct this view from a regex(we get the text pointer in the function)
    // how do we get the text? we get it from a fileName
    // notice that the file name is set in the constructor @View(string fileName)
    // and of course, you can set the file name by @setFileName(string newFileName)
    void create_view_from_regex(string regex) {
        if ("" == fileName) {
            // the file name is unset
        }
        else {
            char * text = DOC::getFile(fileName);
            vector<vector<int>> location = findall(regex.c_str(), text);
            columns.clear();
            vector<Data> column;
            int size = location.size();
            for (int i = 0; i < size; i++) {
                int s = location[i][0];
                int e = location[i][1];
                column.push_back(Data(string(text + s, text + e), s, e));
            }
            columns[groupNames[0]] = column;
        }
    }

    void setFileName(string newFileName) {
        this->fileName = newFileName;
    }


    /*
       check if the parse completed;
    */
    bool parserCompleted() {
        int view_pattern_number = 0;
        for (int i = 0; i < patterns.size(); i++) {
            if (patterns[i].getType() == pattern_type.COLUMN)
                view_pattern_number++;
        }

        if (view_pattern_number == nick_to_real.size())
            return true;
        return false;
    }

    /*
            match the patterns with the items of text;
    */
    View match_patterns() {
        View newView;
        if (!parserCompleted()) {
            return newView;
        }


    }
    /*
        input:the range of the patterns
        output: the result view
    */
    vector<Data> match_range(int start, int end) {
        vector<Data> empty;
        vector<Data> result;
        /*
            if the first pattern is token pattern, then this is a special case, so 
            we deal with it specially.
        */
        if (patterns[start].getType() == pattern_type.TOKEN) {
            if (patterns[start + 1].getType() == pattern_type.TOKEN) {
                cout << "Error! the two token pattern cannot be adjcent!" << endl;
                return empty;
            }
            int s = patterns[start].getStart(), e = patterns[start + 1].getEnd();
            result = deal_single_pattern(patterns[start + 1]);
            result = deal_token_pattern(s, e, result);
            start += 2;
        } else {
            result = deal_single_pattern(patterns[start]);
            start += 1;
        }

        /*
                if the pattern type is a token type, then use the deal_token_pattern 
                to get the result column,  if not then use the edeal_single_pattern.
        */
        for (int i = start; i < end; i++) {
            if (patterns[i].getType() == pattern_type.TOKEN) {
                int s = patterns[i].getStart(), e = patterns[i].getEnd();
                result = deal_token_pattern(result, s, e);
            } else {
                vector<Data> temp = deal_single_pattern(patterns[i]);
                result = connect_two_column(temp, result);
            }
        }

        return result;
    }
    /*
        find the column that is corresponded to the pattern except the token.
    */
    vector<Data> deal_single_pattern(patternItem pattern) {
        vector<Data> result;
        /*
            if the pattern is the column of a view, then return the column of current_view
        */
        if (pattern.getType() == pattern_type.COLUMN) {
            string view_name = nick_to_real[pattern.getView()];
            view current_view = View::getView(view_name);
            if (current_view.hasColumn(pattern.getColumn())) {
                result = current_view.getColumn(pattern.getColumn());
            }
        } else if (pattern.getType() == pattern_type.REGEX) {
            /*
                if the pattern is regex, then parse the regex from the text.
            */
            View current_view = create_view_from_regex(pattern.getRegex());
            if (current_view.size() != 0) {
                map<string, vector<Data>>::iterator it = current_view.begin();
                result = it->second;
            }
        }

        return result;   
    }

    /*
        find the column that is corresponding to a token pattern.
    */

    vector<Data> deal_token_pattern(int start, int end, vector<Data> adjcent) {
        vector<Data> result;
        int index = 0;
        for (int i = 0; i < adjcent.size(); i++) {
            int new_end = adjcent[i].ed;
            for (; index < text_tokens.size(); index++) {
                if (text_tokens[index].start == adjcent[i].st) {
                    for (int k = start; k <= end; k++) {
                        if (index - k >= 0) {
                            string temp = "";
                            for (int g = index - k; g <= index; g++)
                                temp += text_tokens[g].content;
                            Data d(temp, text_tokens[index - k].start, new_end);
                            result.push_back(d);
                        }
                    }
                    break;
                }
            }
        }

        return result;
    }
    /*
        find the column that is corresponding to a token pattern.
    */
    vector<Data> deal_token_pattern(vector<Data> adjcent, int start, int end) {
        vector<Data> result;
        int index = 0;
        for (int i = 0; i < adjcent.size(); i++) {
            int new_start = adjcent[i].st;
            for (; index < text_tokens.size(); index++) {
                if (text_tokens[index].end == adjcent[i].ed) {
                    for (int k = start; k <= end; k++) {
                        if (index + k < text_tokens.size()) {
                            string temp = "";
                            for (int g = index; g <= index + k; g++) {
                                temp += text_tokens[g].content;
                            }

                            Data d(temp, new_start, text_tokens[index + k].end);
                            result.push_back(d);
                        }
                    }
                }
                break;
            }
        }
        return result;
    }
    /*
        connect two column to one column with the position info
    */
    vector<Data> connect_two_column(vector<Data> first, vector<Data> second) {
        vector<Data> result;
        int index = 0;
        for (int i = 0; i < first.size(); i++) {
            int correct_start = 0;
            int first_end = first[i].ed;
            /*
                find the token which end is corresponing to the first column, and 
                record the correct begin of next column. the first column end must
                 apear in the tokens' end, so when find, then break. 
            */
            for (; index < text_tokens.size(); index++) {
                if (text_tokens[i].end == first_end) {
                    correct_start = text_tokens[i + 1].start;
                    break;
                }
            }

            /*
                find the correct the column of second corresponding to correct_start.
                if find then add it to new column.else continue.
            */
            for (int j = 0; j < second.size(); j++) {
                int second_start = second[j].st;
                if (second_start == correct_start) {
                    Data temp(first[i].str + second[j].str, first[i].start, second[j].end);
                    result.push_back(temp);
                } else if (second_start > correct_start) {
                    break;
                }
            }
        }

        return result;
    }
};