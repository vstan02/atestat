/* Compiler - Woden syntax analyzer
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
#include <stdlib.h>

#include "parser.h"
#include "lexer.h"
#include "object.h"

typedef struct parser parser_t;
typedef enum precendense precendense_t;
typedef struct parse_rule parse_rule_t;
typedef void (*parse_t)(parser_t*, bool);

struct parser {
    chunk_t* target;
    bool error;
    bool panic;
    lexer_t lexer;
    token_t current;
    token_t previous;
};

enum precendense {
    PREC_NONE,
    PREC_ASSIGNMENT,
    PREC_OR,
    PREC_AND,
    PREC_EQUALITY,
    PREC_COMPARISON,
    PREC_TERM,
    PREC_FACTOR,
    PREC_UNARY,
    PREC_CALL,
    PREC_PRIMARY
};

struct parse_rule {
    parse_t prefix;
    parse_t infix;
    precendense_t precendense;
};

static void variable(parser_t*, bool);
static void expression(parser_t*, bool);
static void grouping(parser_t*, bool);
static void binary(parser_t*, bool);
static void unary(parser_t*, bool);
static void string(parser_t*, bool);
static void number(parser_t*, bool);
static void literal(parser_t*, bool);

parse_rule_t rules[] = {
    [TOKEN_LEFT_PAREN] = { grouping, NULL, PREC_NONE },
    [TOKEN_RIGHT_PAREN] = { NULL, NULL, PREC_NONE },
    [TOKEN_LEFT_BRACE] = { NULL, NULL, PREC_NONE },
    [TOKEN_RIGHT_BRACE] = { NULL, NULL, PREC_NONE },
    [TOKEN_LEFT_BRACKET] = { NULL, NULL, PREC_NONE },
    [TOKEN_RIGHT_BRACKET] = { NULL, NULL, PREC_NONE },
    [TOKEN_COMMA] = { NULL, NULL, PREC_NONE },
    [TOKEN_DOT] = { NULL, NULL, PREC_NONE },
    [TOKEN_MINUS] = { unary, binary, PREC_TERM },
    [TOKEN_PLUS] = { NULL, binary, PREC_TERM },
    [TOKEN_SEMICOLON] = { NULL, NULL,   PREC_NONE },
    [TOKEN_SLASH] = { NULL, binary, PREC_FACTOR },
    [TOKEN_STAR] = { NULL, binary, PREC_FACTOR },
    [TOKEN_PERCENT] = { NULL, binary, PREC_FACTOR },
    [TOKEN_BANG] = { unary, NULL, PREC_NONE },
    [TOKEN_BANG_EQUAL] = { NULL, binary, PREC_EQUALITY },
    [TOKEN_EQUAL] = { NULL, NULL, PREC_NONE },
    [TOKEN_EQUAL_EQUAL] = { NULL, binary, PREC_EQUALITY },
    [TOKEN_GREATER] = { NULL, binary, PREC_COMPARISON },
    [TOKEN_GREATER_EQUAL] = { binary, NULL, PREC_COMPARISON },
    [TOKEN_LESS] = { NULL, binary, PREC_COMPARISON },
    [TOKEN_LESS_EQUAL] = { NULL, binary, PREC_COMPARISON },
    [TOKEN_IDENTIFIER] = { variable, NULL, PREC_NONE },
    [TOKEN_STRING] = { string, NULL, PREC_NONE },
    [TOKEN_NUMBER] = { number, NULL, PREC_NONE },
    [TOKEN_AND] = { NULL, NULL, PREC_NONE },
    [TOKEN_CLASS] = { NULL, NULL, PREC_NONE },
    [TOKEN_ELSE] = { NULL, NULL, PREC_NONE },
    [TOKEN_FALSE] = { literal, NULL, PREC_NONE },
    [TOKEN_FOR] = { NULL, NULL, PREC_NONE },
    [TOKEN_FUNC] = { NULL, NULL, PREC_NONE },
    [TOKEN_IF] = { NULL, NULL, PREC_NONE },
    [TOKEN_NULL] = { literal, NULL, PREC_NONE },
    [TOKEN_OR] = { NULL, NULL, PREC_NONE },
    [TOKEN_PRINT] = { NULL, NULL, PREC_NONE },
    [TOKEN_RETURN] = { NULL, NULL, PREC_NONE },
    [TOKEN_SUPER] = { NULL, NULL, PREC_NONE },
    [TOKEN_THIS] = { NULL, NULL, PREC_NONE },
    [TOKEN_NEW] = { NULL, NULL, PREC_NONE },
    [TOKEN_TRUE] = { literal, NULL, PREC_NONE },
    [TOKEN_VAR] = { NULL, NULL, PREC_NONE },
    [TOKEN_WHILE] = { NULL, NULL, PREC_NONE },
    [TOKEN_PROGRAM] = { NULL, NULL, PREC_NONE },
    [TOKEN_ERROR] = { NULL, NULL, PREC_NONE },
    [TOKEN_EOF] = { NULL, NULL, PREC_NONE },
};

static void error_at(parser_t* parser, token_t* token, const char* message) {
    if (parser->panic) return;
    parser->panic = true;

    fprintf(stdout, "[line %zu] Error", token->line);

    if (token->type == TOKEN_EOF) {
        fprintf(stdout, " at end");
    } else if (token->type != TOKEN_ERROR) {
        fprintf(stdout, " at '%.*s'", (int)token->size, token->start);
    }

    fprintf(stdout, ": %s\n", message);
    parser->error = true;
}

static inline void error_at_current(parser_t* parser, const char* message) {
    error_at(parser, &parser->current, message);
}

static inline void error(parser_t* parser, const char* message) {
    error_at(parser, &parser->previous, message);
}

static void advance(parser_t* parser) {
    parser->previous = parser->current;

    while (true) {
        parser->current = lexer_next(&parser->lexer);
        if (parser->current.type != TOKEN_ERROR) break;

        error_at_current(parser, parser->current.start);
    }
}

static void consume(parser_t* parser, token_type_t type, const char* message) {
    if (parser->current.type == type) {
        return advance(parser);
    }

    error_at_current(parser, message);
}

static inline void emit_byte(parser_t* parser, byte_t byte) {
    chunk_write(parser->target, byte, parser->previous.line);
}

static inline void emit_return(parser_t* parser) {
    emit_byte(parser, OP_RETURN);
}

static void emit_bytes(parser_t* parser, byte_t byte1, byte_t byte2) {
    emit_byte(parser, byte1);
    emit_byte(parser, byte2);
}

static bool match(parser_t* parser, token_type_t type) {
    if (parser->current.type != type) {
        return false;
    }

    advance(parser);
    return true;
}

static void end_parsing(parser_t* parser) {
    emit_return(parser);

#ifdef DEBUG_PRINT_CODE
    if (!parser.error) {
        disassemble_chunk(parser.target, "code");
    }
#endif
}

static byte_t make_constant(parser_t* parser, value_t value) {
    byte_t byte = chunk_value(parser->target, value);
    if (byte == UINT8_MAX) {
        error(parser, "Too many constants in one chunk.");
        return 0;
    }
    return byte;
}

static inline void emit_constant(parser_t* parser, value_t value) {
    emit_bytes(parser, OP_CONSTANT, make_constant(parser, value));
}

static inline parse_rule_t* get_rule(token_type_t type) {
    return &rules[type];
}

static void parse_precedence(parser_t* parser, precendense_t precedence) {
    advance(parser);
    parse_t prefix_rule = get_rule(parser->previous.type)->prefix;
    if (prefix_rule == NULL) {
        return error(parser, "Expect expression.");
    }
    bool can_assign = precedence <= PREC_ASSIGNMENT;
    prefix_rule(parser, can_assign);

    while (precedence <= get_rule(parser->current.type)->precendense) {
        advance(parser);
        parse_t infix_rule = get_rule(parser->previous.type)->infix;
        infix_rule(parser, can_assign);
    }

    if (can_assign && match(parser, TOKEN_EQUAL)) {
        error(parser, "Invalid assignment target.");
    }
}

static void synchronize(parser_t* parser) {
    parser->panic = false;

    while (parser->current.type != TOKEN_EOF) {
        if (parser->previous.type == TOKEN_SEMICOLON) return;
        switch (parser->current.type) {
            case TOKEN_VAR:
            case TOKEN_FOR:
            case TOKEN_IF:
            case TOKEN_WHILE:
            case TOKEN_PRINT:
            case TOKEN_RETURN:
                return;
            default:;
        }

        advance(parser);
    }
}

static inline byte_t id_constant(parser_t* parser, token_t* name) {
    return make_constant(parser, OBJECT_VAL(string_copy(name->start, name->size)));
}

static byte_t parse_variable(parser_t* parser, const char* error) {
    consume(parser, TOKEN_IDENTIFIER, error);
    return id_constant(parser, &parser->previous);
}

static void define_variable(parser_t* parser, byte_t global) {
    emit_bytes(parser, OP_DEFINE_GLOBAL, global);
}

static void grouping(parser_t* parser, bool) {
    expression(parser, false);
    consume(parser, TOKEN_RIGHT_PAREN, "Expect ')' after expression.");
}

static void binary(parser_t* parser, bool) {
    token_type_t type = parser->previous.type;
    parse_rule_t* rule = get_rule(type);
    parse_precedence(parser, (precendense_t)rule->precendense + 1);

    switch (type) {
        case TOKEN_BANG_EQUAL: emit_byte(parser, OP_NOT_EQUAL); break;
        case TOKEN_EQUAL_EQUAL: emit_byte(parser, OP_EQUAL); break;
        case TOKEN_GREATER: emit_byte(parser, OP_GREATER); break;
        case TOKEN_GREATER_EQUAL: emit_byte(parser, OP_GREATER_EQUAL); break;
        case TOKEN_LESS: emit_byte(parser, OP_LESS); break;
        case TOKEN_LESS_EQUAL: emit_byte(parser, OP_LESS_EQUAL); break;
        case TOKEN_PLUS: emit_byte(parser, OP_ADD); break;
        case TOKEN_MINUS: emit_byte(parser, OP_SUBTRACT); break;
        case TOKEN_STAR: emit_byte(parser, OP_MULTIPLY); break;
        case TOKEN_SLASH: emit_byte(parser, OP_DIVIDE); break;
        case TOKEN_PERCENT: emit_byte(parser, OP_MODULO); break;
        default: return;
    }
}

static void unary(parser_t* parser, bool) {
    token_type_t type = parser->previous.type;
    parse_precedence(parser, PREC_UNARY);

    switch (type) {
        case TOKEN_BANG: emit_byte(parser, OP_NOT); break;
        case TOKEN_MINUS: emit_byte(parser, OP_NEGATE); break;
        default: break;
    }
}

static void literal(parser_t* parser, bool) {
    switch (parser->previous.type) {
        case TOKEN_NULL: emit_byte(parser, OP_NULL); break;
        case TOKEN_TRUE: emit_byte(parser, OP_TRUE); break;
        case TOKEN_FALSE: emit_byte(parser, OP_FALSE); break;
        default: return;
    }
}

static void number(parser_t* parser, bool) {
    double value = strtod(parser->previous.start, NULL);
    emit_constant(parser, NUMBER_VAL(value));
}

static void string(parser_t* parser, bool) {
    emit_constant(parser, OBJECT_VAL(string_copy(parser->previous.start + 1, parser->previous.size - 2)));
}

static void expression(parser_t* parser, bool) {
    parse_precedence(parser, PREC_ASSIGNMENT);
}

static inline void named_variable(parser_t* parser, token_t name, bool can_assign) {
    if (can_assign && match(parser, TOKEN_EQUAL)) {
        expression(parser, can_assign);
        emit_bytes(parser, OP_SET_GLOBAL, id_constant(parser, &name));
    } else {
        emit_bytes(parser, OP_GET_GLOBAL, id_constant(parser, &name));
    }
}

static void variable(parser_t* parser, bool can_assign) {
    named_variable(parser, parser->previous, can_assign);
}

static void print_statement(parser_t* parser) {
    expression(parser, false);
    consume(parser, TOKEN_SEMICOLON, "Expect ';' after value.");
    emit_byte(parser, OP_PRINT);
}

static void expr_statement(parser_t* parser) {
    expression(parser, false);
    consume(parser, TOKEN_SEMICOLON, "Expect ';' after value.");
    emit_byte(parser, OP_POP);
}

static void var_declaration(parser_t* parser) {
    byte_t global = parse_variable(parser, "Expect variable name.");

    if (match(parser, TOKEN_EQUAL)) {
        expression(parser, false);
    } else {
        emit_byte(parser, OP_NULL);
    }
    consume(parser, TOKEN_SEMICOLON, "Expect ';' after variable declaration.");

    define_variable(parser, global);
}

static void statement(parser_t* parser) {
    if (match(parser, TOKEN_PRINT)) {
        print_statement(parser);
    } else {
        expr_statement(parser);
    }
}

static void declaration(parser_t* parser) {
    if (match(parser, TOKEN_VAR)) {
        var_declaration(parser);
    }
}

static void block(parser_t* parser) {
    while (!match(parser, TOKEN_RIGHT_BRACE)) {
        if (match(parser, TOKEN_EOF)) {
            error_at_current(parser, "Expect '}' after block.");
        }

        if (parser->panic) {
            synchronize(parser);
        }

        if (match(parser, TOKEN_VAR)) {
            var_declaration(parser);
        } else {
            statement(parser);
        }
    }
}

static void program(parser_t* parser) {
    while (!match(parser, TOKEN_PROGRAM)) {
        if (match(parser, TOKEN_EOF)) {
            error_at_current(parser, "Expect 'program' section in code.");
        }
        declaration(parser);
    }

    consume(parser, TOKEN_LEFT_BRACE, "Expect '{' at start of 'program'.");
    block(parser);
    consume(parser, TOKEN_EOF, "End of expression.");
}

extern bool parser_parse(chunk_t* chunk, const char* source) {
    parser_t parser = { chunk };
    lexer_init(&parser.lexer, source);

    advance(&parser);
    program(&parser);
    end_parsing(&parser);

    return !parser.error;
}
