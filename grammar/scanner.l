%{
    #include <cerrno>
    #include <climits>
    #include <cstdlib>
    #include <cstring> // strerror
    #include <string>
    #include <iostream>
    #include "driver.hh"
    #include "parser.hh"
%}

%option noyywrap nounput noinput batch debug

%option c++
%option yyclass="Scanner"
%option caseless

%{
  // A number symbol corresponding to the value in S.
  yy::parser::symbol_type make_NUMBER(
    const std::string &s,
    const yy::parser::location_type& loc
  );
  yy::parser::symbol_type make_STRING(
      const std::string &s,
      const yy::parser::location_type& loc
    );
%}

id    [a-zA-Z][a-zA-Z_0-9]*
int   [-+0-9][0-9]*
blank [ \t\r]

%{
  // Code run each time a pattern is matched.
  # define YY_USER_ACTION  loc.columns (yyleng);
%}
%%
%{
  // A handy shortcut to the location held by the driver.
  yy::location& loc = driver.location;
  // Code run each time yylex is called.
  loc.step();
%}

{blank}+   loc.step ();
\n+         {
                loc.lines (yyleng); loc.step ();
                return yy::parser::make_NEWLINE(loc);
            }
"-"        return yy::parser::make_MINUS  (loc);
"+"        return yy::parser::make_PLUS   (loc);
"*"        return yy::parser::make_STAR   (loc);
"/"        return yy::parser::make_SLASH  (loc);
"("        return yy::parser::make_LPAREN (loc);
")"        return yy::parser::make_RPAREN (loc);
"="        return yy::parser::make_ASSIGN (loc);
"::"       return yy::parser::make_DOUBLE_COLON(loc);
","        return yy::parser::make_COMMA(loc);
"'"        return yy::parser::make_QUOTE(loc);
"\""       return yy::parser::make_QUOTE(loc);
">"        return yy::parser::make_GT(loc);
"<"        return yy::parser::make_LT(loc);
">="       return yy::parser::make_GE(loc);
"<="       return yy::parser::make_LE(loc);
"=="       return yy::parser::make_EQ(loc);

"program"  return yy::parser::make_PROGRAM(loc);
"end"      return yy::parser::make_END(loc);

"if"       return yy::parser::make_IF(loc);
"then"     return yy::parser::make_THEN(loc);
"else"     return yy::parser::make_ELSE(loc);
"do"       return yy::parser::make_DO(loc);
"while"    return yy::parser::make_WHILE(loc);

"INTEGER"     return yy::parser::make_INTEGER(loc);
"CHARACTER"   return yy::parser::make_CHARACTER(loc);
"LOGICAL"     return yy::parser::make_LOGICAL(loc);

[\.]TRUE[\.]  return yy::parser::make_TRUE(loc);
[\.]FALSE[\.] return yy::parser::make_FALSE(loc);
[\.]EQV[\.]   return yy::parser::make_EQV(loc);
[\.]AND[\.]   return yy::parser::make_AND(loc);
[\.]OR[\.]    return yy::parser::make_OR(loc);
[\.]NOT[\.]   return yy::parser::make_NOT(loc);

"print"    return yy::parser::make_PRINT(loc);
"read"     return yy::parser::make_READ(loc);

{int}      return make_NUMBER(yytext, loc);
{id}       return yy::parser::make_IDENTIFIER(yytext, loc);

["]([^"\\\n]|\\.|\\\n)*["] return make_STRING(yytext, loc);

.          {
                throw yy::parser::syntax_error(loc, "invalid character: " + std::string(yytext));
           }
<<EOF>>    return yy::parser::make_EOF (loc);

%%

yy::parser::symbol_type make_NUMBER(
  const std::string &s,
  const yy::parser::location_type& loc
) {
  errno = 0;
  long n = strtol(s.c_str(), NULL, 10);
  if (! (INT_MIN <= n && n <= INT_MAX && errno != ERANGE))
    throw yy::parser::syntax_error(loc, "integer is out of range: " + s);
  return yy::parser::make_NUMBER((int) n, loc);
}

yy::parser::symbol_type make_STRING(
  const std::string &s,
  const yy::parser::location_type& loc
) {
  return yy::parser::make_STRING(s.substr(1, s.size() - 2), loc);
}