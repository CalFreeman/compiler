#ifndef NODE_H
#define NODE_H

#include "token.h"
#include <string>
#include "scanner.h"

struct node_t {
  string label;
  node_t *child1, *child2, *child3, *child4;
  token myToken;

};

#endif
