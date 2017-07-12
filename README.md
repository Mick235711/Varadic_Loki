# **Variadic_Loki**
### *Loki Library rewrited in C++0x*

***

In the book `<<Modern C++ Design>>`, Andrew Alexandrew described a lot of useful utilities

**But**, it's already 2017, not 2003!

So, I rewrited it in newer standard --- `C++0x`!

## Sub-Components
#### Abstract Factory
#### Associative Vector
#### Factory
#### Multimethods (Double-Dispatch)
#### Singleton Holder
#### Small Object Allocator
#### Smart Pointer
#### Threading Models
#### TypeInfo
#### TypeList
#### Visitor

-------
## Install
    git clone https://github.com/Mick235711/Variadic_Loki.git
    cd Variadic_Loki
    cmake .
    make

***
## Minor Change
* Remove namespace Loki
* TypeList can be directly used (thanks variadic template)
* Functor can be directly used

## Usage (Functor)
    #include <iostream>
    #include "Functor/Functor.cpp"
    #include "Functor/FunctorHandler.cpp"
    #include "Functor/MemFnHandler.cpp"
    #include "Functor/BinderFirst.cpp"
    #include "Functor/BinderSecond.cpp"
    #include "Functor/FunctorChainer.cpp"
    
    using namespace std;
    
    // test functor
    struct TestFunctor
    {
        char operator()(int i, double d)
        {
            cout << "TestFunctor::operator()(" << i << ", " << d << ") called." << endl;
            return i == 3 ? 'a' : 'b';
        }
    };
    
    // test function
    long TestFunction(long i, float d)
    {
        cout << "TestFunction(" << i << ", " << d << ") called." << endl;
        return i == 3 ? 'a' : 'b';
    }
    
    // test member function
    struct TestMemFn
    {
        short func(unsigned int i, int d)
        {
            cout << "TestMemFn::func(" << i << ", " << d << ") called." << endl;
            return i == 3 ? 'a' : 'b';
        }
    };
    
    // test chainer
    void TestChain()
    {
        cout << "TestChain() called." << endl;
    }
    
    int main()
    {
        TestFunctor f;
        TestMemFn mf;
        Functor<int, long long, long double> cmd(f), cmd2(TestFunction), cmd3(&mf, &TestMemFn::func);
        cout << cmd(3, 3.5) << endl << cmd2(3, 3.5) << endl << cmd3(3, 3.5) << endl;
        cout << cmd(4, 4.5) << endl << cmd2(4, 4.5) << endl << cmd3(4, 4.5) << endl;
        Functor<int, long double> bound = BindFirst(cmd, 3), bound2 = BindSecond(cmd2, 4.5);
        cout << bound(3.5) << endl << bound2(4) << endl;
        auto chained = Chain(TestChain, bound2);
        cout << chained(4) << endl;
        return 0;
    }



#### Output
    TestFunctor::operator()(3, 3.5) called.
    97
    TestFunction(3, 3.5) called.
    97
    TestMemFn::func(3, 3) called.
    97
    TestFunctor::operator()(4, 4.5) called.
    98
    TestFunction(4, 4.5) called.
    98
    TestMemFn::func(4, 4) called.
    98
    TestFunctor::operator()(3, 3.5) called.
    97
    TestFunction(4, 4.5) called.
    98
    TestChain() called.
    TestFunction(4, 4.5) called.
    98
