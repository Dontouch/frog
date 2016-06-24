#include "Entity.h"

void Entity::bindSprite(Sprite* sprite){
	_sprite = sprite;
	_sprite->setName("111");
	this->addChild(_sprite);

	this->setContentSize(_sprite->getContentSize());
	this->setAnchorPoint(Point::ANCHOR_MIDDLE); // живЊ
	_sprite->setPosition(this->getContentSize().width * 0.5f, this->getContentSize().height * 0.5f);
}

Sprite* Entity::getSprite(){
	return _sprite;
}
