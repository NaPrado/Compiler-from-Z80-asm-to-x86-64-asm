#include "BisonActions.h"

/* MODULE INTERNAL STATE */

static CompilerState * _compilerState = NULL;
static Logger * _logger = NULL;

/** Shutdown module's internal state. */
void _shutdownBisonActionsModule() {
	if (_logger != NULL) {
		logDebugging(_logger, "Destroying module: BisonActions...");
		destroyLogger(_logger);
		_logger = NULL;
	}
	_compilerState = NULL;
}

ModuleDestructor initializeBisonActionsModule(CompilerState * compilerState) {
	_compilerState = compilerState;
	_logger = createLogger("BisonActions");
	return _shutdownBisonActionsModule;
}

/* IMPORTED FUNCTIONS */

/* PRIVATE FUNCTIONS */

static void _logSyntacticAnalyzerAction(const char * functionName);

/**
 * Logs a syntactic-analyzer action in DEBUGGING level.
 */
static void _logSyntacticAnalyzerAction(const char * functionName) {
	logDebugging(_logger, "%s", functionName);
}

/* PUBLIC FUNCTIONS */

Program * ExpressionProgramSemanticAction(DataSeg * dataSeg, CodeSeg * codeSeg) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	Program * program = calloc(1, sizeof(Program));
	program->dataSeg = dataSeg;
	program->codeSeg = codeSeg;
	_compilerState->abstractSyntaxtTree = program;
	return program;
}

CodeSeg * CodeSegSemanticAction(CodeBlock * codeBlock) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	CodeSeg * codeSeg = calloc(1, sizeof(CodeSeg));
	codeSeg->codeBlock = codeBlock;
	return codeSeg;
}

DataSeg * DataSegSemanticAction(DataBlock * dataBlock) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	DataSeg * dataSeg = calloc(1, sizeof(DataSeg));
	dataSeg->dataBlock = dataBlock;
	return dataSeg;
}

CodeBlock * CodeBlockInit(CodeLine * line) {
    CodeBlock *block = (CodeBlock *)calloc(1, sizeof(CodeBlock));
    if (line != NULL) {
        block->lines = (CodeLine **)calloc(1, sizeof(CodeLine *));
        block->lines[0] = line;
        block->count = 1;
    } else {
        block->lines = NULL;
        block->count = 0;
    }
    return block;
}

CodeBlock * AppendCodeLine(CodeBlock * block, CodeLine * line) {
    if (line != NULL) {
        block->lines = (CodeLine **)realloc(block->lines, sizeof(CodeLine *) * (block->count + 1));
        block->lines[block->count] = line;
        block->count++;
    }
    return block;
}

DataBlock * DataBlockInit(DataLine * line) {
    DataBlock *block = (DataBlock *)calloc(1, sizeof(DataBlock));
    if (line != NULL) {
        block->lines = (DataLine **)calloc(1, sizeof(DataLine *));
        block->lines[0] = line;
        block->count = 1;
    } else {
        block->lines = NULL;
        block->count = 0;
    }
    return block;
}

DataBlock * AppendDataLine(DataBlock * block, DataLine * line) {
    if (line != NULL) {
        block->lines = (DataLine **)realloc(block->lines, sizeof(DataLine *) * (block->count + 1));
        block->lines[block->count] = line;
        block->count++;
    }
    return block;
}

CodeBlock * Z80CodeBlockInit(CodeLine * line) {
    return CodeBlockInit(line);
}

CodeBlock * Z80CodeBlockAppend(CodeBlock * block, CodeLine * line) {
    return AppendCodeLine(block, line);
}

static int countParams(char **params) {
    if (!params) return 0;
    int count = 0;
    while (params[count] != NULL) count++;
    return count;
}

CodeLine * Z80MakeCodeLineMacroDef(char * name, char ** params, CodeBlock * body) {
    CodeLine *line = (CodeLine *)calloc(1, sizeof(CodeLine));
    line->type = LINE_MACRO;
    
    MacroDef *macro = (MacroDef *)calloc(1, sizeof(MacroDef));
    macro->name = strdup(name);
    macro->params = params;
    macro->paramCount = countParams(params);
    macro->body = body;
    
    line->macro = macro;
    
    free(name);
    
    return line;
}

CodeLine * Z80MakeCodeLineInsn(Instruction * insn) {
    CodeLine *line = (CodeLine *)calloc(1, sizeof(CodeLine));
    line->type = LINE_INSTRUCTION;
    line->instruction = insn;
    return line;
}

CodeLine * Z80MakeCodeLineLabel(char * label) {
    CodeLine *line = (CodeLine *)calloc(1, sizeof(CodeLine));
    line->type = LINE_LABEL;
    line->label = strdup(label);
    
    free(label);
    
    return line;
}

DataLine * Z80MakeDataLineDb(Operand ** exprList) {
    DataLine *line = (DataLine *)calloc(1, sizeof(DataLine));
    line->dataType = DATA_DB;
    line->values = exprList;
    line->valueCount = 0;
    
    if (exprList) {
        while (exprList[line->valueCount] != NULL) {
            line->valueCount++;
        }
    }
    
    return line;
}

DataLine * Z80MakeDataLineDw(Operand ** exprList) {
    DataLine *line = (DataLine *)calloc(1, sizeof(DataLine));
    line->dataType = DATA_DW;
    line->values = exprList;
    line->valueCount = 0;
    
    // Count the values in the list (if implemented)
    if (exprList) {
        while (exprList[line->valueCount] != NULL) {
            line->valueCount++;
        }
    }
    
    return line;
}

DataLine * Z80MakeDataLineDs(Operand * expr) {
    DataLine *line = (DataLine *)calloc(1, sizeof(DataLine));
    line->dataType = DATA_DS;
    
    // DS takes a single expression for size
    if (expr) {
        line->values = (Operand **)calloc(1, sizeof(Operand *));
        line->values[0] = expr;
        line->valueCount = 1;
    } else {
        line->values = NULL;
        line->valueCount = 0;
    }
    
    return line;
    return line;
}

char ** Z80IdListInit(void) {
    char **list = (char **)calloc(1, sizeof(char *));
    list[0] = NULL;
    return list;
}

char ** Z80IdListInit1(char * id) {
    char **list = (char **)calloc(2, sizeof(char *));
    list[0] = strdup(id);
    list[1] = NULL;
    
    free(id);
    
    return list;
}

char ** Z80IdListAppend(char ** list, char * id) {
    int count = countParams(list);
    list = (char **)realloc(list, sizeof(char *) * (count + 2));
    list[count] = strdup(id);
    list[count + 1] = NULL;
    
    free(id);
    
    return list;
}

Instruction * Z80Insn0(InstructionType type) {
    Instruction *insn = (Instruction *)calloc(1, sizeof(Instruction));
    insn->type = type;
    insn->operands = NULL;
    insn->operandCount = 0;
    return insn;
}

Instruction * Z80Insn1(InstructionType type, Operand * op) {
    Instruction *insn = (Instruction *)calloc(1, sizeof(Instruction));
    insn->type = type;
    insn->operands = (Operand **)calloc(1, sizeof(Operand *));
    insn->operands[0] = op;
    insn->operandCount = 1;
    return insn;
}

Instruction * Z80Insn2(InstructionType type, Operand * op1, Operand * op2) {
    Instruction *insn = (Instruction *)calloc(1, sizeof(Instruction));
    insn->type = type;
    insn->operands = (Operand **)calloc(2, sizeof(Operand *));
    insn->operands[0] = op1;
    insn->operands[1] = op2;
    insn->operandCount = 2;
    return insn;
}

Operand * Z80OpReg8(RegisterName reg) {
    Operand *op = (Operand *)calloc(1, sizeof(Operand));
    op->type = OPERAND_REGISTER8;
    op->reg8 = reg;
    return op;
}

Operand * Z80OpReg16(RegisterName reg) {
    Operand *op = (Operand *)calloc(1, sizeof(Operand));
    op->type = OPERAND_REGISTER16;
    op->reg16 = reg;
    return op;
}

Operand * Z80OpMemHL(void) {
    Operand *op = (Operand *)calloc(1, sizeof(Operand));
    op->type = OPERAND_MEMORY_HL;
    return op;
}

Operand * Z80OpMemIdxDisp(RegisterName reg, Operand * disp) {
    Operand *op = (Operand *)calloc(1, sizeof(Operand));
    op->type = OPERAND_MEMORY_IXIY_DISP;
    op->mem_ixiy_disp.base = reg;
    
    Expr *expr = (Expr *)calloc(1, sizeof(Expr));
    if (disp->type == OPERAND_CONSTANT) {
        expr->value = disp->constantValue;
        expr->symbol = NULL;
    } else if (disp->type == OPERAND_SYMBOL) {
        expr->value = 0;
        expr->symbol = strdup(disp->symbol);
    } else {
        expr->value = 0;
        expr->symbol = NULL;
    }
    op->mem_ixiy_disp.disp = expr;
    
    destroyOperand(disp);
    
    return op;
}

Operand * Z80OpMemAbs(Operand * addr) {
    Operand *op = (Operand *)calloc(1, sizeof(Operand));
    op->type = OPERAND_MEMORY_ABS;
    
    Expr *expr = (Expr *)calloc(1, sizeof(Expr));
    if (addr->type == OPERAND_CONSTANT) {
        expr->value = addr->constantValue;
        expr->symbol = NULL;
    } else if (addr->type == OPERAND_SYMBOL) {
        expr->value = 0;
        expr->symbol = strdup(addr->symbol);
    } else {
        expr->value = 0;
        expr->symbol = NULL;
    }
    op->mem_abs = expr;
    
    destroyOperand(addr);
    
    return op;
}

Operand* Z80OpImm(int value) {
    Operand* op = malloc(sizeof(Operand));
    op->type = OPERAND_CONSTANT;
    Expr* expr = malloc(sizeof(Expr)); 
    expr->value = value;
    op->expr = expr;
    
    return op;
}

Operand * Z80OpSymbol(char * symbol) {
    Operand *op = (Operand *)calloc(1, sizeof(Operand));
    op->type = OPERAND_SYMBOL;
    op->symbol = strdup(symbol);
    
    free(symbol);
    
    return op;
}

Operand * Z80OpCond(ConditionType cond) {
    Operand *op = (Operand *)calloc(1, sizeof(Operand));
    op->type = OPERAND_CONDITION;
    op->condition = cond;
    return op;
}

Operand ** Z80ExprListInit(Operand * expr) {
    // Create array with 2 elements: the expr and NULL terminator
    Operand ** list = (Operand **)calloc(2, sizeof(Operand *));
    list[0] = expr;
    list[1] = NULL;  // NULL terminator
    return list;
}

Operand ** Z80ExprListAppend(Operand ** list, Operand * expr) {
    if (!list) {
        return Z80ExprListInit(expr);
    }
    
    // Count current elements
    int count = 0;
    while (list[count] != NULL) {
        count++;
    }
    
    // Reallocate to add new element + NULL terminator
    list = (Operand **)realloc(list, (count + 2) * sizeof(Operand *));
    list[count] = expr;
    list[count + 1] = NULL;  // NULL terminator
    
    return list;
}

Constant * IntegerConstantSemanticAction(const int value) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	Constant * constant = calloc(1, sizeof(Constant));
	constant->value = value;
	return constant;
}