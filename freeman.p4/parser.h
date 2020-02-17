#ifndef PARSER_H
#define PARSER_H
#include "node.h"

//extern FILE* fp;
extern int lineNumber;

node_t* S();
node_t* B();
node_t* V();
node_t* M();
node_t* H();
node_t* R();
node_t* Q();
node_t* T();
node_t* A();
node_t* W();
node_t* I();
node_t* G();
node_t* E();
node_t* Z();

node_t* parser(FILE*);
node_t* getNode(string s);

#endif
