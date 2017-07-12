#include <iostream>
#include "AbstractFactory/AbstractFactory.h"

struct Enemies
{
    virtual void say() const = 0;
    virtual Enemies* Clone() const = 0;
};
struct Soldiers
{
    virtual int health() const = 0;
    virtual Soldiers* Clone() const = 0;
};
struct Monsters
{
    virtual void attack(int) = 0;
    virtual Monsters* Clone() const = 0;
};

// Easy:
struct EasyEnemies : public Enemies
{
    void say() const {std::cout << "Easy!" << std::endl;}
    EasyEnemies* Clone() const {return new EasyEnemies;}
};
struct EasySoldiers : public Soldiers
{
    int health() const {return 20;}
    EasySoldiers* Clone() const {return new EasySoldiers;}
};
struct EasyMonsters : public Monsters
{
    void attack(int point) {std::cout << "I'm hurt! I decrease " << point << " HPs!" << std::endl;}
    EasyMonsters* Clone() const {return new EasyMonsters;}
};

// Hard:
struct HardEnemies : public Enemies
{
    void say() const {std::cout << "HoHo! I'm hard to get!" << std::endl;}
    HardEnemies* Clone() const {return new HardEnemies;}
};
struct HardSoldiers : public Soldiers
{
    int health() const {return 100;}
    HardSoldiers* Clone() const {return new HardSoldiers;}
};
struct HardMonsters : public Monsters
{
    void attack(int point) {std::cout << "You dare hurt me! I just decrease " << point << " HPs!" << std::endl;}
    HardMonsters* Clone() const {return new HardMonsters;}
};

class Game
{
    typedef AbstractFactory<TL::TypeList<Enemies, Soldiers, Monsters>> AbstractLevelFactory;
    typedef ConcreteFactory<AbstractLevelFactory, OpNewFactoryUnit,
        TL::TypeList<EasyEnemies, EasySoldiers, EasyMonsters>> EasyLevelFactory;
    typedef ConcreteFactory<AbstractLevelFactory, PrototypeFactoryUnit> HardLevelFactory;
    
    AbstractLevelFactory* m_pALF;

public:
    void chooseLevel()
    {
        char ch;
        std::cout << "Which Level? (Easy/Hard) (Press E or H) ";
        std::cin >> ch;
        ch = (char)toupper(ch);
        if (ch == 'E')
        {
            m_pALF = new EasyLevelFactory;
        }
        else
        {
            HardLevelFactory* hard = new HardLevelFactory;
            hard->SetPrototype(new HardEnemies);
            hard->SetPrototype(new HardSoldiers);
            hard->SetPrototype(new HardMonsters);
            m_pALF = hard;
        }
    }
    Enemies * CreateEnemies () {return m_pALF->Create<Enemies >();}
    Soldiers* CreateSoldiers() {return m_pALF->Create<Soldiers>();}
    Monsters* CreateMonsters() {return m_pALF->Create<Monsters>();}
};

int main()
{
    Game g;
    g.chooseLevel();
    g.CreateEnemies()->say();
    std::cout << "Solders' health: " << g.CreateSoldiers()->health() << std::endl;
    g.CreateMonsters()->attack(10);
    return 0;
}