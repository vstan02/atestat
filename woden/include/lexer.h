/* Lexer - Woden lexical analyzer
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

#ifndef WODEN_LEXER_H
#define WODEN_LEXER_H

#include <stddef.h>

typedef struct lexer lexer_t;
typedef struct token token_t;
typedef enum token_type token_type_t;

struct lexer {
    size_t line;
    const char* start;
    const char* current;
};

enum token_type {
    TOKEN_LEFT_PAREN,
    TOKEN_RIGHT_PAREN,
    TOKEN_LEFT_BRACE,
    TOKEN_RIGHT_BRACE,
    TOKEN_LEFT_BRACKET,
    TOKEN_RIGHT_BRACKET,
    TOKEN_COMMA,
    TOKEN_DOT,
    TOKEN_MINUS,
    TOKEN_PLUS,
    TOKEN_SEMICOLON,
    TOKEN_SLASH,
    TOKEN_STAR,
    TOKEN_PERCENT,
    TOKEN_BANG,
    TOKEN_BANG_EQUAL,
    TOKEN_EQUAL,
    TOKEN_EQUAL_EQUAL,
    TOKEN_GREATER,
    TOKEN_GREATER_EQUAL,
    TOKEN_LESS,
    TOKEN_LESS_EQUAL,
    TOKEN_IDENTIFIER,
    TOKEN_STRING,
    TOKEN_NUMBER,
    TOKEN_AND,
    TOKEN_OR,
    TOKEN_TRUE,
    TOKEN_FALSE,
    TOKEN_NULL,
    TOKEN_IF,
    TOKEN_ELSE,
    TOKEN_VAR,
    TOKEN_FOR,
    TOKEN_WHILE,
    TOKEN_RETURN,
    TOKEN_FUNC,
    TOKEN_CLASS,
    TOKEN_SUPER,
    TOKEN_THIS,
    TOKEN_NEW,
    TOKEN_PRINT,
    TOKEN_PROGRAM,
    TOKEN_ERROR,
    TOKEN_EOF
};

struct token {
    token_type_t type;
    const char* start;
    size_t size;
    size_t line;
};

extern void lexer_init(lexer_t* lexer, const char* source);

extern token_t lexer_next(lexer_t* lexer);

#endif // WODEN_LEXER_H
