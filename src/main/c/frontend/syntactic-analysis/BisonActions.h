#ifndef BISON_ACTIONS_HEADER
#define BISON_ACTIONS_HEADER

#include "../../support/logging/Logger.h"
#include "../../support/type/CompilerState.h"
#include "../../support/type/ModuleDestructor.h"
#include "../../support/type/TokenLabel.h"
#include "AbstractSyntaxTree.h"
#include "BisonParser.h"
#include <stdlib.h>

/** Initialize module's internal state. */
ModuleDestructor initializeBisonActionsModule();

/**
 * Bison semantic actions.
 */

/* Funciones de construcción del AST */
Block * AppendCodeLine(Block * block, Line * line);
Block * CodeBlockInit(Line * line);
Block * AppendDataLine(Block * block, Line * line);
Block * DataBlockInit(Line * line);
Program * ExpressionProgramSemanticAction(Block * codeBlock, Block * dataBlock);
char ** Z80IdListInit(void);
char ** Z80IdListInit1(char * id);
char ** Z80IdListAppend(char ** list, char * id);
Block * Z80CodeBlockInit(Line * line);
Block * Z80CodeBlockAppend(Block * block, Line * line);
Line * Z80MakeCodeLineMacroDef(char * name, char ** params, Block * body);
Line * Z80MakeCodeLineInsn(Instruction * insn);

/* Funciones de construcción de instrucciones */
Instruction * Z80Insn0(InstructionType type);
Instruction * Z80Insn1(InstructionType type, Operand * op);
Instruction * Z80Insn2(InstructionType type, Operand * op1, Operand * op2);

/* Funciones de construcción de operandos */
Operand * Z80OpReg8(RegisterName reg);
Operand * Z80OpReg16(RegisterName reg);
Operand * Z80OpMemHL(void);
Operand * Z80OpMemIdxDisp(RegisterName reg, Operand * disp);
Operand * Z80OpMemAbs(Operand * addr);
Operand * Z80OpImm(int value);
Operand * Z80OpSymbol(char * symbol);
Operand * Z80OpCond(ConditionType cond);

#endif
