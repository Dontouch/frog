#ifndef _GAMESCENE_H_
#define _GAMESCENE_H_
#include "cocos2d.h"
USING_NS_CC;

class GameScene :public Layer{
public:
	static Scene* createScene();
	CREATE_FUNC(GameScene);
	virtual bool init();
	void story(Ref *sender);
	void endless(Ref* sender);
};

#endif 

