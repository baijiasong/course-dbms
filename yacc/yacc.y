/*  yacc file
    author: dpatrickx*/

%{
    #include "main.h"
    #include "../db/sql.h"
    extern "C"
    {
        void yyerror(const char *s);
        extern int yylex(void);
    }
%}

%token<m_sId>  INTEGER
%token<m_sId>  IDENTIFIER INSERT INTO VALUES YIN VALUEIT
%token<m_sId>  CREATE TABLE PRIMARY KEY TYPE NOTNULL
%token<m_sId>  KIND IO SELECT FROM WHERE EXPRESSION JOIN ON USE IF EXISTS DROP
%token<m_sId>  JUDEOP CONNOP UPDATE SET
%token<m_sId>  DELETE
%token<m_sId>  '+'  '-'  '*'  '/'  '%'  '='  '>'  '<'  '.'
%token<m_sId>  ','  ';'  '!'  '('  ')'  '['  ']'  '{'  '}' '?'
%token<m_cOp>  OPERATOR
%type<m_sql>   sqllist
%type<m_use>   usesql
%type<m_drop>  dropsql
%type<m_ins>   insertsql
%type<m_strv>  tableitems valueitems
%type<m_vecv>  valuesql
%type<m_con>   tablecon
%type<m_cre>   createsql
%type<m_sId>   valueitem

%start sqllist

%%
sqllist:
    {}
    | usesql sqllist
    | dropsql sqllist
    | createsql sqllist
    | insertsql sqllist;

usesql:
    USE IDENTIFIER ';'
    {
        $$.init($2);
        $$.display();
    };

dropsql:
    DROP TABLE IDENTIFIER ';' {
        $$.init($3);
        $$.display();
    }
    | DROP TABLE IF EXISTS IDENTIFIER ';' {
        $$.init($5, 1);
        $$.display();
    }

createsql:
    CREATE TABLE IDENTIFIER '(' tablecon ';' {
        $$.init($3, $5);
        $$.display();
    };
tablecon:
    IDENTIFIER TYPE NOTNULL ',' tablecon {
        $$ = $5;
        $$.name.push_back($1);
        $$.type.push_back($2);
        $$.notNull.push_back(true);
    }
    | IDENTIFIER TYPE ',' tablecon {
        $$ = $4;
        $$.name.push_back($1);
        $$.type.push_back($2);
        $$.notNull.push_back(false);
    }
    | IDENTIFIER TYPE NOTNULL ')' {
        $$.name.push_back($1);
        $$.type.push_back($2);
        $$.notNull.push_back(true);
    }
    | IDENTIFIER TYPE ')' {
        $$.name.push_back($1);
        $$.type.push_back($2);
        $$.notNull.push_back(false);
    }
    | PRIMARY KEY '(' IDENTIFIER ')' ')' {
        $$.priKey = $4;
    };

insertsql:
    INSERT INTO IDENTIFIER '(' tableitems ')' VALUES valuesql {
        InsertSql ins($3, $5, $8);
        ins.display();
    }
    | INSERT INTO IDENTIFIER VALUES valuesql {
        vector<string> temp;
        InsertSql ins($3, temp, $5);
        ins.display();
    };
tableitems:
    IDENTIFIER ',' tableitems {
        $$.push_back($1);
        $$.insert($$.end(), $3.begin(), $3.end());
    }
    | IDENTIFIER {
        $$.push_back($1);
    };
valuesql:
    '(' valueitems ')' ',' valuesql
    {
        $$.push_back($2);
        $$.insert($$.end(), $5.begin(), $5.end());
    }
    | '(' valueitems ')' ';'
    {
        $$.push_back($2);
    };
valueitems:
    valueitem ',' valueitems {
        $$.push_back($1);
        $$.insert($$.end(), $3.begin(), $3.end());
    }
    | valueitem {
        $$.push_back($1);
    };
valueitem:
    INTEGER {$$ = $1;}
    | VALUEIT {$$ = $1;};

%%

void yyerror(const char *s)
{
    cerr<<s<<endl;
}