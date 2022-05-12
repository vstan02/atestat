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

#include <stdio.h>
#include <stdarg.h>
#include <string.h>

#include "vm.h"
#include "value.h"
#include "object.h"
#include "stack.h"
#include "debug.h"
#include "table.h"
#include "array.h"

//#define DEBUG_TRACE_EXECUTION

#define read_byte(vm) (*(vm)->current++)
#define read_const(vm) ((vm)->chunk->constants.values[read_byte(vm)])
#define read_string(vm) AS_STRING(read_const(vm))

#define pop(vm) stack_pop(&(vm)->stack)
#define push(vm, x) stack_push(&(vm)->stack, x)
#define peek(vm, i) (vm)->stack.current[-(i) - 1]

#define binary_operation(vm, type, op) \
    do { \
      if (!IS_NUMBER(peek(vm, 0)) || !IS_NUMBER(peek(vm, 1))) { \
        runtime_error(vm, "Operands must be numbers."); \
        return VM_RUNTIME_ERROR; \
      } \
      double b = AS_NUMBER(pop(vm)); \
      double a = AS_NUMBER(pop(vm)); \
      push(vm, type(a op b)); \
    } while (false)

typedef struct vm vm_t;

struct vm {
    chunk_t* chunk;
    byte_t* current;
    stack_t stack;
    table_t globals;
};

static bool is_falsey(value_t value) {
    return IS_NULL(value) || (IS_BOOL(value) && !AS_BOOL(value));
}

static void runtime_error(vm_t* vm, const char* format, ...) {
    va_list args;
    va_start(args, format);
    vfprintf(stdout, format, args);
    va_end(args);
    fputs("\n", stdout);

    size_t operation = vm->current - vm->chunk->code - 1;
    size_t line = vm->chunk->lines[operation];
    fprintf(stdout, "[line %zu] in script\n", line);
    stack_init(&vm->stack);
}

static void concatinate(vm_t* vm) {
    string_t* y = AS_STRING(pop(vm));
    string_t* x = AS_STRING(pop(vm));

    size_t size = x->size + y->size;
    char* target = array_alloc(char, size + 1);
    memcpy(target, x->target, x->size);
    memcpy(target + x->size, y->target, y->size);
    target[size] = '\0';

    push(vm, OBJECT_VAL(string_make(target, size)));
}

static vm_result_t interpret(vm_t* vm) {
    byte_t operation;
    while (true) {

#ifdef DEBUG_TRACE_EXECUTION
        printf("          { ");
        for (value_t* current = vm->stack.values; current < vm->stack.current; ++current) {
            value_print(*current);
            if (current < vm->stack.current - 1) {
                printf(", ");
            }
        }
        printf(" }");
        printf("\n");
        disassemble_operation(vm->chunk, (size_t)(vm->current - vm->chunk->code));
#endif

        switch (operation = read_byte(vm)) {
            case OP_CONSTANT: {
                push(vm, read_const(vm));
                break;
            }
            case OP_NULL: {
                push(vm, NULL_VAL);
                break;
            }
            case OP_TRUE: {
                push(vm, BOOL_VAL(true));
                break;
            }
            case OP_FALSE: {
                push(vm, BOOL_VAL(false));
                break;
            }
            case OP_NOT: {
                push(vm, BOOL_VAL(is_falsey(pop(vm))));
                break;
            }
            case OP_EQUAL: {
                value_t y = pop(vm);
                value_t x = pop(vm);
                push(vm, BOOL_VAL(value_equal(x, y)));
                break;
            }
            case OP_NOT_EQUAL: {
                value_t y = pop(vm);
                value_t x = pop(vm);
                push(vm, BOOL_VAL(!value_equal(x, y)));
                break;
            }
            case OP_GREATER: {
                binary_operation(vm, BOOL_VAL, >);
                break;
            }
            case OP_GREATER_EQUAL: {
                binary_operation(vm, BOOL_VAL, <);
                push(vm, BOOL_VAL(is_falsey(pop(vm))));
                break;
            }
            case OP_LESS: {
                binary_operation(vm, BOOL_VAL, <);
                break;
            }
            case OP_LESS_EQUAL: {
                binary_operation(vm, BOOL_VAL, >);
                push(vm, BOOL_VAL(is_falsey(pop(vm))));
                break;
            }
            case OP_NEGATE: {
                if (!IS_NUMBER(peek(vm, 0))) {
                    runtime_error(vm, "Operand must be a number.");
                    return VM_RUNTIME_ERROR;
                }
                push(vm, NUMBER_VAL(-AS_NUMBER(pop(vm))));
                break;
            }
            case OP_ADD: {
                if (IS_STRING(peek(vm, 0)) && IS_STRING(peek(vm, 1))) {
                    concatinate(vm);
                } else if (IS_NUMBER(peek(vm, 0)) && IS_NUMBER(peek(vm, 1))) {
                    double y = AS_NUMBER(pop(vm));
                    double x = AS_NUMBER(pop(vm));
                    push(vm, NUMBER_VAL(x + y));
                } else {
                    runtime_error(vm, "Operands must be two numbers or two strings.");
                    return VM_RUNTIME_ERROR;
                }
                break;
            }
            case OP_SUBTRACT: {
                binary_operation(vm, NUMBER_VAL, -);
                break;
            }
            case OP_MULTIPLY: {
                binary_operation(vm, NUMBER_VAL, *);
                break;
            }
            case OP_DIVIDE: {
                binary_operation(vm, NUMBER_VAL, /);
                break;
            }
            case OP_PRINT: {
                value_print(pop(vm));
                printf("\n");
                break;
            }
            case OP_POP: {
                pop(vm);
                break;
            }
            case OP_DEFINE_GLOBAL: {
                string_t* name = read_string(vm);
                table_set(&vm->globals, name, peek(vm, 0));
                pop(vm);
                break;
            }
            case OP_GET_GLOBAL: {
                string_t* name = read_string(vm);
                value_t* value = table_get(&vm->globals, name);
                if (value == NULL) {
                    runtime_error(vm, "Undefined variable '%s'.", name->target);
                    return VM_RUNTIME_ERROR;
                }
                push(vm, *value);
                break;
            }
            case OP_SET_GLOBAL: {
                string_t* name = read_string(vm);
                value_t* value = table_get(&vm->globals, name);
                if (value == NULL) {
                    runtime_error(vm, "Undefined variable '%s'.", name->target);
                    return VM_RUNTIME_ERROR;
                }
                *value = peek(vm, 0);
                break;
            }
            case OP_RETURN: {
                return VM_SUCCESS;
            }
        }
    }
}

extern vm_result_t vm_run(chunk_t* chunk) {
    vm_t vm = { chunk, chunk->code };
    stack_init(&vm.stack);
    table_init(&vm.globals);

    vm_result_t result = interpret(&vm);

    table_free(&vm.globals);
    return result;
}
