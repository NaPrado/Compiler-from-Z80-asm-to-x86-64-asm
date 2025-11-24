#ifndef REGISTER_MAPPER_H
#define REGISTER_MAPPER_H

#include "../../frontend/syntactic-analysis/AbstractSyntaxTree.h"

typedef struct {
    const char* z80_name;
    const char* x86_8bit;
    const char* x86_16bit;
    const char* x86_64bit;
} RegisterMapping;

const char* getX86Register8(RegisterName z80reg);
const char* getX86Register16(RegisterName z80reg);
const char* getX86Register64(RegisterName z80reg);
const char* getRegisterName(RegisterName reg);

#endif
