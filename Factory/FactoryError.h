//
// Created by lee on 17/7/5.
//

#ifndef FACTORY_FACTORYERROR_H
#define FACTORY_FACTORYERROR_H

#include <exception>

// DefaultFactoryError
// Default error handler for Factory
template<typename IdentifierType, typename AbstractProduct>
struct DefaultFactoryError
{
    struct Exception : public std::exception
    {
        const char* what() const noexcept
        {return "Unknown Type";}
    };
    
    static AbstractProduct* OnUnknownType(IdentifierType)
    {throw Exception();}
};

#endif //FACTORY_FACTORYERROR_H
