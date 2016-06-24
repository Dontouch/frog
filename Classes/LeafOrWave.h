#ifndef _LEAFORWAVE_H_
#define _LEAFORWAVE_H_

#include "Entity.h"

class LeafOrWave :public Entity{
public:
	CREATE_FUNC(LeafOrWave);
	virtual bool init();
	void move(float downtime, int downY,bool flag);
};

#endif
