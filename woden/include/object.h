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

#ifndef WODEN_OBJECT_H
#define WODEN_OBJECT_H

#include <stddef.h>
#include <inttypes.h>

#include "value.h"

#define OBJECT_TYPE(value) (AS_OBJECT(value)->type)

#define IS_STRING(value) is_object_type(value, OBJ_STRING)

#define AS_STRING(value)       ((string_t*)AS_OBJECT(value))
#define AS_CSTRING(value)      (((string_t*)AS_OBJECT(value))->target)

typedef enum object_type object_type_t;

enum object_type {
    OBJ_STRING
};

struct object {
    object_type_t type;
};

struct string {
    object_t object;
    size_t size;
    char* target;
    uint32_t hash;
};

static inline bool is_object_type(value_t value, object_type_t type) {
    return IS_OBJECT(value) && AS_OBJECT(value)->type == type;
}

extern void object_print(value_t value);
extern string_t* string_copy(const char* string, size_t size);
extern string_t* string_make(char* string, size_t size);

#endif // WODEN_OBJECT_H
