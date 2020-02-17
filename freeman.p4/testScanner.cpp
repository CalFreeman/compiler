#include <stdio.h>
#include <iostream>
#include <string>
#include "scanner.h"
#include "testScanner.h"
#include "token.h"

using namespace std;

/* token names array of strings*/
string tokenNames[] = {
  "ERROR_tk", "IDENT_tk", "NUM_tk", "OP_tk", "EOF_tk", 
  "BEGIN_tk", "END_tk", "REPEAT_tk",
  "IF_tk", "THEN_tk", "LET_tk", "VOID_tk", 
  "VAR_tk", "RETURN_tk", "SCAN_tk", "WRITE_tk", "PROGRAM_tk",
};


/* printing token as they are scanned */
void printToken(token token){
  cout << "tokenID: " << tokenNames[token.tokenId];
  cout << ", tokenInstance: " << token.tokenInstance;
  cout << " lineNumber: " << token.lineNumber << endl;
}


/*used for EOF token*/
typedef struct tokenKey{
  int key;
  tokenID tokenKeyValue;
} tokenKey;


/* final state tokens used for EOF token*/
tokenKey endStatesE[5] = {
  {1001, IDENT_tk},
  {1002, NUM_tk},
  {1003, OP_tk},
  {1111, EOF_tk},
  {11, ERROR_tk},
};


/* scanner driver*/
void testScanner(FILE *fp){
  int lookahead = 0;
  int lineNumber = 1;

  /* handle the loop so it does not ignore EOF*/
  //while((lookahead = fgetc(fp)) != EOF){
  while(lookahead = fgetc(fp)){
  //do{
    if(lookahead == EOF){
      break;
    }
    ungetc(lookahead, fp);
    token token = scanner(fp, lineNumber);
    token.lineNumber = lineNumber;
    printToken(token);
  } //while ( lookahead = fgetc(fp) != EOF);
  
  token token;
  token.tokenId = endStatesE[3].tokenKeyValue;
  token.tokenInstance = "EOF";
  token.lineNumber = lineNumber;
  printToken(token);
  
  fclose(fp);

}
