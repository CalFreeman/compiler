#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <fstream>
#include <string.h>

#include "scanner.h"
#include "testScanner.h"
#include "token.h"
#include "parser.h"
#include "treePrint.h"
#include "st.h"
#include "codeGenerator.h"

using namespace std;

bool exists_test1 (const std::string& name) {
    if (FILE *file = fopen(name.c_str(), "r")) {
        fclose(file);
        return true;
    } else {
        return false;
    }   
}

int main(int argc, const char* argv[]){

  // file variables  
  string f_name;
  FILE *fp;
  ofstream tempFile;  
  int level = 0;

  if (argc > 2){
    cout << argv[0] << ' ' << argv[1] << ' ' << argv[2] << endl;
    cout << "Fatal: Improper usage" << endl;
    cout << "Usage: P1 [filename]" << endl;
    exit(1);
  } else if ( argc == 2) {
    f_name =  argv[1];
    cout << "attempting to read provided file..." << endl;
  } else if (argc == 1){
    cout << "would be keyboard redirection. . . improper usage atm" << endl;
    exit(1);
  } else {
    cout << "Improper usage" << endl;
  }
  
  // check if file provided is empty
  char ch;
  if(exists_test1(f_name)){
    fp = fopen(f_name.c_str(), "r");
  } else {
    perror("Error: empty file provided exiting program");    
    exit(1);
  }
 
  string f_temp;

  f_temp =  + "temp.asm";
  FILE *ofp = fopen(f_temp.c_str(), "w");
  fileCheck(ofp);
  
 
  /*call testScanner driver*/ //now broken 11/29
  //filter(fp, level);
  //testScanner(fp);  

  /* call parser driver*/
  node_t* parseTree = parser(fp);
    
  /* print tree */  
  printTree(parseTree, level); 

  cout << "starting scope check" << endl;
  scopeCheck(parseTree);
  cout << "scope check done" << endl;

  cout << "Code generator" << endl;
  generator(parseTree);
  cout << "code generation done" << endl; 

  fclose(fp);
  return 0;
}
