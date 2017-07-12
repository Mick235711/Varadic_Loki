//
// Created by lee on 17/6/23.
//

#include "MemFnHandler.h"
#include <utility>

template<typename PTO, typename PTMF, typename R, typename... P>
MemFnHandler<PTO, PTMF, R, P...>::MemFnHandler(const PTO& pObj, PTMF pMemFn)
: m_pObj(pObj), m_pMemFn(pMemFn)
{}

template<typename PTO, typename PTMF, typename R, typename... P>
MemFnHandler<PTO, PTMF, R, P...>* MemFnHandler<PTO, PTMF, R, P...>::Clone() const
{
    return new MemFnHandler(*this);
}

template<typename PTO, typename PTMF, typename R, typename... P>
R MemFnHandler<PTO, PTMF, R, P...>::operator()(P... args)
{
    return ((*m_pObj).*m_pMemFn)(std::forward<P>(args)...);
}
