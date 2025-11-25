// #include "SymbolTable.h"
// #include "../../support/logging/Logger.h"
// #include <stdlib.h>
// #include <string.h>

// #define INITIAL_CAPACITY 16

// static Logger* _logger = NULL;

// SymbolTable* createSymbolTable() {
//     if (_logger == NULL) {
//         _logger = createLogger("SymbolTable");
//     }
//     SymbolTable* table = (SymbolTable*)calloc(1, sizeof(SymbolTable));
//     table->capacity = INITIAL_CAPACITY;
//     table->symbols = (Symbol**)calloc(table->capacity, sizeof(Symbol*));
//     table->count = 0;
//     logDebugging(_logger, "Symbol table created with capacity %d", table->capacity);
//     return table;
// }

// void destroySymbolTable(SymbolTable* table) {
//     if (table == NULL) {
//         return;
//     }
//     for (int i = 0; i < table->count; i++) {
//         if (table->symbols[i] != NULL) {
//             if (table->symbols[i]->name != NULL) {
//                 free(table->symbols[i]->name);
//             }
//             free(table->symbols[i]);
//         }
//     }
//     free(table->symbols);
//     free(table);
//     logDebugging(_logger, "Symbol table destroyed");
// }

// bool addSymbol(SymbolTable* table, const char* name, SymbolType type, int address, MacroDef* macroDef) {
//     if (table == NULL || name == NULL) {
//         return false;
//     }
//     if (symbolExists(table, name)) {
//         logError(_logger, "Symbol '%s' already exists", name);
//         return false;
//     }
//     if (table->count >= table->capacity) {
//         table->capacity *= 2;
//         table->symbols = (Symbol**)realloc(table->symbols, table->capacity * sizeof(Symbol*));
//         logDebugging(_logger, "Symbol table expanded to capacity %d", table->capacity);
//     }
//     Symbol* symbol = (Symbol*)calloc(1, sizeof(Symbol));
//     symbol->name = strdup(name);
//     symbol->type = type;
//     symbol->address = address;
//     symbol->macroDef = macroDef;
//     table->symbols[table->count++] = symbol;
//     logDebugging(_logger, "Added symbol '%s' (type=%d, address=%d)", name, type, address);
//     return true;
// }

// Symbol* findSymbol(SymbolTable* table, const char* name) {
//     if (table == NULL || name == NULL) {
//         return NULL;
//     }
//     for (int i = 0; i < table->count; i++) {
//         if (strcmp(table->symbols[i]->name, name) == 0) {
//             return table->symbols[i];
//         }
//     }
//     return NULL;
// }

// bool symbolExists(SymbolTable* table, const char* name) {
//     return findSymbol(table, name) != NULL;
// }
