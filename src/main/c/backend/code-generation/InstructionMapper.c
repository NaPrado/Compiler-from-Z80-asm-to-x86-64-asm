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
            printf("0x%X", operand->constantValue);
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
            printf("[rbx]");
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
        case INSN_LD:   return "LD";
        case INSN_ADD:  return "ADD";
        case INSN_SUB:  return "SUB";
        case INSN_INC:  return "INC";
        case INSN_DEC:  return "DEC";
        case INSN_AND:  return "AND";
        case INSN_OR:   return "OR";
        case INSN_XOR:  return "XOR";
        case INSN_CP:   return "CP";
        case INSN_JP:   return "JP";
        case INSN_JR:   return "JR";
        case INSN_DJNZ: return "DJNZ"; // todo: abria que hacer el djnz bien
        case INSN_CALL: return "CALL";
        case INSN_RET:  return "RET";
        case INSN_PUSH: return "PUSH";
        case INSN_POP:  return "POP";
        case INSN_NOP:  return "NOP";
        default:        return "UNKNOWN";
    }
}
