#include <iostream>
#include "Visitor.h"
#include "Visitable.h"

class DocElement : public BaseVisitable<>
{
public:
    DEFINE_VISITABLE()
};

class Paragraph : public DocElement
{
public:
    DEFINE_VISITABLE()
    void sayPara() {std::cout << "Saying...." << std::endl;}
};

class RasterBitMap : public DocElement
{
public:
    DEFINE_VISITABLE()
    void display() {std::cout << "Displaying..." << std::endl;}
};

class DisplayVisitor
    : public BaseVisitor,
      public Visitor<TL::TypeList<Paragraph, RasterBitMap>>
{
public:
    explicit DisplayVisitor(int v = 0) :value(v) {}
    void Visit(Paragraph& p)
    {
        p.sayPara();
        value = 5;
    }
    void Visit(RasterBitMap& r)
    {
        r.display();
        value = 7;
    }
    int getValue() const {return value;}
private:
    int value;
};

int main()
{
    DocElement* pDE = new RasterBitMap;
    DisplayVisitor dv(3);
    BaseVisitor& bv = dv;
    pDE->Accept(bv);
    std::cout << dv.getValue() << std::endl;
    return 0;
}
