#include "StudentWorld.h"
#include <string>
using namespace std;

GameWorld* createStudentWorld(string assetDir)
{
	return new StudentWorld(assetDir);
}

StudentWorld::StudentWorld(std::string assetDir) : GameWorld(assetDir)
{
   player = nullptr;
   col = 0;
   row = 0;
   score = 0;
   barrels = min(2 + getCurrentGameLevel(), 21);
   rock_col = 0;
   rock_row = 0;
   barrel_col = 0;
   barrel_row = 0;
   water_col = 0;
   water_row = 0;
   gold_col = 0;
   gold_row = 0;
   item = 8;
   v_squirt = 0;
   v_boulder = 1;
   v_oil = 2;
   v_gold = 3;
   v_sonar = 4;
   v_water = 5;
   v_rprotester = 6;
   v_hprotester = 7;
   iceNPC.resize(item);
}

StudentWorld::~StudentWorld()
{
   for (int i = 0; i < VIEW_HEIGHT; i++)
      for (int j = 0; j < VIEW_WIDTH; j++)
         if (iceBlock[i][j] != nullptr)
            delete iceBlock[i][j];


   for (int k = 0; k < iceNPC.size(); k++)
   {
      for (vector<ActorInterface*>::iterator it = iceNPC[k].begin(); it != iceNPC[k].end();)
      {
         delete *it;
         it = iceNPC[k].erase(it);
      }
   }

   delete player;
}

int StudentWorld::init()
{
   for (int i = 0; i < VIEW_WIDTH; i++)
      for (int j = 0; j < VIEW_HEIGHT; j++)
         iceBlock[i][j] = nullptr;

   makeIce();
   player = new Iceman(this);
   makeTHEnpc();

   return  GWSTATUS_CONTINUE_GAME;
}

void StudentWorld::makeTHEnpc()
{
   makeRocks();
   oilSpots();
   goldReserve();
};

void StudentWorld::protestPicket()
{
   int probabilityOfHardcore = min(90, getCurrentGameLevel() * 10 + 30);
   int plevel = 2 + getCurrentGameLevel() * 1.5;
   int size = iceNPC[v_rprotester].size() + iceNPC[v_hprotester].size();
   int P = min(15, plevel);
   T--;
   if (size <= P && T <= 0)
   {
      T = max(25, 200 - getCurrentGameLevel());
      if (rand() % probabilityOfHardcore * 100 < 100)
         iceNPC[v_hprotester].push_back(new HardcoreProtester(this));
      else
      iceNPC[v_rprotester].push_back(new RegularProtester(this));
   }
}

void StudentWorld::makeWaterGun(bool trigger, int x, int y)
{
   if (trigger = true && iceBlock[y][x] == nullptr && x > -1 && x < 61 && y > -1 && y < 61)
      iceNPC[v_squirt].push_back(new Squirt(x, y, player->getDirection(), this));
};

void StudentWorld::oilSpots()
{
   barrels = min(2 + getCurrentGameLevel(), 21);
   barrel_col = rand() % 60;
   barrel_row = rand() % 57;

   for (int i = 0; i < barrels; i++)
   {
      while (itemMap(barrel_col, barrel_row) == false || iceBlock[barrel_row][barrel_col] == nullptr || (barrel_col > 26 && barrel_col < 34))
      {
         barrel_col = rand() % 60;
         barrel_row = rand() % 57;
      }
      iceNPC[v_oil].push_back(new Oil(barrel_col, barrel_row, this));
   }
}

void StudentWorld::goldReserve()
{
   int G = max(5 - getCurrentGameLevel() / 2, 2);

   for (int i = 0; i < G; i++)
   {
      gold_col = rand() % 60;
      gold_row = rand() % 57;

      while (itemMap(gold_col, gold_row) == false || iceBlock[gold_row][gold_col] == nullptr || (gold_col > 26 && gold_col < 34))
      {
         gold_col = rand() % 60;
         gold_row = rand() % 57;
      }
      iceNPC[v_gold].push_back(new Gold(gold_col, gold_row, this, true));
   }
}

int StudentWorld::randomGoodie()
{
   int G = getCurrentGameLevel() * 25 + 300;

   if (rand() % G * 100 < 100)
   {
      if (rand() % 5 < 1)
         return v_sonar;
      else
         return v_water;
   }

   return -1;
}

void StudentWorld::waterHole()
{
   int digWater = 0;

   if (randomGoodie() == v_water)
   {
      while (digWater != 16)
      {
         water_col = rand() % 60;
         water_row = rand() % 60;

         while (itemMap(water_col, water_row) == false)
         {
            water_col = rand() % 61;
            water_row = rand() % 61;
         }

         for (int i = 0; i < 4; i++)
            for (int j = 0; j < 4; j++)
               if (iceBlock[water_row + j][water_col + i] == nullptr)
               {
                  digWater++;
               }

         if (digWater != 16)
            digWater = 0;
      }
      iceNPC[v_water].push_back(new waterPool(water_col, water_row, this));
   }
};

void StudentWorld::sonarActivate()
{
   if (randomGoodie() == v_sonar)
      iceNPC[v_sonar].push_back(new SonarKit(0, 60, this));
}

bool StudentWorld::itemMap(int x, int y)
{
   double distance = 0.0;
      for (int j = 0; j < iceNPC.size(); j++)
      {
         for (int k = 0; k < iceNPC[j].size(); k++)
         {
               if (abs((x + 2.0) - (iceNPC[j].at(k)->getX()) + 2.0) <= 6.0 || abs((y + 2.0) - (iceNPC[j].at(k)->getY()) + 2.0) <= 6.0)
               {
                  distance = pow((x + 2.0) - (iceNPC[j].at(k)->getX() + 2.0), 2.0) + pow((y + 2.0) - (iceNPC[j].at(k)->getY() + 2.0), 2.0);
                  distance = sqrt(distance);
                  if (distance <= 6.0)
                     return false;
               }
         }
      }
   return true;
}

void StudentWorld::makeRocks()
{
   int digRock = 0;
   int L = getLevel();
   bool tryAgain = true;
   B = min(L / 2 + 2, 9);

     for (int n = 0; n < B; n++)
     {
        digRock = 0;
        while (digRock != 16)
        {
           rock_col = rand() % 61;
           rock_row = rand() % 37 + 20;

           while (itemMap(rock_col, rock_row) == false || rock_col > 26 && rock_col < 35)
           {
              rock_col = rand() % 61;
              rock_row = rand() % 37 + 20;
           }

           for (int i = 0; i < 4; i++)
           {
              for (int j = 0; j < 4; j++)
              {
                 if (iceBlock[rock_row + j][rock_col + i] != nullptr)
                 {
                    iceBlock[rock_row + j][rock_col + i]->setVisible();
                    iceBlock[rock_row + j][rock_col + i] = nullptr;
                    digRock++;
                 }
              }
           }
        }
      iceNPC[v_boulder].push_back(new Boulder(rock_col, rock_row, this));
   }
};

void StudentWorld::makeIce()
{
   for (int j = VIEW_HEIGHT - 1; j >= 0; j--)
   {
      for (int i = 0; i < VIEW_WIDTH; i++)
      {
         if ((j < VIEW_HEIGHT - 4 && (i < 30 || i > 33)) || j < 4)
            iceBlock[j][i] = new iceField(i, j, this);
      }
   }
};

bool StudentWorld::protesterRuled(double x, double y, ptype t)
{
   double distance = 0.0;

   for (int i = v_rprotester; i <= v_hprotester; i++)
   {
      for (int j = 0; j < iceNPC[i].size(); j++)
      {
         if (abs(x - iceNPC[i][j]->getX()) <= 3.0 || abs(y - iceNPC[i][j]->getY()) <= 3.0)
         {
            distance = pow(x - iceNPC[i][j]->getX(), 2.0) + pow(y - iceNPC[i][j]->getY(), 2.0);
            distance = sqrt(distance);
            if (distance <= 3.0)
            {
               if (iceNPC.at(i).at(j)->proleaving() == false)
               {
                  switch (t)
                  {
                  case pgold:
                     if (i = v_rprotester)
                     {
                        increaseScore(25);
                        iceNPC.at(i).at(j)->leaveOilField(true);
                     }
                     else if (i = v_hprotester)
                     {
                        increaseScore(50);
                        int N = max(50, 100 - getCurrentGameLevel() * 10);
                        iceNPC.at(i).at(j)->changeTicks(N);
                        iceNPC.at(i).at(j)->leaveOilField(false);
                     }
                     playSound(SOUND_PROTESTER_FOUND_GOLD);
                     return true;
                     break;

                  case pshot:
                     iceNPC.at(i).at(j)->protesterHitPts(2);

                     if (iceNPC.at(i).at(j)->protesterHitPts(0) <= 0)
                     {
                        if (i = v_rprotester)
                           increaseScore(100);
                        else 
                           increaseScore(250);
                        iceNPC.at(i).at(j)->changeTicks(0);
                        playSound(SOUND_PROTESTER_GIVE_UP);
                     }
                     else
                     {
                        int N = max(50, 100 - getCurrentGameLevel() * 10);
                        iceNPC.at(i).at(j)->changeTicks(N);
                        playSound(SOUND_PROTESTER_ANNOYED);
                     }
                     return true;
                     break;

                  case pboulder:
                     increaseScore(500);
                     playSound(SOUND_PROTESTER_GIVE_UP);
                     iceNPC.at(i).at(j)->changeTicks(0);
                     iceNPC.at(i).at(j)->protesterHitPts(100);
                     return true;
                     break;
                  }
               }
            }
         }
      }
   }
   return false;
}

bool StudentWorld::isBoulder(GraphObject::Direction d, int x, int y)
{
   switch (d) {
   case 3: //left
      for (int j = 0; j < iceNPC[v_boulder].size(); j++)
      {
         if (!(iceNPC.at(v_boulder).at(j)->Xcorr() + 3 != x - 1 ||
            (iceNPC.at(v_boulder).at(j)->Ycorr() + 3 < y ||
               iceNPC.at(v_boulder).at(j)->Ycorr() > y + 3)))
            return true;
      }
      return false;
      break;

   case 4: //right
      for (int j = 0; j < iceNPC[v_boulder].size(); j++)
      {
         if (!(iceNPC.at(v_boulder).at(j)->Xcorr() - 3 != x + 1 ||
            (iceNPC.at(v_boulder).at(j)->Ycorr() + 3 < y ||
               iceNPC.at(v_boulder).at(j)->Ycorr() > y + 3)))
            return true;
      }
      return false;
      break;

   case 2: //down
      for (int j = 0; j < iceNPC[v_boulder].size(); j++)
      {
         if (!(iceNPC.at(v_boulder).at(j)->Ycorr() + 3 != y - 1 ||
            (iceNPC.at(v_boulder).at(j)->Xcorr() + 3 < x ||
               iceNPC.at(v_boulder).at(j)->Xcorr() > x + 3)))
            return true;
      }
      return false;
      break;

   case 1: //up
      for (int j = 0; j < iceNPC[v_boulder].size(); j++)
      {
         if (!(iceNPC.at(v_boulder).at(j)->Ycorr() - 3 != y + 1 ||
            (iceNPC.at(v_boulder).at(j)->Xcorr() + 3 < x ||
               iceNPC.at(v_boulder).at(j)->Xcorr() > x + 3)))
            return true;
      }
      return false;
   }
};


void StudentWorld::cleanUp()
{
   for (int i = 0; i < VIEW_HEIGHT; i++)
      for (int j = 0; j < VIEW_WIDTH; j++)
         if(iceBlock[i][j] != nullptr)
         delete iceBlock[i][j];


   for (int k = 0; k < iceNPC.size(); k++)
   {
      for (vector<ActorInterface*>::iterator it = iceNPC[k].begin(); it != iceNPC[k].end();)
      {
            delete *it;
            it = iceNPC[k].erase(it);
      }
   }

   delete player;
}

void StudentWorld::clearIce()
{
   enum Direction { none, up, down, left, right };
  
   if (player->getDirection() == right && player->getX() + 3 <= VIEW_WIDTH && player->getY() + 3 <= VIEW_HEIGHT)
   {
      for (int i = 0; i <= 3; i++)
         if (iceBlock[player->getY() + i][player->getX() + 3] != nullptr)
            iceBlock[player->getY() + i][player->getX() + 3]->setVisible();

      if (iceBlock[player->getY()][player->getX() + 3] != nullptr || iceBlock[player->getY() + 1][player->getX() + 3] != nullptr ||
         iceBlock[player->getY() + 2][player->getX() + 3] != nullptr || iceBlock[player->getY() + 3][player->getX() + 3] != nullptr)
         playSound(SOUND_DIG);

      for (int i = 0; i <= 3; i++)
         iceBlock[player->getY() + i][player->getX() + 3] = nullptr;
   }
   else if (player->getDirection() == left)
   {
      for (int i = 0; i <= 3; i++)
         if (iceBlock[player->getY() + i][player->getX()] != nullptr)
            iceBlock[player->getY() + i][player->getX()]->setVisible();

      if (iceBlock[player->getY()][player->getX()] != nullptr || iceBlock[player->getY() + 1][player->getX()] != nullptr ||
         iceBlock[player->getY() + 2][player->getX()] != nullptr || iceBlock[player->getY() + 3][player->getX()] != nullptr)
         playSound(SOUND_DIG);

      for (int i = 0; i <= 3; i++)
         iceBlock[player->getY() + i][player->getX()] = nullptr;
   }
   else if (player->getDirection() == up)
   {
      for (int i = 0; i <= 3; i++)
         if (iceBlock[player->getY() + 3][player->getX() + i] != nullptr)
            iceBlock[player->getY() + 3][player->getX() + i]->setVisible();

      if (iceBlock[player->getY() + 3][player->getX()] != nullptr || iceBlock[player->getY() + 3][player->getX() + 1] != nullptr ||
         iceBlock[player->getY() + 3][player->getX() + 2] != nullptr || iceBlock[player->getY() + 3][player->getX() + 3] != nullptr)
         playSound(SOUND_DIG);

      for (int i = 0; i <= 3; i++)
         iceBlock[player->getY() + 3][player->getX() + i] = nullptr;
   }
   else if (player->getDirection() == down)
   {
      for (int i = 0; i <= 3; i++)
         if (iceBlock[player->getY()][player->getX() + i] != nullptr)
            iceBlock[player->getY()][player->getX() + i]->setVisible();

      if (iceBlock[player->getY()][player->getX()] != nullptr || iceBlock[player->getY() ][player->getX() + 1] != nullptr ||
         iceBlock[player->getY() ][player->getX() + 2] != nullptr || iceBlock[player->getY()][player->getX() + 3] != nullptr)
         playSound(SOUND_DIG);

      for (int i = 0; i <= 3; i++)
         iceBlock[player->getY()][player->getX() + i] = nullptr;
   }
};

int StudentWorld::move()
{
   setDisplayText(); 
   waterHole();
   protestPicket();

   if(iceNPC[v_sonar].size() <= 1)
   sonarActivate();

   if (getCurrentHealth() > 0)
      {
         // ask each actor to do something (e.g. move)
         player->doSomething();
         clearIce();

         if (getCurrentHealth() <= 0)
            return GWSTATUS_PLAYER_DIED;
         if (getNumberOfBarrelsRemaining(false) == 0)
         {
            return GWSTATUS_FINISHED_LEVEL;
         }
      }

   for (int k = 0; k < iceNPC.size(); k++)
      for (int s = 0; s < iceNPC[k].size(); s++)
         iceNPC.at(k).at(s)->doSomething();
   
      // Remove newly-dead actors after each tick
   removeDeadGameObjects(); // delete dead game objects
                            // return the proper result
   if (getCurrentHealth() <= 0)
   {
      decLives();
      return GWSTATUS_PLAYER_DIED;
   }

   // If the player has collected all of the Barrels on the level, then
   // return the result that the player finished the level
   if (getNumberOfBarrelsRemaining(false) <= 0)
   {
      playSound(SOUND_FINISHED_LEVEL);
      return GWSTATUS_FINISHED_LEVEL;
   }
   // the player hasn’t completed the current level and hasn’t died
   // let them continue playing the current level
   return GWSTATUS_CONTINUE_GAME;
};

int StudentWorld::getCurrentGameLevel()
{
   return getLevel();
};

int StudentWorld::getNumLivesLeft()
{
   return getLives();
};

int StudentWorld::getCurrentHealth()
{
   health = player->getStatus();
   return health;
};

int StudentWorld::getSquirtsLeftInSquirtGun()
{
   sgun = player->getSquirtsLeft(0);
   return sgun;
};

int StudentWorld::getPlayerGoldCount()
{
   gold = player->getPlayerGold(0);
   return gold;
};

int StudentWorld::getNumberOfBarrelsRemaining(bool found)
{
   if (found == true)
   barrels--;

   return barrels;
};

int StudentWorld::getPlayerSonarChargeCount()
{
   sonar = player->getPlayerSonarCharge(0);
   return sonar;
};

int StudentWorld::getCurrentScore()
{
   return getScore();
};

string StudentWorld::formatDisplay(int level, int lives, int health, int squirts, int gold, int barrelsLeft, int sonar, int score)
{
   return "Lvl: " + std::to_string(level) + " Lives: " + std::to_string(lives) + " Hlth: " + std::to_string(health) + 
          "% Wtr: " + std::to_string(squirts) + " Gld: " + std::to_string(gold) + " Oil Left: " + std::to_string(barrelsLeft) + 
          " Sonar: " + std::to_string(sonar) + " Scr: " + std::to_string(score);
};

void StudentWorld::removeDeadGameObjects()
{
   for (int k = 0; k < iceNPC.size(); k++)
   {
      for (vector<ActorInterface*>::iterator it = iceNPC[k].begin(); it != iceNPC[k].end();)
      {
         if ((*it)->getStatus() == 0)
         {
            delete *it;
            it = iceNPC[k].erase(it);
         }
         else
            ++it;
      }
   }
};

void StudentWorld::setDisplayText()
{
   int level = getCurrentGameLevel();
   int lives = getNumLivesLeft();
   int health = getCurrentHealth();
   int squirts = getSquirtsLeftInSquirtGun();
   int gold = getPlayerGoldCount();
   int barrelsLeft = getNumberOfBarrelsRemaining(false);
   int sonar = getPlayerSonarChargeCount();
   int score = getCurrentScore();

   // Next, create a string from your statistics, of the form:
   // Lvl:	52	Lives:	3	Hlth:	80%	Wtr:	20	Gld:	3	Oil	Left:	2	Sonar:	1	Scr:	321000

    string s = formatDisplay(level, lives, health, squirts, gold, barrelsLeft, sonar, score);
   // Finally, update the display text at the top of the screen with your
   // newly created stats
    setGameStatText(s); // calls our provided GameWorld::setGameStatText
}

// Students:  Add code to this file (if you wish), StudentWorld.h, Actor.h and Actor.cpp
