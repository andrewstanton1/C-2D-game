#include "Actor.h"
#include "StudentWorld.h"

// Students:  Add code to this file (if you wish), Actor.h, StudentWorld.h, and StudentWorld.cpp


ActorInterface::ActorInterface() : GraphObject(IID_PLAYER, 30, 60, right, 1.0, 0)
{
   GraphObject::setVisible(true);
};

ActorInterface::~ActorInterface()
{};

void ActorInterface::setVisible()
{
   GraphObject::setVisible(false);
};

void ActorInterface::makeSound(int noise)
{
   if (SOUND_FALLING_ROCK == noise)
      getWorld()->playSound(SOUND_FALLING_ROCK);
};

StudentWorld* ActorInterface::getWorld()
{
   return sw;
};

bool ActorInterface::playerRuled(double x, double y)
{
   double distance = 0.0;

   if (abs((x + 2.0) - (getWorld()->player->getX() + 2.0)) <= 3.0 || abs((y + 2.0) - (getWorld()->player->getY() + 2.0)) <= 3.0)
   {
      distance = pow((x + 2.0) - (getWorld()->player->getX() + 2.0), 2.0) + pow((y + 2.0) - (getWorld()->player->getY() + 2.0), 2.0);
      distance = sqrt(distance);
      if (distance <= 3.0)
         return true;
   }

   return false;
}

////////////////////////////////////////////////
/////////////// iceField class /////////////////
////////////////////////////////////////////////

iceField::iceField(int r, int c, StudentWorld* sworld) : ActorInterface(IID_ICE, r, c, right, 0.25, 3, sworld)
{
   isw = sworld;
   GraphObject::setVisible(true);
};

iceField::~iceField()
{};

////////////////////////////////////////////////
/////////////// Iceman class ///////////////////
////////////////////////////////////////////////
Iceman::Iceman(StudentWorld* sworld) : ActorInterface(IID_PLAYER, 30, 60, right, 1.0, 0, sworld)
{
   GraphObject::setVisible(true);
   hitPts = 10;
   sgun = 5;
   sonar = 1;
   gold = 0;
   health = 100;
   lives = getWorld()->getLives();
};

void Iceman::doSomething()
{
   int ch;
   if (getWorld()->getKey(ch) == true)
   {
      // user hit a key this tick!
      switch (ch)
      {
      case KEY_PRESS_LEFT:
         if (getDirection() == left)
         {
            if (getX() > 0)
            {
               if (getWorld()->isBoulder(left, getX(), getY()) == false)
                  moveTo(getX() - 1, getY());
            }
            else 
               moveTo(getX(), getY());
         }
         else
            setDirection(left);
         break;
      case KEY_PRESS_RIGHT:
         if (getDirection() == right)
         {
            if (getX() < 60)
            {
               if (getWorld()->isBoulder(right, getX(), getY()) == false)
                  moveTo(getX() + 1, getY());
            }
            else 
               moveTo(getX(), getY());
         }
         else
            setDirection(right);
         break;
      case KEY_PRESS_DOWN:
         if (getDirection() == down)
         {
            if (getY() > 0)
            {
               if (getWorld()->isBoulder(down, getX(), getY()) == false)
                  moveTo(getX(), getY() - 1);
            }
            else 
               moveTo(getX(), getY());
         }
         else
            setDirection(down);
         break;
      case KEY_PRESS_UP:
         if (getDirection() == up)
         {
            if (getY() < 60)
            {
               if (getWorld()->isBoulder(up, getX(), getY()) == false)
                  moveTo(getX(), getY() + 1);
            }
            else 
               moveTo(getX(), getY());
         }
         else
            setDirection(up);
         break;
      case KEY_PRESS_SPACE:
         if (getSquirtsLeft(0) > 0)
         {
            sgun--;

            if (getDirection() == right)
               getWorld()->makeWaterGun(true, getX() + 4, getY());
            else if (getDirection() == left)
               getWorld()->makeWaterGun(true, getX() - 4, getY());
            else if (getDirection() == down)
               getWorld()->makeWaterGun(true, getX(), getY() - 4);
            else if (getDirection() == up)
               getWorld()->makeWaterGun(true, getX(), getY() + 4);

            getWorld()->playSound(SOUND_PLAYER_SQUIRT);
         }
         break;
      case KEY_PRESS_TAB:
         if (getPlayerGold(0) >= 1)
         {
            getWorld()->iceNPC[v_gold].push_back(new Gold(getX(), getY(), getWorld(), false));
            getPlayerGold(-1);
         }
         break;
      case 'z' || 'Z':
      default:
         if (getPlayerSonarCharge(0) >= 1)
         {
            getPlayerSonarCharge(-1);
            for (int i = v_oil; i <= v_gold; i++)
            {
               for (int j = 0; j < getWorld()->iceNPC[i].size(); j++)
               {
                  if (abs((getWorld()->iceNPC[i][j]->getX() + 2) - (getX() + 2)) <= 12.0 || abs((getWorld()->iceNPC[i][j]->getY() + 2) - (getY() + 2)) <= 12.0)
                  {
                     distance = pow((getWorld()->iceNPC[i][j]->getX() + 2) - (getX() + 2), 2) + pow((getWorld()->iceNPC[i][j]->getY() + 2) - (getY() + 2), 2);
                     distance = sqrt(distance);
                     if (distance <= 12.0)
                     {
                        getWorld()->iceNPC[i][j]->GraphObject::setVisible(true);
                     }
                  }
               }
            }
         }
         break;
      }
   }
};

int Iceman::getStatus()
{
   if (health <= 0)
      return 0;
   else
   return health;
};

int Iceman::getHealth(int damage)
{
   health = health + damage;
   return health;
};

int Iceman::numberOfLivesLeft(int death)
{
   lives = lives + death;
   return lives;
};

int Iceman::getSquirtsLeft(int foundOne)
{
   sgun = sgun + foundOne;
   return sgun;
};

int Iceman::getPlayerGold(int foundOne)
{
   gold = gold + foundOne;
   return gold;
};

int Iceman::getPlayerSonarCharge(int foundOne)
{
   sonar = sonar + foundOne;
   return sonar;
};

int Iceman::getPlayerHitPts(int damage)
{
   hitPts = hitPts + damage;
   return hitPts;
};


////////////////////////////////////////////////
/////////////// Protester class ////////////////
////////////////////////////////////////////////
Protester::Protester(int ID, StudentWorld* sworld) : ActorInterface(ID, 60, 60, left, 1.0, 0, sworld)
{
   GraphObject::setVisible(true);
   level = getWorld()->getLevel();
   chant = 0;                                   //protester yells at iceman when near him only only in 15 tick intervals
   atPerp = 0;                                  //protester is a perpendicular opening
   leaving = false;
   move = rand() % 53 + 8;
   pdir = Direction(rand() % 4 + 1);
};

void Protester::moveProtester()
{
   if (move <= 0)
   {
      move = rand() % 53 + 8;
      pdir = Direction(rand() % 4 + 1);

      while(canMove(pdir) == false || pdir == none)
         pdir = Direction(rand() % 4 + 1);

      setDirection(pdir);
   }

   if (crossroad(pdir) == true)
   {
      atPerp = 0;
      setDirection(pdir);
      move = rand() % 53 + 8;
   }

   if (canMove(pdir) == true)
   {
      switch (pdir)
      {
      case up:
         if(getY() < 60)
         moveTo(getX(), getY() + 1);
         break;

      case down:
         if(getY() > 0)
         moveTo(getX(), getY() - 1);
         break;

      case left:
         if(getX() > 0)
         moveTo(getX() - 1, getY());
         break;

      case right:
         if(getX() < 60)
         moveTo(getX() + 1, getY());
         break;
      }
   }
   else
      move = 0;
};

bool Protester::canMove(Direction d)
{
   int x = getX();
   int y = getY();

   switch (d)
   {
   case up:
         for (int j = 0; j < 4; j++)
            if (y >= 60 || getWorld()->iceBlock[y + 4][x + j] != nullptr || getWorld()->isBoulder(up, x, y) == true)
               return false;
         break;

   case down:
      for (int j = 0; j < 4; j++)
         if (y <= 0 ||getWorld()->iceBlock[y - 1][x + j] != nullptr || getWorld()->isBoulder(down, x, y) == true)
            return false;
      break;

   case left:
      for (int j = 0; j < 4; j++)
         if (x <= 0 || getWorld()->iceBlock[y + j][x - 1] != nullptr || getWorld()->isBoulder(left, x, y) == true)
            return false;
      break;

   case right:
      for (int j = 0; j < 4; j++)
         if (x >= 60 || getWorld()->iceBlock[y + j][x + 4] != nullptr || getWorld()->isBoulder(right, x, y) == true)
            return false;
      break;
   }
   return true;
};


bool Protester::crossroad(Direction& d)
{
   if (atPerp < 200)
      return false;
   
   if (d == up || d == down)
   {
      if (canMove(left) == true && canMove(right) == true)
      {
         d = Direction(rand() % 2 + 1);
         return true;
      }
      else if (canMove(left) == true)
      {
         d = left;
         return true;
      }
      else if (canMove(right) == true)
      {
         d = right;
         return true;
      }
      else
         return false;
   }
   else if (d == right || d == left)
   {
      if (canMove(up) == true && canMove(down) == true)
      {
         d = Direction(rand() % 4 + 2);
         return true;
      }
      else if (canMove(up) == true)
      {
         d = up;
         return true;
      }
      else if (canMove(down) == true)
      {
         d = down;
         return true;
      }
      else
         return false;
   }
}

bool Protester::withinDistance(int x, int y)
{
   double distance = 0.0;

   if (abs((x + 2.0) - (getWorld()->player->getX() + 2.0)) <= 4.0 || abs((y + 2.0) - (getWorld()->player->getY() + 2.0)) <= 4.0)
   {
      distance = pow((x + 2.0) - (getWorld()->player->getX() + 2.0), 2.0) + pow((y + 2.0) - (getWorld()->player->getY() + 2.0), 2.0);
      distance = sqrt(distance);

      if (distance <= 4.0)
         return true;
      else
         return false;
   }
   else
      return false;
}

bool Protester::leaveOilField(bool retreat)
{
   if (retreat == true)
   {
      leaving = true;
      return true;
   }
   else
      return false;
}

bool Protester::proleaving()
{
   return leaving;
}

void Protester::changeTicks(int t)
{
   ticksToWaitBetweenMoves = ticksToWaitBetweenMoves + t;
}

bool Protester::yellAtIceman(int x, int y)
{
   Direction getDir = getDirection();

   if (withinDistance(x, y) == true)
   {
      switch (getDir)
      {
      case left:
         if (getWorld()->player->getX() < x && chant == 15)
         {
            chant = 0;
            getWorld()->playSound(SOUND_PROTESTER_YELL);
            return true;
         }
         break;

      case right:
         if (getWorld()->player->getX() > x && chant == 15)
         {
            chant = 0;
            getWorld()->playSound(SOUND_PROTESTER_YELL);
            return true;
         }
         break;

      case down:
         if (getWorld()->player->getY() < y && chant == 15)
         {
            chant = 0;
            getWorld()->playSound(SOUND_PROTESTER_YELL);
            return true;
         }
         break;

      case up:
         if (getWorld()->player->getY() > x && chant == 15)
         {
            chant = 0;
            getWorld()->playSound(SOUND_PROTESTER_YELL);
            return true;
         }
         break;
      }
   }

   return false;
}

bool Protester::findIceman(int x, int y)
{
   int pX = getWorld()->player->getX();
   int pY = getWorld()->player->getY();

   if (x == pX)
   {
      if (pY > y)
      {
         for (int i = 4; i <= pY - y; i++)
         {
            for (int j = 0; j < 4; j++)
            {
               if (getWorld()->iceBlock[y + i][x + j] != nullptr || getWorld()->isBoulder(up, x, y + i - 4) == true)
                  return false;
            }
         }
         if (y < 60)
         {
            setDirection(up);
            moveTo(x, y + 1);
         }
         return true;
      }
      else if (pY < y)
      {
         for (int i = 0; i <= y - pY; i++)
         {
            for (int j = 0; j < 4; j++)
            {
               if (getWorld()->iceBlock[y - i][x + j] != nullptr || getWorld()->isBoulder(down, x, y - i) == true)
                  return false;
            }
         }
         if (y > 0)
         {
            setDirection(down);
            moveTo(x, y - 1);
         }
         return true;
      }
   }
   else if (y == pY)
   {
      if (pX > x)
      {
         for (int i = 4; i <= pX - x; i++)
            for (int j = 0; j < 4; j++)
               if (getWorld()->iceBlock[y + j][x + i] != nullptr || getWorld()->isBoulder(right, x + i - 4, y) == true)
                  return false;
         if (x < 59)
         {
            setDirection(right);
            moveTo(x + 1, y);
         }
         return true;
      }
      else if (pX < x)
      {
         for (int i = 0; i <= x - pX; i++)
            for (int j = 0; j < 4; j++)
               if (getWorld()->iceBlock[y + j][x - i] != nullptr || getWorld()->isBoulder(left, x - i, y) == true)
                  return false;
         if (x > 0)
         {
            setDirection(left);
            moveTo(x - 1, y);
         }
         return true;
      }
   }
   else
   return false;
};


////////////////////////////////////////////////
//////////// RegularProtester class ////////////
////////////////////////////////////////////////
RegularProtester::RegularProtester(StudentWorld* sworld) : Protester(IID_PROTESTER, sworld)
{
   hitPts = 5;
   ticksToWaitBetweenMoves = max(0, 3 - level / 4);
};

void RegularProtester::doSomething()
{
   if (getStatus() <= 0)
      return;
   else if (takeABreak(ticksToWaitBetweenMoves) == true)
   {
      ticksToWaitBetweenMoves--;
      return;
   }
   else
   {
      ticksToWaitBetweenMoves = max(0, 3 - level / 4);
      if (chant < 15)
         chant++;
      if (atPerp < 200)
         atPerp++;
   }

   if (yellAtIceman(getX(), getY()) == true)
   {
      getWorld()->player->getPlayerHitPts(-2);
      return;
   }
   else if (findIceman(getX(), getY()) == true)
   {
      Protester::move = 0;
      return;
   }
   else
   {
      Protester::move--;
      moveProtester();
   }
};

bool RegularProtester::takeABreak(int ticks)
{
   if (ticks <= 0)
      return false;
   else
      return true;
};

int RegularProtester::protesterHitPts(int damage)
{
   hitPts = hitPts - damage;
   return hitPts;
};

int RegularProtester::getStatus()
{
   if (hitPts <= 0)
      return 0;
   else return 1;
};



////////////////////////////////////////////////
//////////// HardProtester class ////////////
////////////////////////////////////////////////
HardcoreProtester::HardcoreProtester(StudentWorld* sworld) : Protester(IID_HARD_CORE_PROTESTER, sworld)
{
   int M = 16 + getWorld()->getCurrentGameLevel() * 2;
   hitPts = 20;
   ticksToWaitBetweenMoves = max(0, 3 - level / 4);
};

void HardcoreProtester::doSomething()
{
   if (getStatus() <= 0)
      return;
   else if (takeABreak(ticksToWaitBetweenMoves) == true)
   {
      ticksToWaitBetweenMoves--;
      return;
   }
   else
   {
      ticksToWaitBetweenMoves = max(0, 3 - level / 4);
      if (chant < 15)
         chant++;
      if (atPerp < 200)
         atPerp++;
   }

   if (yellAtIceman(getX(), getY()) == true)
   {
      getWorld()->player->getPlayerHitPts(-2);
      return;
   }
   else if (findIceman(getX(), getY()) == true)
   {
      Protester::move = 0;
      return;
   }
   else
   {
      Protester::move--;
      moveProtester();
   }
};

bool HardcoreProtester::takeABreak(int ticks)
{
   if (ticks <= 0)
      return false;
   else
      return true;
};

int HardcoreProtester::protesterHitPts(int damage)
{
   hitPts = hitPts - damage;
   return hitPts;
};

int HardcoreProtester::getStatus()
{
   if (hitPts <= 0)
      return 0;
   else return 1;
};


////////////////////////////////////////////////
/////////////// Boulder class //////////////////
////////////////////////////////////////////////
Boulder::Boulder(int col, int row, StudentWorld* sworld) : ActorInterface(IID_BOULDER, col, row, down, 1.0, 1, sworld)
{
   b_row = row;
   b_col = col;
   collapse = false;
   waiting = 1;
   b_health = 1;
   GraphObject::setVisible(true);
}

int Boulder::getStatus()
{
   return b_health;
}

int Boulder::Xcorr()
{
   return b_col;
}

int Boulder::Ycorr()
{
   return b_row;
}

void Boulder::doSomething()
{
   if (getStatus() == 0)
      return;

   if (getWorld()->iceBlock[b_row - 1][b_col] == nullptr && getWorld()->iceBlock[b_row - 1][b_col + 1] == nullptr &&
      getWorld()->iceBlock[b_row - 1][b_col + 2] == nullptr && getWorld()->iceBlock[b_row - 1][b_col + 3] == nullptr || waiting >= 30)
   {
      waiting++;
   }

   if (waiting == 30)
   { 
      makeSound(SOUND_FALLING_ROCK);
      collapse = true;
   }

   if (collapse == true)
   {
      moveTo(getX(), getY() - 1);
      b_row--;

      if (playerRuled(getX(), getY()) == true)
      {
         getWorld()->player->getHealth(-100);
      }

      getWorld()->protesterRuled(getX(), getY() - b_row, getWorld()->pboulder);

      if (getWorld()->iceBlock[b_row][b_col] != nullptr || getWorld()->iceBlock[b_row][b_col + 1] != nullptr ||
         getWorld()->iceBlock[b_row][b_col + 2] != nullptr || getWorld()->iceBlock[b_row][b_col + 3] != nullptr || b_row <= 0 || 
         getWorld()->isBoulder(down, getX(), getY()) == true)
      {
         setVisible();
         b_health = 0;
      }
   }
}

////////////////////////////////////////////////
/////////////// Squirt class ///////////////////
////////////////////////////////////////////////
Squirt::Squirt(int col, int row, Direction d, StudentWorld* sworld) :
   ActorInterface(IID_WATER_SPURT, col, row, d, 1.0, 1, sworld)
{
   GraphObject::setVisible(true);

   x = col;
   y = row;
   dir = d;
   move = 0;
   ammo = true;
   gotEm = false;
};

Squirt::~Squirt()
{};

void Squirt::doSomething()
{
   switch (dir)
   {
      case left:
         if (move == 0)
         {
            for (int i = 0; i < 4; i++)
            {
               for (int j = 0; j < 4; j++)
               {
                  if (getWorld()->iceBlock[y + j][x + i] != nullptr)
                     setVisible();
               }
            }

            for (int i = 0; i < 4; i++)
               if (getWorld()->isBoulder(left, x + 4 - i, y))
                  setVisible();           
         }

         if (getWorld()->iceBlock[y][x - move] != nullptr || move > 3 || getWorld()->isBoulder(left, x + 1 - move, y))
         {
            setVisible();
            ammo = false;
            break;
         }

         if (gotEm == false)
         {
            if (getWorld()->protesterRuled(getX() - move, getY(), getWorld()->pshot))
               gotEm == true;
         }

         if(x - move > -1)
         moveTo(x - move, y);
      break;

      case right:
         if (move == 0)
         {
            for (int i = 0; i < 3; i++)
            {
               for (int j = 0; j < 4; j++)
               {
                  if (getWorld()->iceBlock[y + j][x + i] != nullptr)
                     setVisible();
               }
            }

            for (int i = 0; i < 4; i++)
               if (getWorld()->isBoulder(right, x - 4 + i, y))
                  setVisible();
         }

         if (getWorld()->iceBlock[y][x + move + 3] != nullptr || move > 3 || getWorld()->isBoulder(right, x - 1 + move, y))
         {
            setVisible();
            ammo = false;
            break;
         }

         if (gotEm == false)
         {
            if (getWorld()->protesterRuled(getX() + move, getY(), getWorld()->pshot))
               gotEm == true;
         }

         if (x + move < 61)
            moveTo(x + move, y);
         break;

      case up:
         if (move == 0)
         {
            for (int i = 0; i < 4; i++)
            {
               for (int j = 0; j < 4; j++)
               {
                  if (getWorld()->iceBlock[y - i][x + j] != nullptr)
                     setVisible();
               }
            }

            for (int i = 0; i < 4; i++)
               if (getWorld()->isBoulder(up, x, y - 4 + i))
                  setVisible();
         }

         if (getWorld()->iceBlock[y + move + 3][x] != nullptr || move > 3 || getWorld()->isBoulder(up, x, y - 1 + move))
         {
            setVisible();
            ammo = false;
            break;
         }

         if (gotEm == false)
         {
            if (getWorld()->protesterRuled(getX(), getY() + move, getWorld()->pshot))
               gotEm == true;
         }

         if (y + move < 60)
            moveTo(x, y + move);
         break;

      case down:
         if (move == 0)
         {
            for (int i = 0; i < 4; i++)
            {
               for (int j = 0; j < 4; j++)
               {
                  if (getWorld()->iceBlock[y + i][x + j] != nullptr)
                     setVisible();
               }
            }

            for (int i = 0; i < 4; i++)
               if (getWorld()->isBoulder(down, x, y + 4 - i))
                  setVisible();
         }

         if (getWorld()->iceBlock[y - move][x] != nullptr || move > 3 || getWorld()->isBoulder(down, x, y + 1 - move))
         {
            setVisible();
            ammo = false;
            break;
         }

         if (gotEm == false)
         {
            if (getWorld()->protesterRuled(getX(), getY() - move, getWorld()->pshot))
               gotEm == true;
         }

         if (y - move > -1)
            moveTo(x, y - move);
         break;
   }
   move++;
};

int Squirt::getStatus()
{
   if (ammo == false)
   {
      return 0;
   }
   else
      return -1;
};


////////////////////////////////////////////////
/////////// ActivatingAgent class //////////////
////////////////////////////////////////////////
ActivatingAgent::ActivatingAgent(int ID, int col, int row, StudentWorld* sworld) : ActorInterface(ID, col, row, right, 1.0, 2, sworld)
{
   GraphObject::setVisible(false);
   agent_ID = ID;
   distance = 100.0;
   x = col;
   y = row;
   active = false;
}

void ActivatingAgent::doSomething()
{
   if (abs(x - getWorld()->player->getX()) <= 4.0 || abs(y - getWorld()->player->getY()) <= 4.0)
   {
      distance = pow(x - getWorld()->player->getX(), 2.0) + pow(y - getWorld()->player->getY(), 2.0);
      distance = sqrt(distance);
      if (distance <= 4.0)
      {
         GraphObject::setVisible(true);
         if (distance <= 3.0)
         {
            active = true;
            switch (agent_ID)
            {
            case IID_BARREL:
               getWorld()->playSound(SOUND_FOUND_OIL);
               break;
            case IID_GOLD:
            case IID_SONAR:
            case IID_WATER_POOL:
               getWorld()->playSound(SOUND_GOT_GOODIE);
               break;
            }
         }

         return;
      }
   }
}


////////////////////////////////////////////////
////////////////// Oil class ///////////////////
////////////////////////////////////////////////
Oil::Oil(int col, int row, StudentWorld* sworld) : ActivatingAgent(IID_BARREL, col, row, sworld)
{
   GraphObject::setVisible(false);
}

Oil::~Oil() {};

void Oil::doSomething()
{
   if (getWorld()->getNumberOfBarrelsRemaining(false) <= 0)
      return;

   ActivatingAgent::doSomething();

   if (ActivatingAgent::active == true)
   {
      getWorld()->getNumberOfBarrelsRemaining(true);
      getWorld()->increaseScore(1000);
   }
};

int Oil::getStatus()
{
   if (ActivatingAgent::active == true)
   {
      setVisible();
      return 0;
   }
   else
      return -1;
}


////////////////////////////////////////////////
///////////////// Gold class ///////////////////
////////////////////////////////////////////////
Gold::Gold(int col, int row, StudentWorld* sworld, bool pickupAble) : ActivatingAgent(IID_GOLD, col, row, sworld)
{
   state = pickupAble;    //if false then iceman dropped it to bribe a protester 
   temp = 100;
   found = false;

   if (pickupAble == true)
      GraphObject::setVisible(false);
   else 
      GraphObject::setVisible(true);
};

Gold::~Gold() {};

void Gold::doSomething() 
{
   if (state == true)      //code for gold that the iceman can pick up
   {
      ActivatingAgent::doSomething();

      if (ActivatingAgent::active == true)
      {
         getWorld()->player->getPlayerGold(1);
         getWorld()->increaseScore(10);
         found = true;
      }
   }
   else if (state == false)
   {
      if (getWorld()->protesterRuled(getX(), getY(), getWorld()->pgold))
         found = true;
      temp--;
   }
};
int Gold::getStatus() 
{
   if (found == true || temp <= 0)
   {
      setVisible();
      return 0;
   }
   else
      return -1;
};


////////////////////////////////////////////////
/////////////// SonarKit class /////////////////
////////////////////////////////////////////////
SonarKit::SonarKit(int col, int row, StudentWorld* sworld) : ActivatingAgent(IID_SONAR, col, row, sworld)
{
   GraphObject::setVisible(true);
   found = false;
   T = max(100, (300 - 10 * getWorld()->getCurrentGameLevel()));
};

void SonarKit::doSomething()
{
   T--;

   if (T <= 0)
      return;
   else
      ActivatingAgent::doSomething();

   if (ActivatingAgent::active == true)
   {
      found = active;
      getWorld()->player->getPlayerSonarCharge(1);
      getWorld()->increaseScore(75);
   }
};

int SonarKit::getStatus()
{
   if (found == true || T <= 0)
   {
      setVisible();
      return 0;
   }
   else
      return -1;
};

////////////////////////////////////////////////
/////////////// WaterPool class ////////////////
////////////////////////////////////////////////
waterPool::waterPool(int col, int row, StudentWorld* sworld) : ActivatingAgent(IID_WATER_POOL, col, row, sworld)
{
   GraphObject::setVisible(true);
   found = false;
   evaporate = max(100, 300 - 10 * getWorld()->getCurrentGameLevel());
};

void waterPool::doSomething()
{
   evaporate--;

   if (evaporate <= 0)
      return;
   else
      ActivatingAgent::doSomething();

   if (ActivatingAgent::active == true)
   {
      found = active;
      getWorld()->player->getSquirtsLeft(5);
      getWorld()->increaseScore(100);
   }
};

int waterPool::getStatus()
{
   if (found == true || evaporate <= 0)
   {
      setVisible();
      return 0;
   }
   else
      return -1;
};