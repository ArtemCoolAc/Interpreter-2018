#pragma once
#include "ftab.h"
//#define DEBUG

class nulltree:public tree
{
	public:
	variant* run(field&);
	nulltree();
	~nulltree();
};

class concattree:public tree
{
	tree* left,*right;
	public:
	variant* run(field&);
	concattree(tree*left,tree*right);
	~concattree();
};

class cycletree:public tree
{
	tree* condition,*code;
	int type;
	public:
	variant* run(field&);
	cycletree(tree*condition,tree*code,int type);
	~cycletree();
};

class iftree:public tree
{
	tree* condition,*code;
	int type;
	public:
	variant* run(field&);
	iftree(tree*condition,tree*code,int type);
	~iftree();
};

class commandtree:public tree
{
	tree* command;
	public:
	variant* run(field&);
	commandtree(tree*command);
	~commandtree();
};

class strtree:public tree
{	
	char* str;
	public:
	variant* run(field&);
	strtree(char*string);
	~strtree();
};

class inttree:public tree
{	
	int value;
	public:
	variant* run(field&);
	inttree(int value);
	~inttree();
};

class booltree:public tree
{	
	bool value;
	public:
	variant* run(field&);
	booltree(bool value);
	~booltree();
};

class vartree:public tree
{	
	tree* left,*right; 
	public:
	variant* run(field&);
	vartree(tree*left,tree*right);
	~vartree();
};

class arrvartree:public tree
{	
	tree* left,*right; 
	public:
	variant* run(field&);
	arrvartree(tree*left,tree*right);
	~arrvartree();
};

class calltree:public tree
{	
	function* func;
	tree* exp; 
	public:
	variant* run(field&);
	calltree(tree*expression,function*function);
	~calltree();
};

class returntree:public tree
{	
	tree* exp; 
	public:
	variant* run(field&);
	returntree(tree*expression);
	~returntree();
};

class assignvartree:public tree
{	
	variant* var;
	tree* exp; 
	public:
	variant* run(field&);
	assignvartree(variant*var,tree*varexp);
	~assignvartree();
};

class assigneltree:public tree
{			
	variant* var;
	tree* adress,*exp; 
	public:
	variant* run(field&);
	assigneltree(tree*adress,tree*expression,variant* var);
	~assigneltree();
};

class elementtree:public tree
{	
	tree* left,*right; 
	public:
	variant* run(field&);
	elementtree(tree*left,tree*right);
	~elementtree();
};

class negtree:public tree
{	
	tree* exp; 
	public:
	variant* run(field&);
	negtree(tree*expression);
	~negtree();
};

class plustree:public tree
{	
	tree* left,*right; 
	public:
	variant* run(field&);
	plustree(tree*left,tree*right);
	~plustree();
};

class adressgettree:public tree
{	
	variant* var;
	tree* adress; 
	public:
	variant* run(field&);
	adressgettree(tree*adress,variant*var);
	~adressgettree();
};

class paramtree:public tree
{	
	variant* param;
	public:
	variant* run(field&);
	paramtree(variant*param);
	~paramtree();
};

class cvarianttree:public tree
{	
	variant* param;
	public:
	variant* run(field&);
	cvarianttree(variant*param);
	~cvarianttree();
};

class celementtree:public tree
{	
	element* param;
	public:
	variant* run(field&);
	celementtree(element*param);
	~celementtree();
};

class converttree:public tree
{	
	int type;
	variant* var;
	tree* adress;
	public:
	variant* run(field&);
	converttree(tree*adress,variant*var, int type);
	~converttree();
};

class dconverttree:public tree
{	
	int type;
	variant* var;
	public:
	variant* run(field&);
	dconverttree(variant*var,int type);
	~dconverttree();
};

class adresstree:public tree
{	
	inttree* first,*second;
	public:
	variant* run(field&);
	adresstree(inttree*first,inttree*second);
	~adresstree();
};
