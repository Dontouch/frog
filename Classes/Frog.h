#ifndef _FROG_H_
#define _FROG_H_

#include "Entity.h"
class Frog:public Entity{
public:
	CREATE_FUNC(Frog);
	virtual bool init();
	void jump(float uptime, int upY, float downtime, int downY);
	void jump2(float scale_time, float scale_times);
	void run();
private:
	float _uptime;
	int _upY;
	float _downtime;
	int _downY;
};

#endif
