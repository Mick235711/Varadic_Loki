//
// Created by lee on 17/6/23.
//

#ifndef FUNCTOR_FUNCTOR_H
#define FUNCTOR_FUNCTOR_H

#include <memory>
#include "FunctorImpl.h"
#include "../TypeList/TypeList.h"

template<typename R, typename... P> class FunctorImpl;

template<typename R = void, typename... P>
class Functor
{
public:
    // typedefs
    typedef FunctorImpl<R, P...> Impl;

private:
    // members
    std::shared_ptr<Impl> m_spImpl;
    
public:
    // typedefs
    typedef R                                    result_type;
    typedef Functor<R, P...>                     class_type;
    typedef typename TL::TypeList<R, P...>::Tail param_list_type;
    template<int ind> using                      param = typename TL::TypeAtNonStrict<param_list_type, ind, TL::EmptyType>::type;
    
public:
    // functions
    Functor() = default;
    Functor(const Functor&) = default;
    Functor& operator=(const Functor&) = default;
    Functor(Functor&&) = default;
    Functor& operator=(Functor&&) = default;
    explicit Functor(std::shared_ptr<Impl>&);
    template<typename Func> Functor(const Func&);
    template<typename PTO, typename PTMF> Functor(const PTO&, PTMF);
    R operator()(P...);
};

#endif //FUNCTOR_FUNCTOR_H
