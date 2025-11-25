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
static void generateDataSegment(DataSeg* dataSeg, SymbolTable* table);
static void generateCodeSegment(CodeSeg* codeSeg, SymbolTable* table);
static void generateCodeLine(CodeLine* line, SymbolTable* table);
static void generateDataLine(DataLine* line, SymbolTable* table);
static void generateMacro(MacroDef* macro, SymbolTable* table);
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
	printf("GLOBAL _start\n");
}

static void generateEpilogue() {
	emitComment("Exit program");
	emitCode("mov rax, 1");
	emitCode("xor rdi, rdi");
	emitCode("syscall");
}

static void generateProgram(Program* program, SymbolTable* table) {
	
	if (program->dataSeg != NULL) {
		generateDataSegment(program->dataSeg, table);
	}
	
	if (program->codeSeg != NULL) {
		generateCodeSegment(program->codeSeg, table);
	}
}

static void generateDataSegment(DataSeg* dataSeg, SymbolTable* table) {
	if (dataSeg == NULL || dataSeg->dataBlock == NULL) {
		return;
	}
	
	printf("section .data\n");
	DataBlock* block = dataSeg->dataBlock;
	for (int i = 0; i < block->count; i++) {
		generateDataLine(block->lines[i], table);
	} 
	printf("\n");
}

static void generateCodeSegment(CodeSeg* codeSeg, SymbolTable* table) {
	if (codeSeg == NULL || codeSeg->codeBlock == NULL) {
		return;
	}
	
	printf("section .text\n");
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
			generateMacro(line->macro, table);
			break;
		case LINE_EMPTY:
			break;
	}
}

// todo: se puede separar en dos para data y bss
static void generateDataLine(DataLine* line, SymbolTable* table) {
	if (line == NULL) {
		return;
	}
	
	switch (line->dataType) {
		case DATA_DB:
			printf("  db ");
			for (int i = 0; i < line->valueCount; i++) {
				if (i > 0) printf(", ");
				generateOperand(line->values[i], table);
			}
			break;
		
		case DATA_DW:
			printf("  dw ");
			for (int i = 0; i < line->valueCount; i++) {
				if (i > 0) printf(", ");
				generateOperand(line->values[i], table);
			}
			break;
		
		// todo: revisar si se puede poner resb en .data
		case DATA_DS:
			printf("  resb ");
			if (line->valueCount > 0 && line->values[0]->type == OPERAND_CONSTANT) {
				printf("%d", line->values[0]->constantValue);
			} else {
				printf("1");
			}
			break;
	}
	printf("\n");
}

static void generateMacro(MacroDef* macro, SymbolTable* table) {
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
					generateInstructionWithParams(line->instruction, table, macro->params, macro->paramCount);
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

void executeGenerator(CompilerState* compilerState, SymbolTable* symbolTable) {
	logDebugging(_logger, "Generating x86-64 assembly...");
	generatePrologue();
	generateProgram(compilerState->abstractSyntaxtTree, symbolTable);
	logDebugging(_logger, "Code generation complete.");
}
