#include "eltab.h"

#define CONCATN 0 
#define CYCLEN 1
	#define WHILE 0
	#define UNTIL 1
#define IFN 2
	#define IFLESS 0
	#define IFZERO 1
	#define IFHIGH 2
	#define IFNLESS 3
	#define IFNZERO 4
	#define IFNHIGH 5
	#define IFWALL 6
	#define IFEXIT 7
#define COMMANDN 3
#define STRN 4
#define INTN 5
#define BOOLN 6
#define VARN 7
#define CALLFN 8
#define RETURNN 9
#define ASSIGNN 10
#define CONSTN 11
#define NEGN 12
#define PLUSN 13
#define ADRESSGETN 14
#define PARAMN 15
#define CONVN 16
#define ADRESSN 17
#define ASSIGNVARN 18
#define CVARN 19
#define CELN 20
#define ARRVARN 21
#define DCONVN 22
#define NULLTREE 23

class tree
{
	int operation;
	public:
	int gettype();
	virtual variant* run(field&)=0;
	tree(int operation);
	virtual ~tree();
};





