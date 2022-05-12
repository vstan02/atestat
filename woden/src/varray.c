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

#include "varray.h"
#include "array.h"

#define BASE_SIZE 4

extern void varray_init(varray_t* array) {
    array->size = BASE_SIZE;
    array->length = 0;
    array->values = array_alloc(value_t, BASE_SIZE);
}

extern void varray_free(varray_t* array) {
    free(array->values);
}

extern void varray_push(varray_t* array, value_t value) {
    if (array->length == array->size) {
        array->size *= 2;
        array_resize(value_t, array->values, array->size);
    }

    array->values[array->length++] = value;
}
