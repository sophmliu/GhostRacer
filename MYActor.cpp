#include <cmath>
#include "MYActor.h"
#include "MYStudentWorld.h"
#define _USE_MATH_DEFINES
#include <math.h>

using namespace std;

//ACTOR FUNCTIONS
Actor::Actor(int imageID, int posX, int posY, int orientation, double direction, double size, int depth, double speed, bool alive, bool collision, StudentWorld* world, int hitPoints)
:GraphObject(imageID, posX, posY, orientation, size, depth), Alive(alive), Collision(collision), Speed(speed), PointerToWorld(world), HitPoints(hitPoints), directionMovement(direction), affectedByWater(false)
{}
void Actor::damage(int pointsSubtracted)
{
setHitPoints(getHitPoints()-pointsSubtracted);
    if (getHitPoints() <= 0)
	{die();
     world()->playSound(SOUND_PLAYER_DIE);}
}
void Actor::move()
{double vert_speed = get_vSpeed() -  world()->getRacer()->get_vSpeed();
 double horiz_speed = get_hSpeed();
 double new_y = getY() + vert_speed;
 double new_x = getX() + horiz_speed;
 moveTo(new_x, new_y);
 if (!inBounds())
	die();}
int Actor::doSomething()
{move();
return 1;}

//GHOST RACER FUNCTIONS
GhostRacer::GhostRacer(int posX, int posY, StudentWorld* world)
:Actor(IID_GHOST_RACER, posX, posY, 90, 90, 4.0, 0, 0, true, true, world, 100), SoulsSaved(0),  Water(10)
{}
int GhostRacer::getSoulsSaved()
{return SoulsSaved;}
int GhostRacer::doSomething()
{
if (!isAlive())
	return 0;
	
int X = getX();
if (X <= world()->LEFT_EDGE && getDirectionMovement()>90)
		{damage(10); setDirection(82); setDirectionMovement(82);
    	world()->playSound(SOUND_VEHICLE_CRASH);}
        
if (X >= world()->RIGHT_EDGE && getDirectionMovement()<90)
		{damage(10); setDirection(98); setDirectionMovement(98);
         world()->playSound(SOUND_VEHICLE_CRASH);}

int ch;
if (world()->getKey(ch))
{switch (ch)
	{
	case KEY_PRESS_SPACE:
		if (getWater()>0)
  			{
              int hw_x = getX() + SPRITE_HEIGHT * cos(getDirectionMovement() * M_PI/180);
              int hw_y = getY() + SPRITE_HEIGHT * sin(getDirectionMovement() * M_PI/180);
              HolyWaterSpray* hw = new HolyWaterSpray(hw_x, hw_y, getDirectionMovement(), world());
             world()->addActor(hw);
             world()->playSound(SOUND_PLAYER_SPRAY);
             adjustWater(-1);
            }
		break;
	case KEY_PRESS_LEFT:
		if (getDirectionMovement()<114)
			setDirectionMovement(getDirectionMovement()+8);
   			setDirection(getDirectionMovement());
		break;
	case KEY_PRESS_RIGHT:
		if (getDirectionMovement()>66)
  			setDirectionMovement(getDirectionMovement()-8);
     		setDirection(getDirectionMovement());
		break;
    case KEY_PRESS_UP:
    	if (getSpeed()<5)
     		adjustSpeed(1);
       break;
    case KEY_PRESS_DOWN:
    	if (getSpeed()>-1)
     		adjustSpeed(-1);
       break;
	}
}
move();
return 1;}
void GhostRacer::move()
{double max_shift_per_tick = 4.0;
 int direction = getDirectionMovement();
 double delta_x = cos(direction * M_PI/180) * max_shift_per_tick;
 moveTo(getX() + delta_x, getY());
}

void GhostRacer::spin()
{
 int direction = randInt(0,1);
 int magnitude = randInt(5,20);
 int spin;
 if (direction == 0)
 	spin = -1 * magnitude;
 else
 	spin = magnitude;
 if (getDirection() + spin < 60)
 	{setDirection(60); setDirectionMovement(getDirection());}
 if (getDirection() + spin > 120)
 	{setDirection(120); setDirectionMovement(getDirection());}
 else
 	{setDirection(getDirection() + spin); setDirectionMovement(getDirection());}
}

//BORDERLINE FUNCTIONS
BorderLine::BorderLine(int imageID, int posX, int posY, StudentWorld* world)
:Actor(imageID, posX, posY, 0, 90, 2.0, 1, -4.0, true, false, world, 0)
{
if (posX == world->LEFT_ROAD_BOUND || posX == world->RIGHT_ROAD_BOUND)
	world->updateLastWhiteLine(this);
set_vSpeed(-4.0); set_hSpeed(0);
}

//OIL SLICK FUNCTIONS
OilSlick::OilSlick(int x, int y, StudentWorld* world)
:Actor(IID_OIL_SLICK, x, y, 0, 0, 2.0, 0, 4, true, false, world, 0)
{
set_hSpeed(0);
set_vSpeed(-4);
}

int OilSlick::doSomething()
{
move();
if (world()->overlap(this, world()->getRacer()))
	world()->getRacer()->spin();
return 1;
}

//HOLY WATER SPRAY FUNCTIONS
HolyWaterSpray::HolyWaterSpray(int posX, int posY, int direction, StudentWorld* world)
:Actor(IID_HOLY_WATER_PROJECTILE, posX, posY, direction, direction, 1.0, 1, 0, true, false, world, 0)
{}

int HolyWaterSpray::doSomething()
{
if (!isAlive())
	return 0;
if (world()->waterActivated(this) != NULL)
	{world()->waterActivated(this)->damage(1);
	 die();
  	 return 1;
    }
move();
return 1;
}

void HolyWaterSpray::move()
{
moveForward();
pixelsMoved++;
if (!inBounds())
	{die();
	return;}
if (pixelsMoved >= 160)
	die();
}

//GOODIE FUNCTIONS
Goodie::Goodie(int imageID, int posX, int posY, int orientation, int directionMovement, double size, int depth, StudentWorld* world)
:Actor(imageID, posX, posY, orientation, directionMovement, size, depth, -4, true, false, world, 0)
{
set_vSpeed(-4); set_hSpeed(0);
}

//HEALING GOODIE FUNCTIONS
HealingGoodie::HealingGoodie(int x, int y, StudentWorld* world)
:Goodie(IID_HEAL_GOODIE, x, y, 0, 90, 1.0, 2, world)
{isAffectedByWater(); setHitPoints(0);}

int HealingGoodie::doSomething()
{
move();
if (world()->overlap(this, world()->getRacer()))
	{
	 world()->getRacer()->setHitPoints(getHitPoints()+10);
     die();
     world()->playSound(SOUND_GOT_GOODIE);
     world()->increaseScore(250);
    }
return 1;
}

HolyWaterGoodie::HolyWaterGoodie(int x, int y, StudentWorld* world)
:Goodie(IID_HOLY_WATER_GOODIE, x, y, 90, 90, 2.0, 2, world)
{isAffectedByWater(); setHitPoints(0);}

//HOLY WATER GOODIE FUNCTIONS
int HolyWaterGoodie::doSomething()
{
move();
if(world()->overlap(this,world()->getRacer()))
	{
	world()->getRacer()->adjustWater(10);
	die();
	world()->playSound(SOUND_GOT_GOODIE);
	world()->increaseScore(50);
    }
return 1;}

//SOUL GOODIE FUNCTIONS
SoulGoodie::SoulGoodie(int x, int y, StudentWorld* world)
:Goodie(IID_SOUL_GOODIE, x, y, 0, 90, 4.0, 2, world)
{}

int SoulGoodie::doSomething()
{move();
if (world()->overlap(this, world()->getRacer()))
	{
	world()->getRacer()->incSoulsSaved();
	die();
	world()->playSound(SOUND_GOT_SOUL);
	world()->increaseScore(100);
    }
setDirection(getDirection()-10);
return 1;}

//INTELLIGENT ACTOR FUNCTIONS
Intelligent::Intelligent(int imageID, int posX, int posY, int orientation, int directionMovement, double size, int depth, double horizontalSpeed, double verticalSpeed, StudentWorld* world, int hitPoints)
:Actor(imageID, posX, posY, orientation, directionMovement, size, depth, sqrt(pow(verticalSpeed,2) + pow(horizontalSpeed,2)), true, true, world, hitPoints), MovePlanDis(0)
{set_hSpeed(horizontalSpeed); set_vSpeed(verticalSpeed);}

void Intelligent::newMovePlanDis()
{
set_hSpeed(rand()% 7 + -3);
while (get_hSpeed() == 0)
    set_hSpeed(rand()% 7 + -3);
setMovePlanDis(rand()%29 + 4);
if (get_hSpeed() < 0)
    {setDirection(180); setDirectionMovement(180);}
if (get_hSpeed() > 0)
    {setDirection(0); setDirectionMovement(0);}
}

//HUMAN PEDESTRIAN FUNCTIONS
HumanPedestrian::HumanPedestrian(int posX, int posY, StudentWorld* world)
:Intelligent(IID_HUMAN_PED, posX, posY, 0, 90, 2.0, 0, 0, -4, world, 2)
{isAffectedByWater();}

int HumanPedestrian::doSomething()
{
if (!isAlive())
    return 0;
if (world()->overlap(this,world()->getRacer()))
    {world()->getRacer()->die(); return 1;}
move();
decMovePlanDis();
if (getMovePlanDis() > 0)
    return 1;
newMovePlanDis();
return 1;
}

void HumanPedestrian::damage(int pointsSubtracted)
{
set_hSpeed(get_hSpeed()*-1);
if (getDirection() == 180)
	setDirection(0);
if (getDirection() == 0)
	setDirection(180);
world()->playSound(SOUND_PED_HURT);
}

//ZOMBIE PEDESTRIAN FUNCTIONS
ZombiePedestrian::ZombiePedestrian(int posX, int posY, StudentWorld* world)
:Intelligent(IID_ZOMBIE_PED, posX, posY, 0, 90, 3.0 , 0, 0, -4, world, 2), nextGrunt(0)
{isAffectedByWater();}

int ZombiePedestrian::doSomething()
{
if (!isAlive())
	return 0;
if (world()->overlap(this, world()->getRacer()))
	{world()->getRacer()->damage(5);
	 damage(2);
     return 1;}
if (abs(getX() - world()->getRacer()->getX()) <= 30 && getY() > world()->getRacer()->getY())
	{setDirection(270); setDirectionMovement(270);
	if (getX()-world()->getRacer()->getX() < 0)
		set_hSpeed(1);
  	else if (getX()-world()->getRacer()->getX() > 0)
   		set_hSpeed(-1);
    else if (getX()==world()->getRacer()->getX())
    	set_hSpeed(0);
    nextGrunt--;
    if (nextGrunt <= 0)
    	{world()->playSound(SOUND_ZOMBIE_ATTACK);
         nextGrunt = 20;}
    }
move();
if (getMovePlanDis() > 0)
	{decMovePlanDis(); return 1;}
newMovePlanDis();
return 1;
}
void ZombiePedestrian::damage(int a)
{setHitPoints(getHitPoints()-a);
if (getHitPoints() <= 0)
	{die();
     world()->playSound(SOUND_PED_DIE);
     if (!world()->overlap(this, world()->getRacer()))
     	{int a = randInt(1,5);
         if (a == 5)
         	{HealingGoodie* newHealingGoodie = new HealingGoodie(getX(), getY(), world());
          	world()->addActor(newHealingGoodie);}
        }
    world()->increaseScore(150);}
else if (getHitPoints() > 0)
	world()->playSound(SOUND_PED_HURT);
}

//ZOMBIECAB FUNCTIONS 
ZombieCab::ZombieCab(int posX, int posY, double vSpeed, StudentWorld* world)
:Intelligent(IID_ZOMBIE_CAB, posX, posY, 90, 90, 4.0, 0, 0, vSpeed, world, 3), doneDamage(0)
{set_hSpeed(0); isAffectedByWater();}

int ZombieCab::doSomething()
{
if (!isAlive())
	return 0;
if (world()->overlap(this, world()->getRacer()))
	{if (!hasDoneDamage())
		{world()-> playSound(SOUND_VEHICLE_CRASH);
         world()->getRacer()->damage(20);
         if (getX() - world()->getRacer()->getX() <= 0)
         	{set_hSpeed(-5);
             setDirection(120 + randInt(0,19));
             setDirectionMovement(getDirection());}
         if (getX() - world()->getRacer()->getX() > 0)
         	{set_hSpeed(5);
             setDirection(60 - randInt(0,19));
             setDirectionMovement(getDirection());}
        didDamage();
        }
    }
move();
if (get_vSpeed() > world()->getRacer()->get_vSpeed())
	{if (world()->inLaneFrontBack(getX(),getY(),true) != NULL && (world()->inLaneFrontBack(getX(),getY(),true)->getY() - getY()) < 96)
		{set_vSpeed(get_vSpeed()-.5); return 1;}
  	}
if (get_vSpeed() <= world()->getRacer()->get_vSpeed())
	{if (world()->inLaneFrontBack(getX(),getY(),false) != NULL && (getY() - world()->inLaneFrontBack(getX(),getY(),false)->getY()) < 96)
        {set_vSpeed(get_vSpeed()+.5); return 1;}
    }
decMovePlanDis();
if (getMovePlanDis() <= 0)
	{setMovePlanDis(randInt(4,32));
     set_vSpeed(get_vSpeed() + randInt(-2,2));}
return 1;
}

void ZombieCab::damage(int a)
{
setHitPoints(getHitPoints()-a);
if (getHitPoints() <= 0)
	{die();
	world()->playSound(SOUND_VEHICLE_DIE);
    a = randInt(1,5);
    if (a==1)
     	{OilSlick* newOilSlick = new OilSlick(getX(),getY(),world());
     	 world()->addActor(newOilSlick);}
	world()->increaseScore(200);
	return;}
world()->playSound(SOUND_VEHICLE_HURT);}


//HELPER FUNCTIONS
