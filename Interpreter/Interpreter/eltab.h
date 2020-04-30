#include <unordered_map>
#include <string>
#include "robot.h"

class eltab
{
	std::unordered_map<std::string,variant*> tab;
	bool my_predicate(char c);
	public:
	bool add(char*);
	variant* get(char*);
	bool set(char*,variant*);
	void print();
	eltab(const eltab&);
	void copy(const eltab&);
	void reset();
	eltab();
	~eltab();
};
