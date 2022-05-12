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

#ifndef WODEN_VALUE_H
#define WODEN_VALUE_H

#include <stdbool.h>

#define NULL_VAL ((value_t){ VAL_NULL, { .number = 0 } })
#define BOOL_VAL(value) ((value_t){ VAL_BOOL, { .boolean = value } })
#define NUMBER_VAL(value) ((value_t){ VAL_NUMBER, { .number = value } })
#define OBJECT_VAL(value) ((value_t){ VAL_OBJECT, { .object = (object_t*)value } })

#define AS_BOOL(value) ((value).as.boolean)
#define AS_NUMBER(value) ((value).as.number)
#define AS_OBJECT(value) ((value).as.object)

#define IS_NULL(value) ((value).type == VAL_NULL)
#define IS_BOOL(value) ((value).type == VAL_BOOL)
#define IS_NUMBER(value) ((value).type == VAL_NUMBER)
#define IS_OBJECT(value) ((value).type == VAL_OBJECT)

typedef struct value value_t;
typedef struct object object_t;
typedef struct string string_t;
typedef enum value_type value_type_t;

enum value_type {
    VAL_NULL,
    VAL_BOOL,
    VAL_NUMBER,
    VAL_OBJECT
};

struct value {
    value_type_t type;
    union {
        bool boolean;
        double number;
        object_t* object;
    } as;
};

extern void value_print(value_t value);

extern bool value_equal(value_t x, value_t y);

#endif // WODEN_VALUE_H
