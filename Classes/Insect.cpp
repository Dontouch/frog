#include "Insect.h"

bool Insect::init(){
	auto visibleSize = Director::getInstance()->getVisibleSize();
	_uptime = 1.0f;
	_upY = visibleSize.height*0.208333;//200
	_downtime = 1.0f;
	_downY = -visibleSize.height*0.46875; // -450
	return true;
}

void Insect::move(float uptime, int upY, float downtime, int downY){
	auto up = EaseSineIn::create(MoveBy::create(uptime, Point(0, upY)));
	auto down = EaseSineOut::create(MoveBy::create(downtime, Point(0, downY)));
	auto callfunc = CallFunc::create([=](){ move(_uptime, _upY,_downtime,_downY); });
	auto sequence = Sequence::create(down, up, callfunc, NULL);
	this->runAction(sequence);
}

void Insect::reset(){
	auto visibleSize = Director::getInstance()->getVisibleSize();
	_uptime = 1.0f;
	_upY = visibleSize.height*0.208333;
	_downtime = 1.0f;
	_downY = -visibleSize.height*0.46875;
}

bool Insect::downSpeed(){
	_upY -= 40;
	_downY += 50;
	if (_upY <= 80){
		return true;
	}
	return false;
}

void Insect::run(){
	SpriteFrameCache* spriteFrameCache = SpriteFrameCache::getInstance();
	Vector<SpriteFrame*> frameVec;
	SpriteFrame* spriteFrame;
	for (int i = 2; i >= 1; i--){
		spriteFrame = spriteFrameCache->getSpriteFrameByName(StringUtils::format("insect%d.png", i));
		frameVec.pushBack(spriteFrame);
	}
	Animation* animation = Animation::createWithSpriteFrames(frameVec);
	animation->setLoops(-1);
	animation->setDelayPerUnit(0.25f);
	getSprite()->runAction(Animate::create(animation));
}
