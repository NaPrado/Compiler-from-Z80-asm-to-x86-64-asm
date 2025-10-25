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
CodeBlock * AppendCodeLine(CodeBlock * block, CodeLine * line);
CodeBlock * CodeBlockInit(CodeLine * line);
DataBlock * AppendDataLine(DataBlock * block, DataLine * line);
DataBlock * DataBlockInit(DataLine * line);
CodeSeg * CodeSegSemanticAction(CodeBlock * codeBlock);
DataSeg * DataSegSemanticAction(DataBlock * dataBlock);
Program * ExpressionProgramSemanticAction(DataSeg * dataSeg, CodeSeg * codeSeg);
char ** Z80IdListInit(void);
char ** Z80IdListInit1(char * id);
char ** Z80IdListAppend(char ** list, char * id);
CodeBlock * Z80CodeBlockInit(CodeLine * line);
CodeBlock * Z80CodeBlockAppend(CodeBlock * block, CodeLine * line);
CodeLine * Z80MakeCodeLineMacroDef(char * name, char ** params, CodeBlock * body);
CodeLine * Z80MakeCodeLineInsn(Instruction * insn);
DataLine * Z80MakeDataLineDb(Operand ** exprList);
DataLine * Z80MakeDataLineDw(Operand ** exprList);
DataLine * Z80MakeDataLineDs(Operand * expr);

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
