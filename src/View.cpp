#ifndef __VIEW_H__
#define __VIEW_H__

#include <iostream>
#include <string>
#include <map>
#include <vector>
#include <stack>
#include <set>
#include <iomanip>
#include "patternItem.cpp"
#include "regex.cpp"
#include "DocuSet.cpp"

using namespace std;
struct group {
    int start;
    int end;
    group(int s, int e) {
        start = s;
        end = e;
    }
};
struct text_data {
    int st, ed;
    text_data() {};
    text_data(int st, int ed) {
        this->st = st;
        this->ed = ed;
    }
    string getStr() {
        return string(text + st, text + ed);
    }
};

struct pattern_structure {
    text_data result_text_data;
    int pattern_number;
    vector<int*> pattern_position;
    vector<int> blank_number;
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
// Get the bit-number of the input-number
int get_number(int num) {
    if (num < 10)
        return 1;
    return get_number(num / 10) + 1;
}

void print_border(int cols, int* max) {
        int k = 0;
        for (k = 0; k < cols; k++) {
            if (k == cols -1) {
                cout << "+" << string(max[k]+3,'-') << "+" << endl;
            } else {
                cout << "+" << string(max[k]+3,'-');
            }
        }
}

DocuSet doc;
class View {
private:
    map<string, vector<text_data>> columns;
    string viewId;
    set<string> aliases;


    vector<token> text_tokens = doc.get_tokens();
    string fromViewId;
    string fromViewNickName;
    vector<ColumnNamePair> namePairs;
    string reg;
    string fileName;
    vector<patternItem> patterns;
    map<string, string> nick_to_real;

    map<int, string> groupNames;
    vector<group> groups;
    // 
    stack<int> groupPos;
public:
    // stores all the views with the viewId(viewName) as the key
    static map<string, View> viewManager;
    static char* text;
    // print the view in a special format
    static void print_view(string viewId, string nickName) {
        View * sourceView = View::getView(viewId);
        // Show the name of the view
        cout << "View: " << nickName << endl;

        map<string, vector<text_data>>::iterator it = sourceView->columns.begin();
        int cols = sourceView->columns.size();
        int max[cols];
        int temp = 0;
        int k = 0;
        int max_cols = 9999;
        string fileName = "noname";
        
        if (cols == 0 || sourceView->columns.begin()->second.size() == 0) {
            cout << "Empty set" << endl << endl;
            return;
        }
        it = sourceView->columns.begin();
        for (k = 0; k < cols; k++, it++) {
            vector<text_data>* output = &it->second;
            // Get the width of each row and find the max[k] value in the column
            max[k] = 0;
            for (int i = 0; i < output->size(); ++i) {
                temp = (*output)[i].getStr().size() + get_number((*output)[i].st)
                     + get_number((*output)[i].ed) + 3;
                if (temp > max[k])
                    max[k] = temp;
                if (output->size() < max_cols) max_cols = output->size();
            }
        }
        if (max_cols == 0) return;
        print_border(cols, max);
        // print cols name
        it = sourceView->columns.begin();
        for (k = 0; k < cols; k++, it++) {
            if (k == cols -1) {
                cout << "| " << left << setw(max[k]+1) << it->first << " |" << endl;
            } else {
                cout << "| " << left << setw(max[k] + 2) << it->first;
            }
        }
        print_border(cols, max);
        // print content
        int p;
        vector<text_data>* output[cols];
        for (k = 0, it = sourceView->columns.begin(); k < cols; k++, it++) {
            output[k] = &it->second;
        }
        for (p = 0; p < max_cols; p++) {
            for (k = 0; k < cols; k++) {
                temp = (*output[k])[p].getStr().size() + get_number((*output[k])[p].st)
                     + get_number((*output[k])[p].ed) + 3;
                if (k == cols -1) {
                    cout << "| " << left
                    //<< string(text+(*output[k])[p].st, text+(*output[k])[p].ed)
                    << (*output[k])[p].getStr()
                    << ":("
                    << (*output[k])[p].st
                    << ","
                    << (*output[k])[p].ed
                    << ")"
                    << right << setw(max[k] - temp + 2)  << "|" << endl;
                } else {
                    cout << "| " << left
                    //<< string(text+(*output[k])[p].st, text+(*output[k])[p].ed)
                    << (*output[k])[p].getStr()
                    << ":("
                    << (*output[k])[p].st
                    << ","
                    << (*output[k])[p].ed
                    << ")"
                    << right << setw(max[k] - temp + 3) ;
                }
            }
        }

        print_border(cols, max);
        cout <<  max_cols << " rows in set" << endl << endl;
    }

    // get the view from the given view name
    // return NULL if there is no the view
    static View * getView(string viewId) {
        map<string, View>::iterator iter = viewManager.find(viewId);
        if (iter == viewManager.end()) return NULL;
        else return &(iter->second);
    }

    View() {
        this->fileName = "noname";
    }

    static void put_view(View* v) {
        viewManager.insert(pair<string, View>(v->viewId, *v));
    }

    // return all the columns
    map<string, vector<text_data> > getAllColumns() {
        return columns;
    }
    void set_PatternGroup_st() {
        group temp(patterns.size(), -1);
        groupPos.push(groups.size());
        groups.push_back(temp);
    }

    void set_PatternGroup_ed() {
        if (groupPos.empty()) {
            cout << "Syntax error: Extrat )" << endl;
        } else {
            int index = groupPos.top();
            groupPos.pop();
            groups[index].end = patterns.size() - 1;
        }
    }
    // set all the columns
    void setAllColumns(map<string, vector<text_data> > columns) {
        this->columns = columns;
    }

    // get a column with a given column name
    // notice that before calling the method, we should use @hasColumn method to judge
    // whether this view has the column name with the given name
    // if there is no such a column with the given name, return an empty column,
    // but notice that this situation should not happen(because we have judged whether there is such column)
    vector<text_data> getColumn(string columnName) {
        map<string, vector<text_data> >::iterator iter = columns.find(columnName);
        if (iter != columns.end()) {
            return iter->second;
        }
        else {
            return vector<text_data>();
        }
    }

    // set a column with a given column name and a given column
    void setColumn(string columnName, vector<text_data> column) {
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
        nick_to_real[fromViewNickName] = fromViewId;
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
        if (false) {
            // the file name is unset
        }
        else {
            char * text = doc.get_content(fileName);
            vector<vector<int>> location = findall(regex.c_str(), text);
            columns.clear();
            vector<text_data> column;
            int size = location.size();
            for (int i = 0; i < size; i++) {
                int s = location[i][0];
                int e = location[i][1];
                string content = string(text + s, text + e);
                string temp = "";
                for (int j = DocuSet::tokens.size() - 1; j >= 0; j--) {
                    if (DocuSet::tokens[j].start == s) {
                        temp += DocuSet::tokens[j].content;
                        int end = DocuSet::tokens[j].end;
                        while (true) {
                            if (end == e) {
                                if (temp == content) {
                                    column.push_back(text_data(s, e));
                                }
                                break;
                            } else if (end > e) {
                                break;
                            } else if (end == DocuSet::tokens[j+1].start) {
                                j++;
                                temp += DocuSet::tokens[j].content;
                                end = DocuSet::tokens[j].end;
                            } else {
                                for(end; end < DocuSet::tokens[j+1].start; end++)
                                    temp += " ";
                            }
                        }
                        break;
                    }
                }
            }
            columns[groupNames[0]] = column;
        }
    }

    void setFileName(string newFileName) {
        this->fileName = newFileName;
    }

    void pattern_regex_action() {
        create_view_from_regex(get_reg());
    }
    /*
        add patternItem to Patterns
    */

    void addPatternItem(int type, string current_column, string view_nick_name) {
        patternItem temp(type, current_column, view_nick_name);
        patterns.push_back(temp);
    }
    void addPatternItem(int type, int start, int end) {
        patternItem temp(type, start, end);
        patterns.push_back(temp);
    }
    void addPatternItem(int type, string regex) {
        patternItem temp(type, regex);
        patterns.push_back(temp);
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
    void pattern_action() {
/*        View newView;
        if (!parserCompleted()) {
            return newView;
        }*/
        vector<pattern_structure> toltal_pattern_structure = match_range(0, patterns.size() - 1);
        /*****************This is the test output************************/
        /*for (int i = 0; i < toltal_pattern_structure.size(); i++) {
            pattern_structure temp = toltal_pattern_structure[i];
            cout  << temp.result_text_data.st << ' ' << temp.result_text_data.ed << endl;
            for (int j  = 0;j < temp.pattern_position.size(); j++) {
                cout << temp.pattern_position[j][0] << ' ' << temp.pattern_position[j][1] << endl;     
            }
        }

        cout << "The blank numbers" << endl;
        for (int i = 0; i < toltal_pattern_structure.size(); i++) {
            for (int j = 0; j < toltal_pattern_structure[i].blank_number.size(); j++) {
                cout << toltal_pattern_structure[i].blank_number[j] << ' ';
            }
            cout << endl;
        }*/
        /*************************output end*****************************/
        vector<text_data> total_text_data;
        for (int i = 0; i < toltal_pattern_structure.size(); i++) {
            total_text_data.push_back(toltal_pattern_structure[i].result_text_data);
        }
        /*
            extract all the groups from the toltal_pattern_structure;
        */
        for (int i = 0; i < groups.size(); i++) {
            int group_start = groups[i].start, group_end = groups[i].end;
            vector<text_data> each_group;
            for (int k = 0; k < toltal_pattern_structure.size(); k++) {
                text_data temp_text_data;
                int temp_start = toltal_pattern_structure[k].pattern_position[group_start][0];
                int temp_end = toltal_pattern_structure[k].pattern_position[group_start][1];
                string total_string = total_text_data[k].getStr();
                int total_start = total_text_data[k].st;
                temp_text_data.st = total_start + temp_start;
                temp_text_data.ed = temp_text_data.st + toltal_pattern_structure[k].pattern_position[group_end][1]-
                toltal_pattern_structure[k].pattern_position[group_start][0] + 1;
                each_group.push_back(temp_text_data);
            } 
            columns.insert(pair<string, vector<text_data> >(groupNames[i + 1], each_group));
        }
        columns.insert(pair<string, vector<text_data> >(groupNames[0], total_text_data));
    }
    /*
        input:the range of the patterns
        output: the result view
    */
    vector<pattern_structure> match_range(int start, int end) {
        vector<pattern_structure> empty;
        vector<pattern_structure> result;
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
            vector<text_data> second_pattern = deal_single_pattern(patterns[start + 1]);
            result = deal_token_pattern(s, e, second_pattern);
            start += 2;
        } else {
            vector<text_data> first_pattern = deal_single_pattern(patterns[start]);
            for (int i = 0; i < first_pattern.size(); i++) {
                int *position = new int[2];
                pattern_structure temp;
                temp.result_text_data = first_pattern[i];
                temp.pattern_number = 1;
                position[0] = 0;
                position[1] = first_pattern[i].getStr().length() - 1;
                temp.pattern_position.push_back(position);
                result.push_back(temp);
            }
            start += 1;
        }

        
        /*
                if the pattern type is a token type, then use the deal_token_pattern 
                to get the result column,  if not then use the edeal_single_pattern.
        */
        for (int i = start; i <= end; i++) {
            if (patterns[i].getType() == pattern_type.TOKEN) {
                int s = patterns[i].getStart(), e = patterns[i].getEnd();
                result = deal_token_pattern(result, s, e);
            } else {
                vector<text_data> temp = deal_single_pattern(patterns[i]);

                result = connect_two_column(result, temp);
            }
        }
        return result;
    }
    /*
        find the column that is corresponded to the pattern except the token.
    */
    vector<text_data> deal_single_pattern(patternItem pattern) {
        vector<text_data> result;
        /*
            if the pattern is the column of a view, then return the column of current_view
        */
        if (pattern.getType() == pattern_type.COLUMN) {
            string view_name = nick_to_real[pattern.getView()];
            View* current_view = View::getView(view_name);
            if (current_view->hasColumn(pattern.getColumn())) {
                result = current_view->getColumn(pattern.getColumn());
            }
        } else if (pattern.getType() == pattern_type.REGEX) {
            /*
                if the pattern is regex, then parse the regex from the text.
            */
            View temp1;
            temp1.set_reg(pattern.getRegex());
            temp1.create_view_from_regex(pattern.getRegex());
            if (temp1.columns.size() != 0) {
                map<string, vector<text_data> >::iterator it = temp1.columns.begin();
                result = it->second;
            }
        }

        return result;   
    }

    /*
        find the column that is corresponding to a token pattern.
    */

    vector<pattern_structure> deal_token_pattern(int start, int end, vector<text_data> adjcent) {
        vector<pattern_structure> result;
        int index = 0;
        for (int i = 0; i < adjcent.size(); i++) {
            index = 0;
            int new_end = adjcent[i].ed;
            for (; index < text_tokens.size(); index++) {
                if (text_tokens[index].start == adjcent[i].st) {
                    for (int k = start; k <= end; k++) {
                        if (index - k >= 0) {
                            int *position1 = new int[2];
                            int *position2 = new int[2];
                            pattern_structure temp_pattern;
                            int g = index - k;
                            string temp = text_tokens[g].content;
                            int token_end = text_tokens[g].end;
                            int temp_start = text_tokens[g].start;
                            //find the pattern that is corresponding to the tokens
                            for (g = g + 1; g < index; g++) {
                                for (int l = token_end; l < text_tokens[g].start; l++)
                                    temp += " ";
                                temp += text_tokens[g].content;
                                token_end = text_tokens[g].end;
                            }
                            position1[0] = 0;
                            position1[1] = temp.length() - 1;
                            temp_pattern.pattern_position.push_back(position1);
                            int index_end = text_tokens[index].start;
                            temp_pattern.blank_number.push_back(index_end - token_end);
                            position2[0] = position1[1] + index_end - token_end;
                            for (int l = token_end; l < index_end; l++)
                            temp += " ";
                            position2[1] = position2[0] + text_tokens[index].content.length() - 1;
                            temp_pattern.pattern_position.push_back(position2);
                            temp_pattern.pattern_number = 2;
                            text_data t(text_tokens[index - k].start, new_end);
                            temp_pattern.result_text_data = t;
                            result.push_back(temp_pattern);
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
    vector<pattern_structure> deal_token_pattern(vector<pattern_structure> adjcent, int start, int end) {
        vector<pattern_structure> result;
        int index = 0;
        for (int i = 0; i < adjcent.size(); i++) {
            index = 0;
            int new_start = adjcent[i].result_text_data.st;
            for (; index < text_tokens.size(); index++) {
                int adjcent_end = adjcent[i].result_text_data.ed;
                if (text_tokens[index].end == adjcent_end) {
                    for (int k = start; k <= end; k++) {
                        if (index + k < text_tokens.size()) {
                            int *position = new int[2];
                            pattern_structure temp_pattern = adjcent[i];
                            int g = index;
                            string temp1 = text_tokens[g].content;
                            temp_pattern.blank_number.push_back(text_tokens[g + 1].start - adjcent_end);
                            for (int l = text_tokens[g].end; l < text_tokens[g+1].start; l++)
                                temp1 += " ";
                            string temp = text_tokens[g + 1].content;
                            position[0] = temp1.length();
                            int token_end = text_tokens[g + 1].end;
                            for (g = g + 2; g <= index + k; g++) {
                                for (int l = token_end; l < text_tokens[g].start; l++)
                                    temp += " ";
                                temp += text_tokens[g].content;
                                token_end = text_tokens[g].end;
                            }
                            temp_pattern.pattern_number++;
                            position[1] = position[0] + temp.length() - 1;
                            temp_pattern.pattern_position.push_back(position);
                            text_data d(new_start, text_tokens[index + k].end);
                            temp_pattern.result_text_data = d;
                            result.push_back(temp_pattern);                        
                        }
                    }
                    break;
                }
            }
        }
        return result;
    }
    /*
        connect two column to one column with the position info
    */
    vector<pattern_structure> connect_two_column(vector<pattern_structure> first, vector<text_data> second) {

        vector<pattern_structure> result;
        int index = 0;
        for (int i = 0; i < first.size(); i++) { 
            index = 0;
            int correct_start = 0;
            int first_end = first[i].result_text_data.ed;
            /*
                find the token which end is corresponing to the first column, and 
                record the correct begin of next column. the first column end must
                 apear in the tokens' end, so when find, then break. 
            */
            for (; index < text_tokens.size() - 1; index++) {
                if (text_tokens[index].end == first_end) {
                    correct_start = text_tokens[index + 1].start;
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
                    int *position = new int[2];
                    pattern_structure temp_pattern = first[i];
                    string temp2 = first[i].result_text_data.getStr();
                    temp_pattern.pattern_number++;
                    temp_pattern.blank_number.push_back(second_start - first_end);

                    temp_pattern.pattern_position.push_back(position);
                    for (int l = first_end; l < second_start; l++)
                        temp2 += " ";
                    position[0] = temp2.length();
                    position[1] = position[0] + second[j].getStr().length() - 1;
                    text_data t(first[i].result_text_data.st, second[j].ed);
                    temp_pattern.result_text_data = t;

                    result.push_back(temp_pattern);
                } else if (second_start > correct_start) {
                    break;
                }
            }
        }

        return result;
    }
};
map<string, View> View::viewManager;
char * View::text = doc.get_content("");

#endif
