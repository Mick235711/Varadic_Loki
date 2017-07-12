//
// Created by lee on 17/6/23.
//

#include "Functor.h"
#include "FunctorHandler.h"
#include "MemFnHandler.h"
#include <utility>
#include <type_traits>

template<typename R, typename... P>
Functor<R, P...>::Functor(std::shared_ptr<Impl>& spImpl)
: m_spImpl(spImpl)
{}

template<typename R, typename... P>
R Functor<R, P...>::operator()(P... args)
{
    return (*m_spImpl)(std::forward<P>(args)...);
}

template<typename R, typename... P>
template<typename Func>
Functor<R, P...>::Functor(const Func& func)
: m_spImpl(new FunctorHandler<Func, R, P...>(func))
{}

template<typename R, typename... P>
template<typename PTO, typename PTMF>
Functor<R, P...>::Functor(const PTO& pObj, PTMF pMemFn)
    : m_spImpl(new MemFnHandler<PTO, PTMF, R, P...>(pObj, pMemFn))
{}
