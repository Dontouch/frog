#include "FirstScene.h"
#include "GameScene.h"
#include "SecondScene.h"

FirstScene* FirstScene::create(int pattern){
	auto firstScene = new FirstScene();
	if (firstScene && firstScene->init(pattern)){
		firstScene->autorelease();
		return firstScene;
	}
	else{
		CC_SAFE_DELETE(firstScene);
		return NULL;
	}
}

Scene* FirstScene::createScene(int pattern){
	auto scene = Scene::create();
	auto layer = FirstScene::create(pattern);
	scene->addChild(layer);
	return scene;
}

bool FirstScene::init(int pattern){
	
    auto visibleSize = Director::getInstance()->getVisibleSize();
	srand(int(time(0)));
    
	_velocityX = 0.0f;   //初始化青蛙周在x轴上的初速度
	_pattern = pattern;
	_isCollideValid = true;//碰撞是不是有效
    
	//创建游戏第一次开始的按钮
	auto beginItem = MenuItemImage::create("begin.png", "begin.png", this, menu_selector(FirstScene::begin));
	beginItem->setOpacity(150);
	auto beginMenu = CCMenu::createWithItem(beginItem);
	beginMenu->setPosition(Point(visibleSize.width/2, visibleSize.height/2));
	beginMenu->setName("beginMenu");
	this->addChild(beginMenu, 200);
	
    //创建游戏暂停的按钮
	auto pauseItem = MenuItemImage::create("pause.png", "pause.png", this, menu_selector(FirstScene::pause));
	pauseItem->setName("pauseItem");
	auto pauseMenu = CCMenu::createWithItem(pauseItem);
	pauseMenu->setPosition(Point(visibleSize.width*0.95,visibleSize.height*0.97));
	pauseMenu->setName("pauseMenu");
	this->addChild(pauseMenu,200);
	
    //加载资源 通过帧资源
	SpriteFrameCache* spriteFrameCache = SpriteFrameCache::getInstance();
	spriteFrameCache->addSpriteFramesWithFile("resource1.plist","resource1.png");
	
    //创建得分标签
	_score = 0.0f;
	_label = LabelTTF::create();
	_label->setString("0.0");//默认的分数是0
	_label->setFontSize(25);
	_label->setPosition(visibleSize.width*0.95,visibleSize.height*0.04);
	_label->setColor(Color3B::YELLOW);
	this->addChild(_label,10);
	
    //创建背景
	createBG();
    
	//bgMove(1.0f, 20, 1.0f, -150);
	//创建青蛙
	_frog = Frog::create();
	_frog->bindSprite(Sprite::createWithSpriteFrameName("frog1.png"));
	_frog->setPosition(visibleSize.width /2, visibleSize.height * 0.4);
	_frog->setScale(0.6);
	this->addChild(_frog);
	
    //_frog->run();
	//_frog->jump(1.0f, visibleSize.height*0.2, 1.0f, -visibleSize.height*0.2);
	_curfrogY = _frog->getPositionY(); //计算得分所用
    
    //创建虫子
	for (int i = 0; i < 4; i++){
		auto insect = Insect::create();
		insect->bindSprite(Sprite::createWithSpriteFrameName("insect2.png"));
		insect->setScale(0.6);
		float width;
		auto insectSize = insect->getContentSize();
		do{
			width = CCRANDOM_0_1()*visibleSize.width;
		} while (width <= visibleSize.width*0.1172 + insectSize.width / 2 || width >= visibleSize.width*0.8671875 - insectSize.width / 2);
		insect->setPosition(width, (CCRANDOM_0_1() +1)*visibleSize.height);
		_insects.pushBack(insect);
		//insect->move(1.0f, visibleSize.height*0.208333, 1.0f, -visibleSize.height*0.46875);
		if (i == 0 || i == 1){
			insect->setName("static"); //是指x轴上是否移动而不是本身是否run
			//insect->run();
		} // 两只会移动的虫子
		this->addChild(insect);
	}
    
	// 创建蘑菇
	_mushroom = Mushroom::create();
	_mushroom->bindSprite(Sprite::createWithSpriteFrameName("mushroom.png"));
	_mushroom->setScale(0.7);
	float mushroomposX;
	float rightBorderLimit = visibleSize.width*0.8617 - _mushroom->getContentSize().width / 2;
	float leftBorderLlimit = visibleSize.width*0.1172 + _mushroom->getContentSize().width / 2;
	do{
		mushroomposX = CCRANDOM_0_1()*visibleSize.width;
	} while (mushroomposX <= leftBorderLlimit || mushroomposX >= rightBorderLimit);
	_mushroom->setPosition(mushroomposX, (CCRANDOM_0_1() + 1)*visibleSize.height);
	//_mushroom->move(1.0f, visibleSize.height*0.208333, 1.0f, -visibleSize.height*0.46875);
	this->addChild(_mushroom);
	
    
    //创建蚊子
	for (int i = 0; i < 1; i++){
		auto mosquito = Mosquito::create();
		mosquito->bindSprite(Sprite::createWithSpriteFrameName("mosquito1.png"));
		mosquito->setScale(0.5f);
		mosquito->setPosition(CCRANDOM_0_1()*visibleSize.width, (CCRANDOM_0_1() +1)*visibleSize.height);
		//mosquito->run();
		this->addChild(mosquito);
		_mosquitoes.pushBack(mosquito);
	}
	//启动调度器
	//this->scheduleUpdate();
	//重力感应注册
	//this->setAccelerometerEnabled(true);//开启硬件设备
	auto listener = EventListenerAcceleration::create([&](Acceleration* acc, Event* event){
		//控制减速的速率值越小，更容易改变方向   // 减速度系数(值越小＝转向越快)
		float deceleration = 0.5f;
		//加速计的敏感值越大，主角对加速计的输入会越敏感    // 加速度系数 (值越大 = 越敏感)
		float sensitivity = 20.0f;
		//最大移动距离  // 最大速度
		float maxVelocity = 200.0f;
		// 计算新的初速度
        _velocityX = _velocityX*deceleration + acc->x*sensitivity;
		// 限制最大速度为 ±maxVelocity之间
		if (_velocityX >= maxVelocity)
		{
			_velocityX = maxVelocity;
		}
		else if (_velocityX <= -maxVelocity)
		{
			_velocityX = -maxVelocity;
		}
	});
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);  // 注册到层上
	//玩家速度由一个一次线性方程决定:V= V?*β+ V?*ε,其中,V为终速,V?为初速,为减速系数,V?为加速度,ε为加速系数
    // 其中，β和ε两个系数是两个经验值，你可以自己调整它以达到理想效果。
    /*-----------------------------重力感应注册---------------------------*/
	return true;
}

void FirstScene::update(float dt){   
	// 重力感应改变青蛙X坐标
	auto visibleSize = Director::getInstance()->getVisibleSize();
	auto frogwidth = _frog->getContentSize().width;
	auto leftBorderLimit = frogwidth / 2;
	auto rightBorderLimit = visibleSize.width - frogwidth / 2;
	float posX = _frog->getPositionX() + _velocityX;
	if (posX <= leftBorderLimit){
		posX = leftBorderLimit;
		_velocityX = 0.0f;
	}
	else if (posX >= rightBorderLimit){
		posX = rightBorderLimit;
		_velocityX = 0.0f;
	}
	_frog->setPositionX(posX);
	
    //计算得分
	if (_isCollideValid){
		float a = _frog->getPositionY() - _curfrogY;
		if (a > 0)
			a = a / (visibleSize.height*0.2 * 2);
		else if (a == 0)
			a = 0.01666667;
		else a = a / (visibleSize.height*0.2 * 5);
		_score += a;
		char ch[20];
		sprintf(ch, "%.2lf", _score);
		_label->setString(ch);
		_curfrogY = _frog->getPositionY();
	}
	//如果是剧情模式
	if (_pattern == 1 && _isCollideValid){
		if (_score >= 1.0){
			_isCollideValid = false;
			for (auto insect : _insects)
				insect->stopAllActions();
			_mushroom->stopAllActions();
			_bg1->stopAllActions();
			_bg2->stopAllActions();

			auto visibleSize = Director::getInstance()->getVisibleSize();
			TTFConfig config("fonts/yuehei.otf", 48);
			auto labelScore = Label::createWithTTF(config, "  0  ");
			labelScore->setPosition(visibleSize/ 2);
			labelScore->setColor(Color3B::YELLOW);
			std::string s = "100 m is done !";
			labelScore->setString(s);
			this->addChild(labelScore);
			auto nextItem = MenuItemImage::create("next.png", "next.png", CC_CALLBACK_1(FirstScene::next, this));
			nextItem->setPosition(visibleSize.width/2,visibleSize.height/2 - 100);
			auto menu = Menu::create(nextItem, NULL);
			menu->setPosition(Vec2::ZERO);
			this->addChild(menu, 200);
		}
	}
	// 背景图无限滚动
	if (_bg1->getPositionY() <= -visibleSize.height + 10)
		_bg1->setPositionY(visibleSize.height - 10);
	if (_bg2->getPositionY() <= -visibleSize.height + 10)
		_bg2->setPositionY(visibleSize.height - 10);

	mushroomFunctionInUpdate(); //判断蘑菇移出屏幕以及碰撞检测
	insectsFunctionInUpdate(); //判断虫子移出屏幕以及碰撞检测
	mosquitoFunctionInUpdate(); //蚊子
}

void FirstScene::createBG(){
	auto visibleSize = Director::getInstance()->getVisibleSize();
	_bg1 = Sprite::createWithSpriteFrameName("bg1.png");
	_bg1->setAnchorPoint(Point::ZERO);
	_bg1->setPosition(0, 0);
	_bg2 = Sprite::createWithSpriteFrameName("bg2.png");
	_bg2->setAnchorPoint(Point::ZERO);
	_bg2->setPosition(0, visibleSize.height - 10);
	this->addChild(_bg1);
	this->addChild(_bg2);
}

void FirstScene::insectsFunctionInUpdate(){
	
	auto visibleSize = Director::getInstance()->getVisibleSize();
	float x, y;
	bool flag;
	Vector<Insect*> v;
	v.clear();
	for (auto insect : _insects){
		insect = static_cast<Insect*>(insect);
		if (insect->getPositionY() >= visibleSize.height)
			v.pushBack(insect);
	}
	for (auto insect : _insects){
		insect = static_cast<Insect*>(insect);
		if (insect->getPositionY() <= 0 || insect->isVisible() == false){
			auto insectSize = insect->getContentSize();
			while (1){
				x = visibleSize.width* CCRANDOM_0_1();
				if (x <= (visibleSize.width*0.1172 + insectSize.width / 2) || x >= (visibleSize.width*0.8671 - insectSize.width / 2))
					continue;
				y = visibleSize.height*(CCRANDOM_0_1() + 1);
				Rect a(x - insectSize.width/2, y - insectSize.height/2, insectSize.width, insectSize.height);
				flag = false;
				for (auto inse : v){
					inse = static_cast<Insect*>(inse);
					if (a.intersectsRect(inse->getBoundingBox())){
						flag = true;
						break;
					}
				}
				if (flag)
					continue;
				else break;
			}
			insect->setPosition(x, y);
			insect->setVisible(true);
			float scale = CCRANDOM_0_1();
			while (scale <= 0.4 ||scale >=0.6)
				scale = CCRANDOM_0_1();
			insect->setScale(scale);
		}
	//有2只虫子会动
	if (insect->getName() == "static"){
		MoveTo* moveTo;
		CallFunc* callfunc;
		Sequence* sequence;
		insect->setName("moving");
		float posX = insect->getPositionX();
		auto insectSize = insect->getContentSize();
		auto rightborderlimit = visibleSize.width*0.8671 - insectSize.width / 2;
		auto leftborderlimit = visibleSize.width*0.1172 + insectSize.width / 2;
		if (posX < visibleSize.width / 2){
		    if (rand() % 5 < 4)
			    moveTo = MoveTo::create((rightborderlimit - posX) / 80, Point(rightborderlimit, insect->getPositionY()));
		    else
			    moveTo = MoveTo::create((rightborderlimit - posX) / 120, Point(rightborderlimit, insect->getPositionY()));
		}
		else{
			if (rand() % 5 < 4)
			    moveTo = MoveTo::create((posX - leftborderlimit) / 80, Point(leftborderlimit, insect->getPositionY()));
			else
			    moveTo = MoveTo::create((posX - leftborderlimit) / 120, Point(leftborderlimit, insect->getPositionY()));
		}
		callfunc = CallFunc::create([=](){ insect->setName("static"); });
		sequence = Sequence::create(moveTo, callfunc, NULL);
		insect->runAction(sequence);
	}
	// 检测虫子与青蛙碰撞
	if (_frog->getBoundingBox().intersectsRect(insect->getBoundingBox()) && _isCollideValid){
		if (_frog->getName() != "加速状态"){
			auto scaleTo = ScaleTo::create(1.0f, _frog->getScale() + 0.1f);
			_frog->runAction(scaleTo);
			insect->setVisible(false);
			for (auto insect : _insects)
				insect->downSpeed();
			bool flag = _mushroom->downSpeed();
			if (flag){ // 已经吃了3个虫子或蚊子
                _isCollideValid = false;
				for (auto mosquito : _mosquitoes){
					mosquito = static_cast<Mosquito*>(mosquito);
					mosquito->stopAllActions();
				}
			    _frog->stopAllActions();
			    _frog->getSprite()->stopAllActions();
			    _bg1->stopAllActions();
			    _bg2->stopAllActions();
			    _mushroom->stopAllActions();
			    for (auto insect : _insects){
			        insect = static_cast<Insect*>(insect);
			        insect->stopAllActions();
			     }
			    auto scaleTo = ScaleTo::create(1.0f, 1.0f);
			    auto rotateto = RotateBy::create(1.0, 720);
			    auto moveBy = MoveTo::create(1.0f, Point(_frog->getPositionX(), -100));
			    auto spawn = Spawn::create(rotateto, moveBy, NULL);
				auto callfunc = CallFunc::create([=](){
					this->removeChildByName("pauseMenu");
					auto visibleSize = Director::getInstance()->getVisibleSize();
					TTFConfig config("fonts/yuehei.otf", 48);
					// 添加重新开始按钮
					auto restartItem = MenuItemImage::create("btn_restart02.png", "btn_restart02.png", CC_CALLBACK_1(FirstScene::restart, this));
					restartItem->setPosition(visibleSize.width/2,visibleSize.height/2.3);
					// 添加返回主菜单按钮
					auto backItem = MenuItemImage::create("btn_back02.png", "btn_back02.png", CC_CALLBACK_1(FirstScene::backGameScene, this));
					backItem->setPosition(visibleSize/2);

					auto menu = Menu::create(restartItem, backItem, NULL);
					menu->setPosition(Vec2::ZERO);
					this->addChild(menu,200);
					// 分数
					auto labelScore = Label::createWithTTF(config, "  0  ");
					labelScore->setPosition(Point(visibleSize.width/ 2, visibleSize.height/1.8));
					labelScore->setColor(Color3B::YELLOW);
					char ch[20];
					sprintf(ch, "%.2lf", _score);
					std::string s1 = "your score : ";
					std::string s2 = "m";
					labelScore->setString(s1 + ch + s2);
					this->addChild(labelScore);
				});
			    auto sequecne = Sequence::create(scaleTo,spawn,callfunc, NULL);
			    _frog->runAction(sequecne);
			} // 速度低于80的时候游戏结束
	    }
		else{ // 加速时虫子被撞后
			insect->setVisible(false);
		}
	}
    }
}
void FirstScene::mushroomFunctionInUpdate(){
	auto visibleSize = Director::getInstance()->getVisibleSize();
	//判断蘑菇
	if (_mushroom->getPositionY() <= 0 || _mushroom->isVisible() == false){
		float mushroomposX;
		float rightBorderLimit = visibleSize.width*0.8617 - _mushroom->getContentSize().width / 2;
		float leftBorderLlimit = visibleSize.width*0.1172 + _mushroom->getContentSize().width / 2;
		do{
			mushroomposX = CCRANDOM_0_1()*visibleSize.width;
		} while (mushroomposX <= leftBorderLlimit || mushroomposX >= rightBorderLimit);
		_mushroom->setPosition(mushroomposX, visibleSize.height*(CCRANDOM_0_1() + 10)); // 4秒蘑菇下移一个屏幕高度
		_mushroom->setVisible(true);
	}
	if (_frog->getBoundingBox().intersectsRect(_mushroom->getBoundingBox()) && _isCollideValid){
		if (_frog->getName() != "加速状态"){
			_mushroom->setVisible(false);
			_frog->setName("加速状态");

			_frog->stopAllActions(); // visibleSize.height*0.8 为加速时最高位置 ，-visibleSize.height*0.4为加速后下降位移
			_frog->getSprite()->stopAllActions(); //  停止run
			auto up = EaseSineOut::create(MoveBy::create((visibleSize.height*0.8 - _frog->getPositionY()) / 80, Point(0, visibleSize.height*0.8 - _frog->getPositionY())));
			auto scaleTo = ScaleTo::create((visibleSize.height*0.8 - _frog->getPositionY()) / 80, 0.6f);
			auto spawn = Spawn::create(up, scaleTo, NULL);
			auto delay = DelayTime::create(6.0f - 1.0f - (visibleSize.height*0.8 - _frog->getPositionY()) / 80);
			auto callfunc1 = CallFunc::create([=](){
				_frog->removeChildByName("111");
				_frog->bindSprite(Sprite::createWithSpriteFrameName("frog1.png")); });
			auto down = EaseSineIn::create(MoveBy::create(1.0f, Point(0, -visibleSize.height*0.4)));
			auto callfunc2 = CallFunc::create([=](){ _frog->setName("跳跃状态");
			_frog->jump(1.0f, visibleSize.height*0.2, 1.0f, -visibleSize.height*0.2); 
			_frog->run(); });
			auto sequence = Sequence::create(spawn, delay, callfunc1,down, callfunc2, NULL);
			_frog->runAction(sequence);

			_mushroom->stopAllActions();
			_mushroom->move(0.0f, 0, 6.0f, -6000);
			_mushroom->reset();

			for (auto insect : _insects){
				if (insect->getName() == "moving"){
					insect->setName("static");
				}//停止虫子的动作后虫子的状态恢复不了static
				insect = static_cast<Insect*>(insect);
				insect->stopAllActions();
				insect->move(0.0f, 0, 6.0f, -6000);
				insect->reset();
			}

			_bg1->stopAllActions();
			_bg2->stopAllActions();
			bgMove(0.0f, 0, 6.0f, -1200);
		}
		else{
			_mushroom->setVisible(false);
		}
	}
}

void FirstScene::mosquitoFunctionInUpdate(){
	auto visibleSize = Director::getInstance()->getVisibleSize();
	for (auto mosquito : _mosquitoes){
		mosquito = static_cast<Mosquito*>(mosquito);
		if (mosquito->getPositionY() < 0 || mosquito->isVisible() == false){
			mosquito->setPosition(CCRANDOM_0_1()*visibleSize.width, (CCRANDOM_0_1() + 1)*visibleSize.height);
			mosquito->setVisible(true);
		}
		if (rand() % 6 == 1){
			// 纵向调节
			if (rand() % 5 < 2){
				auto moveBy = MoveBy::create(1.0f, Point(0, 20.0f));
				mosquito->runAction(moveBy);
			}
			else{
				auto moveBy = MoveBy::create(1.0f, Point(0, -20.0f));
				mosquito->runAction(moveBy);
			}
			int posX = mosquito->getPositionX();
            // 横向调节
            if (posX < visibleSize.width / 2){
				auto moveBy = MoveBy::create(1.8f, Point(30.0f, 0));
				mosquito->runAction(moveBy);
			}
			else{
				auto moveBy = MoveBy::create(1.8f, Point(-30.0f, 0));
				mosquito->runAction(moveBy);
			}
		}
		if (_frog->getBoundingBox().intersectsRect(mosquito->getBoundingBox()) && _isCollideValid){
			if (_frog->getName() != "加速状态"){
				auto scaleTo = ScaleTo::create(1.0f, _frog->getScale() + 0.1f);
				_frog->runAction(scaleTo);
				mosquito->setVisible(false);
				for (auto insect : _insects){
					insect->downSpeed();
				}
				bool flag = _mushroom->downSpeed();
				if (flag){
					_isCollideValid = false;
					for (auto mosquito : _mosquitoes){
						mosquito = static_cast<Mosquito*>(mosquito);
						mosquito->stopAllActions();
					}
					_frog->stopAllActions();
					_frog->getSprite()->stopAllActions();
					_bg1->stopAllActions();
					_bg2->stopAllActions();
					_mushroom->stopAllActions();
					for (auto insect : _insects){
						insect = static_cast<Insect*>(insect);
						insect->stopAllActions();
					}
					auto scaleTo = ScaleTo::create(1.0f, 1.0f);
					auto rotateto = RotateBy::create(1.0, 720);
					auto moveBy = MoveTo::create(1.0f, Point(_frog->getPositionX(), -100));
					auto spawn = Spawn::create(rotateto, moveBy, NULL);
					auto callfunc = CallFunc::create([=](){
						this->removeChildByName("pauseMenu");
						auto visibleSize = Director::getInstance()->getVisibleSize();
						TTFConfig config("fonts/yuehei.otf", 48);
						//添加重新开始按钮
						auto restartItem = MenuItemImage::create("btn_restart02.png", "btn_restart02.png", CC_CALLBACK_1(FirstScene::restart, this));
						restartItem->setPosition(visibleSize.width / 2, visibleSize.height / 2.3);
						//添加返回主菜单按钮
                        auto backItem = MenuItemImage::create("btn_back02.png", "btn_back02.png", CC_CALLBACK_1(FirstScene::backGameScene, this));
						backItem->setPosition(visibleSize / 2);

						auto menu = Menu::create(restartItem, backItem, NULL);
						menu->setPosition(Vec2::ZERO);
						this->addChild(menu, 200);
						// 分数
						auto labelScore = Label::createWithTTF(config, "  0  ");
						labelScore->setPosition(Point(visibleSize.width / 2, visibleSize.height / 1.8));
						labelScore->setColor(Color3B::YELLOW);
						char ch[20];
						sprintf(ch, "%.2lf", _score);
						std::string s1 = "your score : ";
						std::string s2 = "m";
						labelScore->setString(s1 + ch + s2);
						this->addChild(labelScore);
					});
					auto sequecne = Sequence::create(scaleTo, spawn, callfunc, NULL);
					_frog->runAction(sequecne);
				} // 速度低于80的时候游戏结束
			}
		}
		else{ // 加速时蚊子被撞后
			mosquito->setVisible(false);
		}
	}
}

void FirstScene::bgMove(float uptime, int upY, float downtime, int downY){
	auto up = MoveBy::create(uptime, Point(0, upY));
	auto down = EaseSineOut::create(MoveBy::create(downtime, Point(0, downY)));
	auto callfunc = CallFunc::create([=](){ this->bgMove(1.0f, 20, 1.0f, -150); }); // 直接用实参初始化
	auto sequence = Sequence::create(down, up, callfunc, NULL);
	_bg1->runAction(sequence);

	auto sequence2 = Sequence::create(down->clone(), up->clone(), NULL);
	_bg2->runAction(sequence2);
}

void FirstScene::start(Ref *sender){
	auto visibleSize = Director::getInstance()->getVisibleSize();
	auto pauseItem = MenuItemImage::create("pause.png", "pause.png", this, menu_selector(FirstScene::pause));
	pauseItem->setName("pauseItem");
	auto pauseMenu = CCMenu::createWithItem(pauseItem);
	pauseMenu->setPosition(Point(visibleSize.width*0.95, visibleSize.height*0.97));
	pauseMenu->setName("pauseMenu");
	this->addChild(pauseMenu, 200);
	Director::getInstance()->resume();
}
void FirstScene::pause(Ref *sender){
	auto visibleSize = Director::getInstance()->getVisibleSize();
	this->removeChildByName("pauseMenu");
	auto startItem = MenuItemImage::create("start.png", "start.png", this, menu_selector(FirstScene::start));
	startItem->setName("startItem");
	auto startMenu = CCMenu::createWithItem(startItem);
	startMenu->setPosition(Point(visibleSize.width*0.95, visibleSize.height*0.97));
	startMenu->setName("pauseMenu");
	this->addChild(startMenu, 200);
	Director::getInstance()->pause();

}

void FirstScene::begin(Ref *sender){
	auto visibleSize = Director::getInstance()->getVisibleSize();
	bgMove(1.0f, 20, 1.0f, -150);
	_frog->run();
	_frog->jump(1.0f, visibleSize.height*0.2, 1.0f, -visibleSize.height*0.2);
	for (auto insect : _insects){
		insect->move(1.0f, visibleSize.height*0.208333, 1.0f, -visibleSize.height*0.46875);
		if (insect->getName() == "static")
			insect->run();
	}
	_mushroom->move(1.0f, visibleSize.height*0.208333, 1.0f, -visibleSize.height*0.46875);
	for (auto mosquito:_mosquitoes)
		mosquito->run();
	this->scheduleUpdate();
	this->setAccelerometerEnabled(true);//开启硬件设备 
	this->removeChildByName("beginMenu");
}

void FirstScene::restart(Ref *sender){
	Director::getInstance()->replaceScene(FirstScene::createScene(_pattern));
}

void FirstScene::backGameScene(Ref *sender){
	Director::getInstance()->replaceScene(TransitionFade::create(2.0, GameScene::createScene()));
}

void FirstScene::next(Ref* sender){
	Director::getInstance()->replaceScene(TransitionFade::create(2.0, SecondScene::createScene(1)));
}
