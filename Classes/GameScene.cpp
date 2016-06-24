#include "GameScene.h"
#include "EndlessSelect.h"
#include "FirstScene.h"
Scene* GameScene::createScene(){
	auto scene = Scene::create();
	auto layer = GameScene::create();
	scene->addChild(layer);
	return scene;
}

bool GameScene::init(){
	auto visibleSize = Director::getInstance()->getVisibleSize();
	// 创建背景
	auto bg = Sprite::create("levelSelect.png");
	bg->setPosition(visibleSize / 2);
	this->addChild(bg);
	// 添加选择关卡按钮
	auto storyItem = MenuItemImage::create("storyPattern.png", "storyPattern.png", this, menu_selector(GameScene::story));
	auto storyMenu = CCMenu::createWithItem(storyItem);
	storyMenu->setPosition(visibleSize.width/2,visibleSize.height/2 - 200);
	this->addChild(storyMenu);

	auto endlessItem = MenuItemImage::create("endlessPattern.png", "endlessPattern.png", this, menu_selector(GameScene::endless));
	auto endlessMenu = CCMenu::createWithItem(endlessItem);
	endlessMenu->setPosition(visibleSize.width / 2, visibleSize.height / 2 + 200);
	this->addChild(endlessMenu);
	return true;
}

void GameScene::story(Ref *sender){
	Director::getInstance()->replaceScene(TransitionFade::create(2.0,FirstScene::createScene(1))); // 随后在firstScene中转到 secondScene
}

void GameScene::endless(Ref *sender){
	Director::getInstance()->replaceScene(EndlessSelect::createScene());  // 无尽模式 选择跳井关卡或者跳荷叶关卡
}
