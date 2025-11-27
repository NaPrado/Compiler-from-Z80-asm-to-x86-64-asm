#ifndef INSTRUCTION_MAPPER_H
#define INSTRUCTION_MAPPER_H

#include "../../frontend/syntactic-analysis/AbstractSyntaxTree.h"

void generateInstruction(Instruction* insn);
void generateInstructionWithParams(Instruction* insn, char** params, int paramCount);
void generateOperand(Operand* operand);
void generateOperandWithParams(Operand* operand, char** params, int paramCount);
const char* conditionToX86(ConditionType cond);
const char* instructionToString(InstructionType type);

#endif
