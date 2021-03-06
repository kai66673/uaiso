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

#ifndef UAISO_ASTEXPR_H__
#define UAISO_ASTEXPR_H__

#include "Ast/AstFwd.h"
#include "Ast/AstList.h"
#include "Ast/AstBase.h"
#include "Semantic/SymbolFwd.h"
#include "Semantic/TypeFwd.h"
#include <vector>

namespace uaiso {

class UAISO_API ExprAst : public Ast
{
public:
    using Ast::Ast;
};

/*!
 * \brief The CondExprAst class
 *
 * A conditional expression.
 */
class UAISO_API CondExprAst final : public ExprAst
{
public:
    AST_CLASS(Cond, Expr)

    CondExprAst()
        : ExprAst(Kind::CondExpr)
    {}

    NAMED_AST_PARAM(Cond, cond, ExprAst)
    NAMED_LOC_PARAM(Question, question)
    NAMED_AST_PARAM(Yes, yes, ExprAst)
    NAMED_LOC_PARAM(Delim, delim)
    NAMED_AST_PARAM(No, no, ExprAst)

    std::unique_ptr<ExprAst> cond_;
    SourceLoc questionLoc_;
    std::unique_ptr<ExprAst> yes_;
    SourceLoc delimLoc_;
    std::unique_ptr<ExprAst> no_;
};

class UAISO_API PrimaryExprAst : public ExprAst
{
public:
    AST_CLASS(Primary, Expr)
    using ExprAst::ExprAst;
};

class UAISO_API CharLitExprAst final : public PrimaryExprAst
{
public:
    AST_CLASS(CharLit, Expr)

    CharLitExprAst()
        : PrimaryExprAst(Kind::CharLitExpr)
    {}

    NAMED_LOC_PARAM(Lit, lit)

    SourceLoc litLoc_;
};

class UAISO_API StrLitExprAst final : public PrimaryExprAst
{
public:
    AST_CLASS(StrLit, Expr)

    StrLitExprAst()
        : PrimaryExprAst(Kind::StrLitExpr)
    {}

    NAMED_LOC_PARAM(Lit, lit)

    SourceLoc litLoc_;
};

class UAISO_API NumLitExprAst final : public PrimaryExprAst
{
public:
    AST_CLASS(NumLit, Expr)

    NumLitExprAst()
        : PrimaryExprAst(Kind::NumLitExpr)
    {
        INIT_VARIETY(NumLitVariety::Unknown);
    }

    APPLY_VARIETY(NumLitVariety)

    NAMED_LOC_PARAM(Lit, lit)

    SourceLoc litLoc_;
};

class UAISO_API BoolLitExprAst final : public PrimaryExprAst
{
public:
    AST_CLASS(BoolLit, Expr)

    BoolLitExprAst()
        : PrimaryExprAst(Kind::BoolLitExpr)
    {}

    NAMED_LOC_PARAM(Lit, lit)

    SourceLoc litLoc_;
};

class UAISO_API NullLitExprAst final : public PrimaryExprAst
{
public:
    AST_CLASS(NullLit, Expr)

    NullLitExprAst()
        : PrimaryExprAst(Kind::NullLitExpr)
    {}

    NAMED_LOC_PARAM(Lit, lit)

    SourceLoc litLoc_;
};

class UAISO_API FuncLitExprAst final : public PrimaryExprAst
{
public:
    AST_CLASS(FuncLit, Expr)

    FuncLitExprAst()
        : PrimaryExprAst(Kind::FuncLitExpr)
    {}

    NAMED_AST_PARAM(Spec, spec, SpecAst)
    NAMED_AST_PARAM(Stmt, stmt, StmtAst)

    std::unique_ptr<SpecAst> spec_;
    std::unique_ptr<StmtAst> stmt_;
};

// TODO: Work this out.
/*!
 * \brief The RecordLitExprAst class
 */
class UAISO_API RecordLitExprAst final : public PrimaryExprAst
{
public:
    AST_CLASS(RecordLit, Expr)

    RecordLitExprAst()
        : PrimaryExprAst(Kind::RecordLitExpr)
    {}

    RecordLitExprAst* setSpec(SpecAst* spec);
    RecordLitExprAst* setExpr(ExprAst* expr);

    NAMED_AST_PARAM(Init, init, ExprAst)

    std::unique_ptr<Ast> exprOrSpec_;
    std::unique_ptr<ExprAst> init_;
};

class UAISO_API ThisExprAst final : public PrimaryExprAst
{
public:
    AST_CLASS(This, Expr)

    ThisExprAst()
        : PrimaryExprAst(Kind::ThisExpr)
    {}

    NAMED_LOC_PARAM(Key, key)

    SourceLoc keyLoc_;
};

class UAISO_API SuperExprAst final : public PrimaryExprAst
{
public:
    AST_CLASS(Super, Expr)

    SuperExprAst()
        : PrimaryExprAst(Kind::SuperExpr)
    {}

    NAMED_LOC_PARAM(Key, key)

    SourceLoc keyLoc_;
};

class UAISO_API ArrayLengthExprAst final : public PrimaryExprAst
{
public:
    AST_CLASS(ArrayLength, Expr)

    ArrayLengthExprAst()
        : PrimaryExprAst(Kind::ArrayLengthExpr)
    {}

    NAMED_LOC_PARAM(Key, key)

    SourceLoc keyLoc_;
};

class UAISO_API IdentExprAst final : public PrimaryExprAst
{
public:
    AST_CLASS(Ident, Expr)

    IdentExprAst()
        : PrimaryExprAst(Kind::IdentExpr)
    {}

    IdentExprAst* setName(NameAstList* names);

    NAMED_AST_PARAM(Name, name, NameAst)

    std::unique_ptr<NameAst> name_;
};

/*!
 * \brief The TypeQueryExprAst class
 */
class UAISO_API TypeQueryExprAst : public PrimaryExprAst
{
public:
    AST_CLASS(TypeQuery, Expr)

    TypeQueryExprAst()
        : PrimaryExprAst(Kind::TypeQueryExpr)
    {}

    NAMED_LOC_PARAM(Key, key)
    NAMED_LOC_PARAM(LDelim, lDelim)
    NAMED_LOC_PARAM(RDelim, rDelim)
    NAMED_AST_PARAM(Spec, spec, SpecAst)
    NAMED_AST_PARAM(Name, name, NameAst)

    SourceLoc keyLoc_;
    SourceLoc lDelimLoc_;
    std::unique_ptr<SpecAst> spec_;
    std::unique_ptr<NameAst> name_;
    SourceLoc rDelimLoc_;
    // TODO: More forms...

    std::unique_ptr<Type> ty_;
};

class UAISO_API UnaryExprAst : public ExprAst
{
public:
    AST_CLASS(Unary, Expr)
    using ExprAst::ExprAst;

    NAMED_LOC_PARAM(Opr, opr)
    NAMED_AST_PARAM(Expr, expr, ExprAst)

    SourceLoc oprLoc_;
    std::unique_ptr<ExprAst> expr_;
};

class UAISO_API AddrOfExprAst final : public UnaryExprAst
{
public:
    AST_CLASS(AddrOf, Expr)

    AddrOfExprAst()
        : UnaryExprAst(Kind::AddrOfExpr)
    {}
};

class UAISO_API PtrDerefExprAst final : public UnaryExprAst
{
public:
    AST_CLASS(PtrDeref, Expr)

    PtrDerefExprAst()
        : UnaryExprAst(Kind::PtrDerefExpr)
    {}
};

class UAISO_API PlusExprAst final : public UnaryExprAst
{
public:
    AST_CLASS(Plus, Expr)

    PlusExprAst()
        : UnaryExprAst(Kind::PlusExpr)
    {}
};

class UAISO_API MinusExprAst final : public UnaryExprAst
{
public:
    AST_CLASS(Minus, Expr)

    MinusExprAst()
        : UnaryExprAst(Kind::MinusExpr)
    {}
};

class UAISO_API LogicNotExprAst final : public UnaryExprAst
{
public:
    AST_CLASS(LogicNot, Expr)

    LogicNotExprAst()
        : UnaryExprAst(Kind::LogicNotExpr)
    {}
};

class UAISO_API IncDecExprAst final : public UnaryExprAst
{
public:
    AST_CLASS(IncDec, Expr)

    IncDecExprAst()
        : UnaryExprAst(Kind::IncDecExpr)
    {}

    IncDecExprAst* setValue(ExprAst* expr)
    {
        expr_.reset(expr);
        return this;
    }

    /* TODO: Refactor to use Variety */
    IncDecExprAst* setPrefixLoc(const SourceLoc& loc)
    {
        oprLoc_ = loc;
        return this;
    }

    /* TODO: Refactor to use Variety */
    IncDecExprAst* setSuffixLoc(const SourceLoc& loc)
    {
        oprLoc_ = loc;
        return this;
    }
};

class UAISO_API BitCompExprAst final : public UnaryExprAst
{
public:
    AST_CLASS(BitComp, Expr)

    BitCompExprAst()
        : UnaryExprAst(Kind::BitCompExpr)
    {}
};

class UAISO_API ChanExprAst final : public UnaryExprAst
{
public:
    AST_CLASS(Chan, Expr)

    ChanExprAst()
        : UnaryExprAst(Kind::ChanExpr)
    {}
};

class UAISO_API BinaryExprAst : public ExprAst
{
public:
    AST_CLASS(Binary, Expr)
    using ExprAst::ExprAst;

    NAMED_AST_PARAM(Expr1, expr1, ExprAst)
    NAMED_LOC_PARAM(Opr, opr)
    NAMED_AST_PARAM(Expr2, expr2, ExprAst)

    std::unique_ptr<ExprAst> expr1_;
    SourceLoc oprLoc_;
    std::unique_ptr<ExprAst> expr2_;
};

class UAISO_API CommaExprAst final : public BinaryExprAst
{
public:
    AST_CLASS(Comma, Expr)

    CommaExprAst()
        : BinaryExprAst(Kind::CommaExpr)
    {}
};

class UAISO_API EqExprAst final : public BinaryExprAst
{
public:
    AST_CLASS(Eq, Expr)

    EqExprAst()
        : BinaryExprAst(Kind::EqExpr)
    {}
};

class UAISO_API RelExprAst final : public BinaryExprAst
{
public:
    AST_CLASS(Rel, Expr)

    RelExprAst()
        : BinaryExprAst(Kind::RelExpr)
    {}
};

class UAISO_API ShiftExprAst final : public BinaryExprAst
{
public:
    AST_CLASS(Shift, Expr)

    ShiftExprAst()
        : BinaryExprAst(Kind::ShiftExpr)
    {}
};

class UAISO_API ConcatExprAst final : public BinaryExprAst
{
public:
    AST_CLASS(Concat, Expr)

    ConcatExprAst()
        : BinaryExprAst(Kind::ConcatExpr)
    {}
};

class UAISO_API IsExprAst final : public BinaryExprAst
{
public:
    AST_CLASS(Is, Expr)

    IsExprAst()
        : BinaryExprAst(Kind::IsExpr)
    {}
};

class UAISO_API InExprAst final : public BinaryExprAst
{
public:
    AST_CLASS(In, Expr)

    InExprAst()
        : BinaryExprAst(Kind::InExpr)
    {}
};

class UAISO_API BitXorExprAst final : public BinaryExprAst
{
public:
    AST_CLASS(BitXor, Expr)

    BitXorExprAst()
        : BinaryExprAst(Kind::BitXorExpr)
    {}
};

class UAISO_API BitOrExprAst final : public BinaryExprAst
{
public:
    AST_CLASS(BitOr, Expr)

    BitOrExprAst()
        : BinaryExprAst(Kind::BitOrExpr)
    {}
};

class UAISO_API BitAndExprAst final : public BinaryExprAst
{
public:
    AST_CLASS(BitAnd, Expr)

    BitAndExprAst()
        : BinaryExprAst(Kind::BitAndExpr)
    {}
};

class UAISO_API LogicOrExprAst final : public BinaryExprAst
{
public:
    AST_CLASS(LogicOr, Expr)

    LogicOrExprAst()
        : BinaryExprAst(Kind::LogicOrExpr)
    {}
};

class UAISO_API LogicAndExprAst final : public BinaryExprAst
{
public:
    AST_CLASS(LogicAnd, Expr)

    LogicAndExprAst()
        : BinaryExprAst(Kind::LogicAndExpr)
    {}
};

class UAISO_API AddExprAst final : public BinaryExprAst
{
public:
    AST_CLASS(Add, Expr)

    AddExprAst()
        : BinaryExprAst(Kind::AddExpr)
    {}
};

class UAISO_API SubExprAst final : public BinaryExprAst
{
public:
    AST_CLASS(Sub, Expr)

    SubExprAst()
        : BinaryExprAst(Kind::SubExpr)
    {}
};

class UAISO_API MulExprAst final : public BinaryExprAst
{
public:
    AST_CLASS(Mul, Expr)

    MulExprAst()
        : BinaryExprAst(Kind::MulExpr)
    {}
};

class UAISO_API DivExprAst final : public BinaryExprAst
{
public:
    AST_CLASS(Div, Expr)

    DivExprAst()
        : BinaryExprAst(Kind::DivExpr)
    {}
};

class UAISO_API ModExprAst final : public BinaryExprAst
{
public:
    AST_CLASS(Mod, Expr)

    ModExprAst()
        : BinaryExprAst(Kind::ModExpr)
    {}
};

class UAISO_API PowerExprAst final : public BinaryExprAst
{
public:
    AST_CLASS(Power, Expr)

    PowerExprAst()
        : BinaryExprAst(Kind::PowerExpr)
    {}
};

class UAISO_API DelExprAst final : public ExprAst
{
public:
    AST_CLASS(Del, Expr)

    DelExprAst()
        : ExprAst(Kind::DelExpr)
    {}

    NAMED_LOC_PARAM(Key, key)
    NAMED_AST_LIST_PARAM(Expr, exprs, ExprAst)

    SourceLoc keyLoc_;
    std::unique_ptr<ExprAstList> exprs_;
};

class UAISO_API AssignExprAst final : public ExprAst
{
public:
    AST_CLASS(Assign, Expr)

    AssignExprAst()
        : ExprAst(Kind::AssignExpr)
    {
        INIT_VARIETY(AssignVariety::Unknow);
    }

    APPLY_VARIETY(AssignVariety)

    NAMED_AST_LIST_PARAM(Expr1, exprs1, ExprAst)
    NAMED_LOC_PARAM(Opr, opr)
    NAMED_AST_LIST_PARAM(Expr2, exprs2, ExprAst)

    std::unique_ptr<ExprAstList> exprs1_;
    SourceLoc oprLoc_;
    std::unique_ptr<ExprAstList> exprs2_;

    // An assignment may also bind names. For instance in dynamic languages
    // such as Python.
    std::vector<Var*> syms_;
};

class UAISO_API MemberAccessExprAst final : public ExprAst
{
public:
    AST_CLASS(MemberAccess, Expr)

    MemberAccessExprAst()
        : ExprAst(Kind::MemberAccessExpr)
    {}

    MemberAccessExprAst* setSpec(SpecAst* spec);
    MemberAccessExprAst* setExpr(ExprAst* expr);
    Ast* exprOrSpec() { return exprOrSpec_.get(); }

    NAMED_LOC_PARAM(Opr, opr)
    NAMED_AST_PARAM(Name, name, NameAst)

    std::unique_ptr<Ast> exprOrSpec_;
    SourceLoc oprLoc_;
    std::unique_ptr<NameAst> name_;
};

/*!
 * \brief The CastExprAst class
 */
class UAISO_API CastExprAst final : public ExprAst
{
public:
    AST_CLASS(Cast, Expr)

    CastExprAst()
        : ExprAst(Kind::CastExpr)
    {}

    NAMED_LOC_PARAM(LDelim, lDelim)
    NAMED_LOC_PARAM(RDelim, rDelim)
    NAMED_LOC_PARAM(Key, key)
    NAMED_AST_PARAM(Spec, spec, SpecAst)
    NAMED_AST_PARAM(Expr, expr, ExprAst)

    SourceLoc keyLoc_;
    SourceLoc lDelimLoc_;
    SourceLoc rDelimLoc_;
    std::unique_ptr<SpecAst> spec_;
    std::unique_ptr<ExprAst> expr_;

    std::unique_ptr<Type> ty_; // Type annotation
};

class UAISO_API WrappedExprAst final : public ExprAst
{
public:
    AST_CLASS(Wrapped, Expr)

    WrappedExprAst()
        : ExprAst(Kind::WrappedExpr)
    {}

    NAMED_LOC_PARAM(LDelim, lDelim)
    NAMED_AST_PARAM(Expr, expr, ExprAst)
    NAMED_LOC_PARAM(RDelim, rDelim)

    SourceLoc lDelimLoc_;
    std::unique_ptr<ExprAst> expr_;
    SourceLoc rDelimLoc_;
};

class UAISO_API VoidInitExprAst final : public ExprAst
{
public:
    AST_CLASS(VoidInit, Expr)

    VoidInitExprAst()
        : ExprAst(Kind::VoidInitExpr)
    {}

    NAMED_LOC_PARAM(Key, key)

    SourceLoc keyLoc_;
};

class UAISO_API RecordInitExprAst final : public ExprAst
{
public:
    AST_CLASS(RecordInit, Expr)

    RecordInitExprAst()
        : ExprAst(Kind::RecordInitExpr)
    {}

    NAMED_AST_PARAM(Spec, spec, SpecAst)
    NAMED_LOC_PARAM(LDelim, lDelim)
    NAMED_AST_LIST_PARAM(Init, inits, ExprAst)
    NAMED_LOC_PARAM(RDelim, rDelim)

    std::unique_ptr<SpecAst> spec_;
    SourceLoc lDelimLoc_;
    std::unique_ptr<ExprAstList> inits_;
    SourceLoc rDelimLoc_;
};

class UAISO_API ArrayInitExprAst final : public ExprAst
{
public:
    AST_CLASS(ArrayInit, Expr)

    ArrayInitExprAst()
        : ExprAst(Kind::ArrayInitExpr)
    {}

    NAMED_LOC_PARAM(LDelim, lDelim)
    NAMED_AST_LIST_PARAM(Init, inits, ExprAst)
    NAMED_LOC_PARAM(RDelim, rDelim)

    SourceLoc lDelimLoc_;
    std::unique_ptr<ExprAstList> inits_;
    SourceLoc rDelimLoc_;
};

class UAISO_API TupleLitExprAst final : public ExprAst
{
public:
    AST_CLASS(TupleLit, Expr)

    TupleLitExprAst()
        : ExprAst(Kind::TupleLitExpr)
    {}

    NAMED_LOC_PARAM(LDelim, lDelim)
    NAMED_AST_LIST_PARAM(Init, inits, ExprAst)
    NAMED_LOC_PARAM(RDelim, rDelim)

    SourceLoc lDelimLoc_;
    std::unique_ptr<ExprAstList> inits_;
    SourceLoc rDelimLoc_;
};

class UAISO_API DesignateExprAst final : public ExprAst
{
public:
    AST_CLASS(Designate, Expr)

    DesignateExprAst()
        : ExprAst(Kind::DesignateExpr)
    {}

    NAMED_AST_PARAM(Id, id, ExprAst)
    NAMED_LOC_PARAM(Delim, delim)
    NAMED_AST_PARAM(Value, value, ExprAst)

    std::unique_ptr<ExprAst> id_;
    SourceLoc delimLoc_;
    std::unique_ptr<ExprAst> value_;
};

class UAISO_API MakeExprAst final : public ExprAst
{
public:
    AST_CLASS(Make, Expr)

    MakeExprAst()
        : ExprAst(Kind::MakeExpr)
    {}

    NAMED_AST_PARAM(Base, base, ExprAst)
    NAMED_LOC_PARAM(LDelim, lDelim)
    NAMED_AST_PARAM(Spec, spec, SpecAst)
    NAMED_LOC_PARAM(Split, split)
    NAMED_AST_LIST_PARAM(Arg, args, ExprAst)
    NAMED_LOC_PARAM(Pack, pack)
    NAMED_LOC_PARAM(RDelim, rDelim)

    std::unique_ptr<ExprAst> base_;
    SourceLoc lDelimLoc_;
    std::unique_ptr<SpecAst> spec_;
    SourceLoc splitLoc_;
    std::unique_ptr<ExprAstList> args_;
    SourceLoc packLoc_;
    SourceLoc rDelimLoc_;

    std::unique_ptr<Type> ty_; // Type annotation
};

class UAISO_API NewExprAst final : public ExprAst
{
public:
    AST_CLASS(New, Expr)

    NewExprAst()
        : ExprAst(Kind::NewExpr)
    {}

    NAMED_LOC_PARAM(Key, key)
    NAMED_LOC_PARAM(LAllocDelim, lAllocDelim)
    NAMED_AST_LIST_PARAM(AllocArg, allocArgs, ExprAst)
    NAMED_LOC_PARAM(RAllocDelim, rAllocDelim)
    NAMED_AST_PARAM(Spec, spec, SpecAst)
    NAMED_LOC_PARAM(LArgDelim, lArgDelim)
    NAMED_AST_LIST_PARAM(Arg, args, ExprAst)
    NAMED_LOC_PARAM(RArgDelim, rArgDelim)

    SourceLoc keyLoc_;
    SourceLoc lAllocDelimLoc_;
    std::unique_ptr<ExprAstList> allocArgs_;
    SourceLoc rAllocDelimLoc_;
    std::unique_ptr<SpecAst> spec_;
    SourceLoc lArgDelimLoc_;
    std::unique_ptr<ExprAstList> args_;
    SourceLoc rArgDelimLoc_;

    std::unique_ptr<Type> ty_; // Type annotation
};

class UAISO_API NestedNewExprAst final : public ExprAst
{
public:
    AST_CLASS(NestedNew, Expr)

    NestedNewExprAst()
        : ExprAst(Kind::NestedNewExpr)
    {}

    NAMED_AST_PARAM(Base, base, ExprAst)
    NAMED_LOC_PARAM(Opr, opr)
    NAMED_AST_PARAM(NestedNew, nestedNew, ExprAst)

    std::unique_ptr<ExprAst> base_;
    SourceLoc oprLoc_;
    std::unique_ptr<ExprAst> nestedNew_;
};

class UAISO_API CallExprAst final : public ExprAst
{
public:
    AST_CLASS(Call, Expr)

    CallExprAst()
        : ExprAst(Kind::CallExpr)
    {}

    NAMED_AST_PARAM(Base, base, ExprAst)
    NAMED_LOC_PARAM(LDelim, lDelim)
    NAMED_AST_LIST_PARAM(Arg, args, ExprAst)
    NAMED_LOC_PARAM(Pack, pack)
    NAMED_LOC_PARAM(RDelim, rDelim)

    std::unique_ptr<ExprAst> base_;
    SourceLoc lDelimLoc_;
    std::unique_ptr<ExprAstList> args_;
    SourceLoc packLoc_;
    SourceLoc rDelimLoc_;
};

class UAISO_API UnpackExprAst final : public ExprAst
{
public:
    AST_CLASS(Unpack, Expr)

    UnpackExprAst()
        : ExprAst(Kind::UnpackExpr)
    {}

    NAMED_LOC_PARAM(Key, key)
    NAMED_AST_PARAM(Expr, expr, ExprAst)

    SourceLoc keyLoc_;
    std::unique_ptr<ExprAst> expr_;
};

class UAISO_API SubrangeExprAst final : public ExprAst
{
public:
    AST_CLASS(Subrange, Expr)

    SubrangeExprAst()
        : ExprAst(Kind::SubrangeExpr)
    {}

    NAMED_AST_PARAM(Low, low, ExprAst)
    NAMED_LOC_PARAM(Delim1, delim1)
    NAMED_AST_PARAM(Hi, hi, ExprAst)
    NAMED_LOC_PARAM(Delim2, delim2)
    NAMED_AST_PARAM(Max, max, ExprAst) // TODO: In Python this is a step.

    std::unique_ptr<ExprAst> low_;
    SourceLoc delim1Loc_;
    std::unique_ptr<ExprAst> hi_;
    SourceLoc delim2Loc_;
    std::unique_ptr<ExprAst> max_;
};

// TODO: Merge this and slice into a common ArrayAccess.
class UAISO_API ArrayIndexExprAst final : public ExprAst
{
public:
    AST_CLASS(ArrayIndex, Expr)

    ArrayIndexExprAst()
        : ExprAst(Kind::ArrayIndexExpr)
    {}

    NAMED_AST_PARAM(Base, base, ExprAst)
    NAMED_LOC_PARAM(LDelim, lDelim)
    NAMED_AST_PARAM(Index, index, ExprAst)
    NAMED_LOC_PARAM(RDelim, rDelim)

    std::unique_ptr<ExprAst> base_;
    SourceLoc lDelimLoc_;
    std::unique_ptr<ExprAst> index_;
    SourceLoc rDelimLoc_;
};

// TODO: Merge this and index into a common ArrayAccess.
class UAISO_API ArraySliceExprAst : public ExprAst
{
public:
    AST_CLASS(ArraySlice, Expr)

    ArraySliceExprAst()
        : ExprAst(Kind::ArraySliceExpr)
    {}

    NAMED_AST_PARAM(Base, base, ExprAst)
    NAMED_LOC_PARAM(LDelim, lDelim)
    NAMED_AST_PARAM(Range, range, ExprAst)
    NAMED_LOC_PARAM(RDelim, rDelim)

    std::unique_ptr<ExprAst> base_;
    SourceLoc lDelimLoc_;
    std::unique_ptr<ExprAst> range_;
    SourceLoc rDelimLoc_;
};

// TODO: Kepp only spec...
class UAISO_API TypeidExprAst final : public ExprAst
{
public:
    AST_CLASS(Typeid, Expr)

    TypeidExprAst()
        : ExprAst(Kind::TypeidExpr)
    {}

    TypeidExprAst* setSpec(SpecAst* spec);
    TypeidExprAst* setExpr(ExprAst* spec);
    Ast* exprOrSpec() { return exprOrSpec_.get(); }

    NAMED_LOC_PARAM(Key, key)
    NAMED_LOC_PARAM(LDelim, lDelim)
    NAMED_LOC_PARAM(RDelim, rDelim)

    SourceLoc keyLoc_;
    SourceLoc lDelimLoc_;
    std::unique_ptr<Ast> exprOrSpec_;
    SourceLoc rDelimLoc_;
};

class UAISO_API AssertExprAst final : public ExprAst
{
public:
    AST_CLASS(Assert, Expr)

    AssertExprAst()
        : ExprAst(Kind::AssertExpr)
    {}

    NAMED_LOC_PARAM(Key, key)
    NAMED_LOC_PARAM(LDelim, lDelim)
    NAMED_AST_PARAM(Expr, expr, ExprAst)
    NAMED_LOC_PARAM(MDelim, mDelim)
    NAMED_AST_PARAM(Message, message, ExprAst)
    NAMED_LOC_PARAM(RDelim, rDelim)

    SourceLoc keyLoc_;
    SourceLoc lDelimLoc_;
    std::unique_ptr<ExprAst> expr_;
    SourceLoc mDelimLoc_;
    std::unique_ptr<ExprAst> message_;
    SourceLoc rDelimLoc_;
};

class UAISO_API TypeAssertExprAst final : public ExprAst
{
public:
    AST_CLASS(TypeAssert, Expr)

    TypeAssertExprAst()
        : ExprAst(Kind::TypeAssertExpr)
    {}

    NAMED_AST_PARAM(Base, base, ExprAst)
    NAMED_LOC_PARAM(Opr, opr)
    NAMED_LOC_PARAM(LDelim, lDelim)
    NAMED_AST_PARAM(Spec, spec, SpecAst)
    NAMED_LOC_PARAM(RDelim, rDelim)

    std::unique_ptr<ExprAst> base_;
    SourceLoc oprLoc_;
    SourceLoc lDelimLoc_;
    std::unique_ptr<SpecAst> spec_;
    SourceLoc rDelimLoc_;

    std::unique_ptr<Type> ty_;
};

class UAISO_API MixinExprAst final : public ExprAst
{
public:
    AST_CLASS(Mixin, Expr)

    MixinExprAst()
        : ExprAst(Kind::MixinExpr)
    {}

    NAMED_LOC_PARAM(Key, key)
    NAMED_LOC_PARAM(LDelim, lDelim)
    NAMED_AST_PARAM(Expr, expr, ExprAst)
    NAMED_LOC_PARAM(RDelim, rDelim)

    SourceLoc keyLoc_;
    SourceLoc lDelimLoc_;
    std::unique_ptr<ExprAst> expr_;
    SourceLoc rDelimLoc_;
};

class UAISO_API ErrorExprAst final : public ExprAst
{
public:
    AST_CLASS(Error, Expr)

    ErrorExprAst()
        : ExprAst(Kind::ErrorExpr)
    {}

    NAMED_LOC_PARAM(Error, error)

    SourceLoc errorLoc_;
};

class UAISO_API PrintExprAst final : public ExprAst
{
public:
    AST_CLASS(Print, Expr)

    PrintExprAst()
        : ExprAst(Kind::PrintExpr)
    {}

    NAMED_LOC_PARAM(Key, key)
    NAMED_LOC_PARAM(Opr, opr)
    NAMED_AST_LIST_PARAM(Expr, exprs, ExprAst)

    SourceLoc keyLoc_;
    SourceLoc oprLoc_;
    std::unique_ptr<ExprAstList> exprs_;
};

class UAISO_API YieldExprAst final : public ExprAst
{
public:
    AST_CLASS(Yield, Expr)

    YieldExprAst()
        : ExprAst(Kind::YieldExpr)
    {}

    NAMED_LOC_PARAM(Key, key)
    NAMED_AST_LIST_PARAM(Expr, exprs, ExprAst)

    SourceLoc keyLoc_;
    std::unique_ptr<ExprAstList> exprs_;
};

/*!
 * \brief The ListCompreExprAst class
 *
 * A list comprehension.
 */
class UAISO_API ListCompreExprAst final : public ExprAst
{
public:
    AST_CLASS(ListCompre, Expr)

    ListCompreExprAst()
        : ExprAst(Kind::ListCompreExpr)
    {}

    NAMED_LOC_PARAM(LDelim, lDelim)
    NAMED_LOC_PARAM(RDelim, rDelim)
    NAMED_LOC_PARAM(Sep, sep)
    NAMED_AST_PARAM(Expr, expr, ExprAst)
    NAMED_AST_LIST_PARAM(Gen, gens, GeneratorAst)

    SourceLoc lDelimLoc_;
    SourceLoc rDelimLoc_;
    SourceLoc sepLoc_;
    std::unique_ptr<ExprAst> expr_;
    std::unique_ptr<GeneratorAstList> gens_;
};

} // namespace uaiso

#endif
