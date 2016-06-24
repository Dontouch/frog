#ifndef _ENTITY_H_
#define _ENTITY_H_

#include "cocos2d.h"
USING_NS_CC;

class Entity :public Node{
public:
	void bindSprite(Sprite* sprite);
	Sprite* getSprite();
private:
	Sprite* _sprite;
};

#endif

