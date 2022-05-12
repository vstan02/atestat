/* Chunk - A chunk of code
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

#ifndef WODEN_CHUNK_H
#define WODEN_CHUNK_H

#include <stddef.h>
#include <inttypes.h>

#include "varray.h"

typedef struct chunk chunk_t;
typedef uint32_t byte_t;
typedef enum operation operation_t;

enum operation {
    OP_CONSTANT,
    OP_NULL,
    OP_TRUE,
    OP_FALSE,
    OP_NOT,
    OP_NEGATE,
    OP_DIVIDE,
    OP_MULTIPLY,
    OP_MODULO,
    OP_SUBTRACT,
    OP_ADD,
    OP_EQUAL,
    OP_NOT_EQUAL,
    OP_LESS,
    OP_LESS_EQUAL,
    OP_GREATER,
    OP_GREATER_EQUAL,
    OP_AND,
    OP_OR,
    OP_PRINT,
    OP_POP,
    OP_DEFINE_GLOBAL,
    OP_GET_GLOBAL,
    OP_SET_GLOBAL,
    OP_RETURN
};

struct chunk {
    size_t size;
    size_t length;
    byte_t* code;
    size_t* lines;
    varray_t constants;
};

extern void chunk_init(chunk_t* chunk);
extern void chunk_free(chunk_t* chunk);

extern byte_t chunk_value(chunk_t* chunk, value_t value);
extern void chunk_write(chunk_t* chunk, byte_t byte, size_t line);

#endif // WODEN_CHUNK_H
