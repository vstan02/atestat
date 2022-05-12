/* Stack - A stack of values
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

#ifndef WODEN_STACK_H
#define WODEN_STACK_H

#include "value.h"

#define STACK_SIZE 256

typedef struct stack stack_t;

struct stack {
    value_t* current;
    value_t values[STACK_SIZE];
};

static inline void stack_init(stack_t* stack) {
    stack->current = stack->values;
}

static inline void stack_push(stack_t* stack, value_t value) {
    *(stack->current++) = value;
}

static inline value_t stack_pop(stack_t* stack) {
    return *(--stack->current);
}

#endif // WODEN_STACK_H
