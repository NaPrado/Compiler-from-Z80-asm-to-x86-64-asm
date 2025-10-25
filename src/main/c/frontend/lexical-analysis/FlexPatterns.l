%top{

#include "../../support/type/CompilationStatus.h"
#include "../../support/type/FlexContext.h"
#include "../../support/type/TokenLabel.h"
#include "../syntactic-analysis/AbstractSyntaxTree.h"
#include "../syntactic-analysis/BisonParser.h"

}

%{

/**
 * @see https://westes.github.io/flex/manual/Definitions-Section.html#Definitions-Section
 */

extern CompilationStatus ArithmeticOperatorLexemeAction(TokenLabel label);
extern CompilationStatus EnterImportExpressionLexemeAction(FlexContext context);
extern CompilationStatus CommentLexemeAction();
extern CompilationStatus EnterInlineCommentLexemeAction(FlexContext context);
extern CompilationStatus EOFLexemeAction();
extern CompilationStatus IgnoredLexemeAction();
extern CompilationStatus IntegerLexemeAction();
extern CompilationStatus LeaveImportExpressionLexemeAction();
extern CompilationStatus LeaveInlineCommentLexemeAction();
extern CompilationStatus ParenthesisLexemeAction(TokenLabel label);
extern CompilationStatus SubexpressionLexemeAction();
extern CompilationStatus UnknownLexemeAction();
extern CompilationStatus Z80RegisterLexemeAction(TokenLabel label);
extern CompilationStatus Z80InstructionLexemeAction(TokenLabel label);
extern CompilationStatus ComaLexemeAction(TokenLabel label);
extern CompilationStatus PlusLexemeAction(TokenLabel label);
extern CompilationStatus LabelLexemeAction();
extern CompilationStatus IdentifierLexemeAction();
extern CompilationStatus Z80MacroLexemeAction(TokenLabel label);
extern CompilationStatus DataDeclarationLexemeAction(TokenLabel label);
extern CompilationStatus NewLineLexemeAction();
extern CompilationStatus EnterSegmentLexemeAction(int segmentType);


%}

/**
 * Flex contexts (a.k.a. start conditions).
 *
 * @see https://westes.github.io/flex/manual/Start-Conditions.html
 */

/**
 * Reusable patterns.
 *
 * @see https://westes.github.io/flex/manual/Matching.html
 * @see https://westes.github.io/flex/manual/Patterns.html
 */
whitespace			[ \t\r]+
digit				[0-9]
hex					[0-9A-Fa-f]
letter				[A-Za-z_]
identifier			{letter}({letter}|{digit})*


%%

%{

/**
 * @see https://westes.github.io/flex/manual/Rules-Section.html#Rules-Section
 */

%}

";".*                                  { return CommentLexemeAction(); }

(?i:Cseg)                              { return EnterSegmentLexemeAction(CSEG); }
(?i:Dseg)                              { return EnterSegmentLexemeAction(DSEG); }


(?i:A)                                 { return Z80RegisterLexemeAction(TOK_REG_A); }
(?i:B)                                 { return Z80RegisterLexemeAction(TOK_REG_B); }
(?i:C)                                 { return Z80RegisterLexemeAction(TOK_REG_C); }
(?i:D)                                 { return Z80RegisterLexemeAction(TOK_REG_D); }
(?i:E)                                 { return Z80RegisterLexemeAction(TOK_REG_E); }
(?i:H)                                 { return Z80RegisterLexemeAction(TOK_REG_H); }
(?i:L)                                 { return Z80RegisterLexemeAction(TOK_REG_L); }
(?i:BC)                                { return Z80RegisterLexemeAction(TOK_REG_BC); }
(?i:DC)                                { return Z80RegisterLexemeAction(TOK_REG_DC); }
(?i:HL)                                { return Z80RegisterLexemeAction(TOK_REG_HL); }
(?i:AF)                                { return Z80RegisterLexemeAction(TOK_REG_AF); }
(?i:SP)                                { return Z80RegisterLexemeAction(TOK_REG_SP); }
(?i:IY)                                { return Z80RegisterLexemeAction(TOK_REG_IY); }
(?i:IX)                                { return Z80RegisterLexemeAction(TOK_REG_IX); }


(?i:ADD)                               { return Z80InstructionLexemeAction(TOK_OP_ADD); }
(?i:SUB)                               { return Z80InstructionLexemeAction(TOK_OP_SUB); }
(?i:INC)                               { return Z80InstructionLexemeAction(TOK_OP_INC); }
(?i:DEC)                               { return Z80InstructionLexemeAction(TOK_OP_DEC); }
(?i:NEG)                               { return Z80InstructionLexemeAction(TOK_OP_NEG); }

(?i:LD)                                { return Z80InstructionLexemeAction(TOK_OP_LD); }

(?i:AND)                               { return Z80InstructionLexemeAction(TOK_OP_AND); }
(?i:OR)                                { return Z80InstructionLexemeAction(TOK_OP_OR); }
(?i:XOR)                               { return Z80InstructionLexemeAction(TOK_OP_XOR); }
(?i:CPL)                               { return Z80InstructionLexemeAction(TOK_OP_CPL); }

(?i:CP)                                { return Z80InstructionLexemeAction(TOK_OP_CP); }

(?i:JP)                                { return Z80InstructionLexemeAction(TOK_OP_JP); }
(?i:JR)                                { return Z80InstructionLexemeAction(TOK_OP_JR); }
(?i:DJNZ)                              { return Z80InstructionLexemeAction(TOK_OP_DJNZ); }

(?i:PUSH)                              { return Z80InstructionLexemeAction(TOK_OP_PUSH); }
(?i:POP)                               { return Z80InstructionLexemeAction(TOK_OP_POP); }

(?i:CALL)                              { return Z80InstructionLexemeAction(TOK_OP_CALL); }
(?i:RET)                               { return Z80InstructionLexemeAction(TOK_OP_RET); }

(?i:NOP)                               { return Z80InstructionLexemeAction(TOK_OP_NOP); }


(?i:NZ)                               { return Z80InstructionLexemeAction(TOK_FLAG_NZ); }
(?i:Z)                               { return Z80InstructionLexemeAction(TOK_FLAG_Z); }
(?i:NC)                               { return Z80InstructionLexemeAction(TOK_FLAG_NC); }
(?i:C)                               { return Z80InstructionLexemeAction(TOK_FLAG_C); }
(?i:PO)                               { return Z80InstructionLexemeAction(TOK_FLAG_PO); }
(?i:PE)                               { return Z80InstructionLexemeAction(TOK_FLAG_PE); }
(?i:P)                               { return Z80InstructionLexemeAction(TOK_FLAG_P); }
(?i:M)                               { return Z80InstructionLexemeAction(TOK_FLAG_M); }

(?i:MACRO)                          { return Z80MacroLexemeAction(MACRO); }
(?i:ENDM)                           { return Z80MacroLexemeAction(ENDM); }

0[xX]{hex}+							{ return IntegerLexemeAction(); }
[[:digit:]]+						{ return IntegerLexemeAction(); }

","                                 { return ComaLexemeAction(COMA); }
"+"                                 { return PlusLexemeAction(PLUS); }
"("									{ return ParenthesisLexemeAction(OPEN_PARENTHESIS); }
")"									{ return ParenthesisLexemeAction(CLOSE_PARENTHESIS); }

(?i:equ)                            { return DataDeclarationLexemeAction(TOK_DATA_EQU); }
(?i:db)                             { return DataDeclarationLexemeAction(TOK_DATA_DB); }
(?i:dw)                             { return DataDeclarationLexemeAction(TOK_DATA_DW); }
(?i:ds)                             { return DataDeclarationLexemeAction(TOK_DATA_DS); }

{identifier}":"                     { return LabelLexemeAction(); }
{identifier}                        { return IdentifierLexemeAction(); }

\n                                  { return NewLineLexemeAction(); }

{whitespace}						{ return IgnoredLexemeAction(); }
.									{ return UnknownLexemeAction(); }

<<EOF>>								{ return EOFLexemeAction(); }

%%

/**
 * @see https://westes.github.io/flex/manual/User-Code-Section.html#User-Code-Section
 */

#include "FlexActions.h"
#include "FlexExport.h"
