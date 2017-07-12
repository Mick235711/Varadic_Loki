//
// Created by lee on 17/7/4.
//

#ifndef FUNCTOR_FUNCTORCHAINER_H
#define FUNCTOR_FUNCTORCHAINER_H

#include "FunctorImpl.h"

template<typename Func1, typename R2, typename... P2>
class FunctorChainer : public FunctorImpl<R2, P2...>
{
public:
    // typedefs
    typedef FunctorChainer<Func1, R2, P2...> class_type;
    typedef FunctorImpl<R2, P2...>           parent_type;
    typedef typename std::decay<Func1>::type func1_type;
    typedef Functor<R2, P2...>               func2_type;
    typedef R2                               result_type;
    typedef TL::TypeList<P2...>              param_list_type;
    template<int ind> using                  param = typename TL::TypeAtNonStrict<param_list_type, ind, TL::EmptyType>::type;

private:
    // members
    func1_type m_fun1;
    func2_type m_fun2;

public:
    // functions
    FunctorChainer() = default;
    FunctorChainer(func1_type, const func2_type&);
    FunctorChainer* Clone() const;
    R2 operator()(P2...);
};

template<typename Func1, typename R, typename... P>
FunctorChainer<Func1, R, P...> Chain(Func1 func1, const Functor<R, P...>& func2)
{
    return FunctorChainer<Func1, R, P...>(func1, func2);
};


#endif //FUNCTOR_FUNCTORCHAINER_H
