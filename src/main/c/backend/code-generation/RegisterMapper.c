#include "RegisterMapper.h"
#include "../../support/logging/Logger.h"
#include <stdlib.h>

static Logger* _logger = NULL;

static RegisterMapping registerMap[] = {
    {"A",  "al",  "ax",   "rax"},   
    {"B",  "bl",  "bx",   "rbx"},   
    {"C",  "bh",  NULL,   NULL},    
    {"D",  "dl",  "dx",   "rdx"},   
    {"E",  "dh",  NULL,   NULL},    
    {"H",  "cl",  "cx",   "rcx"},   
    {"L",  "ch",  NULL,   NULL},   
    
    {"AF", NULL,  "ax",   "rax"},   
    {"BC", NULL,  "bx",   "rbx"},   
    {"DE", NULL,  "dx",   "rdx"},   
    {"HL", NULL,  "cx",   "rcx"},   
    
    {"SP", NULL,  "r8w",  "r8"},    
    {"IX", NULL,  "r9w",  "r9"},    
    {"IY", NULL,  "r10w", "r10"}
};

const char* getX86Register8(RegisterName z80reg) {
    if (_logger == NULL) {
        _logger = createLogger("RegisterMapper");
    }
    if (z80reg >= 0 && z80reg < 14) {
        const char* result = registerMap[z80reg].x86_8bit;
        if (result == NULL) {
            logError(_logger, "No 8-bit mapping for register %d", z80reg);
            return "INVALID";
        }
        return result;
    }
    logError(_logger, "Invalid register index: %d", z80reg);
    return "INVALID";
}

const char* getX86Register16(RegisterName z80reg) {
    if (_logger == NULL) {
        _logger = createLogger("RegisterMapper");
    }
    if (z80reg >= 0 && z80reg < 14) {
        return registerMap[z80reg].x86_16bit;
    }
    logError(_logger, "Invalid register index: %d", z80reg);
    return "INVALID";
}

const char* getX86Register64(RegisterName z80reg) {
    if (_logger == NULL) {
        _logger = createLogger("RegisterMapper");
    }
    if (z80reg >= 0 && z80reg < 14) {
        return registerMap[z80reg].x86_64bit;
    }
    logError(_logger, "Invalid register index: %d", z80reg);
    return "INVALID";
}

const char* getRegisterName(RegisterName reg) {
    if (_logger == NULL) {
        _logger = createLogger("RegisterMapper");
    }
    if (reg >= 0 && reg < 14) {
        return registerMap[reg].z80_name;
    }
    return "UNKNOWN";
}
