
%{
	#include "ftab.h"
	struct val{
	char* text;
	unsigned int strno;
	char* func=nullptr;
	int misc;
	tree* tr;
	};
	#define YYSTYPE val
	#include "lab2.tab.h"
	#include <stdlib.h>
	void yyerror(const char*);
	unsigned int strno=1;
	unsigned int strerrno=1;
	bool ns=false;
	char*curfunc=nullptr;
	bool needfunc=false;
	bool inside = false;
	extern ftab fdefs;
	extern int correct;
%}

%%
VARIANT	{
		inside = true;
		yylval.text = yytext;yylval.strno=strno;yylval.func=curfunc;if(ns)ns=false;else strerrno=strno; return DEF;
	}

[\[\],={};+-]	{
			inside = true;
			yylval.text = yytext;yylval.strno=strno;yylval.func=curfunc;if(ns)ns=false;else strerrno=strno;return *yytext;
		}
\n	{
		if(ns)ns=false;else strerrno=strno;
		strno++;
		ns=true;
		if(inside)
			return '\n';
	}
[ \t]

TRUE|FALSE		{
				inside = true;
				yylval.text = yytext;yylval.strno=strno;yylval.func=curfunc;if(ns)ns=false;else strerrno=strno;return BOOLVAL;
			}

-?[0-9]+		{
				inside = true;
				yylval.text = yytext;
				yylval.strno=strno;
				yylval.func=curfunc;
				if(ns)ns=false;else strerrno=strno;
				return INTVAL;
			}


\"[^"]*\"	{
				inside = true;
		yylval.text = yytext;yylval.strno=strno;yylval.func=curfunc;if(ns)ns=false;else strerrno=strno;return STRVAL;
	}

CONVERT	{
		inside = true;
		yylval.text = yytext;yylval.strno=strno;yylval.func=curfunc;if(ns)ns=false;else strerrno=strno;return CONV;
	}

(BOOL|DIGIT|STRING)	{
				inside = true;
				yylval.text = yytext;yylval.strno=strno;yylval.func=curfunc;if(ns)ns=false;else strerrno=strno;return CONVTYPE;
			}

TO	{
				inside = true;
		yylval.text = yytext;yylval.strno=strno;yylval.func=curfunc;if(ns)ns=false;else strerrno=strno;return CONVTO;
	}

(B|S)DIGITIZE	{
				inside = true;
			yylval.text = yytext;yylval.strno=strno;yylval.func=curfunc;if(ns)ns=false;else strerrno=strno;return DIGZ;
		}

WHILE|UNTIL	{
				inside = true;
			yylval.text = yytext;yylval.strno=strno;yylval.func=curfunc;if(ns)ns=false;else strerrno=strno;return CYCLE;
		}


ENDW|ENDU	{
				inside = true;
			yylval.text = yytext;yylval.strno=strno;yylval.func=curfunc;if(ns)ns=false;else strerrno=strno;return CYCLEEND;
		}

(IFLESS|IFNLESS|IFZERO|IFNZERO|IFHIGH|IFNHIGH|IFWALL|IFEXIT)	{
									inside = true;
									yylval.text = yytext;yylval.strno=strno;yylval.func=curfunc;
									if(ns)ns=false;else strerrno=strno;return IF;
								}

COMMAND {
				inside = true;
		yylval.text = yytext;yylval.strno=strno;yylval.func=curfunc;if(ns)ns=false;else strerrno=strno;return RCOM;
	}

FUNC	{
				inside = true;
		needfunc = true;
		yylval.text = yytext;yylval.strno=strno;yylval.func=curfunc;if(ns)ns=false;else strerrno=strno;return FDEF;
	}

ENDFUNC	{
				inside = false;
		curfunc=nullptr;
		yylval.text = yytext;yylval.strno=strno;yylval.func=curfunc;if(ns)ns=false;else strerrno=strno;return FEND;
	}

PARAM	{
				inside = true;
		yylval.text = yytext;yylval.strno=strno;yylval.func=curfunc;if(ns)ns=false;else strerrno=strno;return FPARAM;
	}

RETURN	{
				inside = true;
		yylval.text = yytext;yylval.strno=strno;yylval.func=curfunc;if(ns)ns=false;else strerrno=strno;return FRET;
	}

CALL	{
				inside = true;
		yylval.text = yytext;yylval.strno=strno;yylval.func=curfunc;if(ns)ns=false;else strerrno=strno;return CALLF;
	}

[a-zA-Z][a-zA-Z0-9]*	{
				inside = true;
				if(needfunc)
				{
					needfunc=false;
					curfunc = yytext;
					if(!fdefs.add(yytext)){fprintf(stderr,"In line %d: function already defined\n",strno);correct=0;}
				}
				yylval.text = yytext;
				yylval.strno=strno;
				yylval.func=curfunc;
				if(ns)ns=false;else strerrno=strno;
				return VAR;
			}

. yyerror("Unknown character");

%%

int yywrap(void){
	return 1;
}
