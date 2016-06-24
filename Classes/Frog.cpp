#include "Frog.h"

bool Frog::init(){
	auto visibleSize = Director::getInstance()->getVisibleSize();
	_uptime = 1.0f;
	_upY = visibleSize.height * 0.2;
	_downtime = 1.0f;
	_downY = -visibleSize.height * 0.2;

	return true;
}

void Frog::jump(float uptime, int upY, float downtime, int downY){
	auto up = EaseSineOut::create(MoveBy::create(uptime, Point(0, upY)));
	auto down = EaseSineIn::create(MoveBy::create(downtime, Point(0, downY)));
	auto callfunc = CallFunc::create([=](){ jump(_uptime, _upY, _downtime, _downY); });
	auto sequence = Sequence::create(up,down, callfunc, NULL);
	this->runAction(sequence);
}

void Frog::jump2(float scale_time, float scale_times){
	auto callfunc1 = CallFunc::create([=](){removeChildByName("111"); bindSprite(Sprite::createWithSpriteFrameName("frog2.png")); });
	auto up = EaseSineOut::create(ScaleTo::create(scale_time, scale_times));
	auto callfunc2 = CallFunc::create([=](){removeChildByName("111"); bindSprite(Sprite::createWithSpriteFrameName("frog1.png")); });
	auto down = EaseSineIn::create(ScaleTo::create(scale_time, 0.400f));
	//auto callfunc = CallFunc::create([=](){ jump2(scale_time, scale_times); });
	//auto sequence = Sequence::create(callfunc1,up,callfunc2,down,callfunc, NULL);
	auto sequence = Sequence::create(callfunc1,up,callfunc2,down, NULL);
	this->runAction(sequence);
}

void Frog::run(){
	SpriteFrameCache* spriteFrameCache = SpriteFrameCache::getInstance();
	Vector<SpriteFrame*> frogFrameVec;
    //这里就是跑动画帧
	SpriteFrame* spriteFrame;
	for (int i = 2; i >=1; i--){
		spriteFrame = spriteFrameCache->getSpriteFrameByName(StringUtils::format("frog%d.png", i));
		frogFrameVec.pushBack(spriteFrame);
	}
	Animation* animation = Animation::createWithSpriteFrames(frogFrameVec);
	animation->setLoops(-1);
	animation->setDelayPerUnit(1.0f);
	getSprite()->runAction(Animate::create(animation));
}
