%skeleton "lalr1.cc"
%require "3.5"

%defines
%define api.token.constructor
%define api.value.type variant
%define parse.assert

%code requires {
    #include <string>
    class Scanner;
    class Driver;
    class Expression;
    class Integer;
}

// %param { Driver &drv }

%define parse.trace
%define parse.error verbose

%code {
    #include "driver.hh"
    #include "location.hh"
    #include "operators.hh"
    #include "types.hh"

    static yy::parser::symbol_type yylex(Scanner &scanner, Driver& driver) {
        return scanner.ScanToken();
    }
}

%lex-param { Scanner &scanner }
%lex-param { Driver &driver }
%parse-param { Scanner &scanner }
%parse-param { Driver &driver }

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
%nterm <Expression*> exp
%nterm <std::vector<std::string>> declaration_list
%nterm <std::string> var_type
%nterm <std::vector<Expression*>> exp_list

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
    | declarations statements {}

declarations:
    %empty {}
    | declarations declaration NEWLINE {}

declaration:
    %empty {}
    | var_type DOUBLE_COLON declaration_list {
        for (const std::string& varName : $3) {
            driver.AddOperator(new Declaration(varName, $1));
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
    | statements statement NEWLINE {}

statement:
    %empty {}
    | "identifier" ASSIGN exp {
        driver.AddOperator(new AssignOperator($1, $3));
    }
    | if_statement {
    }
    | PRINT STAR exp_list {
        driver.AddOperator(new PrintOperator($3));
    }

exp_list:
    %empty {
        $$ = std::vector<Expression*> ();
    }
    | exp_list COMMA exp {
        $1.push_back($3);
        $$ = $1;
    }

%left "+" "-";
%left "*" "/";

if_statement:
    IF "(" exp ")" THEN NEWLINE statements else_if_statements {}

else_if_statements:
    else_statement {}
    | ELSE IF "(" exp ")" THEN NEWLINE statements else_if_statements {}

else_statement:
    END IF {}
    | ELSE NEWLINE statements END IF {}

exp:
    "number" {$$ = new ValueExpression(new Integer($1));}
    | "identifier" {$$ = new IdentifierExpression($1);}
    | exp "+" exp {$$ = new SumExpression($1, $3); }

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
