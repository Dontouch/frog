
#include "EndlessSelect.h"
#include "FirstScene.h"
#include "SecondScene.h"

Scene* EndlessSelect::createScene(){
	auto scene = Scene::create();
	auto layer = EndlessSelect::create();
	scene->addChild(layer);
	return scene;
}

bool EndlessSelect::init(){
	auto visibleSize = Director::getInstance()->getVisibleSize();
	auto storyItem = MenuItemImage::create("mushroom.png", "mushroom.png", this, menu_selector(EndlessSelect::first));
	auto storyMenu = CCMenu::createWithItem(storyItem);
	storyMenu->setPosition(visibleSize.width / 2, visibleSize.height / 2 - 200);
	this->addChild(storyMenu);

	auto endlessItem = MenuItemImage::create("broken.png", "broken.png", this, menu_selector(EndlessSelect::second));
	auto endlessMenu = CCMenu::createWithItem(endlessItem);
	endlessMenu->setPosition(visibleSize.width / 2, visibleSize.height / 2 + 200);
	this->addChild(endlessMenu);
	return true;
}
void EndlessSelect::first(Ref *sender){
	Director::getInstance()->replaceScene(FirstScene::createScene(2));
}

void EndlessSelect::second(Ref* sender){
	Director::getInstance()->replaceScene(SecondScene::createScene(2));
}
