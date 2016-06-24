#ifndef _MOSQUITO_H_
#define _MOSQUITO_H_
#include "Entity.h"

class Mosquito :public Entity{
public:
	CREATE_FUNC(Mosquito);
	virtual bool init();
	void move(float uptime, int upY, float downtime, int downY);
	void reset();
	bool downSpeed();
	void run();
private:
	float _uptime;
	int _upY;
	float _downtime;
	int _downY;
};

#endif
