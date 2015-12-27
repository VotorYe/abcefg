#pragma once
#include "Lexer.h"
#include "Token.h"
#include "Tag.h"
#include "View.h"
//#include "PatternItem.h"

#include <map>

class Parser {
private:
    Lexer* lex;
    Token look = Token(1);
    map<string, View> viewList;
    View currentView;
public:
    Parser(Lexer* l) {this->lex = l; move();}
    void move() {this->look = lex->scan();}
    void match(int t) {
        if (look.tag == t) move();
        else {} //error handle
    }

    void program() {
        while (look.tag != Tag.END) {
            currentView = View();

        }
    }

    void aql_stmt() {
        // aql_stmt
        if (look.tag == Tag.CREATE) {
            create_stmt();
            match(Tag.SEMICOLON);
        } else if(look.tag == Tag.OUTPUT) {
            output_stmt();
            match(Tag.SEMICOLON);
        }
    }

    void create_stmt() {
        match(Tag.CREATE); match(Tag.VIEW);
        currentView.setViewId(look.get_lexeme()); match(Tag.ID);
        match(Tag.AS);
        view_stmt();
    }

    void view_stmt() {
        if (look.tag == Tag.SELECT) {
            select_stmt();
        } else if (look.tag == Tag.EXTRACT) {
            extract_stmt();
        }
    }

    void select_stmt() {
        match(Tag.SELECT);

        select_list();
        match(Tag.FROM);
        from_item();
        currentView.select_action();
    }

    void select_list() {
        select_item();
        if (look.tag == Tag.COMMA) {
            match(Tag.COMMA);
            select_list();
        }
    }

    void select_item() {
        string viewName = look.get_lexeme(); move();
        match(Tag.DOT);
        string header = look.get_lexeme();  move();
        if (look.tag == Tag.AS) {
            match(Tag.AS);
            currentView.add_select_item(viewName, header, look.get_lexeme()); // newHeader
            match(Tag.ID);
        } else {
            currentView.add_select_item(viewName, header, header);
        }
    }

    void from_item() {
        string fullViewName = look.get_lexeme(); match(Tag.ID);
        currentView.set_selectFrom_viewID(fullViewName, look.get_lexeme()); // nickName
        match(Tag.ID);
    }

    void extract_stmt() {
        match(Tag.EXTRACT);
        extract_spec();
        match(Tag.FROM);
        from_item();
    }

    void extract_spec() {
        if (look.tag == Tag.REGEX) {
            regex_spec();
        } else if (look.tag == Tag.PATTERN) {
            pattern_spec();
        } else {
            // error handle
        }
    }

    void regex_spec() {
        match(Tag.REGEX);
        currentView.set_reg(look.get_lexeme()); match(Tag.REG);
        match(Tag.ON);
        match(Tag.ID);  // doc Nick name
        match(Tag.DOT);
        match(Tag.ID);  // text

        name_spec();
    }

    void name_spec() {
        if (look.tag == Tag.AS) {
            match(Tag.AS);
            string headerName = look.get_lexeme(); match(Tag.ID);
            currentView.add_group(0, headerName);
        } else if (look.tag == Tag.RETURN) {
            match(Tag.RETURN);
            group_spec();
        } else {
            // error
        }
    }

    void group_spec() {
        single_group();
        if (look.tag == Tag.AND) {
            match(Tag.AND);
            group_spec();
        }
    }

    void single_group() {
        match(Tag.GROUP);
        int group_num = look.get_value(); match(Tag.NUM);
        match(Tag.AS);
        currentView.add_group(group_num, look.get_lexeme());
        match(Tag.ID);
    }

    void pattern_spec() {
        match(Tag.PATTERN);
        pattern_expr();
        name_spec();
    }

    void pattern_expr() {
        pattern_pkg();
        if (look.tag != Tag.FROM) {
            pattern_expr();
        }
    }

    void pattern_pkg() {
        if (look.tag == Tag.LEFTPARENTHESIS) {
            pattern_group();
        } else {
            atom();
        }
    }

    void pattern_group() {
        match(Tag.LEFTPARENTHESIS);
        currentView.set_PatternGroup_st();
        pattern_expr();
        match(Tag.RIGHTPARENTHESIS);
        currentView.set_PatternGroup_ed();
    }

    void atom() {
        if (look.tag == Tag.LESS) {
            match(Tag.LESS);
            if (look.tag == Tag.ID) {
                string viewNickName = look.get_lexeme();
                match(Tag.ID); match(Tag.DOT);

                currentView.addPatternItem(pattern_type.COLUMN, viewNickName, look.get_lexeme());
                match(Tag.ID); match(Tag.GREATER);
            } else if (look.tag == Tag.TOKEN) {
                match(Tag.TOKEN); match(Tag.GREATER); match(Tag.LEFTBRACE);
                int min = look.get_value();
                match(Tag.NUM); match(Tag.COMMA);

                currentView.addPatternItem(pattern_type.TOKEN, min, look.get_value());
                match(Tag.NUM); match(Tag.RIGHTBRACE);
            }
        } else if (look.tag == Tag.REG) {
                currentView.addPatternItem(pattern_type.REGEX, look.get_lexeme()); match(Tag.REG);
        }
    }

    void output_stmt() {
        match(Tag.OUTPUT); match(Tag.VIEW);
        string viewId = look.get_lexeme(); match(Tag.ID);
        string nickName = "";
        if (look.tag == Tag.AS) {
            match(Tag.AS);
            View::print_view(viewId, look.get_lexeme());
        } else {
            nickName = viewId;

        }
    }
};
