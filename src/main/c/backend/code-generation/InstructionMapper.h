#ifndef INSTRUCTION_MAPPER_H
#define INSTRUCTION_MAPPER_H

#include "../../frontend/syntactic-analysis/AbstractSyntaxTree.h"
#include "../domain-specific/SymbolTable.h"

void generateInstruction(Instruction* insn, SymbolTable* table);
void generateInstructionWithParams(Instruction* insn, SymbolTable* table, char** params, int paramCount);
void generateOperand(Operand* operand, SymbolTable* table);
void generateOperandWithParams(Operand* operand, SymbolTable* table, char** params, int paramCount);
const char* conditionToX86(ConditionType cond);
const char* instructionToString(InstructionType type);

#endif
