#include <iostream>
#include "Functor.cpp"
#include "FunctorHandler.cpp"
#include "MemFnHandler.cpp"
#include "BinderFirst.cpp"
#include "BinderSecond.cpp"
#include "FunctorChainer.cpp"

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
