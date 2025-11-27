#include "InstructionMapper.h"
#include "RegisterMapper.h"
#include "../../support/logging/Logger.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static Logger* _logger = NULL;

static int findParamIndex(const char* symbol, char** params, int paramCount) {
    if (symbol == NULL || params == NULL) {
        return -1;
    }
    for (int i = 0; i < paramCount; i++) {
        if (params[i] != NULL && strcmp(symbol, params[i]) == 0) {
            return i;
        }
    }
    return -1;
}

void generateInstruction(Instruction* insn, SymbolTable* table) {
    if (_logger == NULL) {
        _logger = createLogger("InstructionMapper");
    }
    
    if (insn == NULL) {
        logError(_logger, "Null instruction");
        return;
    }
    
    logDebugging(_logger, "Generating instruction: %s", instructionToString(insn->type));

    switch (insn->type) {
        case INST_LD:
            // LD dst, src -> mov dst, src
            if (insn->operandCount >= 2) {
                // Si el destino es un registro de 16 bits, limpiar el registro de 64 bits
                if (insn->operands[0]->type == OPERAND_REGISTER16) {
                    printf("  xor ");
                    printf("%s", getX86Register64(insn->operands[0]->reg16));
                    printf(", ");
                    printf("%s", getX86Register64(insn->operands[0]->reg16));
                    printf("\n");
                }
                
                printf("  mov ");
                generateOperand(insn->operands[0], table);
                printf(", ");
                generateOperand(insn->operands[1], table);
                printf("\n");
            }
            break;
            
        case INST_ADD:
            // ADD operand -> add al, operand (A = A + operand en Z80)
            if (insn->operandCount == 1) {
                printf("  add al, ");
                generateOperand(insn->operands[0], table);
                printf("\n");
            } else if (insn->operandCount == 2) {
                // ADD dst, src
                printf("  add ");
                generateOperand(insn->operands[0], table);
                printf(", ");
                generateOperand(insn->operands[1], table);
                printf("\n");
            }
            break;
            
        case INST_SUB:
            // SUB operand -> sub al, operand
            if (insn->operandCount == 1) {
                printf("  sub al, ");
                generateOperand(insn->operands[0], table);
                printf("\n");
            } else if (insn->operandCount == 2) {
                printf("  sub ");
                generateOperand(insn->operands[0], table);
                printf(", ");
                generateOperand(insn->operands[1], table);
                printf("\n");
            }
            break;
            
        case INST_INC:
            // INC operand -> inc operand
            if (insn->operandCount >= 1) {
                printf("  inc ");
                generateOperand(insn->operands[0], table);
                printf("\n");
            }
            break;
            
        case INST_DEC:
            // DEC operand -> dec operand
            if (insn->operandCount >= 1) {
                printf("  dec ");
                generateOperand(insn->operands[0], table);
                printf("\n");
            }
            break;
            
        case INST_AND:
            // AND operand -> and al, operand
            if (insn->operandCount == 1) {
                printf("  and al, ");
                generateOperand(insn->operands[0], table);
                printf("\n");
            } else if (insn->operandCount == 2) {
                printf("  and ");
                generateOperand(insn->operands[0], table);
                printf(", ");
                generateOperand(insn->operands[1], table);
                printf("\n");
            }
            break;
            
        case INST_OR:
            // OR operand -> or al, operand
            if (insn->operandCount == 1) {
                printf("  or al, ");
                generateOperand(insn->operands[0], table);
                printf("\n");
            } else if (insn->operandCount == 2) {
                printf("  or ");
                generateOperand(insn->operands[0], table);
                printf(", ");
                generateOperand(insn->operands[1], table);
                printf("\n");
            }
            break;
            
        case INST_XOR:
            // XOR operand -> xor al, operand
            if (insn->operandCount == 1) {
                printf("  xor al, ");
                generateOperand(insn->operands[0], table);
                printf("\n");
            } else if (insn->operandCount == 2) {
                printf("  xor ");
                generateOperand(insn->operands[0], table);
                printf(", ");
                generateOperand(insn->operands[1], table);
                printf("\n");
            }
            break;
            
        case INST_CP:
            // CP operand -> cmp al, operand
            if (insn->operandCount >= 1) {
                printf("  cmp al, ");
                generateOperand(insn->operands[0], table);
                printf("\n");
            }
            break;
            
        case INST_JP:
            // JP [cond,] addr -> jmp/j<cond> addr
            if (insn->operandCount == 1) {
                printf("  jmp ");
                generateOperand(insn->operands[0], table);
                printf("\n");
            } else if (insn->operandCount == 2) {
                // JP cond, addr
                printf("  j");
                generateOperand(insn->operands[0], table);
                printf(" ");
                generateOperand(insn->operands[1], table);
                printf("\n");
            }
            break;
            
        case INST_JR:
            // JR [cond,] disp -> jmp/j<cond> (relative jump)
            if (insn->operandCount == 1) {
                printf("  jmp ");
                generateOperand(insn->operands[0], table);
                printf("\n");
            } else if (insn->operandCount == 2) {
                printf("  j");
                generateOperand(insn->operands[0], table);
                printf(" ");
                generateOperand(insn->operands[1], table);
                printf("\n");
            }
            break;
            
        case INST_DJNZ:
            // DJNZ disp -> dec + jnz
            // En Z80: decrementa B y salta si B != 0
            printf("  dec bl ");
            if (insn->operandCount >= 1) {
                printf("  jnz ");
                generateOperand(insn->operands[0], table);
                printf("\n");
            }
            break;
            
        case INST_CALL:
            // CALL addr -> call addr
            if (insn->operandCount >= 1) {
                printf("  call ");
                generateOperand(insn->operands[0], table);
                printf("\n");
            }
            break;
            
        case INST_RET:
            // RET -> ret
            printf("  ret\n");
            break;
            
        case INST_PUSH:
            // PUSH reg -> push reg
            if (insn->operandCount >= 1) {
                printf("  push ");
                generateOperand(insn->operands[0], table);
                printf("\n");
            }
            break;
            
        case INST_POP:
            // POP reg -> pop reg
            if (insn->operandCount >= 1) {
                printf("  pop ");
                generateOperand(insn->operands[0], table);
                printf("\n");
            }
            break;
            
        case INST_NOP:
            // NOP -> nop
            printf("  nop\n");
            break;
            
        default:
            logError(_logger, "Unsupported instruction: %s", instructionToString(insn->type));
            printf("  ; TODO: Implement %s\n", instructionToString(insn->type));
            break;
    }
}

void generateOperand(Operand* operand, SymbolTable* table) {
    generateOperandWithParams(operand, table, NULL, 0);
}

void generateOperandWithParams(Operand* operand, SymbolTable* table, char** params, int paramCount) {
    if (_logger == NULL) {
        _logger = createLogger("InstructionMapper");
    }
    
    if (operand == NULL) {
        logError(_logger, "Null operand");
        return;
    }

    switch (operand->type) {
        case OPERAND_CONSTANT:
            printf("0x%X", operand->expr->value);
            break;
            
        case OPERAND_SYMBOL: {
            int paramIdx = findParamIndex(operand->symbol, params, paramCount);
            if (paramIdx >= 0) {
                printf("%%%d", paramIdx + 1);
            } else {
                printf("%s", operand->symbol);
            }
            break;
        }
            
        case OPERAND_REGISTER8:
            printf("%s", getX86Register8(operand->reg8));
            break;
            
        case OPERAND_REGISTER16:
            printf("%s", getX86Register16(operand->reg16));
            break;
            
        case OPERAND_CONDITION:
            printf("%s", conditionToX86(operand->condition));
            break;
            
        case OPERAND_MEMORY_HL:
            printf("[rcx]");
            break;
            
        case OPERAND_MEMORY_IXIY_DISP: {
            const char* baseReg = getX86Register64(operand->mem_ixiy_disp.base);
            if (operand->mem_ixiy_disp.disp != NULL) {
                if (operand->mem_ixiy_disp.disp->symbol != NULL) {
                    int paramIdx = findParamIndex(operand->mem_ixiy_disp.disp->symbol, params, paramCount);
                    if (paramIdx >= 0) {
                        printf("[%s+%%%d]", baseReg, paramIdx + 1);
                    } else {
                        printf("[%s+%s]", baseReg, operand->mem_ixiy_disp.disp->symbol);
                    }
                } else {
                    printf("[%s+%d]", baseReg, operand->mem_ixiy_disp.disp->value);
                }
            } else {
                printf("[%s]", baseReg);
            } 
            break;
        }
            
        case OPERAND_MEMORY_ABS:
            if (operand->mem_abs != NULL) {
                if (operand->mem_abs->symbol != NULL) {
                    int paramIdx = findParamIndex(operand->mem_abs->symbol, params, paramCount);
                    if (paramIdx >= 0) {
                        printf("[%%%d]", paramIdx + 1);
                    } else {
                        printf("[%s]", operand->mem_abs->symbol);
                    }
                } else {
                    printf("[0x%X]", operand->mem_abs->value);
                }
            }
            break;
            
        default:
            logError(_logger, "Unknown operand type: %d", operand->type);
            break;
    }
}

void generateInstructionWithParams(Instruction* insn, SymbolTable* table, char** params, int paramCount) {
    if (_logger == NULL) {
        _logger = createLogger("InstructionMapper");
    }
    
    if (insn == NULL) {
        logError(_logger, "Null instruction");
        return;
    }
    
    printf("  %s ", instructionToString(insn->type));
    
    for (int i = 0; i < insn->operandCount; i++) {
        if (i > 0) printf(", ");
        generateOperandWithParams(insn->operands[i], table, params, paramCount);
    }
    
    printf("\n");
}

const char* conditionToX86(ConditionType cond) {
    switch (cond) {
        case COND_NZ: return "nz";
        case COND_Z:  return "z";
        case COND_NC: return "nc";
        case COND_C:  return "c";
        case COND_PO: return "po";
        case COND_PE: return "pe";
        case COND_P:  return "ns";
        case COND_M:  return "s";
        default:      return "INVALID";
    }
}

const char* instructionToString(InstructionType type) {
    switch (type) {
        case INST_LD:   return "mov";
        case INST_ADD:  return "add";
        case INST_SUB:  return "sub";
        case INST_INC:  return "inc";
        case INST_DEC:  return "dec";
        case INST_AND:  return "and";
        case INST_OR:   return "or";
        case INST_XOR:  return "xor";
        case INST_CP:   return "cp";
        case INST_JP:   return "jp";
        case INST_JR:   return "jr";
        case INST_DJNZ: return "djnz"; // todo: abria que hacer el djnz bien
        case INST_CALL: return "call";
        case INST_RET:  return "ret";
        case INST_PUSH: return "push";
        case INST_POP:  return "pop";
        case INST_NOP:  return "nop";
        default:        return "unknown";
    }
}
