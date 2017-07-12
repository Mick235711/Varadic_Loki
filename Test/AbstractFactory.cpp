//
// Created by lee on 17/7/12.
//

#define BOOST_TEST_MODULE TestAbstractFactory
#include <boost/test/included/unit_test.hpp>
#include "AbstractFactory/AbstractFactory.h"
#include <cstdlib>

BOOST_AUTO_TEST_SUITE(AbstractFactoryTest)

BOOST_AUTO_TEST_CASE(case1)
    {
        struct Enemies
        {
            virtual std::string say() const = 0;
            
            virtual Enemies *Clone() const = 0;
        };
        struct Soldiers
        {
            virtual int health() const = 0;
            
            virtual Soldiers *Clone() const = 0;
        };
        struct Monsters
        {
            virtual std::string attack(int) = 0;
            
            virtual Monsters *Clone() const = 0;
        };
        
        // Easy:
        struct EasyEnemies : public Enemies
        {
            std::string say() const
            { return "Easy!\n"; }
            
            EasyEnemies *Clone() const
            { return new EasyEnemies; }
        };
        struct EasySoldiers : public Soldiers
        {
            int health() const
            { return 20; }
            
            EasySoldiers *Clone() const
            { return new EasySoldiers; }
        };
        struct EasyMonsters : public Monsters
        {
            std::string attack(int point)
            { return std::string("") + "I'm hurt! I decrease " + char(point + '0') + " HPs!\n"; }
            
            EasyMonsters *Clone() const
            { return new EasyMonsters; }
        };
        
        // Hard:
        struct HardEnemies : public Enemies
        {
            std::string say() const
            { return "HoHo! I'm hard to get!\n"; }
            
            HardEnemies *Clone() const
            { return new HardEnemies; }
        };
        struct HardSoldiers : public Soldiers
        {
            int health() const
            { return 100; }
            
            HardSoldiers *Clone() const
            { return new HardSoldiers; }
        };
        struct HardMonsters : public Monsters
        {
            std::string attack(int point)
            { return std::string("") + "You dare hurt me! I just decrease " + char(point + '0') + " HPs!\n"; }
            
            HardMonsters *Clone() const
            { return new HardMonsters; }
        };
        
        class Game
        {
            typedef AbstractFactory<TL::TypeList<Enemies, Soldiers, Monsters>> AbstractLevelFactory;
            typedef ConcreteFactory<AbstractLevelFactory, OpNewFactoryUnit,
                TL::TypeList<EasyEnemies, EasySoldiers, EasyMonsters>> EasyLevelFactory;
            typedef ConcreteFactory<AbstractLevelFactory, PrototypeFactoryUnit> HardLevelFactory;
            
            AbstractLevelFactory *m_pALF;
        
        public:
            char chooseLevel()
            {
                char chars[] = {'E', 'H'};
                char ch = chars[rand() % 2];
                ch = (char) toupper(ch);
                if (ch == 'E')
                {
                    m_pALF = new EasyLevelFactory;
                } else
                {
                    HardLevelFactory *hard = new HardLevelFactory;
                    hard->SetPrototype(new HardEnemies);
                    hard->SetPrototype(new HardSoldiers);
                    hard->SetPrototype(new HardMonsters);
                    m_pALF = hard;
                }
                return ch;
            }
            
            Enemies *CreateEnemies()
            { return m_pALF->Create<Enemies>(); }
            
            Soldiers *CreateSoldiers()
            { return m_pALF->Create<Soldiers>(); }
            
            Monsters *CreateMonsters()
            { return m_pALF->Create<Monsters>(); }
        };
        Game g;
        char ch = g.chooseLevel();
        if (ch == 'E')
        {
            BOOST_REQUIRE_EQUAL(g.CreateEnemies()->say(), "Easy!\n");
            BOOST_REQUIRE_EQUAL(g.CreateSoldiers()->health(), 20);
            BOOST_REQUIRE_EQUAL(g.CreateMonsters()->attack(1), "I'm hurt! I decrease 1 HPs!\n");
        }
        else
        {
            BOOST_REQUIRE_EQUAL(g.CreateEnemies()->say(), "HoHo! I'm hard to get!\n");
            BOOST_REQUIRE_EQUAL(g.CreateSoldiers()->health(), 100);
            BOOST_REQUIRE_EQUAL(g.CreateMonsters()->attack(1), "You dare hurt me! I just decrease 1 HPs!\n");
        }
    }

BOOST_AUTO_TEST_SUITE_END()
