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

#include <string.h>
#include <stdbool.h>
#include <stddef.h>

#include "lexer.h"

#define KWS_NUM 16

typedef struct keyword keyword_t;
struct keyword {
    const char* value;
    token_type_t type;
};

static keyword_t keywords[KWS_NUM] = {
    { "true", TOKEN_TRUE },
    { "false", TOKEN_FALSE },
    { "null", TOKEN_NULL },
    { "if", TOKEN_IF },
    { "else", TOKEN_ELSE },
    { "var", TOKEN_VAR },
    { "for", TOKEN_FOR },
    { "while", TOKEN_WHILE },
    { "return", TOKEN_RETURN },
    { "function", TOKEN_FUNC },
    { "class", TOKEN_CLASS },
    { "super", TOKEN_SUPER },
    { "this", TOKEN_THIS },
    { "new", TOKEN_NEW },
    { "program", TOKEN_PROGRAM },
    { "print", TOKEN_PRINT }
};

static inline bool is_alpha(char c) {
    return c == '_'
        || c >= 'a' && c <= 'z'
        || c >= 'A' && c <= 'Z';
}

static inline bool is_digit(char c) {
    return c >= '0' && c <= '9';
}

static inline bool is_alphanum(char c) {
    return is_alpha(c) || is_digit(c);
}

static inline bool at_end(lexer_t* lexer) {
    return *lexer->current == '\0';
}

static inline char advance(lexer_t* lexer) {
    return (++lexer->current)[-1];
}

static inline token_t error_token(lexer_t* lexer, const char* message) {
    return (token_t) {
        .type = TOKEN_ERROR,
        .start = message,
        .size = strlen(message),
        .line = lexer->line
    };
}

static inline token_t make_token(lexer_t* lexer, token_type_t type) {
    return (token_t) {
        .type = type,
        .start = lexer->start,
        .size = (size_t)(lexer->current - lexer->start),
        .line = lexer->line
    };
}

static inline token_t assert_token(lexer_t* lexer, char expected, token_type_t type) {
    if (*lexer->current != expected) {
        return error_token(lexer, "Unexpected character.");
    }

    ++lexer->current;
    return make_token(lexer, type);
}

static token_type_t id_token(lexer_t* lexer) {
    size_t size = (size_t)(lexer->current - lexer->start);
    for (size_t i = 0; i < KWS_NUM; ++i) {
        if (!memcmp(lexer->start, keywords[i].value, size)) {
            return keywords[i].type;
        }
    }
    return TOKEN_IDENTIFIER;
}

static void skip_whitespace(lexer_t* lexer) {
    while (true) {
        switch (*lexer->current) {
            case ' ':
            case '\r':
            case '\t':
                advance(lexer);
                break;
            case '\n':
                ++lexer->line;
                advance(lexer);
                break;
            case '/': {
                if (lexer->current[1] != '/') return;
                while (*lexer->current != '\n') {
                    advance(lexer);
                }
                break;
            }
            default: return;
        }
    }
}

static token_t choose_token(lexer_t* lexer, char next, token_type_t first, token_type_t second) {
    if (*lexer->current != next) {
        return make_token(lexer, second);
    }

    ++lexer->current;
    return make_token(lexer, first);
}

static token_t make_number(lexer_t* lexer) {
    while (is_digit(*lexer->current)) {
        advance(lexer);
    }

    if (*lexer->current == '.' && is_digit(lexer->current[1])) {
        advance(lexer);
        while (is_digit(*lexer->current)) {
            advance(lexer);
        }
    }

    return make_token(lexer, TOKEN_NUMBER);
}

static token_t make_string(lexer_t* lexer) {
    while (*lexer->current != '\'' && !at_end(lexer)) {
        if (*lexer->current == '\n') ++lexer->line;
        advance(lexer);
    }

    if (at_end(lexer)) {
        return error_token(lexer, "Unterminated string.");
    }

    advance(lexer);
    return make_token(lexer, TOKEN_STRING);
}

static token_t make_id(lexer_t* lexer) {
    while (is_alphanum(*lexer->current)) {
        advance(lexer);
    }

    return make_token(lexer, id_token(lexer));
}

extern void lexer_init(lexer_t* lexer, const char* source) {
    lexer->line = 1;
    lexer->current = lexer->start = source;
}

extern token_t lexer_next(lexer_t* lexer) {
    skip_whitespace(lexer);

    lexer->start = lexer->current;
    if (at_end(lexer)) {
        return make_token(lexer, TOKEN_EOF);
    }

    char current = advance(lexer);
    if (is_alpha(current)) {
        return make_id(lexer);
    }

    if (is_digit(current)) {
        return make_number(lexer);
    }

    switch (current) {
        case '\'': return make_string(lexer);
        case '(': return make_token(lexer, TOKEN_LEFT_PAREN);
        case ')': return make_token(lexer, TOKEN_RIGHT_PAREN);
        case '{': return make_token(lexer, TOKEN_LEFT_BRACE);
        case '}': return make_token(lexer, TOKEN_RIGHT_BRACE);
        case '[': return make_token(lexer, TOKEN_LEFT_BRACKET);
        case ']': return make_token(lexer, TOKEN_RIGHT_BRACKET);
        case ',': return make_token(lexer, TOKEN_COMMA);
        case '.': return make_token(lexer, TOKEN_DOT);
        case ';': return make_token(lexer, TOKEN_SEMICOLON);
        case '-': return make_token(lexer, TOKEN_MINUS);
        case '+': return make_token(lexer, TOKEN_PLUS);
        case '/': return make_token(lexer, TOKEN_SLASH);
        case '*': return make_token(lexer, TOKEN_STAR);
        case '%': return make_token(lexer, TOKEN_PERCENT);
        case '=': return choose_token(lexer, '=', TOKEN_EQUAL_EQUAL, TOKEN_EQUAL);
        case '!': return choose_token(lexer, '=', TOKEN_BANG_EQUAL, TOKEN_BANG);
        case '<': return choose_token(lexer, '=', TOKEN_LESS_EQUAL, TOKEN_LESS);
        case '>': return choose_token(lexer, '=', TOKEN_GREATER_EQUAL, TOKEN_GREATER);
        case '|': return assert_token(lexer, '|', TOKEN_OR);
        case '&': return assert_token(lexer, '&', TOKEN_AND);
        default: return error_token(lexer, "Unexpected character.");
    }
}
