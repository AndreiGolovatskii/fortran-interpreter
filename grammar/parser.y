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
    #include "types.hh"
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
    EOF 0 "end of file"

    COMMA ","

    PROGRAM "program"
    END "end"
    NEWLINE "newline"

    ASSIGN "="
    MINUS "-"
    PLUS "+"
    SLASH "/"
    STAR "*"
    GT ">"
    LT "<"
    EQV
    NOT
    AND
    OR

    LPAREN "("
    RPAREN ")"
    DOUBLE_COLON "::"
    QUOTE "'"
    DOUBLE_QUOTE "\""

    IF
    ELSE
    THEN
    DO

    INTEGER
    CHARACTER
    LOGICAL

    TRUE
    FALSE

    PRINT
;

%token <std::string> IDENTIFIER "identifier"
%token <int> NUMBER "number"
%token <std::string> STRING

%nterm <std::shared_ptr<TTypeDescription>> var_type
%nterm <std::vector<std::string>> type_unnamed_parameters
%nterm <std::vector<std::string>> paren_type_unnamed_parameters
%nterm <std::vector<std::pair<std::string, std::string>>> type_named_parameters
%nterm <std::vector<std::pair<std::string, std::string>>> paren_type_named_parameters

%nterm <std::unique_ptr<TExpression>> exp
%nterm <std::vector<std::unique_ptr<TExpression>>> exp_list

%nterm <std::vector<std::unique_ptr<TStatement>>> declarations
%nterm <std::vector<std::unique_ptr<TStatement>>> declaration
%nterm <std::vector<std::string>> declaration_list

%nterm <std::vector<std::unique_ptr<TStatement>>> statements
%nterm <std::unique_ptr<TStatement>> statement

%nterm <std::unique_ptr<TIfStatement>> if_statement
%nterm <std::unique_ptr<TIfStatement>> if_part
%nterm <std::unique_ptr<TIfStatement>> else_if_part

%nterm <std::unique_ptr<TDoLoopStatement>> do_loop
%nterm <std::unique_ptr<TExpression>> do_loop_step

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
        driver.SetDeclarations(std::move($1));
        driver.SetStatements(std::move($2));
    }

declarations:
    %empty {}
    | declarations declaration NEWLINE {
        for (std::unique_ptr<TStatement>& declaration : $2) {
            $1.push_back(std::move(declaration));
        }
        $$ = std::move($1);
    }

declaration:
    %empty {}
    | var_type DOUBLE_COLON declaration_list {
        std::vector<std::unique_ptr<TStatement>> declarations;
        for (const std::string& varName : $3) {
            declarations.push_back(std::make_unique<TDeclaration>(varName, $1));
        }
        $$ = std::move(declarations);
    }
    | var_type declaration_list {
        std::vector<std::unique_ptr<TStatement>> declarations;
        for (const std::string& varName : $2) {
            declarations.push_back(std::make_unique<TDeclaration>(varName, $1));
        }
        $$ = std::move(declarations);
    }

var_type:
    INTEGER paren_type_named_parameters {
        $$ = std::shared_ptr<TTypeDescription>(std::make_unique<TIntegerDescription>($2));
    }
    | INTEGER paren_type_unnamed_parameters {
         $$ = std::shared_ptr<TTypeDescription>(std::make_unique<TIntegerDescription>($2));
    }
    | CHARACTER paren_type_named_parameters {
        $$ = std::shared_ptr<TTypeDescription>(std::make_unique<TCharacterDescription>($2));
    }
    | CHARACTER paren_type_unnamed_parameters {
        $$ = std::shared_ptr<TTypeDescription>(std::make_unique<TCharacterDescription>($2));
    }
    | LOGICAL paren_type_unnamed_parameters {
        $$ = std::shared_ptr<TTypeDescription>(std::make_unique<TLogicalDescription>($2));
    }
    | LOGICAL paren_type_named_parameters {
        $$ = std::shared_ptr<TTypeDescription>(std::make_unique<TLogicalDescription>($2));
    }


paren_type_named_parameters:
    %empty {
        $$ = std::vector<std::pair<std::string, std::string>>();
    }
    | LPAREN type_named_parameters RPAREN {
        $$ = std::move($2);
    }

type_named_parameters:
    "identifier" ASSIGN "identifier" {
        $$ = std::vector<std::pair<std::string, std::string>>();
        $$.emplace_back($1, $3);
    }
    | "identifier" ASSIGN "number" {
        $$ = std::vector<std::pair<std::string, std::string>>();
        $$.emplace_back($1, std::to_string($3));
    }
    | "identifier" ASSIGN "identifier" COMMA type_named_parameters {
        $$ = std::move($5);
        $$.emplace_back($1, $3);
    }
    | "identifier" ASSIGN "number" COMMA type_named_parameters {
        $$ = std::move($5);
        $$.emplace_back($1, std::to_string($3));
    }

paren_type_unnamed_parameters:
    LPAREN type_unnamed_parameters RPAREN {
        $$ = std::move($2);
    }

type_unnamed_parameters:
    "identifier" {
        $$ = std::vector<std::string>();
        $$.emplace_back(std::move($1));
    }
    | "number" {
        $$ = std::vector<std::string>();
        $$.emplace_back(std::to_string($1));
    }
    | "identifier" COMMA type_unnamed_parameters {
       $$ = std::move($3);
       $$.emplace_back(std::move($1));
    }
    | "number" COMMA type_unnamed_parameters {
       $$ = std::move($3);
       $$.emplace_back(std::to_string($1));
    }


declaration_list:
    "identifier" {
        std::vector<std::string> list;
        list.push_back($1);
        $$ = std::move(list);
    }
    | declaration_list "," "identifier" {
        $1.push_back($3);
        $$ = std::move($1);
    }

statements:
    %empty {}
    | statements statement NEWLINE {
        $1.push_back(std::move($2));
        $$ = std::move($1);
    }

statement:
    %empty {}
    | "identifier" ASSIGN exp {
        $$ = std::make_unique<TAssignStatement>($1, std::move($3));
    }
    | if_statement {
        $$ = std::move($1);
    }
    | do_loop {
        $$ = std::move($1);
    }
    | PRINT STAR exp_list {
        $$ = std::make_unique<TPrintStatement>(std::move($3));
    }

exp_list:
    %empty {
        $$ = std::move(std::vector<std::unique_ptr<TExpression>>());
    }
    | exp_list COMMA exp {
        $1.push_back(std::move($3));
        $$ = std::move($1);
    }

if_statement:
    else_if_part {$$ = std::move($1);}

if_part:
    IF "(" exp ")" THEN NEWLINE statements {
        auto stat = std::make_unique<TIfStatement>();
        stat->AddIf(std::move($3), std::move($7));
        $$ = std::move(stat);
    }

else_if_part:
    if_part {
        $$ = std::move($1);
    }
    | else_if_part ELSE IF "(" exp ")" THEN NEWLINE statements {
        $1->AddIf(std::move($5), std::move($9));
        $$ = std::move($1);
    }
    | else_if_part ELSE NEWLINE statements END IF {
        $1->AddElse(std::move($4));
        $$ = std::move($1);
    }
    | else_if_part END IF {
        $$ = std::move($1);
    }

do_loop:
    DO "identifier" ASSIGN exp COMMA exp do_loop_step NEWLINE statements END DO {
        $$ = std::make_unique<TDoLoopStatement>(std::move($2), std::move($4), std::move($6), std::move($7), std::move($9));
    }

do_loop_step:
    %empty {
        $$ = std::make_unique<TValueExpression>(std::make_unique<TInteger>(1));
    }
    | COMMA exp {
        $$ = std::move($2);
    }


%left "+" "-";
%left "*" "/";

exp:
    "number" {$$ = std::make_unique<TValueExpression>(std::make_unique<TInteger>($1));}
    | "identifier" {$$ = std::make_unique<TIdentifierExpression>(std::move($1));}
    | exp "+" exp {$$ = std::make_unique<TSumExpression>(std::move($1), std::move($3)); }
    | exp "-" exp {$$ = std::make_unique<TSubExpression>(std::move($1), std::move($3)); }
    | exp "*" exp {$$ = std::make_unique<TMulExpression>(std::move($1), std::move($3)); }
    | exp "/" exp {$$ = std::make_unique<TDivExpression>(std::move($1), std::move($3)); }
    | exp ">" exp {$$ = std::make_unique<TGtExpression>(std::move($1), std::move($3)); }
    | exp "<" exp {$$ = std::make_unique<TLtExpression>(std::move($1), std::move($3)); }

    | "(" exp ")" {$$ = std::move($2); };
    | STRING {$$ = std::make_unique<TValueExpression>(std::make_unique<TCharacter>($1));}
    | TRUE {$$ = std::make_unique<TValueExpression>(std::make_unique<TLogical>(true));}
    | FALSE {$$ = std::make_unique<TValueExpression>(std::make_unique<TLogical>(false));}
    | exp EQV exp {$$ = std::make_unique<TEqvExpression>(std::move($1), std::move($3)); }
    | NOT exp {$$ = std::make_unique<TNotExpression>(std::move($2)); }
    | exp AND exp {$$ = std::make_unique<TAndExpression>(std::move($1), std::move($3));}
    | exp OR exp {$$ = std::make_unique<TOrExpression>(std::move($1), std::move($3));}
%%

void
yy::parser::error(const location_type& l, const std::string& m)
{
  std::cerr << l << ": " << m << '\n';
}
