#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "GameWorld.h"
#include "GameConstants.h"
#include <algorithm>
#include <iostream>
#include <string>
#include <vector>
#include "Actor.h"

using namespace std;

// Students:  Add code to this file, StudentWorld.cpp, Actor.h, and Actor.cpp

class Ice;
class Iceman;

class StudentWorld : public GameWorld
{
private:
   int col;
   int row;
   int health;
   int sgun;
   int gold;
   int barrels;
   int sonar;
   int score;
   int hitPts;
   int item;
   int barrel_col;
   int barrel_row;
   int water_col;
   int water_row;
   int v_squirt;
   int v_boulder;
   int v_oil;
   int v_gold;
   int v_sonar;
   int v_water;
   int v_rprotester;
   int v_hprotester;

public:
   Iceman* player;
   iceField *iceBlock[VIEW_HEIGHT][VIEW_WIDTH];   
   vector< vector <ActorInterface*> > iceNPC;                              //list of all objects that do move or do something but are non playable
   int rock_col;
   int rock_row;
   int gold_col;
   int gold_row;
   int B;
   int T = 0;
   enum ptype { pgold, pshot, pboulder };
   StudentWorld(std::string assetDir);
   virtual ~StudentWorld();
   void setDisplayText();
   virtual int init();
   virtual int move();
   void makeIce();
   bool protesterRuled(double, double, StudentWorld::ptype);
   bool isBoulder(GraphObject::Direction, int, int);
   void makeRocks();
   void oilSpots();
   void goldReserve();
   void waterHole();
   void sonarActivate();
   int randomGoodie();
   void makeWaterGun(bool, int, int);
   void protestPicket();
   void clearIce();
   void makeTHEnpc();
   bool itemMap(int, int);
   virtual void cleanUp();
   int getCurrentGameLevel();
   int getNumLivesLeft();
   int getCurrentHealth();
   int getSquirtsLeftInSquirtGun();
   int getPlayerGoldCount();
   int getNumberOfBarrelsRemaining(bool);
   int getPlayerSonarChargeCount();
   int getCurrentScore();
   void removeDeadGameObjects();
   // Next, create a string from your statistics, of the form:
   // Lvl:	52	Lives:	3	Hlth:	80%	Wtr:	20	Gld:	3	Oil	Left:	2	Sonar:	1	Scr:	321000
   string formatDisplay(int, int, int, int, int, int, int ,int);
   // Finally, update the display text at the top of the screen with your
   // newly created stats
};

#endif // STUDENTWORLD_H_
