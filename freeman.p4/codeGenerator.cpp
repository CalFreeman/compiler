#include <iostream>
#include <vector>
#include <stdlib.h>
#include "codeGenerator.h"
#include <sstream>

FILE *ofp;

void fileCheck(FILE* of){
  ofp = of;
}

vector<string> scope;
vector<string> scopeTemp;
int start = 0;
int i = 0;
int temp = 0;
int totalVars = 0;
int totalVarsTemp = 0;

void generator(node_t* node);
void genS(node_t* node);
void genV(node_t* node);
void genB(node_t* node);
void genQ(node_t* node);
void genT(node_t* node);
void genA(node_t* node);
void genW(node_t* node);
void genI(node_t* node);
void genG(node_t* node);
void genE(node_t* node);
void genZ(node_t* node);
void genM(node_t* node);
void genH(node_t* node);
void genR(node_t* node);
void createVar(token tk);
void createVarTemp(token tk);
void printVars();

string newT() {
    std::ostringstream ostringstream1;
    ostringstream1 << "T" << totalVars++;
    scopeTemp.push_back(ostringstream1.str());
    return ostringstream1.str();

}

// starting at root
void generator(node_t* node){
  cout << "Starting Code Genrator\n";
  if(node == NULL) {return;}

  genS(node);
  fprintf(ofp, "STOP\n");
  void printVars(); 
  for(int i = 0; i<scope.size();  i++){
    fprintf(ofp, static_cast<string>(scope[i] + "0 \n").c_str());
  }

}

void genS(node_t* node){
  cout << "prog. gen. <S>\n";
  if(node == NULL) {return;}
 
  genV(node->child2);
  genB(node->child3);
  return;

}

void genV(node_t* node){
  cout << "prog. gen. <V>\n";
  if(node == NULL) {return;}
  //cout << "tkinstance->child1 "<< node->child1->myToken.tokenInstance << endl;
  //cout << "tkinstance->2 "<< node->child2->myToken.tokenInstance << endl;
  //cout << "tkinstance->3 "<< node->child3->myToken.tokenInstance << endl;
  //cout << "tkinstance->4 "<< node->child4->myToken.tokenInstance << endl;
  if(node->child2 != NULL){
    scope.push_back(node->child2->myToken.tokenInstance);
    std:: string tempVar = newT();
    fprintf(ofp, "READ %s\n", tempVar.c_str());

    fprintf(ofp, "%s", static_cast<std::string>("STORE\n").c_str());
  }
  genV(node->child4);  

  return;
}

void genB(node_t* node){
  cout << "prog. gen. <B>\n";
  if(node == NULL) {return;} 
  genV(node->child1);
  genQ(node->child2);
  return;

}

void genQ(node_t* node){
  cout << "prog. gen. <Q>\n";
  if(node == NULL) {return;}

  genT(node->child1);
  genQ(node->child3);
  return;
}

void genT(node_t* node){
  cout << "prog. gen. <T> \n";
  if(node == NULL) {return;}
  if(node->child1->label == ",") {
    generator(node->child1);
  }
  //cout <<" T_ " << node->child1->myToken.tokenId << endl;  
  
  if(node->child1->myToken.tokenId == SCAN_tk){
    genA(node->child2);
    return;
  } else if (node->child1->myToken.tokenId == WRITE_tk){
    genW(node->child2);
    return;
  } else if (node->child1->myToken.tokenId == BEGIN_tk){
    genB(node->child2);
    return;
  } else if (node->child1->myToken.tokenId == IF_tk){
    genI(node->child2);
    return;
  } else if (node->child1->myToken.tokenId == REPEAT_tk){
    genG(node->child2);
    return;
  } else if (node->child1->myToken.tokenId == LET_tk){
    genE(node->child2);
    return;
  }
 
  return;
}

void genA(node_t* node){
  cout << "prog gen. <A> : " << endl;
  if(node == NULL) {return;}
  std:: string tempVar = newT();
  fprintf(ofp, "STORE %s\n", tempVar.c_str());
  return;
}

void genW(node_t* node){
  cout << "prog gen. <W>\n";
  if(node == NULL) {return;}
  genM(node->child1);
  std:: string tempVar = newT();
  fprintf(ofp, "WRITE %s\n", tempVar.c_str());
  return;
}

void genI(node_t* node){
  cout << "prog gen. <I>\n";
  if(node == NULL) {return;}
  genM(node->child1);
  genZ(node->child2);
  genM(node->child3);
  genT(node->child4);

  return;
}

void genG(node_t* node){
  cout << "prog gen. <G>\n";
  if(node == NULL) {return;}
  genM(node->child1);
  genZ(node->child2);
  genM(node->child3);
  genT(node->child4);

  return;
}

void genZ(node_t* node){
  cout << "prog gen. <Z>\n";
  if(node == NULL) {return;}
  // get RO and put in file
  return;
}

void genE(node_t* node){
  cout << "prog gen. <E>\n";
  if(node == NULL) {return;}
  genM(node->child2);

  return;
}

void genM(node_t* node){
  cout << "prog gen. <M>\n";
  if(node == NULL) {return;}
  genH(node->child1);
  fprintf(ofp, "STORE %s\n", node->child2->myToken.tokenInstance.c_str());
  genM(node->child3);

  return;
}

void genH(node_t* node){
  cout << "prog gen. <H>\n";
  if(node == NULL) {return;}
  if(node->child2 != NULL){
    genR(node->child2);
  }
  if(node->child1 != NULL){
    genR(node->child1);
  }
  return;
}

void genR(node_t* node){
  cout << "prog gen. <R>\n";
  if(node == NULL) {return;}

  return;
}




void createVarTemp(token tk){
  scopeTemp.push_back(tk.tokenInstance.c_str());
  totalVarsTemp++;
  return;
}

void createVar(token tk){
  for(i = 0; i < scope.size(); i++){
  // check if it exist  
  }
  
  scope.push_back(tk.tokenInstance.c_str());
  totalVars++;
  return;
}

void printVars(){
  for (int i = 0; i < scope.size(); i++){
    //fprintf(ofp, "%s\n", scope[i].c_str());
  }
}
