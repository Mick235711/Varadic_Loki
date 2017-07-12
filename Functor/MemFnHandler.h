//
// Created by lee on 17/6/23.
//

#ifndef FUNCTOR_MEMFNHANDLER_H
#define FUNCTOR_MEMFNHANDLER_H

#include "FunctorImpl.h"
#include "../TypeList/TypeList.h"

template<typename PTO, typename PTMF, typename R, typename... P>
class MemFnHandler : public FunctorImpl<R, P...>
{
public:
    // typedefs
    typedef PTO                              pointer_to_obj;
    typedef PTMF                             pointer_to_mem_func;
    typedef R                                result_type;
    typedef TL::TypeList<P...>               param_list_type;
    template<int ind> using                  param = typename TL::TypeAtNonStrict<param_list_type, ind, TL::EmptyType>::type;
    typedef MemFnHandler<PTO, PTMF, R, P...> class_type;
    
public:
    // functions
    MemFnHandler() = default;
    MemFnHandler(const PTO&, PTMF);
    MemFnHandler* Clone() const;
    R operator()(P...);

private:
    PTO m_pObj;
    PTMF m_pMemFn;
};


#endif //FUNCTOR_MEMFNHANDLER_H
