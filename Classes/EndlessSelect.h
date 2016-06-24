#ifndef _ENDLESSSELECT_H_
#define _ENDLESSSELECT_H_
#include "cocos2d.h"
USING_NS_CC;


//无尽模式
class EndlessSelect :public Layer{
public:
	static Scene* createScene();
	CREATE_FUNC(EndlessSelect);
	virtual bool init();
	void first(Ref *sender);
	void second(Ref* sender);
};

#endif 

