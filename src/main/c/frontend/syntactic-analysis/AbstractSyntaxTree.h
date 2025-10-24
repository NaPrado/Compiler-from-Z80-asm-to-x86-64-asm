#ifndef ABSTRACT_SYNTAX_TREE_HEADER
#define ABSTRACT_SYNTAX_TREE_HEADER

#include "../../support/logging/Logger.h"
#include "../../support/type/ModuleDestructor.h"
#include <stdlib.h>

/** Initialize module's internal state. */
ModuleDestructor initializeAbstractSyntaxTreeModule();

/**
 * This type definitions allows self-referencing types (e.g., an expression
 * that is made of another expressions, such as talking about you in 3rd
 * person, but without the madness).
 */

typedef enum InstructionType ExpressionType;
typedef enum RegisterName FactorType;
typedef enum ConditionType ConditionType;

typedef struct Program Program;
typedef struct CodeBlock CodeBlock;
typedef struct DataBlock DataBlock;
typedef struct Constant Constant;
typedef struct DataLine DataLine;


/**
 * Node types for the Abstract Syntax Tree (AST).
 */



enum InstructionType {
	INST_LD, INST_ADD, INST_SUB, INST_INC, INST_DEC,
    INST_AND, INST_OR, INST_XOR, INST_CP,
    INST_JP, INST_JR, INST_DJNZ, INST_CALL,
    INST_RET, INST_PUSH, INST_POP, INST_NOP
};

enum RegisterName {
    REG_A, REG_B, REG_C, REG_D, REG_E, REG_H, REG_L,
    REG_AF, REG_BC, REG_DE, REG_HL, REG_SP, REG_IX, REG_IY
};

enum ConditionType{
    COND_NZ, COND_Z, COND_NC, COND_C,
    COND_PO, COND_PE, COND_P, COND_M
};

struct Constant {
	int value;
};

struct Program {
	union {
		DataSeg * data_seg;
		CodeSeg * code_seg;
	};
};

struct DataBlock {
};

struct CodeBlock {
};

/**
 * Node recursive super-duper-trambolik-destructors.
 */

void destroyConstant(Constant * constant);
void destroyExpression(Expression * expression);
void destroyFactor(Factor * factor);
void destroyProgram(Program * program);

#endif
