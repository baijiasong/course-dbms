// some datatype used by sql.h
// author: dpatrickx

#ifndef AUXSQL_H
#define AUXSQL_H

#include "para.h"
#include <iostream>
#include <string>
#include <vector>
#include <stdlib.h>
using namespace std;


class TableCon{
public:
    vector<string> name;
    vector<string> type;
    vector<string> length;
    vector<bool>   notNull;
    string priKey;
    vector<string> checkAttrs;
    vector<vector<string> > checkVal;

    TableCon() {}

    void init(vector<string> n, vector<string> t, string p) {
        name = n;
        type = t;
        priKey = p;
    }

    void display() {
        cout<<"attr size is "<<name.size()<<endl;
        for (int i = 0; i < name.size(); i++) {
            cout<<i<<" :";
            cout<<name[i]<<' '<<type[i]<<' '<<notNull[i]<<endl;
        }
        cout<<"primary key is "<<priKey<<endl;
    }
};

// attr + 1 - 1
// 1 + 1 - 1
// -1
// 'sdfsdf'
class Expression {
public:
    vector<string> numbers;
    vector<string> ops;
    string str;
    int value;

    Expression() {
        str = "";
        value = 0;
    }
    Expression(vector<string> n, vector<string> o) {
        numbers = n;
        ops = o;
    }
    void init(vector<string> n, vector<string> o) {
        numbers = n;
        ops = o;
    }
    void transVal(string num) {
        value = atoi(num.c_str());
    }
    int isNull() {
        if (numbers.size()==0 and str=="" and value == 0)
            return 1;
        return 0;
    }
    void display() {
        if (numbers.size() == ops.size()) {
            for (int i = 0;i < numbers.size();i++) {
                cout<<ops[i]<<' '<<numbers[i]<<' ';

            }
        } else {
            int j = 0;
            for (int i = 0;i < numbers.size();i++) {
                if (j<ops.size() && ops[j]=="(") {
                    cout<<' '<<ops[j]<<' ';
                    j++;
                }
                cout<<numbers[i]<<' ';
                if (j<ops.size() && ops[j]==")") {
                    cout<<' '<<ops[j]<<' ';
                    j++;
                }
                if (j < ops.size()) {
                    cout<<ops[j]<<' ';
                    j++;
                }
            }
        }
        if (str != "")
            cout<<str;
        cout<<"\nvalue = "<<value<<endl;
    }
};
class AttrItem {
public:
    string tableName;
    string attrName;
    AttrItem() {
        tableName = "";
        attrName = "";
    }
    AttrItem(string s1, string s2) {
        tableName = s1;
        attrName = s2;
    }

    void init(string s1, string s2) {
        tableName = s1;
        attrName = s2;
    }

    int isNull() {
        if (tableName=="" && attrName=="")
            return 1;
        return 0;
    }

    void display() {
        if (attrName == "*") {
            cout<<"*";
            return;
        }
        if (tableName!="")
            cout<<tableName<<'.'<<attrName;
        else
            cout<<attrName;
    }
};
// attr1 >/</= attr2
// attr1 >/</= expression
class CondItem {
public:
    string judgeOp; // more/less/equl
    AttrItem attr1, attr2;
    Expression expression;
    CondItem() {}
    CondItem(string j, AttrItem a1, AttrItem a2, Expression e) {
        judgeOp = j;
        attr1 = a1;
        attr2 = a2;
        expression = e;
    }

    void init(string j, AttrItem a1, AttrItem a2, Expression e) {
        judgeOp = j;
        attr1 = a1;
        attr2 = a2;
        expression = e;
    }

    void display() {
        if (expression.isNull()) {
            attr1.display();
            cout<<" "<<judgeOp<<' ';
            attr2.display();
        } else {
            attr1.display();
            cout<<" "<<judgeOp<<" ";
            if (!attr2.isNull()) {
                attr2.display();
                cout<<' ';
            }
            expression.display();
        }
    }
};
// (tb1.attr1 judeop tb2.attr2) or/and/not ...
class CondSql {
public:
    vector<CondItem> conditions;
    vector<string> connops;  // OR AND NOT

    CondSql() {}
    CondSql(vector<CondItem> cond, vector<string> conn) {
        conditions = cond;
        connops = conn;
    }

    void display() {
        if (conditions.size() > 0) {
            for (int i = 0; i < conditions.size(); i++) {
                conditions[i].display();
                if (i != conditions.size()-1)
                    cout<<connops[i]<<' ';
            }
        }
    }
};
// kind iokind join tb2 condition
class JoinSql {
public:
    string kind;    // left/right/inner
    string iokind;  // inner/outer
    string tb2;
    CondSql condition; // on
    JoinSql() {
        kind = "";
        iokind = "";
    }
    JoinSql(string k, string i, string t, CondSql c) {
        kind = k;
        iokind = i;
        tb2 = t;
        condition = c;
    }

    int isNull() {
        if (kind=="" && iokind=="")
            return 1;
        return 0;
    }

    void init(string k, string i, string t, CondSql c) {
        kind = k;
        iokind = i;
        tb2 = t;
        condition = c;
    }

    void display() {
        cout<<kind<<' '<<iokind<<' '<<tb2<<' '<<"on"<<endl;
        condition.display();
    }
};

class OPS {
public:
    vector<string> ops;
    vector<string> names;
};

#endif