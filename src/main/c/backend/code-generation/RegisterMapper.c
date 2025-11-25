#include "RegisterMapper.h"
#include "../../support/logging/Logger.h"
#include <stdlib.h>

static Logger* _logger = NULL;

static RegisterMapping registerMap[] = {
    {"A",  "al",    "ax",    "rax"},
    {"F",  "ah",    "ax",    "rax"},
    {"B",  "bh",    "bx",    "rbx"},
    {"C",  "bl",    "bx",    "rbx"},
    {"D",  "dh",    "dx",    "rdx"},
    {"E",  "dl",    "dx",    "rdx"},
    {"H",  "sih",   "si",    "rsi"},
    {"L",  "sil",   "si",    "rsi"},
    {"AF", NULL,    "ax",    "rax"},
    {"BC", NULL,    "bx",    "rbx"},
    {"DE", NULL,    "dx",    "rdx"},
    {"HL", NULL,    "si",    "rsi"},
    {"SP", NULL,    "sp",    "rsp"},
    {"IX", NULL,    "r8w",   "r8"},
    {"IY", NULL,    "r9w",   "r9"}
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
