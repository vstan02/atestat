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

#include <string.h>

#include "table.h"
#include "array.h"
#include "object.h"

struct table_storage {
    string_t* key;
    value_t value;
    table_storage_t* next;
};

extern void table_init(table_t* table) {
    for (size_t i = 0; i < TABLE_SIZE; ++i) {
        table->storages[i] = NULL;
    }
}

extern void table_free(table_t* table) {
    for (size_t i = 0; i < TABLE_SIZE; ++i) {
        table_storage_t* storage = table->storages[i];
        while (storage != NULL) {
            table_storage_t* current = storage;
            storage = storage->next;
            free(current);
        }
    }
}

extern value_t* table_get(table_t* table, string_t* key) {
    table_storage_t* storage = table->storages[key->hash % TABLE_SIZE];
    while (storage != NULL) {
        if (!strcmp(storage->key->target, key->target)) {
            return &storage->value;
        }
        storage = storage->next;
    }
    return NULL;
}

extern void table_set(table_t* table, string_t* key, value_t value) {
    table_storage_t* storage = (table_storage_t*) malloc(sizeof(table_storage_t));
    storage->key = key;
    storage->value = value;

    size_t index = key->hash % TABLE_SIZE;
    storage->next = table->storages[index];
    table->storages[index] = storage;
}
