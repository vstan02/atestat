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

#include "chunk.h"
#include "array.h"

#define BASE_SIZE 4

extern void chunk_init(chunk_t* chunk) {
    chunk->size = BASE_SIZE;
    chunk->length = 0;
    chunk->code = array_alloc(byte_t, BASE_SIZE);
    chunk->lines = array_alloc(size_t, BASE_SIZE);
    varray_init(&chunk->constants);
}

extern void chunk_free(chunk_t* chunk) {
    free(chunk->code);
    free(chunk->lines);
    varray_free(&chunk->constants);
}

extern uint32_t chunk_value(chunk_t* chunk, value_t value) {
    varray_push(&chunk->constants, value);
    return chunk->constants.length - 1;
}

extern void chunk_write(chunk_t* chunk, byte_t byte, size_t line) {
    if (chunk->length == chunk->size) {
        chunk->size *= 2;
        array_resize(size_t, chunk->lines, chunk->size);
        array_resize(byte_t, chunk->code, chunk->size);
    }

    chunk->lines[chunk->length] = line;
    chunk->code[chunk->length] = (byte_t) byte;
    ++chunk->length;
}
