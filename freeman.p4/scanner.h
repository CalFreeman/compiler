#ifndef SCANNER_H
#define SCANNER_H

#include <stdio.h>
#include <iostream>
#include <string>
#include "token.h"

using namespace std;

token scanner(FILE *fp, int &lineNumber);
token filter(FILE*, int&);

#endif
