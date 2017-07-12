//
// Created by lee on 17/6/23.
//

#include "FunctorHandler.h"

template<typename Func, typename R, typename... P>
FunctorHandler<Func, R, P...>::FunctorHandler(const func_type& func)
: m_func(func)
{}

template<typename Func, typename R, typename... P>
FunctorHandler<Func, R, P...>* FunctorHandler<Func, R, P...>::Clone() const
{
    return new FunctorHandler(*this);
}

template<typename Func, typename R, typename... P>
R FunctorHandler<Func, R, P...>::operator()(P... args)
{
    return m_func(std::forward<P>(args)...);
}
