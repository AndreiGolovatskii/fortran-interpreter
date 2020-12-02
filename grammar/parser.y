%skeleton "lalr1.cc"
%require "3.5"

%defines
%define api.token.constructor
%define api.value.type variant
%define parse.assert

%code requires {
    #include <string>
    class Scanner;
    class TDriver;
    #include "statements_forward_declaration.hh"
}

// %param { TDriver &drv }

%define parse.trace
%define parse.error verbose

%code {
    #include "driver.hh"
    #include "location.hh"
    #include "statements_forward_declaration.hh"
    #include "types.hh"

    static yy::parser::symbol_type yylex(Scanner &scanner, TDriver& driver) {
        return scanner.ScanToken();
    }
}

%lex-param { Scanner &scanner }
%lex-param { TDriver &driver }
%parse-param { Scanner &scanner }
%parse-param { TDriver &driver }

%locations

%define api.token.prefix {TOK_}

%token
    ASSIGN "="
    COMMA ","
    DOUBLE_COLON
    END "end"
    EOF 0 "end of file"
    MINUS "-"
    NEWLINE "newline"
    PLUS "+"
    PROGRAM "program"
    STAR "*"
    SLASH "/"
    LPAREN "("
    RPAREN ")"
    PRINT
    INTEGER
    IF
    ELSE
    THEN
;

%token <std::string> IDENTIFIER "identifier"
%token <int> NUMBER "number"
%nterm <TExpression*> exp
%nterm <std::vector<std::string>> declaration_list
%nterm <std::string> var_type
%nterm <std::vector<TExpression*>> exp_list
%nterm <std::vector<TStatement*>> statements
%nterm <TStatement*> statement
%nterm <TIfStatement*> if_part
%nterm <TIfStatement*> else_if_part
%nterm <TIfStatement*> if_statement

//%printer { yyo << $$; } <*>;

%%
%start unit;
unit:
    "program" "identifier" "newline" operators "end" "program" "identifier" mb_newlines {
        assert($2.size() == $7.size());
        for (size_t i = 0; i < $2.size(); ++i) {
            assert(std::tolower($2[i]) == std::tolower($7[i]));
        }
    }
    | "program" "identifier" "newline" operators "end" "program" mb_newlines {}
    | "program" "identifier" "newline" operators "end" mb_newlines {}
    | operators "end" "program" mb_newlines {}
    | operators "end" mb_newlines {}

mb_newlines:
    %empty {}
    | "newline" mb_newlines {}

operators:
    %empty {}
    | declarations statements {
        for (auto* op : $2) {
            driver.AddStatement(op);
        }
    }

declarations:
    %empty {}
    | declarations declaration NEWLINE {}

declaration:
    %empty {}
    | var_type DOUBLE_COLON declaration_list {
        for (const std::string& varName : $3) {
            driver.AddStatement(new TDeclaration(varName, $1));
        }
    }


var_type:
    INTEGER {$$ = "integer";}


declaration_list:
    "identifier" {
        std::vector<std::string> list;
        list.push_back($1);
        $$ = list;
    }
    | declaration_list "," "identifier" {
        $1.push_back($3);
        $$ = $1;
    }

statements:
    %empty {}
    | statements statement NEWLINE {
        $1.push_back($2);
        $$ = $1;
    }

statement:
    %empty {}
    | "identifier" ASSIGN exp {
        $$ = new TAssignStatement($1, $3);
    }
    | if_statement {
        $$ = $1;
    }
    | PRINT STAR exp_list {
        $$ = new TPrintStatement($3);
    }

exp_list:
    %empty {
        $$ = std::vector<TExpression*> ();
    }
    | exp_list COMMA exp {
        $1.push_back($3);
        $$ = $1;
    }

%left "+" "-";
%left "*" "/";

if_statement:
    else_if_part {$$ = $1;}

if_part:
    IF "(" exp ")" THEN NEWLINE statements {
        TIfStatement* stat = new TIfStatement();
        stat->AddIf($3, std::move($7));
        $$ = stat;
    }

else_if_part:
    if_part {
        $$ = $1;
    }
    | else_if_part ELSE IF "(" exp ")" THEN NEWLINE statements {
        $1->AddIf($5, std::move($9));
        $$ = $1;
    }
    | else_if_part ELSE NEWLINE statements END IF {
        $1->AddElse(std::move($4));
        $$ = $1;
    }
    | else_if_part END IF {
        $$ = $1;
    }

exp:
    "number" {$$ = new TValueExpression(new TInteger($1));}
    | "identifier" {$$ = new TIdentifierExpression($1);}
    | exp "+" exp {$$ = new TSumExpression($1, $3); }

    /* TODO
    | exp "-" exp {$$ = $1 - $3; }
    | exp "*" exp {$$ = $1 * $3; }
    | exp "/" exp {$$ = $1 / $3; }
    | "(" exp ")" {$$ = $2; };
    */

%%

void
yy::parser::error(const location_type& l, const std::string& m)
{
  std::cerr << l << ": " << m << '\n';
}