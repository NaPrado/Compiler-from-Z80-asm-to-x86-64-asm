#include "Generator.h"
#include "InstructionMapper.h"
#include "RegisterMapper.h"
#include <stdarg.h>
#include <stdio.h>

/* MODULE INTERNAL STATE */

static Logger* _logger = NULL;

/** Shutdown module's internal state. */
void _shutdownGeneratorModule() {
	if (_logger != NULL) {
		logDebugging(_logger, "Destroying module: Generator...");
		destroyLogger(_logger);
		_logger = NULL;
	}
}

ModuleDestructor initializeGeneratorModule() {
	_logger = createLogger("Generator");
	return _shutdownGeneratorModule;
}

/** PRIVATE FUNCTIONS */

static void emitCode(const char* format, ...);
static void emitLabel(const char* label);
static void emitComment(const char* comment);
static void generatePrologue();
static void generateEpilogue();
static void generateProgram(Program* program);
static void generateDataSegment(DataSeg* dataSeg);
static void generateCodeSegment(CodeSeg* codeSeg);
static void generateCodeLine(CodeLine* line);
static void generateDataLine(DataLine* line);
static void generateMacro(MacroDef* macro);
static void emitCode(const char* format, ...) {
	va_list args;
	va_start(args, format);
	printf("  ");
	vprintf(format, args);
	printf("\n");
	fflush(stdout);
	va_end(args);
}

static void emitLabel(const char* label) {
	printf("%s:\n", label);
	fflush(stdout);
}

static void emitComment(const char* comment) {
	printf("  ; %s\n", comment);
	fflush(stdout);
}

static void generatePrologue() {
	emitComment("Generated from Z80 assembly");
	printf("GLOBAL _start\n\n");
}

static void generateEpilogue() {
	emitComment("Exit program");
	emitCode("mov rax, 1");
	emitCode("xor rdi, rdi");
	emitCode("syscall");
}

static void generateProgram(Program* program) {
	
	if (program->dataSeg != NULL) {
		generateDataSegment(program->dataSeg);
	}
	
	if (program->codeSeg != NULL) {
		generateCodeSegment(program->codeSeg);
	}
}

static void generateDataSegment(DataSeg* dataSeg) {
	if (dataSeg == NULL || dataSeg->dataBlock == NULL) {
		return;
	}
	
	printf("section .data\n");
	DataBlock* block = dataSeg->dataBlock;
	for (int i = 0; i < block->count; i++) {
		generateDataLine(block->lines[i]);
	} 
	printf("\n");
}

static void generateCodeSegment(CodeSeg* codeSeg) {
	if (codeSeg == NULL || codeSeg->codeBlock == NULL) {
		return;
	}
	
	printf("section .text\n");
	emitLabel("_start");
	
	CodeBlock* block = codeSeg->codeBlock;
	for (int i = 0; i < block->count; i++) {
		generateCodeLine(block->lines[i]);
	}
	
	printf("\n");
	generateEpilogue();
}

static void generateCodeLine(CodeLine* line) {
	if (line == NULL) {
		return;
	}
	
	switch (line->type) {
		case LINE_LABEL:
			emitLabel(line->label);
			break;
		case LINE_INSTRUCTION:
			generateInstruction(line->instruction);
			break;
		case LINE_MACRO:
			generateMacro(line->macro);
			break;
		case LINE_EMPTY:
			break;
	}
}

static void generateDataLine(DataLine* line) {
	if (line == NULL) {
		return;
	}
	
	switch (line->dataType) {
		case DATA_DB:
			printf("%s", line->label);
			printf("  db ");
			for (int i = 0; i < line->valueCount; i++) {
				if (i > 0) printf(", ");
				generateOperand(line->values[i]);
			}
			break;
		
		case DATA_DW:
			printf("%s", line->label);
			printf("  dw ");
			for (int i = 0; i < line->valueCount; i++) {
				if (i > 0) printf(", ");
				generateOperand(line->values[i]);
			}
			break;
		
		case DATA_DS:
			printf("%s", line->label);
			printf("  db ");
			if (line->valueCount > 0 && line->values[0]->type == OPERAND_CONSTANT) {
				printf("%d", line->values[0]->expr->value);
			} else {
				printf("1");
			}
			printf(" dup(0)");
			break;
	}
	printf("\n");
}

static void generateMacro(MacroDef* macro) {
	if (macro == NULL) {
		return;
	}
	
	printf("%%macro %s %d\n", macro->name, macro->paramCount);
	
	if (macro->body != NULL && macro->body->lines != NULL) {
		for (int i = 0; i < macro->body->count; i++) {
			CodeLine* line = macro->body->lines[i];
			if (line == NULL) continue;
			
			switch (line->type) {
				case LINE_LABEL:
					emitLabel(line->label);
					break;
				case LINE_INSTRUCTION:
					generateInstructionWithParams(line->instruction, macro->params, macro->paramCount);
					break;
				case LINE_MACRO:
					logError(_logger, "Nested macros not supported");
					break;
				case LINE_EMPTY:
					break;
			}
		}
	}
	
	printf("%%endmacro\n\n");
}

/** PUBLIC FUNCTIONS */

void executeGenerator(CompilerState* compilerState) {
	logDebugging(_logger, "Generating x86-64 assembly...");
	generatePrologue();
	generateProgram(compilerState->abstractSyntaxtTree);
	logDebugging(_logger, "Code generation complete.");
}
