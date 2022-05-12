/* Table - A hash table
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

#ifndef WODEN_TABLE_H
#define WODEN_TABLE_H

#include <stddef.h>

#include "value.h"

#define TABLE_SIZE 10

typedef struct table table_t;
typedef struct table_storage table_storage_t;

struct table {
    table_storage_t* storages[TABLE_SIZE];
};

extern void table_init(table_t* table);
extern void table_free(table_t* table);

extern value_t* table_get(table_t* table, string_t* key);
extern void table_set(table_t* table, string_t* key, value_t value);

#endif // WODEN_TABLE_H
