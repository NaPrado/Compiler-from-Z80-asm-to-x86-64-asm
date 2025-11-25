#ifndef SYMBOL_TABLE_H
#define SYMBOL_TABLE_H

#include "../code-generation/Generator.h"
#include "../../frontend/syntactic-analysis/AbstractSyntaxTree.h"
#include <stdbool.h>

typedef enum {
    SYMBOL_LABEL,
    SYMBOL_MACRO,
    SYMBOL_DATA_LABEL
} SymbolType;

typedef struct {
    char* name;
    SymbolType type;
    int address;
    MacroDef* macroDef;
} Symbol;

typedef struct {
    Symbol** symbols;
    int count;
    int capacity;
} SymbolTable;

// SymbolTable* createSymbolTable();
// void destroySymbolTable(SymbolTable* table);
// bool addSymbol(SymbolTable* table, const char* name, SymbolType type, int address, MacroDef* macroDef);
// Symbol* findSymbol(SymbolTable* table, const char* name);
// bool symbolExists(SymbolTable* table, const char* name);

#endif
