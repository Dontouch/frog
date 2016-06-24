#ifndef _MUSHROOM_H_
#define _MUSHROOM_H_

#include "Entity.h"

class Mushroom :public Entity{
public:
	CREATE_FUNC(Mushroom);
	virtual bool init();
	void move(float uptime, int upY, float downtime, int downY);
	void reset();
	bool downSpeed();
private:
	float _uptime;
	int _upY;
	float _downtime;
	int _downY;
};

#endif
