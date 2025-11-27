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

typedef enum InstructionType InstructionType;
typedef enum RegisterName RegisterName;
typedef enum ConditionType ConditionType;
typedef enum LineType LineType;
typedef enum OperandType OperandType;
typedef enum DataType DataType;

typedef struct Constant Constant;
typedef struct Program Program;
typedef struct CodeSeg CodeSeg;
typedef struct DataSeg DataSeg;
typedef struct CodeBlock CodeBlock;
typedef struct DataBlock DataBlock;
typedef struct CodeLine CodeLine;
typedef struct DataLine DataLine;
typedef struct MacroDef MacroDef;
typedef struct Instruction Instruction;
typedef struct Operand Operand;
typedef struct Expr Expr;


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

enum OperandType{
    OPERAND_REGISTER8,
    OPERAND_REGISTER16,
    OPERAND_CONSTANT,
    OPERAND_CONDITION,
    OPERAND_MEMORY_HL,
    OPERAND_MEMORY_IXIY_DISP,
    OPERAND_MEMORY_ABS,
    OPERAND_SYMBOL
};

enum LineType{
   	LINE_INSTRUCTION, 
    LINE_MACRO,
    LINE_LABEL,
    LINE_EMPTY
};

enum DataType{
    DATA_DB,  // Define Byte
    DATA_DW,  // Define Word
    DATA_DS   // Define Storage
};

// Estructuras del AST

struct Constant {
	int value;
};

struct Program {
	DataSeg * dataSeg;
	CodeSeg * codeSeg;
};

struct CodeSeg {
	CodeBlock * codeBlock;
};

struct DataSeg {
	DataBlock * dataBlock;
};

struct CodeBlock {
	CodeLine** lines;
    int count;
};

struct DataBlock {
	DataLine** lines;
    int count;
};

struct CodeLine {
    LineType type;
    union {
        Instruction* instruction;
        MacroDef* macro;
        char* label;
    };
};

struct DataLine {
    char* label;        // Label obligatorio
    DataType dataType;  // DB, DW, or DS
    Operand** values;   // List of values (for DB/DW) or single size value (for DS)
    int valueCount;     // Number of values
};

struct MacroDef {
    char* name;
    char** params;   
    int paramCount;
    CodeBlock* body;    
};

struct Expr {
    int value;      
    char *symbol;  
};

struct Instruction {
    InstructionType type;
    Operand** operands;
    int operandCount;
};

struct Operand {
    OperandType type;
    union {
        RegisterName reg8;
        RegisterName reg16;
        int constantValue;
        ConditionType condition;
        char *symbol;
        Expr *expr; 
        struct {
            RegisterName base;
            Expr *disp;
        } mem_ixiy_disp;
        Expr *mem_abs;
    };
};

/**
 * Node recursive super-duper-trambolik-destructors.
 */

void destroyConstant(Constant *constant);
void destroyExpression(Expr *expression);
void destroyOperand(Operand *operand);
void destroyInstruction(Instruction *instruction);
void destroyMacroDef(MacroDef *macro);
void destroyCodeLine(CodeLine *line);
void destroyDataLine(DataLine *line);
void destroyCodeBlock(CodeBlock *block);
void destroyDataBlock(DataBlock *block);
void destroyCodeSeg(CodeSeg *codeSeg);
void destroyDataSeg(DataSeg *dataSeg);
void destroyProgram(Program *program);

#endif
