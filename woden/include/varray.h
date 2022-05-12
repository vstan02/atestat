/* VArray - A dynamic array of values
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

#ifndef WODEN_VARRAY_H
#define WODEN_VARRAY_H

#include <stddef.h>

#include "value.h"

typedef struct varray varray_t;

struct varray {
    size_t size;
    size_t length;
    value_t* values;
};

extern void varray_init(varray_t* array);
extern void varray_free(varray_t* array);

extern void varray_push(varray_t* array, value_t value);

#endif // WODEN_VARRAY_H
