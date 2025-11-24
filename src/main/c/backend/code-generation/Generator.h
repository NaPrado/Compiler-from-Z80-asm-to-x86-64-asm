#ifndef GENERATOR_HEADER
#define GENERATOR_HEADER

#include "../../frontend/syntactic-analysis/AbstractSyntaxTree.h"
#include "../../support/language/String.h"
#include "../../support/logging/Logger.h"
#include "../../support/type/CompilerState.h"
#include "../../support/type/ModuleDestructor.h"
#include "../domain-specific/SymbolTable.h"
#include <stdarg.h>
#include <stdio.h>

ModuleDestructor initializeGeneratorModule();
void executeGenerator(CompilerState* compilerState, SymbolTable* symbolTable);

#endif
