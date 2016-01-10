/******************************************************************************
 * Copyright (c) 2014-2015 Leandro T. C. Melo (ltcmelo@gmail.com)
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

#include "Python/PyIncrementalLexer.h"
#include "Python/PyLexer.h"
#include "Parsing/ParsingContext.h"
#include "Parsing/IncrementalLexer__.h"

using namespace uaiso;

PyIncrementalLexer::PyIncrementalLexer()
{
    P->context_.reset(new ParsingContext);
    P->context_->setAllowComments(true);
}

PyIncrementalLexer::~PyIncrementalLexer()
{}

void PyIncrementalLexer::lex(const std::string& source)
{
    P->phrasing_.reset(new Phrasing);
    P->context_->collectPhrasing(P->phrasing_.get());

    PyLexer lexer;
    lexer.setContext(P->context_.get());
    lexer.setBuffer(source.c_str(), source.size());

    Token tk;
    do {
        tk = lexer.lex();
    } while (tk != TK_EOP);
}
