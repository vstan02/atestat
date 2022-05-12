/* Debug - Common functions for debugging
 * Copyright (C) 2021 Stan Vlad <vstan02@protonmail.com>
 *
 * This file is part of Woden.
 *
 * Woden is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <https://www.gnu.org/licenses/>.
 */

#include <stdio.h>

#include "debug.h"
#include "value.h"

static size_t simple_operation(const char* name, size_t offset) {
    printf("%s\n", name);
    return offset + 1;
}

static size_t const_operation(const char* name, chunk_t* chunk, size_t offset) {
    size_t constant = (size_t) chunk->code[offset + 1];
    printf("%-16s %4zu '", name, constant);
    value_print(chunk->constants.values[constant]);
    printf("'\n");
    return offset + 2;
}

extern void disassemble_chunk(chunk_t* chunk, const char* name) {
    printf("== %s ==\n", name);

    for (size_t offset = 0; offset < chunk->length;) {
        offset = disassemble_operation(chunk, offset);
    }
}

extern size_t disassemble_operation(chunk_t* chunk, size_t offset) {
    printf("%04zu ", offset);
    if (offset && chunk->lines[offset] == chunk->lines[offset - 1]) {
        printf("   | ");
    } else {
        printf("%4zu ", chunk->lines[offset]);
    }

    operation_t operation = (operation_t) chunk->code[offset];
    switch (operation) {
        case OP_CONSTANT:
            return const_operation("OP_CONSTANT", chunk, offset);
        case OP_NULL:
            return simple_operation("OP_NULL", offset);
        case OP_TRUE:
            return simple_operation("OP_TRUE", offset);
        case OP_FALSE:
            return simple_operation("OP_FALSE", offset);
        case OP_NOT:
            return simple_operation("OP_NOT", offset);
        case OP_NEGATE:
            return simple_operation("OP_NEGATE", offset);
        case OP_EQUAL:
            return simple_operation("OP_EQUAL", offset);
        case OP_NOT_EQUAL:
            return simple_operation("OP_NOT_EQUAL", offset);
        case OP_LESS:
            return simple_operation("OP_LESS", offset);
        case OP_LESS_EQUAL:
            return simple_operation("OP_LESS_EQUAL", offset);
        case OP_GREATER:
            return simple_operation("OP_GREATER", offset);
        case OP_GREATER_EQUAL:
            return simple_operation("OP_GREATER_EQUAL", offset);
        case OP_AND:
            return simple_operation("OP_AND", offset);
        case OP_OR:
            return simple_operation("OP_OR", offset);
        case OP_ADD:
            return simple_operation("OP_ADD", offset);
        case OP_SUBTRACT:
            return simple_operation("OP_SUBTRACT", offset);
        case OP_MULTIPLY:
            return simple_operation("OP_MULTIPLY", offset);
        case OP_DIVIDE:
            return simple_operation("OP_DIVIDE", offset);
        case OP_MODULO:
            return simple_operation("OP_MODULO", offset);
        case OP_RETURN:
            return simple_operation("OP_RETURN", offset);
        case OP_PRINT:
            return simple_operation("OP_PRINT", offset);
        case OP_POP:
            return simple_operation("OP_POP", offset);
        case OP_DEFINE_GLOBAL:
            return simple_operation("OP_DEFINE_GLOBAL", offset);
        case OP_GET_GLOBAL:
            return simple_operation("OP_GET_GLOBAL", offset);
        case OP_SET_GLOBAL:
            return simple_operation("OP_SET_GLOBAL", offset);
        default:
            printf("Unknown operation %d\n", operation);
            return offset + 1;
    }
}
