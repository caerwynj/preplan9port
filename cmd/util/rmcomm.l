%s COMMENT
%%
"/*" { BEGIN COMMENT;}
<COMMENT>"*/" {BEGIN 0;}
<COMMENT>.|\n {;}
.|\n ECHO;
%%
main() { yylex(); }
