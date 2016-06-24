#include "Mushroom.h"

bool Mushroom::init(){
	auto visibleSize = Director::getInstance()->getVisibleSize();
	_uptime = 1.0f;
	_upY = visibleSize.height*0.208333;
	_downtime = 1.0f;
	_downY = -visibleSize.height*0.46875;
	return true;
}

void Mushroom::move(float uptime, int upY, float downtime, int downY){
	auto up = EaseSineIn::create(MoveBy::create(uptime, Point(0, upY)));
	auto down = EaseSineOut::create(MoveBy::create(downtime, Point(0, downY)));
	auto callfunc = CallFunc::create([=](){ move(_uptime, _upY, _downtime, _downY); });
	auto sequence = Sequence::create(down, up, callfunc, NULL);
	this->runAction(sequence);
}

void Mushroom::reset(){
	auto visibleSize = Director::getInstance()->getVisibleSize();
	_uptime = 1.0f;
	_upY = visibleSize.height*0.208333;
	_downtime = 1.0f;
	_downY = -visibleSize.height*0.46875;
}

bool Mushroom::downSpeed(){
	_upY -= 40;
	_downY += 50;
	if (_upY <= 80){
		return true;
	}
	return false;
}
