#ifndef ACTOR_H_
#define ACTOR_H_
#include <string>
#define _USE_MATH_DEFINES
#include <math.h>

#include "GraphObject.h"
class StudentWorld; 

class Actor: public GraphObject
{
public:
	//ESSENTIAL 
	Actor(int imageID, int posX, int posY, int orientation, double direction, double size, int depth, double speed, bool alive, bool collision, StudentWorld* world, int hitPoints);
	virtual int doSomething();
	
        
  	//RETRIEVAL & UPDATING VARIABLES
   double getSpeed()
		{return Speed;}
	void adjustSpeed(int a)
		{Speed += a;}
	StudentWorld* world()
		{return PointerToWorld;}
	double getDirectionMovement()
		{return directionMovement;}
	void setDirectionMovement(double a)
		{directionMovement = a;}
    int getHitPoints()
		{return HitPoints;}
	void setHitPoints(int a)
		{HitPoints = a;}
	virtual double get_hSpeed()
		{return hSpeed;}
	virtual double get_vSpeed()
		{return vSpeed;}
	void set_hSpeed(int a)
		{hSpeed = a;}
	void set_vSpeed(int a)
		{vSpeed = a;}
        
  	//HELPER & BOOLS
    void isAffectedByWater()
    	{affectedByWater = true;}
    bool isItAffectedByWater()
        {return affectedByWater;}
	bool inBounds()
		{return (getX() >= 0 && getX() < VIEW_WIDTH && getY() >= 0 && getY() < VIEW_HEIGHT);}
	bool isAlive()
		{return Alive;}
	bool isCollision()
		{return Collision;}
  
    //CHANGES SCOREBOARD
    virtual void damage(int pointsSubtracted);
	void die()
		{Alive = false;}
        
protected:
    virtual void move();
    
private:
	bool affectedByWater;
	int HitPoints;
	double hSpeed;
	double vSpeed;
	double directionMovement;
	StudentWorld* PointerToWorld;
	bool Alive;
	bool Collision;
	double Speed;
};

class Goodie: public Actor
{
public:
Goodie(int imageID, int posX, int posY, int orientation, int directionMovement, double size, int depth, StudentWorld* world); 
};

class Intelligent: public Actor
{
public:
	Intelligent(int imageID, int posX, int posY, int orientation, int directionMovement, double size, int depth, double verticalSpeed, double horizontalSpeed, StudentWorld* world, int hitPoints);
	double getMovePlanDis()
		{return MovePlanDis;}
	void setMovePlanDis(int a)
		{MovePlanDis = a;}
	void newMovePlanDis();
    void decMovePlanDis()
		{MovePlanDis--;}

private:
	double MovePlanDis;
};

class HolyWaterSpray: public Actor
{
public:
HolyWaterSpray(int posX, int posY, int direction, StudentWorld* world);
virtual int doSomething();
virtual void move();

private:
int pixelsMoved;
};

class GhostRacer: public Actor
{
public:
	GhostRacer(int posX, int posY, StudentWorld* world);
	virtual int doSomething();
	virtual void move();
	int getSoulsSaved();
	void incSoulsSaved()
		{SoulsSaved++;}
	int getWater()
		{return Water;}
	void adjustWater(int adjustWaterBy)
		{Water += adjustWaterBy;}
    void spin();
	virtual double get_vSpeed()
		{return getSpeed() * sin(getDirectionMovement() * M_PI/180);}
	virtual double get_hSpeed()
		{return getSpeed() * cos(getDirectionMovement() * M_PI/180);}

private:
	int Water;
	int SoulsSaved;
};

class BorderLine: public Actor
{
public:
	BorderLine(int imageID, int posX, int posY, StudentWorld* world);
};

class OilSlick: public Actor
{
public:
	OilSlick(int x, int y, StudentWorld* world);
	virtual int doSomething();
};

class HumanPedestrian: public Intelligent
{
public:
	HumanPedestrian(int posX, int posY, StudentWorld* world);
	virtual int doSomething();
	virtual void damage(int pointSubtracted);

};

class ZombiePedestrian: public Intelligent
{
public:
	ZombiePedestrian(int posX, int posY, StudentWorld* world);
	virtual int doSomething();
	virtual void damage(int a);

private:
	int nextGrunt; 
};

class ZombieCab: public Intelligent
{
public:
	ZombieCab(int posX, int posY, double vSpeed, StudentWorld* world);
	virtual int doSomething();
    bool hasDoneDamage()
    	{return doneDamage;}
    void didDamage()
    	{doneDamage = true;}
    virtual void damage(int a);
    
private:
	bool doneDamage;
};

class HealingGoodie: public Goodie
{
public:
HealingGoodie(int x, int y, StudentWorld* world);
virtual int doSomething();

};

class HolyWaterGoodie: public Goodie
{
public:
HolyWaterGoodie(int x, int y, StudentWorld* world);
virtual int doSomething();
};

class SoulGoodie: public Goodie
{
public:
SoulGoodie(int x, int y, StudentWorld* world);
virtual int doSomething();
};


#endif // ACTOR_H_


