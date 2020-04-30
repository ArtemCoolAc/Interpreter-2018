#include "moretrees.h"
#include <cstring>

//#define DEBUG

char* copystr(char* s)
{
	char* res;
	int len = strlen(s);
	res=new char[len];
	res[0]=0;
	strcpy(res,s);
	return res;
}

bool isc(int type)
{
	if((type == RETURNN) || (type == CONCATN) || (type == CYCLEN) || (type == IFN))
		return true;
	return false;
}

variant* nulltree::run(field& f)
{
#ifdef DEBUG
	printf("Running null...\nOK\n");
#endif
	return nullptr;
}
nulltree::nulltree():tree(NULLTREE){}
nulltree::~nulltree(){}

variant* concattree::run(field& f)
{
#ifdef DEBUG
	printf("Running concatenation...\n");
#endif
	variant* returnable;
	if(left)
	{
#ifdef DEBUG
		printf("ConcatenationL: Running %d\n",left);
#endif
		returnable = left->run(f);
		if(returnable && isc(left->gettype()))
		{
#ifdef DEBUG
			printf("OK\n");
#endif
			return returnable;
		}
		delete returnable;
	}
	if(right)
	{
#ifdef DEBUG
		printf("ConcatenationR: Running %d\n",right);
#endif
		returnable = right->run(f);
		if(returnable && isc(right->gettype()))
		{
#ifdef DEBUG
			printf("OK\n");
#endif
			return returnable;
		}
		delete returnable;
	}
#ifdef DEBUG
	printf("OK\n");
#endif
	return nullptr;
}
concattree::concattree(tree* left, tree* right):left(left),right(right),tree(CONCATN){}
concattree::~concattree(){delete left; delete right;}

variant* cycletree::run(field& f)
{
#ifdef DEBUG
	printf("Running cycle...\n");
#endif
	variant* returnable;
#ifdef DEBUG
	printf("Cycle(condition): Running %d\n",condition);
#endif
	for(returnable=condition->run(f);type?!(returnable->get(1,0).getb()):(returnable->get(1,0).getb())
		;
#ifdef DEBUG
		printf("Cycle(condition): Running %d\n",condition),
#endif
		returnable = condition->run(f))
	{
		delete returnable;
#ifdef DEBUG
		printf("Cycle(code): Running %d\n",code);
#endif
		returnable = code->run(f);
		if(returnable && isc(code->gettype()))
			{
#ifdef DEBUG
				printf("OK\n");
#endif
				return returnable;
			}
		delete returnable;
	}	
	delete returnable;
#ifdef DEBUG
	printf("OK\n");
#endif
	return nullptr;
}
cycletree::cycletree(tree* cond, tree* code,int type) : condition(cond), code(code), type(type), tree(CYCLEN) {}
cycletree::~cycletree() {delete condition; delete code;}

variant* iftree::run(field& f)
{
#ifdef DEBUG
	printf("Running if...\n");
#endif
	bool good;
	char* object;
	variant* returnable;
#ifdef DEBUG
	printf("If(condition): Running %d\n",condition);
#endif
	returnable = condition->run(f);
	switch (type)
	{
		case 0:
			if (returnable->get(1, 0).geti() < 0) good = true;
			else good = false;
			break;
		case 1:
			if (returnable->get(1, 0).geti() == 0) good = true;
			else good = false;
			break;
		case 2:
			if (returnable->get(1, 0).geti() > 0) good = true;
			else good = false;
			break;
		case 3:
			if (returnable->get(1, 0).geti() < 0) good = false;
			else good = true;
			break;
		case 4:
			if (returnable->get(1, 0).geti() == 0) good = false;
			else good = true;
			break;
		case 5:
			if (returnable->get(1, 0).geti() > 0) good = false;
			else good = true;
			break;
		case IFWALL:
			object = returnable->get(1, 0).gets();
			if (strcmp(object, "WALL") == 0) good = true;
			else good = false;
			//delete nstr;
			break;
		case IFEXIT:
			object = returnable->get(1, 0).gets();
			if (strcmp(object, "EXIT") == 0) good = true;
			else good = false;
			//delete nstr;
			break;
		default:
			good = false;
			break;
	}
	delete returnable;
	if (good)
	{
#ifdef DEBUG
		printf("If(code): Running %d\n",code);
#endif
		returnable = code->run(f);
		if (returnable && isc(code->gettype()))
			{
#ifdef DEBUG
				printf("OK\n");
#endif
				return returnable;
			}
	}
#ifdef DEBUG
	printf("OK\n");
#endif
	return nullptr;
}
iftree::iftree (tree* cond,tree* code, int type) : condition(cond), code(code), type(type), tree(IFN) {}
iftree::~iftree() {delete condition; delete code;}

variant* commandtree::run(field& f)
{
#ifdef DEBUG
	printf("Running command...\n");
#endif
	variant* robot_command = command->run(f);
	variant* ret=NULL;
	if (robot_command) ret = f.act(*robot_command);
	delete robot_command;
#ifdef DEBUG
	printf("OK\n");
#endif
	return ret;
}
commandtree::commandtree (tree* command) : command(command), tree(COMMANDN) {}
commandtree::~commandtree () {delete command;}

variant* strtree::run(field& f)
{
#ifdef DEBUG
	printf("Running str...\n");
#endif
	variant* returnable = new variant;
	returnable->get(1, 0).sets(str);
	returnable->get(0, 0).setb(false);
	returnable->get(0, 1).setb(false);
	returnable->get(0, 2).setb(true);
#ifdef DEBUG
	printf("OK\n");
#endif
	return returnable;
}
strtree::strtree (char* string) : str(copystr(string)), tree(STRN) {}
strtree::~strtree(){/*delete str;*/}

variant* inttree::run(field& f)
{
#ifdef DEBUG
	printf("Running int...\n");
#endif
	variant* returnable = new variant;
	returnable->get(1, 0).seti(value);
	returnable->get(0, 0).setb(false);
	returnable->get(0, 1).setb(true);
	returnable->get(0, 2).setb(false);
#ifdef DEBUG
	printf("OK\n");
#endif
	return returnable;
}
inttree::inttree (int integer) : value(integer), tree(INTN) {}
inttree::~inttree(){}

variant* booltree::run(field& f)
{
#ifdef DEBUG
	printf("Running bool...\n");
#endif
	variant* returnable = new variant;
	returnable->get(1, 0).setb(value);
	returnable->get(0, 0).setb(true);
	returnable->get(0, 1).setb(false);
	returnable->get(0, 2).setb(false);
#ifdef DEBUG
	printf("OK\n");
#endif
	return returnable;
}
booltree::booltree (bool boolean) : value(boolean), tree(BOOLN){}
booltree::~booltree(){}

variant* vartree::run(field& f)
{
#ifdef DEBUG
	printf("Running {...}...\n");
#endif
	variant* returnable = new variant;
#ifdef DEBUG
	printf("{...}: Running %d\n",left);
#endif
	variant* left_ret = left->run(f);
#ifdef DEBUG
	printf("{...}: Running %d\n",right);
#endif
	variant* right_ret = right->run(f);	
	returnable->combinex(*left_ret, *right_ret);
	delete left_ret;
	delete right_ret;
#ifdef DEBUG
	printf("OK\n");
#endif
	return returnable;
}
vartree::vartree(tree* left,tree* right) : left(left), right(right), tree(VARN) {}
vartree::~vartree() {delete left; delete right;}

variant* arrvartree::run(field& f)
{
#ifdef DEBUG
	printf("Running {{...}{...}}...\n");
#endif
	variant* ret = new variant;
#ifdef DEBUG
	printf("{{...}{...}}: Running %d\n",left);
#endif
	variant* left_ret = left->run(f);
#ifdef DEBUG
	printf("{{...}{...}}: Running %d\n",right);
#endif
	variant* right_ret = right->run(f);
	ret->combiney(*left_ret,*right_ret);
	delete left_ret;
	delete right_ret;
#ifdef DEBUG
	printf("OK\n");
#endif
	return ret;
}
arrvartree::arrvartree (tree* left, tree* right) : left(left), right(right), tree(ARRVARN) {}
arrvartree::~arrvartree() {delete left; delete right;}

variant* calltree::run(field& f)
{
#ifdef DEBUG
	printf("Running call...\n");
	printf("Call(expr): Running %d\n",exp);
#endif
	variant* eret = exp->run(f);
#ifdef DEBUG
	printf("Call(func): Running %d\n",func);
#endif
	variant* returnable = func->run(*eret,f);
	delete eret;
#ifdef DEBUG
	printf("OK\n");
#endif
	return returnable;
}
calltree::calltree(tree* exp,function* function) : exp(exp), func(function), tree(CALLFN) {}
calltree::~calltree(){delete exp;}

variant* returntree::run(field& f)
{
#ifdef DEBUG
	printf("Running return...\n");
	printf("Return: running %d\n",exp);
#endif
	variant* ret = exp->run(f);
#ifdef DEBUG
	printf("OK\n");
#endif
	return ret;
}
returntree::returntree(tree* expr) : exp(expr), tree(RETURNN) {}
returntree::~returntree() {delete exp;}

variant* assignvartree::run(field& f)
{
#ifdef DEBUG
	printf("Running assignvar...\n");
	printf("Assignvar: Running %d\n",exp);
#endif
	variant* ret = exp->run(f);
	var->rfl(*ret);
#ifdef DEBUG
	printf("OK\n");
#endif
	return ret;
}
assignvartree::assignvartree(variant* variable, tree* expr) : var(variable), exp(expr), tree(ASSIGNVARN) {}
assignvartree::~assignvartree() {delete exp;}

variant* assigneltree::run(field& f)
{
#ifdef DEBUG
	printf("Running assign..\n");
	printf("Assign(expr): Running %d\n",exp);
#endif
	variant* element_ret = exp->run(f);
#ifdef DEBUG
	printf("Assign(adress): Running %d\n",adress);
#endif
	variant* address_ret = adress->run(f);
	var->get(address_ret->get(1, 0).geti(), address_ret->get(1, 1).geti()) = element_ret->get(1, 0);
	delete address_ret;
#ifdef DEBUG
	printf("OK\n");
#endif
	return element_ret;
}
assigneltree::assigneltree (tree* address, tree* expr, variant* variable) : adress(address), exp(expr), var(variable), tree(ASSIGNN) {}
assigneltree::~assigneltree() {delete adress;delete exp;}

variant* elementtree::run(field& f)
{
#ifdef DEBUG
	printf("Running {{...;...}}...\n");
#endif
	variant* ret = new variant;
#ifdef DEBUG
	printf("{{...;...}}: Running %d\n",left);
#endif
	variant* left_ret = left->run(f);
#ifdef DEBUG
	printf("{{...;...}}: Running %d\n",right);
#endif
	variant* right_ret = right->run(f);
	if (left_ret->get(0, 0).getb())
	{
		ret->get(0, 0).setb(true);
		ret->get(1, 0).setb(left_ret->get(1, 0).getb());
	}
	if (left_ret->get(0, 1).getb())
	{
		ret->get(0, 1).setb(true);
		ret->get(1, 0).seti(left_ret->get(1, 0).geti());
	}
	if (left_ret->get(0, 2).getb())
	{
		ret->get(0, 2).setb(true);
		char* v = left_ret->get(1, 0).gets();
		ret->get(1, 0).sets(v);
		delete v;
	}
	if (right_ret->get(0, 0).getb())
	{
		ret->get(0, 0).setb(true);
		ret->get(1, 0).setb(right_ret->get(1, 0).getb());
	}
	if (right_ret->get(0, 1).getb())
	{
		ret->get(0, 1).setb(true);
		ret->get(1, 0).seti(right_ret->get(1, 0).geti());
	}
	if (right_ret->get(0, 2).getb())
	{
		ret->get(0, 2).setb(true);
		char* v = right_ret->get(1, 0).gets();
		ret->get(1, 0).sets(v);
		delete v;
	}
	delete left_ret;
	delete right_ret;
#ifdef DEBUG
	printf("OK\n");
#endif
	return ret;
}
elementtree::elementtree (tree* left, tree* right) : left(left), right(right), tree(CONSTN) {}
elementtree::~elementtree() {delete left; delete right;}

variant* negtree::run(field& f)
{
#ifdef DEBUG
	printf("Running negation...\n");
#endif
	variant* negative_ret = new variant;
#ifdef DEBUG
	printf("Negation: Running %d\n",exp);
#endif
	variant* ret = exp->run(f);
	*negative_ret = -(*ret);
	delete ret;
#ifdef DEBUG
	printf("OK\n");
#endif
	return negative_ret;
}
negtree::negtree (tree* expr) : exp(expr), tree(NEGN) {}
negtree::~negtree(){delete exp;}

variant* plustree::run(field& f)
{
#ifdef DEBUG
	printf("Running plus...\n");
	printf("Plus(left): Running %d\n",left);
#endif
	variant* left_ret=left->run(f);
#ifdef DEBUG
	printf("Plus(right): Running %d\n",right);
#endif
	variant* right_ret=right->run(f);
	variant* returnable = new variant;
	*returnable = (*left_ret + *right_ret);
	delete left_ret;
	delete right_ret;
#ifdef DEBUG
	printf("OK\n");
#endif
	return returnable;
}
plustree::plustree (tree* left_operand, tree* right_operand) : left(left_operand), right(right_operand), tree(PLUSN) {}
plustree::~plustree() {delete left; delete right;}

variant* adressgettree::run(field& f)
{
#ifdef DEBUG
	printf("Running adressget...\n");
#endif
	variant* returnable = new variant;
#ifdef DEBUG
	printf("Adressget: Running %d\n",adress);
#endif
	variant* address_ret=adress->run(f);
	returnable->set(1, 0, var->get(address_ret->get(1, 0).geti(), address_ret->get(1, 1).geti()));
	delete address_ret;
#ifdef DEBUG
	printf("OK\n");
#endif
	return returnable;
}
adressgettree::adressgettree (tree* address, variant* variable):adress(address), var(variable), tree(ADRESSGETN) {}
adressgettree::~adressgettree() {delete adress;}

variant* paramtree::run(field& f)
{
#ifdef DEBUG
	printf("Running param...\n");
#endif
	variant* param_ret = new variant(*param);
	param_ret->addf();
#ifdef DEBUG
	printf("OK\n");
#endif
	return param_ret;
}
paramtree::paramtree(variant* param) : param(param), tree(PARAMN) {}
paramtree::~paramtree() {}

variant* cvarianttree::run(field& f)
{
#ifdef DEBUG
	printf("Running cvar...\n");
#endif
	variant* ret = new variant;
	*ret = *param;
#ifdef DEBUG
	printf("OK\n");
#endif
	return ret;
}
cvarianttree::cvarianttree(variant* param) : param(param), tree(CVARN) {}
cvarianttree::~cvarianttree(){delete param;}

variant* celementtree::run(field& f)
{
#ifdef DEBUG
	printf("Running celem...\n");
#endif
	variant* ret = new variant;
	ret->set(1, 0, *param);
#ifdef DEBUG
	printf("OK\n");
#endif
	return ret;
}
celementtree::celementtree (element* param) : param(param), tree(CELN) {}
celementtree::~celementtree() {delete param;}

variant* converttree::run(field& f)
{
#ifdef DEBUG
	printf("Running convert...\n");
	printf("Convert: Running %d\n",adress);
#endif
	variant* conv_ret=adress->run(f);
	var->get(conv_ret->get(1,0).geti(),conv_ret->get(1,1).geti()).convert(type % 3, type / 3);
	delete conv_ret;
#ifdef DEBUG
	printf("OK\n");
#endif
	return nullptr;
}
converttree::converttree (tree* address, variant* variable, int type) : adress(address), var(variable), type(type), tree(CONVN) {}
converttree::~converttree() {delete adress;}

variant* dconverttree::run(field& f)
{
#ifdef DEBUG
	printf("Running dconvert...\n");
#endif
	var->convert(type % 3, type / 3);
#ifdef DEBUG
	printf("OK\n");
#endif
	return nullptr;
}
dconverttree::dconverttree(variant* variable, int type):var(variable), type(type), tree(DCONVN){}
dconverttree::~dconverttree(){}

variant* adresstree::run(field& f)
{
#ifdef DEBUG
	printf("Running adress...\n");
#endif
	variant* ret = new variant;
#ifdef DEBUG
	printf("Adress: Running %d\n",first);
#endif
	variant* first_ind_ret=first->run(f);
#ifdef DEBUG
	printf("Adress: Running %d\n",second);
#endif
	variant* second_ind_ret=second->run(f);
	ret->set(1,0,first_ind_ret->get(1,0));
	ret->set(1,1,second_ind_ret->get(1,0));
	delete first_ind_ret;
	delete second_ind_ret;
#ifdef DEBUG
	printf("OK\n");
#endif
	return ret;
}
adresstree::adresstree (inttree* first_ind, inttree* second_ind) : first(first_ind), second(second_ind), tree(ADRESSN) {}
adresstree::~adresstree(){delete first; delete second;}
