#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"

// Students:  Add code to this file, Actor.cpp, StudentWorld.h, and StudentWorld.cpp
class StudentWorld;
class GameWorld;

class ActorInterface : public GraphObject {
public:
   ActorInterface();
   ActorInterface(int ID, int col, int row, enum Direction dir, double size, int depth, StudentWorld* sworld) : 
      GraphObject(ID, col, row, dir, size, depth) {
      a_row = row;
      a_col = col;
      sw = sworld;
   };

   virtual ~ActorInterface();                   
   virtual void doSomething() = 0 {};
   virtual int getStatus() { return health; };        //all drived classes use to to dermine if it should be deleted 
   virtual void changeTicks(int) {};                  //Protesters update ticks if stunned 
   virtual StudentWorld* getWorld();                  //polymorphism to StudentWorld objects
   virtual void setVisible();                         //turn visibility off
   virtual void makeSound(int);
   virtual bool playerRuled(double, double);          //player is withing a distance of another object
   virtual int Xcorr() { return a_col; };
   virtual int Ycorr() { return a_row; };
   virtual int protesterHitPts(int) { return 0; };
   virtual bool leaveOilField(bool) { return false; };
   virtual bool proleaving() { return false; };
   int boulder = 1;                    //indices for 2D vector of ice objects
   int squirt = 0;
   int v_oil = 2;
   int v_gold = 3;
   int v_sonar = 4;
   int v_water = 5;
   int v_regularProtester = 6;
   int v_hardcoreProtester = 7;
   double distance = 100;
   
private:
   StudentWorld* sw;
   int health;
   int a_col;
   int a_row;
};

class iceField : public ActorInterface {        //class for ice field 2d array
public:
   iceField(int, int, StudentWorld*); 
   virtual ~iceField();
   virtual void doSomething() override {};

private:
   StudentWorld* isw;
};

class Iceman : public ActorInterface {
public:
   Iceman(StudentWorld*);                       
   virtual ~Iceman() {};
   void setVisible() {};
   virtual void doSomething() override;
   int getStatus() override;
   int getHealth(int);
   int numberOfLivesLeft(int);
   int getPlayerHitPts(int);
   int getSquirtsLeft(int);
   int getPlayerGold(int);
   int getPlayerSonarCharge(int);
   // Next, create a string from your statistics, of the form:
   // Lvl:	52	Lives:	3	Hlth:	80%	Wtr:	20	Gld:	3	Oil	Left:	2	Sonar:	1	Scr:	321000
  // string someFunctionYouUseToFormatThingsNicely(level, lives, health,
    //  squirts, gold, barrelsLeft, sonar, score);

private:
   int hitPts;
   int sgun;
   int sonar;
   int gold;
   int health;
   int lives;
   int barrels;
};

class Protester : public ActorInterface
{
public:
   Protester(int, StudentWorld*);
   virtual ~Protester() {};
   bool yellAtIceman(int, int);           //iceman is in front of a protester, yell at him
   bool findIceman(int, int);             //iceman is in direct sight of protester
   bool withinDistance(int, int);         //basic function to find distance between iceman and a protester 
   void moveProtester();                  //check to see if next move is possible and update direction 
   virtual int protesterHitPts(int) = 0;  
   bool crossroad(Direction&);            //if protester is a intersection 
   bool canMove(Direction);               //check to see if next move is possible
   virtual void changeTicks(int) override;//update waiting time
   virtual bool leaveOilField(bool) override;//if protester is annoyed and is going to leave
   virtual bool proleaving() override;       //protester is currently leaving 
   Direction pdir;
   int level;
   int chant;
   int move;
   int hitPts;
   int atPerp;
   bool leaving;
   int ticksToWaitBetweenMoves;

};

class RegularProtester : public Protester
{
public:
   RegularProtester(StudentWorld*);
   virtual ~RegularProtester() {};
   virtual void doSomething() override;
   virtual int getStatus() override;
   virtual int protesterHitPts(int) override;
   bool takeABreak(int);
private:
   int hitPts;
};

class HardcoreProtester : public Protester
{
public:
   HardcoreProtester(StudentWorld*);
   virtual ~HardcoreProtester() {};
   virtual void doSomething() override;
   virtual int getStatus() override;
   virtual int protesterHitPts(int) override;
   bool takeABreak(int);
private:
   int hitPts;
};

class Boulder : public ActorInterface {
public:
   Boulder(int, int, StudentWorld*);
   virtual ~Boulder() {};
   virtual void doSomething() override;
   int getStatus() override;
   int Xcorr() override;
   int Ycorr() override;

private:
   int waiting;
   bool collapse;
   bool iswaiting;
   int b_row;
   int b_col;
   int b_health;
};

class Squirt : public ActorInterface {
public:
   Squirt(int col, int row, Direction d, StudentWorld*);
   virtual ~Squirt();
   virtual void doSomething() override; 
   int getStatus() override;

private:
   int x;
   int y;
   Direction dir;
   bool ammo;
   int move;
   bool gotEm;
};

class ActivatingAgent : public ActorInterface {
public:
   ActivatingAgent(int, int, int, StudentWorld*);
   virtual ~ActivatingAgent() {};
   void doSomething();
   bool active;

private:
   double distance;
   int agent_ID;
   int x;
   int y;
};

class Oil : public ActivatingAgent {
public:
   Oil(int, int, StudentWorld*);
   virtual ~Oil();
   void virtual doSomething() override;
   int getStatus() override;
};

class Gold : public ActivatingAgent
{
public:
   Gold(int, int, StudentWorld*, bool);
   virtual ~Gold();
   virtual void doSomething() override;
   int getStatus() override;
private:
   bool state;
   bool found;
   int temp;
};

class SonarKit : public ActivatingAgent
{
public:
   SonarKit(int, int, StudentWorld*);
   virtual ~SonarKit() {};
   virtual void doSomething() override;
   int getStatus() override;
   
private:
   int T;
   bool found;
};

class waterPool : public ActivatingAgent
{
public:
   waterPool(int, int, StudentWorld*);
   virtual ~waterPool() {};
   virtual void doSomething() override;
   int getStatus() override;

private:
   int evaporate;
   bool found;

};
#endif // ACTOR_H_
