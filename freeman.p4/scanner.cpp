#include <iostream>
#include "scanner.h"
#include "token.h"

#define CLMVAR 14
#define STATES 5
#define KEYWORDS 13

int keywordFlag = 0;
int errorStateFlag = 0;
 
using namespace std;

/*************************FSA TABLE*************************/
int fsaTable[7][18] = {
  {   4,   6,   6,   6,   6,   6,   6,   6,   6,   6,   6,   6,   6,   6,   1,   3,   0,1111},
  {1001,1001,1001,1001,1001,1001,1001,1001,1001,1001,1001,1001,1001,1001,   2,   2,1001,  11},
  {1001,1001,1001,1001,1001,1001,1001,1001,1001,1001,1001,1001,1001,1001,   2,   2,1001,  11},
  {1002,1002,1002,1002,1002,1002,1002,1002,1002,1002,1002,1002,1002,1002,1002,   3,1002,  11},
  {5,   1003,1003,1003,1003,1003,1003,1003,1003,1003,1003,1003,1003,1003,1003,1003,1003,  11},
  {1003,1003,1003,1003,1003,1003,1003,1003,1003,1003,1003,1003,1003,1003,1003,1003,1003,  11},
  {1003,1003,1003,1003,1003,1003,1003,1003,1003,1003,1003,1003,1003,1003,1003,1003,1003,1003}
};

/*************************FSA TABLE END*************************/


/*************************COLUMN CODE*************************/

typedef struct tableKey{
  int key;
  int value;
}tableKey;


/* chars for operators and delimeters column index of FSA */
/*NEED TO FIGURE OUT HOW T HANDLE == this might be a problem in future*/
/*do i need to use a full table?*/
tableKey charColumn[CLMVAR] = {
  {'=', 0},
  {'<', 1},
  {'>', 2},
  {':', 3},
  {'&', 4},
  {'+', 5},
  {'-', 6},
  {'*', 7},
  {'/', 8},
  {'#', 9},
  {'.', 10},
  {'[', 11},
  {']', 12},
  {',', 13},
};


/*get the column index for FSA Table of the value passed*/
	/* values must be adjusted getColumnIndex*/
int getColumnIndex(char lookahead){
  /*if passed value is equal to operator or delimeter */
  for(int i =0; i < CLMVAR; i++){
    if(charColumn[i].key == lookahead) {
      return charColumn[i].value;
    }
  }
  
  /*othewise determine if passed value is char, int, spcae or EOF. (else pass)*/
  /*have: isalpha, isdigit, ispace, EOF, else*/
  /*think about isalpha lower, isalpha upper, isdigit, ispace, EOF, else */
  if(isalpha(lookahead)) {
    if(islower(lookahead)){
      return 14;
    } else if (isupper(lookahead)){
      return 14;
    }
  } else if (isdigit(lookahead)) {
    return 15;
  } else if (isspace(lookahead)) {
    return 16;
  } else if (lookahead == EOF) {
    return 17;
  } else {
    return 18;
  }  
}

/*************************COLUMN CODE END*************************/


/*************************TOKEN CODE BEGIN*************************/

typedef struct tokenKey{
  int key;
  tokenID tokenKeyValue;
} tokenKey;

/* define keywords these match enum tokenID in token.h*/
token keywords[KEYWORDS] = {
  {ERROR_tk, "test", 0},
  {BEGIN_tk, "begin", 0},
  {END_tk,"end",0},
  {REPEAT_tk,"repeat",0},
  {IF_tk, "if", 0},
  {THEN_tk, "then", 0},
  {LET_tk, "let", 0},
  {VOID_tk, "void", 0},
  {VAR_tk, "var", 0},
  {RETURN_tk, "return", 0},
  {SCAN_tk, "scan", 0},
  {WRITE_tk, "write", 0},
  {PROGRAM_tk, "program", 0},
};


/* final state tokens*/
tokenKey endStates[STATES] = {
  {1001, IDENT_tk},
  {1002, NUM_tk},
  {1003, OP_tk},
  {1111, EOF_tk},
  {11, ERROR_tk},
};

token keywordFilter(int j, string instanceW, int nextState, int lineNumber){
  token token;
  if(keywords[j].tokenInstance == instanceW){
    //cout << "token.tokenInstance = " << keywords[j].tokenInstance << endl;
    token.tokenId = keywords[j].tokenId;
    token.tokenInstance = instanceW;
    token.lineNumber = lineNumber;
    keywordFlag = 1;
    //cout << token.tokenId;
  }
  return token;
}

/*************************TOKEN CODE END*************************/

/****************************FILTER******************************/
/*filter to replace testscanner*/
token filter(FILE *fp, int &lineNumber){
  int lookahead;
  token tk;
  while(lookahead = fgetc(fp)){
    if(lookahead == EOF){
      break;
    }
   
    if(lookahead == '\n'){
      lineNumber++;
    }   

    ungetc(lookahead, fp);
    tk = scanner(fp, lineNumber);
    tk.lineNumber = lineNumber;
    //cout << "tokenID: " << tk.tokenId << endl;;
    return tk;
  } /*while(lookahead = fetc(fp))*/ 
  
  token token;
  token.tokenId = endStates[3].tokenKeyValue;
  token.tokenInstance = "EOF";
  token.lineNumber = lineNumber;
  return tk;
}


/****************************************************************/

token scanner(FILE *fp, int &lineNumber){
  /* lookahead variable */
  int currentState = 0;
  int nextState = 0;
  int lookahead;
  int columnIndex;
  string instance;
  token token;
  /*iteration vars*/
  int i = 0;
  int j = 0;


  while(currentState < 1000 && currentState >= 0){
    lookahead = fgetc(fp);
    errorStateFlag = 0;
    /* lineNumber increment if needed*/
    if(lookahead == '\n'){
      lineNumber++;
    }

    /* skip over comments FILTER_1*/
    if(lookahead == '%'){
      //cout << "IGNORING COMMENT\n";
      do{
        lookahead = fgetc(fp);
        if(lookahead == '\n'){
          lineNumber++;
        }
      }while(lookahead != '%');
      lookahead = fgetc(fp); 
    }

    /* column index FILTER1 */
    /* map character to FSA Table column number FILTER_1*/
    columnIndex = getColumnIndex(lookahead);
    //cout << "FSA column index: " << columnIndex << endl;
      
    if(columnIndex > 17){
      cout << "Scanner Error: unallowed Operator Or Delimiter" << endl;
      token.lineNumber = lineNumber;
      token.tokenId = endStates[5].tokenKeyValue;
      token.tokenInstance = "ERROR_tk";
      break;
    }
 
    /* get next state */
    /*if this state > 1000 assign it a token*/
    nextState = fsaTable[currentState][columnIndex];
    //cout << "nextState fsaTable: " << nextState << endl;    
    
    /* determine next state */
    /* if final state*/
    if(nextState > 1000){
      keywordFlag = 0; 
      /*if EOF token*/
      if(nextState == 1111){
        //cout << "nextState fsaTable: " << nextState << endl;
        cout << "EOF_tk\n";
        token.lineNumber = lineNumber;
        token.tokenId = endStates[4].tokenKeyValue;
        token.tokenInstance = "EOF_tk";
        return token;
      }      

      /*if keyword FILTER 2*/
      /* iterate through each state*/
      if(nextState == 1001){
        for(j = 0; j < KEYWORDS; j++){
          //cout << instance << endl;
          token = keywordFilter(j, instance, nextState, lineNumber);
        }
      }

      
      /* iterate through each end state and assign token value*/
      if(keywordFlag == 0){
        for(i = 0; i < STATES; i++){
          if(endStates[i].key == nextState){
            //cout << "nextState " << endl;
            token.tokenId = endStates[i].tokenKeyValue;
            token.tokenInstance = instance;
            token.lineNumber = lineNumber;
          }
        }
      }
     
      if(keywordFlag == 1){
        token.tokenInstance = instance;
      }
      /*failsafe return*/ //DOES NOT EFFECT EOF RETURN
      return token;

    } else {
      /*else not final state */
      /*static_cast<char> to obtain ascii value*/
      char word = static_cast<char>(lookahead);
      
      /*if firts char is uppercase error*/
      if(errorStateFlag == 0){
        if(isupper(word)){
          cout << "Scanner Error: identifier starting with capitol" << endl;
        }
        errorStateFlag = 1;
      }

      /* if not a space append character at end */ 
      if(!isspace(word)){
        instance += word;
      }

      /* assign nextState to current for next itteration can be done BELOW*/
    }

  /* assign nextState to current for next itteration can be done ABOVE*/ 
  currentState = nextState; 
 
  } /* END WHILE(CURRENT < 1000) */

  
  return token; 
}


