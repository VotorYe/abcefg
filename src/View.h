#pragma once

#include <string>
#include <map>
#include <vector>
#include <set>
#include "regex.cpp"
#include "DocuSet.h"

using namespace std;

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

class View {
private:
    map<string, vector<Data> > columns;
    string viewId;
    set<string> aliases;

    string fromViewId;
    string fromViewNickName;
    vector<ColumnNamePair> namePairs;

    string reg;
    map<int, string> groupNames;

    string fileName;

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

    View(string fileName) {
        this->fileName = fileName;
    }

    // return all the columns
    map<string, vector<Data> > getAllColumns() {
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
            char * text = DocuSet::get_content(fileName);
            vector<vector<int>> location = findall(regex.c_str(), text);
            columns.clear();
            vector<Data> column;
            int size = location.size();
            for (int i = 0; i < size; i++) {
                string content = string(text + s, text + e);
                int s = location[i][0];
                int e = location[i][1];
                for (int j = DocuSet::tokens.size() - 1; j >= 0; j--) {
                    if (DocuSet::tokens[j].content == content
                        && DocuSet::tokens[j].start == s
                        && DocuSet::tokens[j].end == e) {
                        column.push_back(Data(content, s, e));
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
        if ("" == fileName) {
            // the file name is unset
        }
        else {
            char * text = DocuSet::get_content(fileName);
            vector<vector<int>> location = findall(reg.c_str(), text);
            columns.clear();
            vector<Data> column;
            int size = location.size();
            for (int i = 0; i < size; i++) {
                string content = string(text + s, text + e);
                int s = location[i][0];
                int e = location[i][1];
                for (int j = DocuSet::tokens.size() - 1; j >= 0; j--) {
                    if (DocuSet::tokens[j].content == content
                        && DocuSet::tokens[j].start == s
                        && DocuSet::tokens[j].end == e) {
                        column.push_back(Data(content, s, e));
                        break;
                    }
                }
            }
            columns[groupNames[0]] = column;
        }
    }

    void pattern_action() {

    }

    void set_PatternGroup_st() {

    }
    
    void set_PatternGroup_ed() {

    }

    void addPatternItem() {

    }

};