/* VM - Woden virtual machine
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

#ifndef WODEN_VM_H
#define WODEN_VM_H

#include "chunk.h"

typedef enum vm_result vm_result_t;

enum vm_result {
    VM_SUCCESS,
    VM_SYNTAX_ERROR,
    VM_RUNTIME_ERROR
};

extern vm_result_t vm_run(chunk_t* chunk);

#endif // WODEN_VM_H
