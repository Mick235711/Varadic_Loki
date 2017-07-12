#include <iostream>
#include "Dispatcher.h"
#include <vector>
#include <random>
#include <ctime>

struct Shape
{
    virtual void draw() = 0;
};

struct Rectangle : public Shape
{
    void draw() {std::cout << "Rectangle" << std::endl;}
};

struct Circle : public Shape
{
    void draw() {std::cout << "Circle" << std::endl;}
};

struct Ellipse : public Shape
{
    void draw() {std::cout << "Ellipse" << std::endl;}
};

struct RoundedRectangle : public Rectangle
{
    void draw() override {std::cout << "RoundedRectangle" << std::endl;}
};

std::vector<Shape*> shapes;

struct Drawer
{
    std::string Fire(Rectangle&, Rectangle&) {return "Rectangle-Rectangle";}
    std::string Fire(Rectangle&, Circle&) {return "Rectangle-Circle";}
    std::string Fire(Rectangle&, Ellipse&) {return "Rectangle-Ellipse";}
    std::string Fire(Circle&, Rectangle&) {return "Circle-Rectangle";}
    std::string Fire(Circle&, Circle&) {return "Circle-Circle";}
    std::string Fire(Circle&, Ellipse&) {return "Circle-Ellipse";}
    std::string Fire(Ellipse&, Rectangle&) {return "Ellipse-Rectangle";}
    std::string Fire(Ellipse&, Circle&) {return "Ellipse-Circle";}
    std::string Fire(Ellipse&, Ellipse&) {return "Ellipse-Ellipse";}
    
    std::string OnError(Shape& s1, Shape& s2) {s1.draw(); s2.draw(); return "Error";}
};

std::string FireRR(Rectangle&, Rectangle&) {return "Rectangle-Rectangle";}
std::string FireRC(Rectangle&, Circle&) {return "Rectangle-Circle";}
std::string FireRE(Rectangle&, Ellipse&) {return "Rectangle-Ellipse";}
std::string FireCR(Circle&, Rectangle&) {return "Circle-Rectangle";}
std::string FireCC(Circle&, Circle&) {return "Circle-Circle";}
std::string FireCE(Circle&, Ellipse&) {return "Circle-Ellipse";}
std::string FireER(Ellipse&, Rectangle&) {return "Ellipse-Rectangle";}
std::string FireEC(Ellipse&, Circle&) {return "Ellipse-Circle";}
std::string FireEE(Ellipse&, Ellipse&) {return "Ellipse-Ellipse";}

struct FnFireRR {std::string operator()(Rectangle&, Rectangle&) {return "Rectangle-Rectangle";}};
struct FnFireRC {std::string operator()(Rectangle&, Circle&) {return "Rectangle-Circle";}};
struct FnFireRE {std::string operator()(Rectangle&, Ellipse&) {return "Rectangle-Ellipse";}};
struct FnFireCR {std::string operator()(Circle&, Rectangle&) {return "Circle-Rectangle";}};
struct FnFireCC {std::string operator()(Circle&, Circle&) {return "Circle-Circle";}};
struct FnFireCE {std::string operator()(Circle&, Ellipse&) {return "Circle-Ellipse";}};
struct FnFireER {std::string operator()(Ellipse&, Rectangle&) {return "Ellipse-Rectangle";}};
struct FnFireEC {std::string operator()(Ellipse&, Circle&) {return "Ellipse-Circle";}};
struct FnFireEE {std::string operator()(Ellipse&, Ellipse&) {return "Ellipse-Ellipse";}};

typedef StaticDispatcher<Drawer,
    Shape, TL::TypeList<Rectangle, Circle, Ellipse>,
    Shape, TL::TypeList<Rectangle, Circle, Ellipse>,
    std::string, false> SDispatch;

typedef FnDispatcher<Shape, Shape, std::string> FDispatch;
typedef FunctorDispatcher<Shape, Shape, std::string> FunDispatch;

int main()
{
    Drawer drawer;
    Shape* randomArray[] = {new Rectangle, new Circle, new Ellipse, new RoundedRectangle};
    std::default_random_engine e;
    e.seed((unsigned)time(nullptr));
    std::uniform_int_distribution<unsigned int> u(0, 3);
    int i;
    for (i = 1; i <= 100; i++)
        shapes.push_back(randomArray[u(e)]);
    std::uniform_int_distribution<unsigned int> u2(0, 99);
    for (i = 1; i <= 5; i++)
    {
        Shape* s1 = shapes[u(e)];
        Shape* s2 = shapes[u(e)];
        s1->draw(); s2->draw();
        std::cout << SDispatch::Go(*s1, *s2, drawer) << std::endl;
    }
    std::cout << std::endl;
    FDispatch fd;
    fd.Add<Rectangle, Ellipse, FireRE>();
    fd.Add<Ellipse, Circle, FireEC, false>();
    fd.Add<Circle, Rectangle, FireCR>();
    for (i = 1; i <= 5; i++)
    {
        try
        {
            Shape* s1 = shapes[u(e)];
            Shape* s2 = shapes[u(e)];
            s1->draw(); s2->draw();
            std::cout << fd.Go(*s1, *s2) << std::endl;
        }
        catch (std::runtime_error e)
        {
            std::cout << e.what() << std::endl;
        }
    }
    std::cout << std::endl;
    FnFireRC FFRC;
    FnFireCE FFCE;
    FnFireER FFER;
    FunDispatch fund;
    fund.Add<Rectangle, Circle>(FFRC);
    fund.Add<Circle, Ellipse, false>(FFCE);
    fund.Add<Ellipse, Rectangle>(FFER);
    for (i = 1; i <= 5; i++)
    {
        try
        {
            Shape* s1 = shapes[u(e)];
            Shape* s2 = shapes[u(e)];
            s1->draw(); s2->draw();
            std::cout << fd.Go(*s1, *s2) << std::endl;
        }
        catch (std::runtime_error e)
        {
            std::cout << e.what() << std::endl;
        }
    }
    return 0;
}
