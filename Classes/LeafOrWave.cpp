#include "LeafOrWave.h"

bool LeafOrWave::init(){
	return true;
}

void LeafOrWave::move(float downtime, int downY, bool flag){
	auto visibleSize = Director::getInstance()->getVisibleSize();
	auto down = EaseSineOut::create(MoveBy::create(downtime, Point(0, downY)));
	//auto callfunc = CallFunc::create([=](){ move(downtime,downY); });
	Sequence* sequence = Sequence::create(down, NULL);
	if (!flag){
		this->runAction(sequence);
	}
	else{
		auto repeat = Repeat::create(down, 2);
		auto callfunc = CallFunc::create([=](){getParent()->removeChild(this, true); });
		auto sequence = Sequence::create(repeat, callfunc, NULL);
		this->runAction(sequence);
	}
}
