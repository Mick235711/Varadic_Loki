//
// Created by lee on 17/7/5.
//

#ifndef FACTORY_FACTORY_H
#define FACTORY_FACTORY_H

#include "FactoryError.h"
#include "../AssocVector/AssocVector.h"

// Factory
// A generic object factory
template
    <
        typename AbstractProduct,
        typename IdentifierType,
        typename ProductCreator = AbstractProduct* (*)(),
        template<typename, typename>
            class FactoryErrorPolicy = DefaultFactoryError
    >
class Factory : public FactoryErrorPolicy<IdentifierType, AbstractProduct>
{
public:
    bool Register(const IdentifierType& id, ProductCreator creator)
    {
        return m_associations.insert(typename IdToProductMap::value_type(id, creator)).second;
    }
    
    bool Unregister(const IdentifierType& id)
    {
        return m_associations.erase(id) == 1;
    }
    
    AbstractProduct* CreateObject(const IdentifierType& id)
    {
        typename IdToProductMap::iterator i = m_associations.find(id);
        if (i != m_associations.end())
        {
            return (i->second)();
        }
        return this->OnUnknownType(id);
    }

private:
    typedef AssocVector<IdentifierType, ProductCreator> IdToProductMap;
    IdToProductMap m_associations;
};

#endif //FACTORY_FACTORY_H
