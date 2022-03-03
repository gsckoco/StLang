#ifndef STERR_H
#define STERR_H

#include "StLex.h"
#include "StExpressions.h"

#include "stdio.h"
#include "signal.h"

void StErr_ExpectedToken(enum StLex_Tokens token, StLex_Loc* loc);

#endif