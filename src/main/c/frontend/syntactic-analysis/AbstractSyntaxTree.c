#include "AbstractSyntaxTree.h"

/* MODULE INTERNAL STATE */

static Logger * _logger = NULL;

/** Shutdown module's internal state. */
void _shutdownAbstractSyntaxTreeModule() {
	if (_logger != NULL) {
		logDebugging(_logger, "Destroying module: AbstractSyntaxTree...");
		destroyLogger(_logger);
		_logger = NULL;
	}
}

ModuleDestructor initializeAbstractSyntaxTreeModule() {
	_logger = createLogger("AbstractSyntaxTree");
	return _shutdownAbstractSyntaxTreeModule;
}

/* PUBLIC FUNCTIONS */

void destroyConstant(Constant * constant) {
	logDebugging(_logger, "Executing destructor: %s", __FUNCTION__);
	if (constant != NULL) {
		free(constant);
	}
}

void destroyExpression(Expr *expression) {
	logDebugging(_logger, "Executing destructor: %s", __FUNCTION__);
    if (!expression) return;
    if (expression->symbol) {
        free(expression->symbol);
    }
    free(expression);
}

void destroyOperand(Operand *operand) {
	logDebugging(_logger, "Executing destructor: %s", __FUNCTION__);
    if (!operand) return;

    switch (operand->type) {
        case OPERAND_SYMBOL:
            free(operand->symbol);
            break;
        case OPERAND_CONSTANT:
            destroyExpression(operand->expr);
            break;
        case OPERAND_MEMORY_IXIY_DISP:
            destroyExpression(operand->mem_ixiy_disp.disp);
            break;
        case OPERAND_MEMORY_ABS:
            destroyExpression(operand->mem_abs);
            break;
        default:
            logDebugging(_logger, "No additional resources to free for operand type %d", operand->type);
            break;
    }

    free(operand);
}

void destroyInstruction(Instruction *instruction) {
	logDebugging(_logger, "Executing destructor: %s", __FUNCTION__);
    if (!instruction) return;

    if (instruction->operands) {
        for (int i = 0; i < instruction->operandCount; i++) {
            destroyOperand(instruction->operands[i]);
        }
        free(instruction->operands);
    }

    free(instruction);
}

void destroyMacroDef(MacroDef *macro) {
	logDebugging(_logger, "Executing destructor: %s", __FUNCTION__);
    if (!macro) return;

    free(macro->name);

    if (macro->params) {
        for (int i = 0; i < macro->paramCount; i++) {
            free(macro->params[i]);
        }
        free(macro->params);
    }

    destroyCodeBlock(macro->body);

    free(macro);
}

void destroyCodeLine(CodeLine *line) {
	logDebugging(_logger, "Executing destructor: %s", __FUNCTION__);
    if (!line) return;

    switch (line->type) {
        case LINE_INSTRUCTION:
            destroyInstruction(line->instruction);
            break;
        case LINE_MACRO:
            destroyMacroDef(line->macro);
            break;
        case LINE_LABEL:
            free(line->label);
            break;
        default:
            logDebugging(_logger, "No additional resources to free for code line type %d", line->type);
            break;
    }

    free(line);
}

void destroyDataLine(DataLine *line) {
	logDebugging(_logger, "Executing destructor: %s", __FUNCTION__);
    if (!line) return;

    if (line->label) {
        free(line->label);
    }

    if (line->values) {
        for (int i = 0; i < line->valueCount; i++) {
            destroyOperand(line->values[i]);
        }
        free(line->values);
    }

    free(line);
}

void destroyCodeBlock(CodeBlock *block) {
	logDebugging(_logger, "Executing destructor: %s", __FUNCTION__);
    if (!block) return;

    if (block->lines) {
        for (int i = 0; i < block->count; i++) {
            destroyCodeLine(block->lines[i]);
        }
        free(block->lines);
    }

    free(block);
}

void destroyDataBlock(DataBlock *block) {
	logDebugging(_logger, "Executing destructor: %s", __FUNCTION__);
    if (!block) return;

    if (block->lines) {
        for (int i = 0; i < block->count; i++) {
            destroyDataLine(block->lines[i]);
        }
        free(block->lines);
    }

    free(block);
}

void destroyCodeSeg(CodeSeg *codeSeg) {
	logDebugging(_logger, "Executing destructor: %s", __FUNCTION__);
    if (!codeSeg) return;

    destroyCodeBlock(codeSeg->codeBlock);

    free(codeSeg);
}

void destroyDataSeg(DataSeg *dataSeg) {
	logDebugging(_logger, "Executing destructor: %s", __FUNCTION__);
    if (!dataSeg) return;

    destroyDataBlock(dataSeg->dataBlock);

    free(dataSeg);
}

void destroyProgram(Program *program) {
	logDebugging(_logger, "Executing destructor: %s", __FUNCTION__);
    if (!program) return;

    destroyDataSeg(program->dataSeg);
    destroyCodeSeg(program->codeSeg);

    free(program);
}
