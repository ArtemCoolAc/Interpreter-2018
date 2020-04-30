#include <unordered_map>
#include <string>
#include "tree.h"


class ftab;

class function
{
	tree* stree;
	eltab deftab;
	public:
	variant param;
	variant* getdef(char*);
	bool adddef(char*);
	void settree(tree*);
	variant* run(variant,field&);
	function();
	~function();
};

class ftab
{
	std::unordered_map<std::string,function*> tab;
	public:
	bool add(char*);
	variant* getdef(char*,char*);
	function* getf(char*);
	bool adddef(char*,char*);
	void settree(char*,tree*);
	ftab();
	~ftab();
};

