//
// Created by lee on 17/7/5.
//

#ifndef ASSOCVECTOR_ASSOCVECTOR_H
#define ASSOCVECTOR_ASSOCVECTOR_H

#include <utility>
#include <functional>
#include <vector>

// AssocVectorCompare
// Compare class for AssocVector
namespace detail
{
    template<typename Value, typename C>
    class AssocVectorCompare : public C
    {
        typedef std::pair<typename C::first_argument_type, Value> Data;
        typedef typename C::first_argument_type first_argument_type;

    public:
        AssocVectorCompare() = default;
        
        AssocVectorCompare(const C& src) : C(src) {}
        
        bool operator()(const first_argument_type& lhs, const first_argument_type& rhs) const
        {return C::operator()(lhs, rhs);}
        
        bool operator()(const Data& lhs, const Data& rhs) const
        {return C::operator()(lhs.first, rhs.first);}
        
        bool operator()(const Data& lhs, const first_argument_type& rhs) const
        {return C::operator()(lhs.first, rhs);}
        
        bool operator()(const first_argument_type& lhs, const Data& rhs) const
        {return C::operator()(lhs, rhs.first);}
    };
} // namespace detail

// AssocVector
// An associative vector build as a syntastic drop-in replacement for std::map
// Changed guarantees:
// * iterators are invalidated by insert and erase
// * the complexity of insert/erase is O(N), not O(log N)
// * the complexity of find is O(1), not O(log N)
// * value_type is std::pair<key_type, mapped_type>, not std::pair<const key_type, mapped_type>
// * iterators are random_iterators
template
    <
        typename pkey_type,
        typename pmapped_type,
        typename pkey_compare = std::less<pkey_type>,
        typename pallocator_type = std::allocator<std::pair<pkey_type, pmapped_type>>
    >
class AssocVector
    : private std::vector<std::pair<pkey_type, pmapped_type>, pallocator_type>,
      private detail::AssocVectorCompare<pmapped_type, pkey_compare>
{
public:
    typedef pkey_type key_type;
    typedef pmapped_type mapped_type;
    typedef pkey_compare key_compare;
    typedef pallocator_type allocator_type;

private:
    typedef key_type K;
    typedef mapped_type V;
    typedef key_compare C;
    typedef allocator_type A;
    typedef std::vector<std::pair<K, V>, A> BaseType;
    typedef detail::AssocVectorCompare<V, C> Compare;

public:
    typedef typename BaseType::value_type value_type;
    typedef typename A::reference reference;
    typedef typename A::const_reference const_reference;
    typedef typename BaseType::iterator iterator;
    typedef typename BaseType::const_iterator const_iterator;
    typedef typename BaseType::size_type size_type;
    typedef typename BaseType::difference_type difference_type;
    typedef typename A::pointer pointer;
    typedef typename A::const_pointer const_pointer;
    typedef typename BaseType::reverse_iterator reverse_iterator;
    typedef typename BaseType::const_reverse_iterator const_reverse_iterator;
    
    class value_compare
        : public std::binary_function<value_type, value_type, bool>,
          private C
    {
        friend class AssocVector;

    protected:
        value_compare(C pred) : C(pred) {}

    public:
        bool operator()(const value_type& lhs, const value_type& rhs) const
        {return C::operator()(lhs.first, rhs.first);}
    };
    
    // construct/copy/destroy
    
    explicit AssocVector(const C& comp = C(), const A& alloc = A())
    : BaseType(alloc), Compare(comp)
    {}
    
    template<typename InputIter>
    AssocVector(InputIter first, InputIter last, const C& comp = C(), const A& alloc = A())
    : BaseType(first, last, alloc), Compare(comp)
    {
        Compare& me = *this;
        std::sort(begin(), end(), me);
    }
    
    AssocVector(const std::initializer_list<value_type>& il, const C& comp = C(), const A& alloc = A())
    : BaseType(il, alloc), Compare(comp)
    {}
    
    AssocVector& operator=(const AssocVector& rhs)
    {
        AssocVector(rhs).swap(*this);
        return *this;
    }
    
    // iterators:
    iterator begin() noexcept {return BaseType::begin();}
    const_iterator begin() const noexcept {return BaseType::begin();}
    iterator end() noexcept {return BaseType::end();}
    const_iterator end() const noexcept {return BaseType::end();}
    reverse_iterator rbegin() noexcept {return BaseType::rbegin();}
    const_reverse_iterator rbegin() const noexcept {return BaseType::rbegin();}
    reverse_iterator rend() noexcept {return BaseType::rend();}
    const_reverse_iterator rend() const noexcept {return BaseType::rend();}
    
    // capacity:
    bool empty() const noexcept {return BaseType::empty();}
    size_type size() const noexcept {return BaseType::size();}
    size_type max_size() const noexcept {return BaseType::max_size();}
    
    // element access:
    V& operator[](const K& key)
    {return insert(value_type(key, V())).first->second;}
    
    // modifiers:
    std::pair<iterator, bool> insert(const value_type& val)
    {
        bool found{true};
        iterator i{lower_bound(val.first)};
        
        if (i == end() || this->operator()(val.first, i->first))
        {
            i = BaseType::insert(i, val);
            found = false;
        }
        return std::make_pair(i, !found);
    }
    
    iterator insert(iterator pos, const value_type& val)
    {
        if (pos != end() && this->operator()(*pos, val) &&
            (pos == end() - 1 ||
                (!this->operator()(val, pos[1]) &&
                this->operator()(pos[1], val))))
        {
            return BaseType::insert(pos, val);
        }
        return insert(val).first;
    }
    
    template<typename InputIter>
    void insert(InputIter first, InputIter last)
    {for (; first != last; ++first) insert(*first);}
    
    void erase(iterator pos)
    {BaseType::erase(pos);}
    
    size_type erase(const K& k)
    {
        iterator i{find(k)};
        if (i == end()) return 0;
        erase(i);
        return 1;
    }
    
    void erase(iterator first, iterator last)
    {BaseType::erase(first, last);}
    
    void swap(AssocVector& other)
    {
        using std::swap;
        BaseType::swap(other);
        Compare& me = *this;
        Compare& rhs = other;
        swap(me, rhs);
    }
    
    void clear() {BaseType::clear();}
    
    // observers:
    C key_comp() const {return *this;}
    
    value_compare value_comp() const
    {
        const C& comp = *this;
        return value_compare(comp);
    }
    
    iterator find(const K& k)
    {
        iterator i{lower_bound(k)};
        if (i != end() && this->operator()(k, i->first))
        {
            i = end();
        }
        return i;
    }
    const_iterator find(const K& k) const
    {
        const_iterator i{lower_bound(k)};
        if (i != end() && this->operator()(k, i->first))
        {
            i = end();
        }
        return i;
    }
    
    size_type count(const K& k) const
    {return find(k) != end();}
    
    iterator lower_bound(const K& k)
    {
        Compare& me = *this;
        return std::lower_bound(begin(), end(), k, me);
    }
    
    const_iterator lower_bound(const K& k) const
    {
        Compare& me = const_cast<AssocVector&>(*this);
        return std::lower_bound(begin(), end(), k, me);
    }
    
    iterator upper_bound(const K& k)
    {
        Compare& me = const_cast<AssocVector&>(*this);
        return std::upper_bound(begin(), end(), k, me);
    }
    
    const_iterator upper_bound(const K& k) const
    {
        Compare& me = const_cast<AssocVector&>(*this);
        return std::upper_bound(begin(), end(), k, me);
    }
    
    std::pair<iterator, iterator> equal_range(const K& k)
    {
        Compare& me = const_cast<AssocVector&>(*this);
        return std::equal_range(begin(), end(), k, me);
    }
    
    std::pair<const_iterator, const_iterator> equal_range(const K& k) const
    {
        Compare& me = const_cast<AssocVector&>(*this);
        return std::equal_range(begin(), end(), k, me);
    }
    
    friend bool operator==(const AssocVector& lhs, const AssocVector& rhs)
    {
        const BaseType& me = lhs;
        return me == rhs;
    }
    
    bool operator<(const AssocVector& rhs) const
    {
        const BaseType& me = *this;
        const BaseType& you = rhs;
        return me < you;
    }
    
    friend bool operator!=(const AssocVector& lhs, const AssocVector& rhs)
    {return !(lhs == rhs);}
    
    friend bool operator>(const AssocVector& lhs, const AssocVector& rhs)
    {return rhs < lhs;}
    
    friend bool operator>=(const AssocVector& lhs, const AssocVector& rhs)
    {return !(lhs < rhs);}
    
    friend bool operator<=(const AssocVector& lhs, const AssocVector& rhs)
    {return !(rhs < lhs);}
};

// specialized algorithms:
template<typename K, typename V, typename C, typename A>
void swap(AssocVector<K, V, C, A>& lhs, AssocVector<K, V, C, A>& rhs)
{lhs.swap(rhs);}

#endif //ASSOCVECTOR_ASSOCVECTOR_H
