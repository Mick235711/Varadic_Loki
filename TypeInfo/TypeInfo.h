//
// Created by lee on 17/7/5.
//

#ifndef TYPEINFO_TYPEINFO_H
#define TYPEINFO_TYPEINFO_H

#include <typeinfo>

// TypeInfo
// a first-class, comparable wrapper over std::type_info
class TypeInfo
{
public:
    // Constructors
    TypeInfo(); // needed for containers
    TypeInfo(const std::type_info&); // non-explicit
    
    // Access for the wrapped std::type_info
    const std::type_info& get() const noexcept;
    // Compatibility functions
    bool before(const TypeInfo&) const noexcept;
    const char* name() const noexcept;

private:
    const std::type_info* m_pInfo;
};

// Comparison operators
inline bool operator==(const TypeInfo& lhs, const TypeInfo& rhs)
{return (lhs.get() == rhs.get()) != 0;}

inline bool operator<(const TypeInfo& lhs, const TypeInfo& rhs)
{return lhs.before(rhs);}

inline bool operator!=(const TypeInfo& lhs, const TypeInfo& rhs)
{return !(lhs == rhs);}

inline bool operator>(const TypeInfo& lhs, const TypeInfo& rhs)
{return rhs < lhs;}

inline bool operator<=(const TypeInfo& lhs, const TypeInfo& rhs)
{return !(rhs < lhs);}

inline bool operator>=(const TypeInfo& lhs, const TypeInfo& rhs)
{return !(lhs < rhs);}

#endif //TYPEINFO_TYPEINFO_H
