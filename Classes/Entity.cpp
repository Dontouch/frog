#include "Entity.h"

void Entity::bindSprite(Sprite* sprite){
	_sprite = sprite;
	_sprite->setName("111");
	this->addChild(_sprite);

	this->setContentSize(_sprite->getContentSize());
	this->setAnchorPoint(Point::ANCHOR_MIDDLE); //对精灵设置锚点
	_sprite->setPosition(this->getContentSize().width * 0.5f, this->getContentSize().height * 0.5f); // 设置精灵的位置
}

Sprite* Entity::getSprite(){
	return _sprite;
}
