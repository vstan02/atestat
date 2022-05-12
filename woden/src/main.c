/* Woden - A simple programming language
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
#include <stdlib.h>

#include "chunk.h"
#include "parser.h"
#include "vm.h"

char* read_file(const char* path) {
    FILE* file = fopen(path, "r+");
    if (file == NULL) {
        fprintf(stderr, "Could not open file \"%s\".\n", path);
        exit(74);
    }

    fseek(file, 0L, SEEK_END);
    size_t file_size = ftell(file);
    rewind(file);

    char* buffer = (char*) malloc(file_size + 1);
    if (buffer == NULL) {
        fprintf(stderr, "Not enough memory to read \"%s\".\n", path);
        exit(1);
    }
    size_t bytes = fread(buffer, sizeof(char), file_size, file);
    if (bytes < file_size) {
        fprintf(stderr, "Could not read file \"%s\".\n", path);
        exit(1);
    }
    buffer[bytes] = '\0';

    fclose(file);
    return buffer;
}

int main(int argc, char** argv) {
    chunk_t chunk;
    char* code = read_file(argv[1]);

    chunk_init(&chunk);
    if (!parser_parse(&chunk, code)) {
        return 0;
    }

    if (vm_run(&chunk) != VM_SUCCESS) {
        return 0;
    }

    chunk_free(&chunk);
    return 0;
}
