/* Object - A Woden object
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

#include <string.h>
#include <stdio.h>

#include "object.h"
#include "array.h"

static uint32_t string_hash(const char* key, size_t size) {
    uint32_t hash = 2166136261u;
    for (int i = 0; i < size; ++i) {
        hash ^= (uint8_t)key[i];
        hash *= 16777619;
    }
    return hash;
}

static object_t* object_alloc(size_t size, object_type_t type) {
    object_t* object = (object_t*)malloc(size);
    object->type = type;
    return object;
}

extern string_t* string_alloc(char* chars, size_t size, uint32_t hash) {
    string_t* string = (string_t*)object_alloc(sizeof(string_t), OBJ_STRING);
    string->size = size;
    string->target = chars;
    string->hash = hash;
    return string;
}

extern string_t* string_make(char* chars, size_t size) {
    uint32_t hash = string_hash(chars, size);
    return string_alloc(chars, size, hash);
}

extern string_t* string_copy(const char* source, size_t size) {
    char* target = array_alloc(char, size + 1);
    memcpy(target, source, size);
    target[size] = '\0';
    return string_make(target, size);
}

extern void object_print(value_t value) {
    switch (OBJECT_TYPE(value)) {
        case OBJ_STRING: printf("%s", AS_CSTRING(value)); break;
    }
}
