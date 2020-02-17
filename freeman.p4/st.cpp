#include <set>
#include <string>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "st.h"
#include "node.h"

std::set <std::string> STV;

bool verify(token tk){
  if(STV.count(tk.tokenInstance) == 0) {
  cout << "verify--> " << tk.tokenInstance << " ";
    return 0;
  }
  cout << "verify-> " << tk.tokenInstance << endl;
  return 1;
}

int insert(token tk){
  if(STV.count(tk.tokenInstance) == 0){
    STV.insert(tk.tokenInstance);
    cout << "inserted-> " << tk.tokenInstance << endl;
    return 1;
  } else {
    cout << "inserted-> " << tk.tokenInstance << " ";
  return 0;
  }
}

void scopeCheck(node_t* node){

  if(node == NULL){
    return;
  }

  token token = node->myToken;
  //cout << node->label;
  //cout << "tokenId " << token.tokenId << " token.tokenInstance " << token.tokenInstance << endl;

  

  if(strcmp(node->label.c_str(), "IDENTIFIER .") == 0){
    if(insert(node->myToken) == 0){
      printf("identifier already initialised\n");
      cout << token.tokenInstance;
      perror("exiting"); exit(1);
      return;
    } 
  } else if ( strcmp(node->label.c_str(), "IDENTIFIER") == 0){
    if(verify(node->myToken) == 0){
      printf("identifier not declared.\n");
      cout << token.tokenInstance;
      perror("exiting"); exit(1);
      return; /* unused returns*/
    }
  }

  scopeCheck(node->child1);
  scopeCheck(node->child2);
  scopeCheck(node->child3);
  scopeCheck(node->child4);

}

