/* VArray Test - Tests for VArray
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

#include <glib.h>

#include "test.h"
#include "varray.h"

#define VALUES 6
#define TEST_PATH "/varray"

#define foreach(index, from, to) \
    for (size_t index = from; index < to; ++index)

static void test_main(void);

extern void add_varray_tests(void) {
    g_test_add_func(TEST_PATH, test_main);
}

static void test_main(void) {
    varray_t array;
    int values[VALUES] = { 43, 23, 49, 47, 45, 29 };

    varray_init(&array);
    foreach(i, 0, VALUES) {
        varray_push(&array, NUMBER_VAL(values[i]));
    }

    foreach(i, 0, VALUES) {
        g_assert_cmpint(values[i], ==, AS_NUMBER(array.values[i]));
    }
    varray_free(&array);
}
