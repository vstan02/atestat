/* Value - A Woden value
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
#include <string.h>

#include "value.h"
#include "object.h"

extern void value_print(value_t value) {
    switch (value.type) {
        case VAL_NULL: printf("null"); break;
        case VAL_NUMBER: printf("%g", AS_NUMBER(value)); break;
        case VAL_BOOL: printf(AS_BOOL(value) ? "true" : "false"); break;
        case VAL_OBJECT: object_print(value); break;
        default: printf("<VALUE>");
    }
}

extern bool value_equal(value_t x, value_t y) {
    if (x.type != y.type) return false;
    switch (x.type) {
        case VAL_BOOL: return AS_BOOL(x) == AS_BOOL(y);
        case VAL_NULL: return true;
        case VAL_NUMBER: return AS_NUMBER(x) == AS_NUMBER(y);
        case VAL_OBJECT: {
            string_t* a = AS_STRING(x);
            string_t* b = AS_STRING(y);
            return a->size == b->size && memcmp(a->target, b->target, a->size) == 0;
        }
        default: return false;
    }
}
