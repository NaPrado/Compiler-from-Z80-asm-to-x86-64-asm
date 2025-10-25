%{

#include "../../support/type/TokenLabel.h"
#include "AbstractSyntaxTree.h"
#include "BisonActions.h"

/**
 * The error reporting function for Bison parser.
 *
 * @todo Add location to the grammar and "pushToken" API function.
 *
 * @see https://www.gnu.org/software/bison/manual/html_node/Error-Reporting-Function.html
 * @see https://www.gnu.org/software/bison/manual/html_node/Tracking-Locations.html
 */
void yyerror(const YYLTYPE * location, const char * message) {}

%}

// You touch this, and you die.
%define api.pure full
%define api.push-pull push
%define api.value.union.name SemanticValue
%define parse.error detailed
%locations

%union {
	/** Terminals. */
	signed int integer;
	char * string;
	TokenLabel token;

	/** Non-terminals. */
	/* AST node pointers used as semantic values */
	Constant * constant;
	CodeSeg * codeSeg;
	DataSeg * dataSeg;
	CodeBlock * codeBlock;
	DataBlock * dataBlock;
	CodeLine * codeLine;
	DataLine * dataLine;
	Program * program;
	Instruction * instruction;
	Operand * operand;
	Operand ** operandList;
	char ** idList;
}

/**
 * Destructors. This functions are executed after the parsing ends, so if the
 * AST must be used in the following phases of the compiler you shouldn't used
 * this approach for the AST root node ("program" non-terminal, in this
 * grammar), or it will drop the entire tree even if the parsing succeeds.
 *
 * @see https://www.gnu.org/software/bison/manual/html_node/Destructor-Decl.html
 */
/**%destructor { destroyConstant($$); } <constant> */


/** Terminals. */
%token <integer> INTEGER
%token <string> ID
%token <string> LABEL

%token <token>TOK_REG_A
%token <token>TOK_REG_B
%token <token>TOK_REG_C
%token <token>TOK_REG_D
%token <token>TOK_REG_E
%token <token>TOK_REG_H
%token <token>TOK_REG_L
%token <token>TOK_REG_BC
%token <token>TOK_REG_DC
%token <token>TOK_REG_HL
%token <token>TOK_REG_AF
%token <token>TOK_REG_SP
%token <token>TOK_REG_IY
%token <token>TOK_REG_IX
%token <token>TOK_REG_DE



%token <token>TOK_OP_ADD
%token <token>TOK_OP_SUB
%token <token>TOK_OP_INC
%token <token>TOK_OP_DEC
%token <token>TOK_OP_NEG

%token <token>TOK_OP_LD

%token <token>TOK_OP_AND
%token <token>TOK_OP_OR
%token <token>TOK_OP_XOR
%token <token>TOK_OP_CPL

%token <token>TOK_OP_CP

%token <token>TOK_OP_JP
%token <token>TOK_OP_JR
%token <token>TOK_OP_DJNZ

%token <token>TOK_OP_PUSH
%token <token>TOK_OP_POP

%token <token>TOK_OP_CALL
%token <token>TOK_OP_RET

%token <token>TOK_OP_NOP


%token <token>TOK_FLAG_NZ
%token <token>TOK_FLAG_Z
%token <token>TOK_FLAG_NC
%token <token>TOK_FLAG_C
%token <token>TOK_FLAG_PO
%token <token>TOK_FLAG_PE
%token <token>TOK_FLAG_P
%token <token>TOK_FLAG_M


%token <token> TOK_DATA_EQU
%token <token> TOK_DATA_DB
%token <token> TOK_DATA_DW
%token <token> TOK_DATA_DEFM
%token <token> TOK_DATA_DS


%token <token>COMA
%token <token>PLUS
%token <token>OPEN_PARENTHESIS
%token <token>CLOSE_PARENTHESIS

%token <token>OPEN_COMMENT
%token <token>CLOSE_COMMENT

%token <token> IGNORED
%token <token> UNKNOWN

%token <token> MACRO
%token <token> ENDM

%token <token> CSEG
%token <token> DSEG

%token <token> NEW_LINE

/** Non-terminals. */

%type <operand>   cond
%type <dataLine>  dataLine
%type <codeLine>  codeLine
%type <dataBlock> dataBlock
%type <codeBlock> codeBlock
%type <codeSeg>    codeSeg
%type <dataSeg>    dataSeg
%type <program>   program

/* estos son necesarios para las reglas de abajo */
%type <operand>   operand reg8 reg16 mem_hl mem_ixiy_disp mem_abs expr
%type <codeLine>  macroDef
%type <instruction> instruction
%type <codeBlock> macroBody
%type <idList>    macroParamListOpt macroParamList
%type <operandList>    exprList


/**
 * Precedence and associativity.
 *
 * @see https://en.cppreference.com/w/cpp/language/operator_precedence.html
 * @see https://www.gnu.org/software/bison/manual/html_node/Precedence.html
 */
%left ADD SUB
%left MUL DIV

%%

// IMPORTANT: To use λ in the following grammar, use the %empty symbol.

program
	: dataSeg codeSeg                { $$ = ExpressionProgramSemanticAction($1, $2); }
	;

dataSeg
	: DSEG dataBlock                 { $$ = DataSegSemanticAction($2); }
	| %empty			             { $$ = DataSegSemanticAction(NULL); }
	;

codeSeg
	: CSEG codeBlock                 { $$ = CodeSegSemanticAction($2); }
	| codeBlock                      { $$ = CodeSegSemanticAction($1); }
	| %empty                         { $$ = CodeSegSemanticAction(NULL); }
	;

codeBlock
    : codeBlock codeLine      { $$ = AppendCodeLine($1, $2); }
    | codeLine                { $$ = CodeBlockInit($1); }
    ;

dataBlock
    : dataBlock dataLine      { $$ = AppendDataLine($1, $2); }
    | dataLine                { $$ = DataBlockInit($1); }
    ;

codeLine
	: macroDef                          { $$ = $1; }
	| instruction NEW_LINE              { $$ = Z80MakeCodeLineInsn($1); }
	| NEW_LINE                          { $$ = NULL; }
	;

dataLine
	: TOK_DATA_DB exprList NEW_LINE        { $$ = Z80MakeDataLineDb($2); }
	| TOK_DATA_DW exprList NEW_LINE        { $$ = Z80MakeDataLineDw($2); }
	| TOK_DATA_DS expr NEW_LINE            { $$ = Z80MakeDataLineDs($2); }
	;

macroDef
	: ID MACRO macroParamListOpt NEW_LINE macroBody ENDM NEW_LINE	{ $$ = Z80MakeCodeLineMacroDef($1, $3, $5); }
	;

macroParamListOpt
	: %empty                     { $$ = Z80IdListInit(); }
	| macroParamList                  { $$ = $1; }
	;

macroParamList
	: ID                      { $$ = Z80IdListInit1($1); }
	| macroParamList COMA ID { $$ = Z80IdListAppend($1, $3); }
	;

macroBody
	: %empty                         { $$ = Z80CodeBlockInit(NULL); }
	| macroBody instruction NEW_LINE      { $$ = Z80CodeBlockAppend($1, Z80MakeCodeLineInsn($2)); }
	| macroBody NEW_LINE                  { $$ = $1; }
	;


instruction
	: TOK_OP_LD  operand COMA operand                          { $$ = Z80Insn2(INST_LD,  $2, $4); }
	| TOK_OP_ADD operand                                        { $$ = Z80Insn1(INST_ADD, $2); }
	| TOK_OP_SUB operand                                        { $$ = Z80Insn1(INST_SUB, $2); }
	| TOK_OP_INC operand                                        { $$ = Z80Insn1(INST_INC, $2); }
	| TOK_OP_DEC operand                                        { $$ = Z80Insn1(INST_DEC, $2); }
	| TOK_OP_AND operand                                        { $$ = Z80Insn1(INST_AND, $2); }
	| TOK_OP_OR  operand                                        { $$ = Z80Insn1(INST_OR,  $2); }
	| TOK_OP_XOR operand                                        { $$ = Z80Insn1(INST_XOR, $2); }
	| TOK_OP_CP  operand                                        { $$ = Z80Insn1(INST_CP,  $2); }
	| TOK_OP_JP  operand                                        { $$ = Z80Insn1(INST_JP,  $2); }
	| TOK_OP_JP	 cond COMA operand								{ $$ = Z80Insn2(INST_JP, $2, $4); }
	| TOK_OP_JR  operand                                        { $$ = Z80Insn1(INST_JR,  $2); }
	| TOK_OP_JR  cond COMA operand                             { $$ = Z80Insn2(INST_JR,  $2, $4); }
	| TOK_OP_DJNZ  operand                                      { $$ = Z80Insn1(INST_DJNZ,  $2); }
	| TOK_OP_CALL operand                                       { $$ = Z80Insn1(INST_CALL,$2); }
	| TOK_OP_RET                                                { $$ = Z80Insn0(INST_RET); }
	| TOK_OP_PUSH operand                                       { $$ = Z80Insn1(INST_PUSH,$2); }
	| TOK_OP_POP  operand                                       { $$ = Z80Insn1(INST_POP, $2); }
	| TOK_OP_NOP                                                { $$ = Z80Insn0(INST_NOP); }
	;

cond
  : TOK_FLAG_NZ  												{ $$ = Z80OpCond(COND_NZ); }
  | TOK_FLAG_Z   												{ $$ = Z80OpCond(COND_Z);  }
  | TOK_FLAG_NC  												{ $$ = Z80OpCond(COND_NC); }
  | TOK_FLAG_C   												{ $$ = Z80OpCond(COND_C);  }
  | TOK_FLAG_PO  												{ $$ = Z80OpCond(COND_PO); }
  | TOK_FLAG_PE  												{ $$ = Z80OpCond(COND_PE); }
  | TOK_FLAG_P   												{ $$ = Z80OpCond(COND_P);  }
  | TOK_FLAG_M   												{ $$ = Z80OpCond(COND_M);  }
  ;

/* ===== operandos (modos principales del Z80) ===== */
operand
	: reg8                                               { $$ = $1; }
	| reg16                                              { $$ = $1; }
	| mem_hl                                             { $$ = $1; }
	| mem_ixiy_disp                                      { $$ = $1; }
	| mem_abs                                            { $$ = $1; }
	| expr                                               { $$ = $1; }
	;

reg8
	: TOK_REG_A                                          { $$ = Z80OpReg8(REG_A); }
	| TOK_REG_B                                          { $$ = Z80OpReg8(REG_B); }
	| TOK_REG_C                                          { $$ = Z80OpReg8(REG_C); }
	| TOK_REG_D                                          { $$ = Z80OpReg8(REG_D); }
	| TOK_REG_E                                          { $$ = Z80OpReg8(REG_E); }
	| TOK_REG_H                                          { $$ = Z80OpReg8(REG_H); }
	| TOK_REG_L                                          { $$ = Z80OpReg8(REG_L); }
	;

reg16
	: TOK_REG_AF                                         { $$ = Z80OpReg16(REG_AF); }
	| TOK_REG_BC                                         { $$ = Z80OpReg16(REG_BC); }
	| TOK_REG_DE                                         { $$ = Z80OpReg16(REG_DE); }
	| TOK_REG_HL                                         { $$ = Z80OpReg16(REG_HL); }
	| TOK_REG_SP                                         { $$ = Z80OpReg16(REG_SP); }
	| TOK_REG_IX                                         { $$ = Z80OpReg16(REG_IX); }
	| TOK_REG_IY                                         { $$ = Z80OpReg16(REG_IY); }
	;

/* (HL) */
mem_hl
	: OPEN_PARENTHESIS TOK_REG_HL CLOSE_PARENTHESIS       { $$ = Z80OpMemHL(); }
	;

/* (IX + d) | (IY + d) — d=expr */
mem_ixiy_disp
	: OPEN_PARENTHESIS TOK_REG_IX PLUS expr CLOSE_PARENTHESIS
	                                                    { $$ = Z80OpMemIdxDisp(REG_IX, $4); }
	| OPEN_PARENTHESIS TOK_REG_IY PLUS expr CLOSE_PARENTHESIS
	                                                    { $$ = Z80OpMemIdxDisp(REG_IY, $4); }
	;

/* (nn) absoluto */
mem_abs
	: OPEN_PARENTHESIS expr CLOSE_PARENTHESIS            { $$ = Z80OpMemAbs($2); }
	;

/* inmediatos o símbolos */
expr
	: INTEGER                                           { $$ = Z80OpImm($1); }
	| ID                                         		{ $$ = Z80OpSymbol($1); }
	;

/* lista de expresiones para declaraciones de datos */
exprList
	: expr                                              { $$ = NULL; /* TODO: implementar lista */ }
	| exprList COMA expr                                { $$ = NULL; /* TODO: implementar lista */ }
	;

%%
