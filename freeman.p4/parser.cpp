#include <iostream>
#include <stdlib.h>
#include "parser.h"
#include "token.h"

token tk;
FILE *fp = NULL;
int lineNumber = 1;
int level = 0;

/* <Z> -> < | > | : | = | == */
node_t* Z(){
  node_t *node = getNode("<Z>");
  //cout << "Z() " << tk.tokenId << " " << tk.tokenInstance << endl;
  if(tk.tokenInstance == "<"){
    node->child1 = getNode("<");
    tk = filter(fp, lineNumber);
    return node;
  } else if (tk.tokenInstance == ">"){
    node->child1 = getNode(">");
    tk = filter(fp, lineNumber);
    return node;
  } else if (tk.tokenInstance == ":"){
    node->child1 = getNode(":");
    tk = filter(fp, lineNumber);
    return node;
  } else if (tk.tokenInstance == "="){
    node->child1 = getNode("=");
    tk = filter(fp, lineNumber);
    return node;
  } else if (tk.tokenInstance == "=="){
    node->child1 = getNode("==");
    tk = filter(fp, lineNumber);
    return node;
  } else {
    perror("Z() wrong token, expeceted OP_tks");
    exit(1);
  }

}

/* <R> -> identifier | number */
node_t* R(){
  node_t *node = getNode("<R>");
  cout << "R() " << tk.tokenId << " " << tk.tokenInstance << endl;
  if(tk.tokenId == IDENT_tk) {
    node->child1 = getNode("IDENTIFIER");
    node->child1->myToken = tk;
    tk = filter(fp, lineNumber);
    //cout << "R()_if " << tk.tokenId << " " << tk.tokenInstance << endl;
    return node;
  } else if (tk.tokenId == NUM_tk){
    node->child1 = getNode("NUMBER");
    node->child1->myToken = tk;
    tk = filter(fp, lineNumber);
    return node;
  } else {
    perror("R() wrong token, expected IDENT or NUM");
    exit(1);
  }
}

/* <H> -> &<R>|<R> */
node_t* H(){
  node_t *node = getNode("<H>");
  cout << "H() " << tk.tokenId << " " << tk.tokenInstance << endl;
  if(tk.tokenId == OP_tk && tk.tokenInstance == "&"){
    node->child1 = getNode("&");
    tk = filter(fp, lineNumber);
    node->child2 = R();
    return node;
  } else if (tk.tokenId == IDENT_tk || tk.tokenId == NUM_tk){
    node->child1 = R(); // check if wrong token?
    return node;
  } else {
    perror("H() wrong token, expected IDENT or NUM");
    //exit(1);
    return node;
  }
}


/* <M> -> <H>+<M> | <H>-<M> | <H>/<M> | <H>*<M> | <H> */
node_t* M(){
  node_t *node = getNode("<M>");
  //cout << "M() " << tk.tokenId << " " << tk.tokenInstance << endl;
  node->child1 = H();
  if(tk.tokenId == OP_tk && tk.tokenInstance == "+"){ 
    node->child2 = getNode("+");
    tk = filter(fp, lineNumber);
    node->child3 = M();
  } else if(tk.tokenId == OP_tk && tk.tokenInstance == "-"){
    node->child2 = getNode("-");
    tk = filter(fp, lineNumber);
    node->child3 = M();
    return node;
  } else if (tk.tokenId == OP_tk && tk.tokenInstance == "/"){
    node->child2 = getNode("/");
    tk = filter(fp, lineNumber);
    node->child3 = M();
    return node;
  } else if (tk.tokenId == OP_tk && tk.tokenInstance == "*"){
    node->child2 = getNode("*");
    tk = filter(fp, lineNumber);
    node->child3 = M();
    return node;
  } else {
    return node;
  }
}

/* <A> -> scan identifier | scan number */
node_t* A(){
  node_t *node = getNode("<A>");
  cout << "A() " << tk.tokenId << " " << tk.tokenInstance << endl;
  if(tk.tokenId == IDENT_tk){
    //cout << "A() id_tk" << endl;
    node->child1 = getNode("IDENTIFIER");
    node->child1->myToken = tk;
    tk = filter(fp, lineNumber);
    return node; 
  } else if (tk.tokenId == NUM_tk){
    node->child1 = getNode("NUMBER");
    node->child1->myToken = tk;
    tk = filter(fp, lineNumber);
    return node;
  } else {
    perror("A() wrong token, expected ident or num");
    exit(1);
  } 
}

/* <W> -> write<M> */
node_t* W(){
  node_t *node = getNode("<W>");
  //cout << "W() " << tk.tokenId << " " << tk.tokenInstance << endl;
  //tk = filter(fp, lineNumber);
  node->child1 = M();
  return node;
  
}
/* <I> -> if[<M><Z><M>]<T> */
node_t* I(){
  node_t *node = getNode("<I>");
  //cout << "I() " << tk.tokenId << " " << tk.tokenInstance << endl;
  if(tk.tokenInstance == "["){
    tk = filter(fp, lineNumber);
    node->child1 = M();
    node->child2 = Z();
    node->child3 = M();
    if(tk.tokenInstance == "]"){
      tk = filter(fp, lineNumber);
      node->child4 = T();
      return node;
    } else {
      perror("I() wrong token, expected ]");
      exit(1);
    }
  } else {
   perror("I() wrong token, expected [");
   exit(1);
  }
}

/* <G> repeat[<M><Z><M>]<T> */
node_t* G(){
  node_t *node = getNode("<G>");
  //cout << "G() " << tk.tokenId << " " << tk.tokenInstance << endl;
  if(tk.tokenInstance == "["){
    tk = filter(fp, lineNumber);
    node->child1 = M();
    node->child2 = Z();
    node->child3 = M();
    if(tk.tokenInstance == "]"){
      tk = filter(fp, lineNumber);
      //cout << "G()_] " << tk.tokenId << " " << tk.tokenInstance << endl;
      node->child4 = T();
      return node;
    } else {
      perror("G() wrong token, expected ]");
      exit(1);
    }
  } else {
    perror("G() wrong token, expected [");
    exit(1);
  }
}

/* <E> let identifier :<M> */
node_t* E(){
  node_t *node = getNode("<E>");
  cout  << "E() " << tk.tokenId << " " << tk.tokenInstance << endl;
  if(tk.tokenId == LET_tk){
    tk = filter(fp, lineNumber);
    cout << "E()_ " << tk.tokenId << " " << tk.tokenInstance << endl;
    if(tk.tokenId == IDENT_tk){
      node->child1 = getNode("IDENTIFIER");
      node->child1->myToken = tk;
      tk = filter(fp, lineNumber);
      cout << "E()__ " << tk.tokenId << " " << tk.tokenInstance << endl;
      if(tk.tokenInstance == ":"){
        tk = filter(fp, lineNumber);
        node->child2 = M();
        return node;
      } else {
        perror("E() wrong token, expected OP_tk");
        exit(1);
      }
    } 

  } else {
    perror("E() wrong token, expected LET");
    exit(1);
  }
}


/* <T> -> <A>, | <W>, | <B> | <I>, | <G>, | <E> , */
node_t* T(){
  node_t *node = getNode("<T>");
  cout << "<T> " << tk.tokenId << " " << tk.tokenInstance<< endl;
  if(tk.tokenId == SCAN_tk){
    node->child1 = getNode("scan");
    tk = filter(fp, lineNumber);
    node->child2 = A();
    //tk = filter(fp, lineNumber);
    if(tk.tokenInstance != ","){
      //perror("T()_scan missing ,");
    } else {
      tk = filter(fp, lineNumber);
      return node;
    }
  } else if (tk.tokenId == WRITE_tk){
    node->child1 = getNode("write");
    tk = filter(fp, lineNumber);
    node->child2 = W();
    //tk = filter(fp, lineNumber);
    //cout << "_write " << tk.tokenId << " " << tk.tokenInstance << endl;
    if(tk.tokenInstance != ","){
      //perror("T()_write missing ,");
    } else {
      tk = filter(fp, lineNumber);
      return node;
    }
  } else if (tk.tokenId == BEGIN_tk){
    node->child1 = B(); // begin -> end case
  } else if (tk.tokenId == IF_tk){
    node->child1 = getNode("if");
    tk = filter(fp, lineNumber);
    node->child2 = I();
    //tk = filter(fp, lineNumber);
    //cout << "_if " << tk.tokenId << " " << tk.tokenInstance << endl;
    if(tk.tokenInstance != ","){
      //perror("T()_if missing ,");
    } else {
      tk = filter(fp, lineNumber);
      return node;
    }
  } else if (tk.tokenId == REPEAT_tk){
    node->child1 = getNode("repeat");
    tk = filter(fp, lineNumber);
    node->child2 = G();
    //tk = filter(fp, lineNumber);
    if(tk.tokenInstance != ","){
      //perror("T()_repeat missing ,");
    } else {
      tk = filter(fp, lineNumber);
      return node;
    }
  } else if (tk.tokenId == LET_tk){
    node->child1 = getNode("let");
    //tk = filter(fp, lineNumber);
    node->child2 = E();
    if(tk.tokenInstance != ","){
      //perror("T()_let missing ,");
    } else {
      tk = filter(fp, lineNumber);
      return node;
    }
  } else {
    //perror("T() wrong token");
    return node;
    //exit(1);
  }

  return node;
}

/* <Q> -> <T>#<Q> | empty */
node_t* Q(){  
  node_t *node = getNode("<Q>");
  cout << "Q()" << tk.tokenId << " " << tk.tokenInstance << endl;
  if(tk.tokenId == REPEAT_tk || tk.tokenId == IF_tk || tk.tokenId == LET_tk || 
    tk.tokenId == SCAN_tk || tk.tokenId == WRITE_tk || tk.tokenId == BEGIN_tk){ 
    
    node->child1 = T();
    //tk = filter(fp, lineNumber);
    cout << "_Q() " << tk.tokenInstance << " " << tk.tokenId << " " << endl;
    if(tk.tokenId == OP_tk && tk.tokenInstance == "#"){ /*tk.tokenInstance == '#' */
      node->child2 = getNode("#");
      tk = filter(fp, lineNumber);
      node->child3 = Q();
      return node;
    } else if (tk.tokenId == END_tk){
      return node;
    } else {
      cout << "Q()-> " << tk.tokenId << " " << tk.tokenInstance << endl;
      //perror("Q() wrong token, expected # or END");
      exit(1);
      return node;
    }
  // return node;
  }
    //perror("Q() empty set");
    return node;
}

/* <V> -> empty | var identifier . <V> */
node_t* V(){
  node_t *node = getNode("<V>");
  //cout << "V() " << tk.tokenId << " " << tk.tokenInstance << endl;
  if(tk.tokenId == REPEAT_tk || tk.tokenId == IF_tk || tk.tokenId == LET_tk || tk.tokenId == SCAN_tk
    || tk.tokenId == WRITE_tk || tk.tokenId == END_tk || tk.tokenId == BEGIN_tk) {
    return node; /* return if empty, checked the post token */
  } else if (tk.tokenId == VAR_tk){
    //node->child1 = getNode("VAR");
    //node->child1->myToken = tk; /* dont need to store?*/
    tk = filter(fp, lineNumber);
    if(tk.tokenId == IDENT_tk){ // not sure if this is correct comparsion
      node->child2 = getNode("IDENTIFIER .");
      node->child2->myToken = tk;
      tk = filter(fp, lineNumber);
      if(tk.tokenId == OP_tk && tk.tokenInstance == "."){
        //node->child3 = getNode(".");
        tk = filter(fp, lineNumber);
        node->child4 = V();
        //cout << "end of var id .\n";
        return node;
      } else {
        perror("V() wrong tk expected OP_tk");
        exit(1);
      }
    } else {
      perror("V() wrong tk expected IDENT_tk");
      exit(1);
    }
  } else {
    perror("V() wrong tk expected VAR_tk");
    exit(1);
  }
  return node;
}

// <B> -> begin <V> <Q> end
node_t* B(){
  node_t *node = getNode("<B>");
  cout << "B() " << tk.tokenId << " " << tk.tokenInstance << endl;
  if(tk.tokenId == BEGIN_tk){
    //cout << "begin_tk\n";
    tk = filter(fp, lineNumber);
    node->child1 = V();
    node->child2 = Q();
    cout << "B() " << tk.tokenId << " " << tk.tokenInstance << endl;
    if(tk.tokenId == END_tk){
      tk = filter(fp, lineNumber);
      cout << "B()_end " << tk.tokenId << " " << tk.tokenInstance << endl;
      return node;
    } else { 
      perror("B() wrong tk expected END_tk");
      exit(1);
    }
  } else {
    perror("B() wrong tk expected BEGIN_tk");
    exit(1);
  }
  return node;
} 

// <S> -> program <V> <B>
node_t* S(){
  node_t *node = getNode("<S>");
  cout << "S()" << tk.tokenId << " " << tk.tokenInstance << endl; /**/
  if(tk.tokenId == PROGRAM_tk){
    //cout << " program_tk\n"; /**/
    tk = filter(fp, lineNumber);
    node->child2 = V();
    node->child3 = B();
    return node;
  } else {
    perror("S() Error expected PROGRAM_tk");
    exit(1);
  }
  return node;
}


// get node
node_t *getNode(string s){
  node_t *node = new node_t;
  node->label = s;
  return node;
}

// parser driver
node_t* parser(FILE* input_file) {
  fp = input_file;
  node_t *parseTreeNode;
  cout << "lets parse\n";
  tk = filter(fp, lineNumber);
  //cout << "---" << tk.tokenInstance << "\n";
  parseTreeNode = S();

  cout << "Parsed successfully.\n";
  return parseTreeNode;

}
