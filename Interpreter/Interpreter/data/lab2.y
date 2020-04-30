%{
	#define YYSTYPE val
	#include <stdio.h>
	#include "moretrees.h"
	struct val{
	char* text;
	unsigned int strno;
	char* func=nullptr;
	int misc;
	tree* tr;
	};
	int yylex(void);
	void yyerror(const char*);
	extern FILE *yyin;
	extern unsigned int strno;
	extern unsigned int strerrno;	
	int correct=1;
	eltab deftab;
	ftab fdefs;
	tree* stree;
	field* rfield;
	unsigned int lasterror=0;
%}

%token DEF
%token BOOLVAL
%token INTVAL
%token STRVAL
%token CONV
%token CONVTYPE
%token CONVTO
%token DIGZ
%token CYCLE
%token CYCLEEND
%token IF
%token RCOM
%token FDEF
%token FEND
%token FRET
%token FPARAM
%token CALLF
%token VAR

%right ','
%right ';'
%right '+' '='
%right '-'
%right RCOM
%right CALLF VAR
%right '\n'
%%
start:
	program	{if(correct)printf("correct! \n");}
	|	{yyerror("Empty file");return 1;}
	;

program:
	fdefs code 	{stree=$2.tr;}	// содержит определения функций + код вне ф-ий (аналог main'a, только без "main")
	|code		{stree=$1.tr;}  // либо без определения функций тупо код
	|fdefs		{stree=nullptr;} // а если кода вне функций нет, то ничего не вызывается)))
	;

fdefs:	
	fdef fdefs	// определение функций содержит определение функции и себя же (функций может быть много)
	|fdef // только одно определение
	;	

fdef:	FDEF VAR fcode FEND	{if(!$3.misc){fprintf(stderr,"In line %d: return from function is not guaranteed\n",$2.strno); // нужные компоненты функции
				correct=0;}fdefs.settree($2.text,$3.tr);}
	|FDEF VAR FEND	{fprintf(stderr,"In line %d: return from function is not guaranteed (no function)\n",$2.strno);correct=0;} // нет кода)))
	;

fcode:
	nfsent fcode	{$$.misc=$1.misc||$2.misc;$$.tr = new concattree($1.tr,$2.tr);}
	|nfsent		{$$.misc=$1.misc;$$.tr=$1.tr;}
	;

nfsent:
	fsent '\n'	{$$.misc=$1.misc;$$.tr=$1.tr;} // различные конструкции языка
	|'\n'		{$$.misc=0;$$.tr=new nulltree;} // пустая строка
	;

fmsent: // код в случае выполнения условия
	fsent fmsent	{$$.tr = new concattree($1.tr,$2.tr);}
	|fsent		{$$.misc=$1.misc;$$.tr=$1.tr;}
	;


fsent:
	fvardef 	{$$.misc=0;$$.tr=$1.tr;} // определение (объявление) переменной
	|IF fexpr fmsent	{$$.misc=0;$$.tr=new iftree($2.tr,$3.tr,$1.text[2]=='N'?($1.text[3]=='L'?(IFNLESS):($1.text[3]=='Z'?IFNZERO:IFNHIGH)):
			($1.text[2]=='L'?(IFLESS):($1.text[2]=='Z'?IFZERO:($1.text[2]=='H'?IFHIGH:($1.text[2]=='W'?IFWALL:IFEXIT)))));}
	|fassign 	{$$.misc=0;$$.tr=$1.tr;} // присваивание
	|CYCLE fexpr  fcode CYCLEEND 	{if($1.text[0]!=$4.text[3]) // какой-нить цикл
					{fprintf(stderr,"In line %d: cycle opener and closer types don't match\n",$1.strno);correct=0;}
					$$.misc=0;$$.tr=new cycletree($2.tr,$3.tr,$1.text[0]=='W'?WHILE:UNTIL);}
	|RCOM STRVAL 	{$$.misc=0;char*b=$2.text+1;while(*b!='\"')b++;*b=0;$$.tr=new commandtree(new strtree($2.text+1));*b='\"';} // строковая команда роботу
	|RCOM fdexpr 	{$$.misc=0;$$.tr=new commandtree($2.tr);} // 
	|CALLF VAR fdexpr 		{$$.misc=0;if(!fdefs.getf($2.text)) // вызов функции с передачей параметра
					{fprintf(stderr,"In line %d: function is not declared\n",$2.strno);correct=0;}
					$$.tr=new calltree($3.tr,fdefs.getf($2.text));}
	|fconvert	{$$.tr=$1.tr;} // конвертирование
	|fdigitize	{$$.tr=$1.tr;} // цифрирование
	|FRET fdexpr	{$$.misc=1;$$.tr=new returntree($2.tr);} // возврат значений из функции
	|error		{if(lasterror!=strerrno){fprintf(stderr,"In line %d: unapproptiate token\n",strerrno);lasterror=strerrno;} // если нашлась какая-то неведомая фигня, то ошибка "нет такого токена"
			correct=0;$$.tr=nullptr;}
	;

fvardef: // определения в ФУНКЦИЯХ
	DEF VAR adress '=' dconst	{if(!fdefs.adddef($2.func,$2.text)){ 
					fprintf(stderr,"In line %d: variant already defined\n",$2.strno);correct=0;}
					$$.tr=new assignvartree(fdefs.getdef($2.func,$2.text),$5.tr);
					delete $3.tr;}
	| DEF VAR '=' dconst		{if(!fdefs.adddef($2.func,$2.text)){ 
					fprintf(stderr,"In line %d: variant already defined\n",$2.strno);correct=0;}
					$$.tr=new assignvartree(fdefs.getdef($2.func,$2.text),$4.tr);}
	| DEF VAR adress		{delete $3.tr;if(!fdefs.adddef($2.func,$2.text)){
					fprintf(stderr,"In line %d: variant already defined\n",$2.strno);correct=0;}$$.tr=nullptr;}
	| DEF VAR			{if(!fdefs.adddef($2.func,$2.text)){
					fprintf(stderr,"In line %d: variant already defined\n",$2.strno);correct=0;}$$.tr=nullptr;}
	;

vardef: // определения в "главной" части программы (совместить нельзя, так как для этой части есть deftab, для каждой функции своя такая табличка, чтобы все переменные были локальными в ф-иях)
	DEF VAR adress '=' dconst	{if(!deftab.add($2.text)){
					fprintf(stderr,"In line %d: variant already defined\n",$2.strno);correct=0;}
					$$.tr=new assignvartree(deftab.get($2.text),$5.tr);
					delete $3.tr;}
	|DEF VAR '=' dconst		{if(!deftab.add($2.text)){
					fprintf(stderr,"In line %d: variant already defined\n",$2.strno);correct=0;}
					$$.tr=new assignvartree(deftab.get($2.text),$4.tr);}
	| DEF VAR adress		{if(!deftab.add($2.text)){fprintf(stderr,"In line %d: variant already defined\n",$2.strno);correct=0;}
					$$.tr=nullptr;delete $3.tr;}
	| DEF VAR			{if(!deftab.add($2.text)){fprintf(stderr,"In line %d: variant already defined\n",$2.strno);correct=0;}
					$$.tr=nullptr;}
	;

dconst:
	'{'varval'}'			{$$.tr=$2.tr;} // полная инициализация
	;

varval:
	const varval			{$$.tr=new vartree($1.tr,$2.tr);} // несколько элементов
	|const				{$$.tr=$1.tr;}
	;

assign:
	elassign	{$$.tr=$1.tr;}
	|varassign	{$$.tr=$1.tr;}
	;
elassign: // присваивание по индексу
	VAR adress '=' expr	{if(!deftab.get($1.text)){fprintf(stderr,"In line %d: variant is not declared\n",$1.strno);correct=0;}
				$$.tr=new assigneltree($2.tr,$4.tr,deftab.get($1.text));}
	;
varassign: // полное присваивание
	VAR '=' dexpr		{if(!deftab.get($1.text)){fprintf(stderr,"In line %d: variant is not declared\n",$2.strno);correct=0;}
				$$.tr=new assignvartree(deftab.get($1.text),$3.tr);}
	;

fassign:
	felassign	{$$.tr=$1.tr;}
	|fvarassign	{$$.tr=$1.tr;}
	;
felassign: // присваивание по индексу
	VAR adress '=' fexpr	{if(!fdefs.getdef($1.func,$1.text)){
				fprintf(stderr,"In line %d: variant is not declared\n",$1.strno);correct=0;}
				$$.tr=new assigneltree($2.tr,$4.tr,fdefs.getdef($2.func,$1.text));}
	;
fvarassign: // полное присваивание
	VAR '=' fdexpr		{if(!fdefs.getdef($1.func,$1.text)){fprintf(stderr,"In line %d: variant is not declared\n",$2.strno);correct=0;}
				$$.tr=new assignvartree(fdefs.getdef($1.func,$1.text),$3.tr);}
	;

fexpr:
	'-' fexpr		{$$.tr=new negtree($2.tr);} // унарный оператор "минус"/замены команд/логическое отрицание
	|fexpr '+' fexpr	{$$.tr=new plustree($1.tr,$3.tr);} // оператор сложения
	|'{'elval'}'		{$$.tr=$2.tr;} // константной хрени
	|'{''}'			{$$.tr=new celementtree(new element);}
	|VAR adress		{if(!fdefs.getdef($1.func,$1.text)){fprintf(stderr,"In line %d: variant is not declared\n",$1.strno);correct=0;} // кокретный элемент из переменной по индексу
				$$.tr=new adressgettree($2.tr,fdefs.getdef($1.func,$1.text));}
	|FPARAM	adress		{$$.tr=new adressgettree($2.tr,&((fdefs.getf($1.func))->param));} // конкретный элемент из параметра по индексу
	|felassign		{$$.tr=$1.tr;}	
	;


	
fdexpr: // что подходит для полного присвоения
	'-' fdexpr		{$$.tr=new negtree($2.tr);} // унарный оператор "минус"/замены команд/логическое отрицание
	|fdexpr '+' fdexpr	{$$.tr=new plustree($1.tr,$3.tr);} // оператор сложения
	|dconst			{$$.tr=$1.tr;} // константная хрень
	|VAR			{if(!fdefs.getdef($1.func,$1.text)){fprintf(stderr,"In line %d: variant is not declared\n",$1.strno);correct=0;} // полная переменная
				$$.tr=new paramtree(fdefs.getdef($1.func,$1.text));}
	|FPARAM			{$$.tr=new paramtree(&((fdefs.getf($1.func))->param));} // параметр функции
	|RCOM STRVAL 		{char*b=$2.text+1;while(*b!='\"')b++;*b=0;$$.tr=new commandtree(new strtree($2.text+1));*b='\"';} // команда робота
	|RCOM fdexpr 		{$$.tr=new commandtree($2.tr);}
	|CALLF VAR fdexpr 	{if(!fdefs.getf($2.text)) // присвоение переменной после выполнения функции аля x = CALL LAB x
				{fprintf(stderr,"In line %d: function is not declared\n",$2.strno);correct=0;}
				$$.tr=new calltree($3.tr,fdefs.getf($2.text));}
	|fvarassign		{$$.tr=$1.tr;}	
	;
	
const:
	'{'elarr'}'	{$$.tr=$2.tr;}	// полная инициализация эл-та 
	;
	
elarr:
	elarr';'elarr	{$$.tr=new arrvartree($1.tr,$3.tr);}  // инициализация эл-та i,j; инициализация эл-та i,j+1;...
	|elval		{$$.tr=$1.tr;}
	|		{$$.tr=new celementtree(new element);}
	;

elval:
	elval ',' elval	{$$.tr=new elementtree($1.tr,$3.tr);} // инициализатор 1 эл-та i,j; инициализатор 2 эл-та i,j;....
	| BOOLVAL	{$$.tr=new booltree($1.text[0]=='T'?1:0);} // логическое
	| INTVAL	{$$.tr=new inttree(atoi($1.text));} // целое
	| STRVAL	{char*b=$1.text+1; // строковое
			while(*b!='\"')
				b++;
			*b=0;
			$$.tr=new strtree($1.text+1);
			*b='\"';}
	;
	
convert: // конвертирование в "главной" части
	CONV CONVTYPE CONVTO CONVTYPE VAR adress 	{if(!deftab.get($5.text)){
							fprintf(stderr,"In line %d: variant is not declared\n",$5.strno);correct=0;}
							int n=$4.text[0]=='D'?1:0;n=$4.text[0]=='S'?2:n;n=n*3;
							n=$2.text[0]=='D'?n+1:n;n=$2.text[0]=='S'?n+2:n;
							$$.tr=new converttree($6.tr,deftab.get($5.text),n);} // переменной по индексу (конкретного элемента)
	|CONV CONVTYPE CONVTO CONVTYPE VAR 		{if(!deftab.get($5.text)){
							fprintf(stderr,"In line %d: variant is not declared\n",$1.strno);correct=0;}
							int n=$4.text[0]=='D'?1:0;n=$4.text[0]=='S'?2:n;n=n*3;
							n=$2.text[0]=='D'?n+1:n;n=$2.text[0]=='S'?n+2:n;
							$$.tr=new dconverttree(deftab.get($5.text),n);} // полностью переменной
	;

fconvert: //конвертирование в ФУНКЦИЯХ
	CONV CONVTYPE CONVTO CONVTYPE VAR 		{if(!fdefs.getdef($2.func,$5.text)){
							fprintf(stderr,"In line %d: variant is not declared\n",$1.strno);correct=0;}
							int n=$4.text[0]=='D'?1:0;n=$4.text[0]=='S'?2:n;n=n*3;
							n=$2.text[0]=='D'?n+1:n;n=$2.text[0]=='S'?n+2:n;
							$$.tr=new dconverttree(fdefs.getdef($2.func,$5.text),n);} // полностью переменной
	|CONV CONVTYPE CONVTO CONVTYPE FPARAM		{int n=$4.text[0]=='D'?1:0;n=$4.text[0]=='S'?2:n;n=n*3;
							n=$2.text[0]=='D'?n+1:n;n=$2.text[0]=='S'?n+2:n;
							$$.tr=new dconverttree(&((fdefs.getf($2.func))->param),n);} // полностью параметра функци
	|CONV CONVTYPE CONVTO CONVTYPE VAR adress	{if(!fdefs.getdef($2.func,$5.text)){
							fprintf(stderr,"In line %d: variant is not declared\n",$1.strno);correct=0;}
							int n=$4.text[0]=='D'?1:0;n=$4.text[0]=='S'?2:n;n=n*3;
							n=$2.text[0]=='D'?n+1:n;n=$2.text[0]=='S'?n+2:n;
							$$.tr=new converttree($6.tr,fdefs.getdef($2.func,$5.text),n);} // переменной по индексу (конкретного элемента)
	|CONV CONVTYPE CONVTO CONVTYPE FPARAM adress	{int n=$4.text[0]=='D'?1:0;n=$4.text[0]=='S'?2:n;n=n*3;
							n=$2.text[0]=='D'?n+1:n;n=$2.text[0]=='S'?n+2:n;
							$$.tr=new converttree($6.tr,&((fdefs.getf($2.func))->param),n);} // параметра по индексу (конкретного элемента)
	;

adress: // индексация 
	'['INTVAL ',' INTVAL ']'	{if(atoi($2.text)<0||atoi($4.text)<0){
					fprintf(stderr,"In line %d: incorrect adress parameters\n",$2.strno);correct=0;}
					$$.tr=new adresstree(new inttree(atoi($2.text)),new inttree(atoi($4.text)));}
	;

digitize: // цифрование в "главной" части
	DIGZ VAR	{if(!deftab.get($2.text)){
			fprintf(stderr,"In line %d: variant is not declared\n",$2.strno);correct=0;}
			$$.tr=new dconverttree(deftab.get($2.text),$1.text[0]=='B'?3:5);}
	;

fdigitize: // цифрование в функции
	DIGZ VAR	{if(!fdefs.getdef($1.func,$2.text)){
			fprintf(stderr,"In line %d: variant is not declared\n",$1.strno);correct=0;}
			$$.tr=new dconverttree(fdefs.getdef($1.func,$2.text),$1.text[0]=='B'?3:5);}
	|DIGZ FPARAM	{$$.tr=new dconverttree(&((fdefs.getf($2.func))->param),$1.text[0]=='B'?3:5);} // можно цифровать параметр
	;

code: // уже пошла солянка в ГЛАВНОЙ части (аналогично, но с некоторыми изменениями)
	nsent code 	{$$.tr=new concattree($1.tr,$2.tr);}
	|nsent		{$$.tr=$1.tr;}
	;

nsent:
	sent '\n'	{$$.tr=$1.tr;} // различные конструкции языка
	|ifsent '\n'	{$$.tr=$1.tr;} // условный оператор
	|'\n'		{$$.tr=new nulltree;} // пустая строка
	;

msent: // в случае выполнения условия
	sent msent	{$$.tr = new concattree($1.tr,$2.tr);}
	|sent		{$$.tr=$1.tr;}
	|ifsent		{$$.tr=$1.tr;}

ifsent:  // отдельное правило для IF нужно, чтобы в случае выполнения условия можно было поставить ещё одно условие, и ещё одно, и ... , а потом выполнить какой-нить кусок кода
	IF expr msent	{$$.misc=0;$$.tr=new iftree($2.tr,$3.tr,$1.text[2]=='N'?($1.text[3]=='L'?(IFNLESS):($1.text[3]=='Z'?IFNZERO:IFNHIGH)):
			($1.text[2]=='L'?(IFLESS):($1.text[2]=='Z'?IFZERO:($1.text[2]=='H'?IFHIGH:($1.text[2]=='W'?IFWALL:IFEXIT)))));}

sent: 	
	vardef 		{$$.tr=$1.tr;} // объявление переменной
	|assign 	{$$.tr=$1.tr;} // присваивание
	|CYCLE expr code CYCLEEND 	{if($1.text[0]!=$4.text[3]){
					fprintf(stderr,"In line %d: cycle opener and closer types don't match\n",$1.strno);correct=0;}
					$$.misc=0;$$.tr=new cycletree($2.tr,$3.tr,$1.text[0]=='W'?WHILE:UNTIL);} // цикл
	|RCOM STRVAL 	{$$.misc=0;
			char*b=$2.text+1;while(*b!='\"')b++;*b=0;
			$$.tr=new commandtree(new strtree($2.text+1));
			*b='\"';} // команда роботу
	|RCOM dexpr 	{$$.misc=0;$$.tr=new commandtree($2.tr);}
	|CALLF VAR dexpr 		{if(!fdefs.getf($2.text)){
					fprintf(stderr,"In line %d: function is not declared\n",$2.strno);correct=0;}
					$$.tr=new calltree($3.tr,fdefs.getf($2.text));}	// вызов функции
	|convert	{$$.tr=$1.tr;} // конвертирование
	|digitize	{$$.tr=$1.tr;} // цифрирование
	|error		{if(lasterror!=strerrno){fprintf(stderr,"In line %d: unapproptiate token\n",strerrno);lasterror=strerrno;}
			correct=0;$$.tr=nullptr;} // несанкционированный токен
	;
	
expr:
	'-' expr	{$$.tr=new negtree($2.tr);} // унарный оператор минус/ логическое отрицание/ замена команд роботу на противоположные
	|expr '+' expr	{$$.tr=new plustree($1.tr,$3.tr);} // оператор сложения
	|'{'elval'}'	{$$.tr=$2.tr;} // конкретная хрень
	|'{''}'		{$$.tr=new celementtree(new element);}
	|VAR adress	{if(!deftab.get($1.text)){fprintf(stderr,"In line %d: variant is not declared\n",$1.strno);correct=0;} // 
			$$.tr=new adressgettree($2.tr,deftab.get($1.text));}
	|FPARAM	adress	{fprintf(stderr,"In line %d: parameter is is only declared inside of a function\n",$1.strno);correct=0;
			$$.tr=new paramtree(nullptr);delete $2.tr;}
	|elassign	{$$.tr=$1.tr;}
	;	

dexpr:
	'-' dexpr	{$$.tr=new negtree($2.tr);} // унарный оператор минус/замены команд роботу/логическое отрицание
	|dexpr '+' dexpr	{$$.tr=new plustree($1.tr,$3.tr);} // оператор сложения
	|dconst		{$$.tr=$1.tr;} // константная хрень
	|VAR		{if(!deftab.get($1.text)){fprintf(stderr,"In line %d: variant is not declared\n",$1.strno);correct=0;} // полная переменная
			$$.tr=new paramtree(deftab.get($1.text));}
	|RCOM STRVAL 	{$$.misc=0;
			char*b=$2.text+1;while(*b!='\"')b++;*b=0;
			$$.tr=new commandtree(new strtree($2.text+1));
			*b='\"';} // строковая команда роботу
	|RCOM dexpr 	{$$.misc=0;$$.tr=new commandtree($2.tr);} // многозадачная команда роботу
	|CALLF VAR dexpr 		{if(!fdefs.getf($2.text)){
					fprintf(stderr,"In line %d: function is not declared\n",$2.strno);correct=0;}
					$$.tr=new calltree($3.tr,fdefs.getf($2.text));} // вызов функции
	|FPARAM		{fprintf(stderr,"In line %d: parameter is is only declared inside of a function\n",$1.strno);correct=0; // параметр
			$$.tr=new paramtree(nullptr);}
	|varassign	{$$.tr=$1.tr;} // присваивание
	;
	

%%
void yyerror(const char*s){
	fprintf(stderr,"%s\n",s);
}


int main(int argc,char**argv) 
{
	//yyin = fopen("progj.txt", "r");
	FILE* f = fopen("labi1.txt", "r");
	yyin = fopen("prog44.txt", "r");
	//FILE* f = fopen("labijump.txt", "r");
	if(!yyin)
	{
		printf("Failed to open %s\n",argv[1]);
		return 0;
	}
	if(!f)
	{
		printf("Failed to open %s\n",argv[2]);
		return 0;
	}
	rfield = new field(f);
	fclose(f);
	yyparse();
	fclose(yyin);
	if(correct&&stree)
		delete stree->run(*rfield);
	if(correct)
		deftab.print();
	printf("\nMaze:\n\n");
	rfield->print();
	delete rfield;
	delete stree;
	return 0;
}
