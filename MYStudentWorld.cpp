#include "MYStudentWorld.h"
#include "GameConstants.h"
#include <string>
#include <sstream>
#include <iostream>
#include <iomanip>
#include <iterator>
using namespace std;
///ESSENTIALS//////
GameWorld* createStudentWorld(string assetPath)
{
	return new StudentWorld(assetPath);
}

StudentWorld::StudentWorld(string assetPath)
: GameWorld(assetPath), PointerToWorld(this), Bonus(5000)
{}

StudentWorld::~StudentWorld()
{cleanUp();}

int StudentWorld::init()
{
int N = VIEW_HEIGHT / SPRITE_HEIGHT;
int M = VIEW_HEIGHT / (4*SPRITE_HEIGHT);
//initialize RACER
RACER = new GhostRacer(128, 32, PointerToWorld);
//create road
for (int j = 0; j < N; j++)
	{
     ACTORS.push_back(new BorderLine(IID_YELLOW_BORDER_LINE, LEFT_EDGE, j * SPRITE_HEIGHT, PointerToWorld));
     ACTORS.push_back(new BorderLine(IID_YELLOW_BORDER_LINE, RIGHT_EDGE, j * SPRITE_HEIGHT, PointerToWorld));
     }
for (int j = 0; j < M; j++)
	{
     ACTORS.push_back(new BorderLine(IID_WHITE_BORDER_LINE, LEFT_ROAD_BOUND, j * (4*SPRITE_HEIGHT), PointerToWorld));
     ACTORS.push_back(new BorderLine(IID_WHITE_BORDER_LINE, RIGHT_ROAD_BOUND, j * (4*SPRITE_HEIGHT), PointerToWorld));
	}
//SCOREBOARD
ostringstream GameStatText;
GameStatText << "Score: " << getScore() << setw(7) << "Lvl: " << getLevel() << setw(14) << "Souls2Save: " << getLevel()*2 + 5 - RACER->getSoulsSaved() << setw(9) << "Lives: " << getLives() << setw(10) << "Health: " << RACER->getHitPoints() << setw(10) << "Sprays: " << RACER->getWater() << setw(9) << "Bonus: " << getBonus();
string text = GameStatText.str();
setGameStatText(text);
return GWSTATUS_CONTINUE_GAME;
}

int StudentWorld::move()
{
addRoadMarkers();
addHumanPeds();
addZombiePeds();
addZombieCabs();
addOilSlicks();
addHolyWater();
addLostSouls();


if (RACER->isAlive())
	RACER->doSomething();
for (int j = 0; j <ACTORS.size(); j++)
{
	if (ACTORS[j]->isAlive())
		{
			ACTORS[j]->doSomething();
            if (!RACER->isAlive())
            	{decLives();
             	 Bonus = 5000;
            	return GWSTATUS_PLAYER_DIED;}
            if (RACER->getSoulsSaved() == 2*getLevel() + 5)
            	{increaseScore(getBonus());
                playSound(SOUND_FINISHED_LEVEL);
				return GWSTATUS_FINISHED_LEVEL;}
   		}
}

for (int j = 0; j < ACTORS.size(); j++)
{if (!ACTORS[j]->isAlive())
	{Actor* p = ACTORS[j];
	ACTORS.erase(ACTORS.begin() + j);
	delete p;}}
 
ostringstream GameStatText;
GameStatText << "Score: " << getScore() << setw(7) << "Lvl: " << getLevel() << setw(14) << "Souls2Save: " << 2 * getLevel() + 5 - RACER->getSoulsSaved() << setw(9) << "Lives: " << getLives() << setw(10) << "Health: " << RACER->getHitPoints() << setw(10) << "Sprays: " << RACER->getWater() << setw(9) << "Bonus: " << getBonus();
string text = GameStatText.str();
setGameStatText(text);

if (Bonus > 0)
	Bonus--;
return GWSTATUS_CONTINUE_GAME;
}

void StudentWorld::cleanUp()
{
for (int j = 0; j < ACTORS.size(); j++)
	{delete ACTORS[j];}
ACTORS.clear();
delete RACER;
}

////GENERATE ACTORS////////
void StudentWorld::addActor(Actor* a)
{ACTORS.push_back(a);}

void StudentWorld::addRoadMarkers()
{
int new_border_y = VIEW_HEIGHT - SPRITE_HEIGHT;
int delta_y = new_border_y - lastWhiteLine->getY();
if (delta_y >= SPRITE_HEIGHT)
	{BorderLine* yellowLeft = new BorderLine(IID_YELLOW_BORDER_LINE, LEFT_EDGE, new_border_y, PointerToWorld);
     BorderLine* yellowRight = new BorderLine(IID_YELLOW_BORDER_LINE, RIGHT_EDGE, new_border_y, PointerToWorld);
     addActor(yellowLeft); addActor(yellowRight);}

if (delta_y >= 4*SPRITE_HEIGHT)
	{
	BorderLine* whiteLeft = new BorderLine(IID_WHITE_BORDER_LINE, LEFT_ROAD_BOUND, new_border_y, PointerToWorld);
     BorderLine* whiteRight = new BorderLine(IID_WHITE_BORDER_LINE, RIGHT_ROAD_BOUND, new_border_y, PointerToWorld);
     addActor(whiteLeft); addActor(whiteRight);
    }
}

void StudentWorld::addOilSlicks()
{
int ChanceOilSlick = max(150 - getLevel() * 10, 40);
int a = randInt(0, ChanceOilSlick-1);
if (a == 0)
	{OilSlick* newOilSlick = new OilSlick(randInt(LEFT_EDGE + 1, RIGHT_EDGE - 1), VIEW_HEIGHT, PointerToWorld);
	addActor(newOilSlick);
    }
}

void StudentWorld::addHolyWater()
{int ChanceOfHolyWater = 100 + 10 * getLevel();
 int a = randInt(0, ChanceOfHolyWater-1);
 if (a == 0)
 { HolyWaterGoodie* newHolyWater = new HolyWaterGoodie(randInt(LEFT_EDGE+1, RIGHT_EDGE-1), VIEW_HEIGHT, PointerToWorld);
  addActor(newHolyWater);}
}

void StudentWorld::addLostSouls()
{int ChanceOfLostSoul = 100;
int a = randInt (0, ChanceOfLostSoul-1);
 if (a == 0)
 { SoulGoodie* newLostSoul = new SoulGoodie(randInt(RIGHT_EDGE-1, LEFT_EDGE+1), VIEW_HEIGHT, PointerToWorld);
  addActor(newLostSoul);}
}

void StudentWorld::addHumanPeds()
{int ChanceHumanPed = max(200 - getLevel() * 10, 30);
 int a = randInt(0, ChanceHumanPed-1);
 if (a == 0)
 	{HumanPedestrian* newHuman = new HumanPedestrian(randInt(0,VIEW_WIDTH), VIEW_HEIGHT, PointerToWorld);
  	addActor(newHuman);}
}

void StudentWorld::addZombiePeds()
{int ChanceZombiePed = max(200 - getLevel() * 10, 20);
 int a = randInt(0, ChanceZombiePed-1);
 if (a == 0)
 	{ZombiePedestrian* newZombiePed = new ZombiePedestrian(randInt(0, VIEW_WIDTH), VIEW_HEIGHT, PointerToWorld);
  	addActor(newZombiePed);}
}

void StudentWorld::addZombieCabs()
{
int LaneCenters[3] = {MIDDLE_LEFT, MIDDLE_MIDDLE, MIDDLE_RIGHT};
bool checked[3] = {false, false, false};
int chosenLane = -1;
int newY = -1;
int newSpeed = -1;
int ChanceVehicle = max(100 - getLevel() * 10, 20);
int a = randInt(0, ChanceVehicle-1);
if (a == 0)
	{
	int startLane = randInt(0, 2);
	int cur_lane = LaneCenters[startLane];
	int yMinInLane = inLaneMaxMin(cur_lane, false);
    int yMaxInLane = inLaneMaxMin(cur_lane, true);
    if (yMinInLane > (VIEW_HEIGHT/3) || yMinInLane == -1)
        	{newY = SPRITE_HEIGHT/2;
             newSpeed = RACER->get_vSpeed() + randInt(2,4);
             chosenLane = cur_lane;}
    if (yMaxInLane < (VIEW_HEIGHT * 2/3) || yMaxInLane == -1)
        	 {newY = VIEW_HEIGHT-SPRITE_HEIGHT/2;
              newSpeed = RACER->get_vSpeed() - randInt(2,4);
              chosenLane = cur_lane;}
              
    checked[startLane] = true;
    
    if (chosenLane == -1)
    {
    for (int j=0; j < 3; j++)
		{if (checked[j] == false)
        {
         int cur_lane = LaneCenters[j];
         int yMinInLane = inLaneMaxMin(cur_lane, false);
         int yMaxInLane = inLaneMaxMin(cur_lane, true);
         if (yMinInLane > (VIEW_HEIGHT/3) || yMinInLane == -1)
        	{newY = SPRITE_HEIGHT/2;
             newSpeed = RACER->get_vSpeed() + randInt(2,4);
             chosenLane = cur_lane;
             break;}
   		 if (yMaxInLane < (VIEW_HEIGHT * 2/3) || yMaxInLane == -1)
        	 {newY = VIEW_HEIGHT - SPRITE_HEIGHT / 2;
              newSpeed = RACER->get_vSpeed() - randInt(2,4);
              chosenLane = cur_lane;
              break;}}
    	}
	}
if (chosenLane != -1)
	{
	ZombieCab* newZombieCab = new ZombieCab(chosenLane, newY, newSpeed, PointerToWorld);
	addActor(newZombieCab);
	}
}
}

///HELPER FUNCTIONS////////////
Actor* StudentWorld::waterActivated(HolyWaterSpray* spray)
{
for (int i = 0; i < ACTORS.size(); i++)
{
if (overlap(spray, ACTORS[i]) && ACTORS[i]->isItAffectedByWater())
	return ACTORS[i];
}
return NULL;
}

bool StudentWorld::overlap(Actor* a, Actor* b)
{
double delta_x= abs(a->getX() - b->getX());
 double delta_y = abs(a->getY() - b->getY());
 double radius_sum = a->getRadius() + b->getRadius();
 if (delta_x < radius_sum*.25 && delta_y < radius_sum*.6)
 	return true;
 return false;
}

Actor* StudentWorld::inLaneFrontBack(int xCoord, int yCoord, bool frontOrBack)
{Actor* closest = NULL;
 int min = VIEW_HEIGHT;
 
 for (int n = 0; n < ACTORS.size(); n++)
	{if (ACTORS[n]->isCollision())
    {if ((frontOrBack && ACTORS[n]->getY()>yCoord && inLane(xCoord,ACTORS[n]->getX()))
    || (!frontOrBack && ACTORS[n]->getY()<yCoord && inLane(xCoord,ACTORS[n]->getX())))
          {if (abs(ACTORS[n]->getY()-yCoord) < min)
            closest = ACTORS[n]; min = abs(ACTORS[n]->getY()-yCoord);}
    }
    }
return closest;
}

bool StudentWorld::inLane(int xCoord, int otherX)
{
 if (xCoord >= LEFT_EDGE && xCoord < LEFT_ROAD_BOUND)
     {if ( otherX >= LEFT_EDGE && otherX < LEFT_ROAD_BOUND)
            return true;}
 if (xCoord >= LEFT_ROAD_BOUND && xCoord < RIGHT_ROAD_BOUND)
     {if (otherX >= LEFT_ROAD_BOUND && otherX < RIGHT_ROAD_BOUND)
             return true;}
 if (xCoord >= RIGHT_ROAD_BOUND && xCoord < RIGHT_EDGE)
     {if (otherX >= RIGHT_ROAD_BOUND && otherX < RIGHT_EDGE)
             return true;}
return false;
}

int StudentWorld::inLaneMaxMin(int xCoord, bool topOrBottom)
{
int max = 0;
int min = VIEW_HEIGHT;
bool somethingInLane = false;

if (inLane(xCoord, RACER->getX()))
	{somethingInLane = true;
     max = RACER->getY();
	 min = RACER->getY();}
    
for (int n = 0; n < ACTORS.size(); n++)
	{if (inLane(xCoord, ACTORS[n]->getX()) && ACTORS[n]->isCollision())
     	{somethingInLane = true;
		 if (ACTORS[n]->getY() > max)
        	max = ACTORS[n]->getY();}}
        
for (int n = 0; n < ACTORS.size(); n++)
	{if (inLane(xCoord, ACTORS[n]->getX()) && ACTORS[n]->isCollision())
		{somethingInLane = true;
    	 if (ACTORS[n]->getY() < min)
        	min = ACTORS[n]->getY();}}
            
if (somethingInLane)
	{if (topOrBottom)
		return max;
	return min;}
return -1;
}
