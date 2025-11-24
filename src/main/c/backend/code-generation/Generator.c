#include "Generator.h"
#include "InstructionMapper.h"
#include "RegisterMapper.h"
#include <stdarg.h>
#include <stdio.h>


// todo: revisar todo

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
static void generateProgram(Program* program, SymbolTable* table);
static void generateDataSegment(DataSeg* dataSeg);
static void generateCodeSegment(CodeSeg* codeSeg, SymbolTable* table);
static void generateCodeLine(CodeLine* line, SymbolTable* table);
static void generateDataLine(DataLine* line);

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
	printf("; Generated from Z80 assembly\n");
}

static void generateEpilogue() {
	emitComment("Exit program");
	emitCode("mov rax, 60");
	emitCode("xor rdi, rdi");
	emitCode("syscall");
}

static void generateProgram(Program* program, SymbolTable* table) {
	generatePrologue();
	
	if (program->dataSeg != NULL) {
		generateDataSegment(program->dataSeg);
	}
	
	if (program->codeSeg != NULL) {
		generateCodeSegment(program->codeSeg, table);
	}
}

static void generateDataSegment(DataSeg* dataSeg) {
	if (dataSeg == NULL || dataSeg->dataBlock == NULL) {
		return;
	}
	
	printf(".section .data\n");
	DataBlock* block = dataSeg->dataBlock;
	for (int i = 0; i < block->count; i++) {
		generateDataLine(block->lines[i]);
	}
	printf("\n");
}

static void generateCodeSegment(CodeSeg* codeSeg, SymbolTable* table) {
	if (codeSeg == NULL || codeSeg->codeBlock == NULL) {
		return;
	}
	
	printf(".section .text\n");
	printf(".globl _start\n");
	emitLabel("_start");
	
	CodeBlock* block = codeSeg->codeBlock;
	for (int i = 0; i < block->count; i++) {
		generateCodeLine(block->lines[i], table);
	}
	
	printf("\n");
	generateEpilogue();
}

static void generateCodeLine(CodeLine* line, SymbolTable* table) {
	if (line == NULL) {
		return;
	}
	
	switch (line->type) {
		case LINE_LABEL:
			emitLabel(line->label);
			break;
		case LINE_INSTRUCTION:
			generateInstruction(line->instruction, table);
			break;
		case LINE_MACRO:
			logError(_logger, "Macros should be expanded before code generation");
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
			printf("  .byte ");
			break;
		case DATA_DW:
			printf("  .word ");
			break;
		case DATA_DS:
			printf("  .space ");
			break;
	}
	
	for (int i = 0; i < line->valueCount; i++) {
		if (i > 0) {
			printf(", ");
		}
		if (line->values[i]->type == OPERAND_CONSTANT) {
			printf("0x%X", line->values[i]->constantValue);
		}
	}
	printf("\n");
}

/** PUBLIC FUNCTIONS */

void executeGenerator(CompilerState* compilerState, SymbolTable* symbolTable) {
	logDebugging(_logger, "Generating x86-64 assembly...");
	generateProgram(compilerState->abstractSyntaxtTree, symbolTable);
	logDebugging(_logger, "Code generation complete.");
}
