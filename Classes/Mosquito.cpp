#include "Mosquito.h"

bool Mosquito::init(){
	auto visibleSize = Director::getInstance()->getVisibleSize();
	_uptime = 1.0f;
	_upY = visibleSize.height*0.208333;//200
	_downtime = 1.0f;
	_downY = -visibleSize.height*0.46875; // -450
	return true;
}

void Mosquito::move(float uptime, int upY, float downtime, int downY){
	auto up = EaseSineIn::create(MoveBy::create(uptime, Point(0, upY)));
	auto down = EaseSineOut::create(MoveBy::create(downtime, Point(0, downY)));
	auto callfunc = CallFunc::create([=](){ move(_uptime, _upY, _downtime, _downY); });
	auto sequence = Sequence::create(down, up, callfunc, NULL);
	this->runAction(sequence);
}

void Mosquito::reset(){
	auto visibleSize = Director::getInstance()->getVisibleSize();
	_uptime = 1.0f;
	_upY = visibleSize.height*0.208333;
	_downtime = 1.0f;
	_downY = -visibleSize.height*0.46875;
}

bool Mosquito::downSpeed(){
	_upY -= 40;
	_downY += 50;
	if (_upY <= 80){
		return true;
	}
	return false;
}

void Mosquito::run(){
	SpriteFrameCache* spriteFrameCache = SpriteFrameCache::getInstance();
	Vector<SpriteFrame*> frogFrameVec;
	SpriteFrame* spriteFrame;
	for (int i = 2; i >= 1; i--){
		spriteFrame = spriteFrameCache->getSpriteFrameByName(StringUtils::format("mosquito%d.png", i));
		frogFrameVec.pushBack(spriteFrame);
	}
	Animation* animation = Animation::createWithSpriteFrames(frogFrameVec);
	animation->setLoops(-1);
	animation->setDelayPerUnit(0.2f);
	getSprite()->runAction(Animate::create(animation));
}



