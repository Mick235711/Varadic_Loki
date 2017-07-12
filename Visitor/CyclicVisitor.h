//
// Created by lee on 17/7/5.
//

#ifndef VISITOR_CYCLICVISITOR_H
#define VISITOR_CYCLICVISITOR_H

#include "Visitor.h"

// CyclicVisitor
// Implementation of GoF Cyclic Visitor
template<typename R, typename TList>
class CyclicVisitor : public Visitor<TList, R>
{
public:
    typedef R ReturnType;
    
    template<typename Visited>
    ReturnType GenericVisit(Visited& host)
    {
        Visitor<Visited, ReturnType>& subObj = *this;
        return subObj.Visit(host);
    }
};

// DEFINE_CYCLIC_VISITABLE
// Put it in every concrete CyclicVisitable's public section
#define DEFINE_CYCLIC_VISITABLE(SomeVisitor) \
    virtual SomeVisitor::ReturnType Accept(SomeVisitor& guest) \
    {return guest.GenericVisit(*this);}

#endif //VISITOR_CYCLICVISITOR_H
