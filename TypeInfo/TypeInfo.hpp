//
// Created by lee on 17/7/5.
//

#include <cassert>
#include "TypeInfo.h"

inline TypeInfo::TypeInfo()
{
    class Nil {};
    m_pInfo = &typeid(Nil);
    assert(m_pInfo);
}

inline TypeInfo::TypeInfo(const std::type_info& ti)
: m_pInfo(&ti)
{assert(m_pInfo);}

inline bool TypeInfo::before(const TypeInfo& rhs) const noexcept
{
    assert(m_pInfo);
    return m_pInfo->before(*rhs.m_pInfo) != 0;
}

inline const std::type_info& TypeInfo::get() const noexcept
{
    assert(m_pInfo);
    return *m_pInfo;
}

inline const char* TypeInfo::name() const noexcept
{
    assert(m_pInfo);
    return m_pInfo->name();
}
