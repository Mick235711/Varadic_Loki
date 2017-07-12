//
// Created by lee on 17/7/5.
//

#ifndef VISITOR_VISITOR_H
#define VISITOR_VISITOR_H

#include "TypeList/TypeList.h"

// BaseVisitor
// The base class of any Acyclic Visitor
class BaseVisitor
{
public:
    virtual ~BaseVisitor() = default;
};

// Visitor
// The building block of Acyclic Visitor
template<typename T, typename R = void>
class Visitor
{
public:
    typedef R ReturnType;
    virtual ReturnType Visit(T&) = 0;
};

// Visitor (specializations)
// make it easier to define concrete Visitor
template<typename Head, typename R>
class Visitor<TL::TypeList<Head>, R>
    : public Visitor<Head, R>
{
public:
    typedef R ReturnType;
};
template<typename Head, typename... Tail, typename R>
class Visitor<TL::TypeList<Head, Tail...>, R>
    : public Visitor<Head, R>,
      public Visitor<TL::TypeList<Tail...>, R>
{
public:
    typedef R ReturnType;
};

// BaseVisitorImpl
// Implements non-strict visitation (you can implement only part of Visitor)
template<typename TList, typename R = void> class BaseVisitorImpl;
template<typename Head, typename R>
class BaseVisitorImpl<TL::TypeList<Head>, R>
    : public Visitor<Head, R>
{
public:
    typedef R ReturnType;
    virtual ReturnType Visit(Head&) {return R();}
};
template<typename Head, typename... Tail, typename R>
class BaseVisitorImpl<TL::TypeList<Head, Tail...>, R>
    : public Visitor<Head, R>,
      public BaseVisitorImpl<TL::TypeList<Tail...>, R>
{
public:
    typedef R ReturnType;
    virtual ReturnType Visit(Head&) {return R();}
};

#endif //VISITOR_VISITOR_H
