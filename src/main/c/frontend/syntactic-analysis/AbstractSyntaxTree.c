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
        default:
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

    destroyBlock(macro->body);

    free(macro);
}

void destroyLine(Line *line) {
	logDebugging(_logger, "Executing destructor: %s", __FUNCTION__);
    if (!line) return;

    switch (line->type) {
        case LINE_INSTRUCTION:
            destroyInstruction(line->instruction);
            break;
        case LINE_MACRO:
            destroyMacroDef(line->macro);
            break;
    }

    free(line);
}

void destroyBlock(Block *block) {
	logDebugging(_logger, "Executing destructor: %s", __FUNCTION__);
    if (!block) return;

    if (block->lines) {
        for (int i = 0; i < block->count; i++) {
            destroyLine(block->lines[i]);
        }
        free(block->lines);
    }

    free(block);
}

void destroyProgram(Program *program) {
	logDebugging(_logger, "Executing destructor: %s", __FUNCTION__);
    if (!program) return;

    destroyBlock(program->data);
    destroyBlock(program->code);

    free(program);
}
