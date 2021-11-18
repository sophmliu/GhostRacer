#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "GameWorld.h"
#include <string>
#include <vector>
#include "MYActor.h"
#include <string>

using namespace std;
class StudentWorld : public GameWorld
{
public:
    //CLASS CONSTANTS
	const int LEFT_EDGE = ROAD_CENTER - ROAD_WIDTH/2;
	const int RIGHT_EDGE = ROAD_CENTER + ROAD_WIDTH/2;
	const int LEFT_ROAD_BOUND = LEFT_EDGE + ROAD_WIDTH/3;
    const int RIGHT_ROAD_BOUND = RIGHT_EDGE - ROAD_WIDTH/3;
    const int MIDDLE_MIDDLE = ROAD_CENTER;
    const int MIDDLE_LEFT = ROAD_CENTER - ROAD_WIDTH/3;
    const int MIDDLE_RIGHT = ROAD_CENTER + ROAD_WIDTH/3;
    
private:
	StudentWorld* PointerToWorld;
	GhostRacer* RACER;
	vector<Actor*> ACTORS;
	int Bonus;
	BorderLine* lastWhiteLine;
	
	//GENERATE ACTORS
	void addRoadMarkers();
	void addHumanPeds();
	void addZombiePeds();
	void addZombieCabs();
	void addOilSlicks();
	void addHolyWater();
	void addLostSouls();
	
public:
	//ESSENTIALS
    StudentWorld(std::string assetPath);
    ~StudentWorld();
	virtual int init();
	virtual int move();
	virtual void cleanUp();
	void addActor(Actor* a);
	
	//RETRIEVAL & UPDATING
	GhostRacer* getRacer()
		{return RACER;}
  	void updateLastWhiteLine(BorderLine* b)
   		{lastWhiteLine = b;}
     int getBonus()
		{return Bonus;}
        
    //HELPER
    Actor* waterActivated(HolyWaterSpray* spray);
    bool overlap(Actor* a, Actor* b);
    Actor* inLaneFrontBack(int xCoord, int yCoord, bool frontOrBack);
    bool inLane(int xCoord, int otherX);
    int inLaneMaxMin(int xCoord, bool topOrBottom);
};

#endif // STUDENTWORLD_H_
