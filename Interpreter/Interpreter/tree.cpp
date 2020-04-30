#include "tree.h"

int tree::gettype() {return operation;}

tree::tree(int operation) : operation(operation) {}
tree::~tree(){}
