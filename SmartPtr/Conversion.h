//
// Created by lee on 17/7/4.
//

#ifndef SMARTPTR_CONVERSION_H
#define SMARTPTR_CONVERSION_H

// AllowConversion
// Allows implicit conversion from SmartPtr to the pointee type
struct AllowConversion
{
    enum {allow = true};
    enum {explicit_conv = false};
    
    void swap(AllowConversion&) {}
};

// ExplicitConversion
// Allows explicit conversion from SmartPtr to the pointee type
struct ExplicitConversion
{
    enum {allow = false};
    enum {explicit_conv = true};
    
    void swap(ExplicitConversion&) {}
};

// DisallowConversion
// Disallow conversion from SmartPtr to the pointee type
struct DisallowConversion
{
    DisallowConversion() = default;
    
    DisallowConversion(const AllowConversion&) {}
    
    enum {allow = false};
    enum {explicit_conv = false};
    
    void swap(DisallowConversion&) {}
};

#endif //SMARTPTR_CONVERSION_H
