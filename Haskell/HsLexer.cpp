/******************************************************************************
 * Copyright (c) 2014-2016 Leandro T. C. Melo (ltcmelo@gmail.com)
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301
 * USA
 *****************************************************************************/

/*--------------------------*/
/*--- The UaiSo! Project ---*/
/*--------------------------*/

#include "Haskell/HsLexer.h"
#include "Haskell/HsKeywords.h"
#include "Haskell/HsLang.h"
#include "Common/Assert.h"
#include "Common/Trace__.h"
#include "Parsing/ParsingContext.h"
#include "Parsing/Lexeme.h"
#include <cctype>

#define TRACE_NAME "HsLexer"

using namespace uaiso;

namespace {

const HsLang hsLang;

} // anonymous

HsLexer::HsLexer()
    : bits_(0)
{
    bit_.atLineStart_ = true;
}

HsLexer::~HsLexer()
{}

Token HsLexer::lex()
{
    Token tk = TK_INVALID;
    updatePos();

LexNextToken:
    mark_ = curr_; // Mark the start of the upcoming token.

    char ch = peekChar();

    // If an opening brace is found, consume it and continue. Otherwise,
    // auto-insert one and keep track of the scope.
    if (bit_.wantBrace_) {
        maybeSkipSpaces(ch);
        bit_.wantBrace_ = false;
        if (ch == '{') {
            consumeChar();
            layoutStack_.push(std::make_pair(false, -1));
        } else {
            bit_.waitOffsetMark_ = true;
            layoutStack_.push(std::make_pair(true, -1));
        }
        tk = TK_LBRACE;
        goto LexDone;
    }

    // At a line start the layout must be checked. Either a semicolon may be
    // inserted as a separator or a scope delimiting closing brace (in the case
    // the matching opening brace has been auto-inserted).
    if (bit_.atLineStart_ && !bit_.waitOffsetMark_ && !layoutStack_.empty()) {
        const Layout& layout = layoutStack_.top();
        if (layout.first) {
            maybeSkipSpaces(ch);
            if (ch != '\n') {
                if (col_ == layout.second) {
                    tk = TK_SEMICOLON;
                    goto LexDone;
                }
                if (col_ < layout.second) {
                    layoutStack_.pop();
                    tk = TK_RBRACE;
                    goto LexDone;
                }
            }
        }
    }

    switch (ch) {
    case 0:
        // TODO: Clarify braces/semicolon rule on "invalid" lexeme.
        return TK_EOP;

    case '\n':
        bit_.atLineStart_ = true;
        consumeChar();
        handleNewLine();
        updatePos();
        goto LexNextToken;

    case '\f':
    case '\t':
    case ' ':
        skipSpaces(ch);
        goto LexNextToken;

    case '(':
    case ')':
    case ',':
    case ';':
    case '[':
    case ']':
    case '`':
        tk = lexSpecial(ch);
        break;

    case '{':
        layoutStack_.push(std::make_pair(false, -1));
        tk = lexSpecial(ch);
        break;

    case '}':
        if (!layoutStack_.empty() && !layoutStack_.top().first)
            layoutStack_.pop();
        tk = lexSpecial(ch);
        break;

    case '!':
    case '#':
    case '$':
    case '%':
    case '&':
    case '*':
    case '+':
    case '/':
    case '>':
    case '?':
    case '@':
    case '\\':
    case '^':
    case '|':
    case '~':
        tk = lexAscSymbol(ch);
        break;

    case '.':
        tk = lexAscSymbolMaybe2(ch, '.', TK_DOT_DOT);
        break;

    case '-':
        tk = lexAscSymbolMaybe2(ch, '>', TK_DASH_ARROW);
        break;

    case '=':
        tk = lexAscSymbolMaybe2(ch, '>', TK_EQ_ARROW);
        break;

    case '<':
        tk = lexAscSymbolMaybe2(ch, '-', TK_ARROW_DASH);
        break;

    case ':':
        tk = lexAscSymbolMaybe2(ch, ':', TK_COLON_COLON);
        break;

    case '"':
    case '\'':
        tk = lexStrLit(ch, false, &hsLang);
        context_->trackLexeme<StrLit>(mark_, curr_ - mark_, LineCol(line_, col_));
        break;

    default:
        if (hsLang.isIdentFirstChar(ch)) {
            tk = lexIdentOrKeyword(ch, &hsLang);
            break;
        }

        if (std::isdigit(ch)) {
            tk = lexNumLit(ch, &hsLang);
            context_->trackLexeme<NumLit>(mark_, curr_ - mark_, LineCol(line_, col_));
            break;
        }

        // Don't know what this is.
        consumeChar();
        PRINT_TRACE("Unknown char %c at %d,%d\n", ch, 0, 0);
        break;
    }

    if (bit_.waitOffsetMark_) {
        bit_.waitOffsetMark_ = false;
        UAISO_ASSERT(!layoutStack_.empty(), return TK_INVALID);
        layoutStack_.top().second = col_;
    }

LexDone:
    bit_.atLineStart_ = false;

    LineCol lineCol(line_, col_);
    context_->trackToken(tk, lineCol);
    context_->trackPhrase(tk, lineCol, curr_ - mark_);

    return tk;
}

namespace {

// A few tokens indexed by their corresponding ASCII value.
const int oprtrTable[] =
{
    // Category `special`
    TK_LPAREN,                                             // 0
    TK_RPAREN,
    TK_COMMA,
    TK_SEMICOLON,
    TK_LBRACKET,
    TK_RBRACKET,
    TK_BACKTICK,
    TK_LBRACE,
    TK_RBRACE,                                             // 8

    // Category `ascSymbol`
    TK_EXCLAM,                                             // 9
    TK_POUND,
    TK_DOLLAR,
    TK_PERCENT,
    TK_AMPER,
    TK_ASTERISK,
    TK_PLUS,                                               // 15
    TK_MINUS,
    TK_DOT,
    TK_SLASH,
    TK_COLON,
    TK_LS,
    TK_EQ,
    TK_GR,
    TK_QUESTION,                                           // 23
    TK_AT,
    TK_BACKSLASH,
    TK_CARET,
    TK_PIPE,
    TK_TILDE,                                              // 28

    // Indexing
    -1, -1, -1, -1,
    9,                                                     // 33
    -1,
    10, 11, 12, 13,
    -1,                                                    // 39
    0, 1, 14, 15, 2, 16, 17, 18,
    -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1,                            // 51...57
    19, 3, 20, 21, 22, 23, 24,
    -1, -1, -1, -1, -1, -1,                                // 65...70
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,                // 71...80
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,                // 81...90
    4, 25, 5, 26,
    -1,
    6,
    -1, -1, -1, -1,                                        // 97...100
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,                // 101...110
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,                // 111...120
    -1, -1,
    7, 27, 8, 28
};

} // anonymous

Token HsLexer::lexOprtrTable(char& ch)
{
    UAISO_ASSERT(ch >= 33 && ch <= 126, return TK_INVALID);

    const char index = oprtrTable[ch];
    ch = consumeCharPeekNext();

    return Token(oprtrTable[index]);
}

Token HsLexer::lexSpecial(char& ch)
{
    return lexOprtrTable(ch);
}

Token HsLexer::lexAscSymbol(char& ch)
{
    Token tk = lexOprtrTable(ch);
    return lexAscSymbolMaybeMore(ch, tk);
}

Token HsLexer::lexAscSymbolMaybe2(char& ch, const char& match, Token reserved)
{
    Token tk = lexOprtrTable(ch);
    if (ch == match) {
        ch = consumeCharPeekNext();
        tk = reserved;
    }

    return lexAscSymbolMaybeMore(ch, tk);
}

Token HsLexer::lexAscSymbolMaybeMore(char &ch, Token tk)
{
    if (!isAscSymbol(ch))
        return tk;

    do {
        ch = consumeCharPeekNext();
    } while (isAscSymbol(ch));

    return TK_CUSTOM_OPRTR;
}

bool HsLexer::isAscSymbol(const char ch) const
{
    switch (ch) {
    case '!':
    case '#':
    case '$':
    case '%':
    case '&':
    case '*':
    case '+':
    case '/':
    case '>':
    case '?':
    case '@':
    case '\\':
    case '^':
    case '|':
    case '~':
    case '.':
    case '=':
    case '<':
    case ':':
        return true;

    default:
        return false;
    }
}

Token HsLexer::filterKeyword(const char* spell, size_t len) const
{
    return HsKeywords::filter(spell, len);
}

void HsLexer::inspectKeyword(Token tk)
{
    switch (tk) {
    case TK_WHERE:
    case TK_LET:
    case TK_DO:
    case TK_OF:
        bit_.wantBrace_ = true;
        break;

    default:
        break;
    }
}

Token HsLexer::classifyIdent(char& ch)
{
    if (mark_[0] >= 97) {
        context_->trackLexeme<Ident>(mark_, curr_ - mark_, LineCol(line_, col_));
        return TK_IDENT;
    }

    // We have a capitalized identifier. However, we must check what comes
    // after it because it can actually be a qualified name or a qualified
    // operator, which are lexed entirely as a single lexeme.
    Token tk = TK_CAPITAL_IDENT;

    std::function<void ()> classifyRecursively = [&]() {
        if (ch != '.') {
            context_->trackLexeme<Ident>(mark_, curr_ - mark_, LineCol(line_, col_));
            return;
        }

        // The presence of the `.` is not sufficient condition to characterize
        // a qualification. It must be followed by an identifier or operator.
        char next = peekChar(1);

        if (hsLang.isIdentChar(next)) {
            ch = consumeCharPeekNext(1);
            while (hsLang.isIdentChar(ch))
                ch = consumeCharPeekNext();
            tk = TK_CAPITAL_IDENT_LIST;
            classifyRecursively();
        }

        if (isAscSymbol(next)) {
            ch = consumeCharPeekNext(1);
            while (isAscSymbol(ch))
                ch = consumeCharPeekNext();
            context_->trackLexeme<Ident>(mark_, curr_ - mark_, LineCol(line_, col_));
            tk = TK_QUALIFIED_OPRTR;
        }
    };
    classifyRecursively();

    return tk;
}
