#include "SecondScene.h"
#include "GameScene.h"

SecondScene* SecondScene::create(int pattern){
	auto secondScene = new SecondScene();
	if (secondScene && secondScene->init(pattern)){
		secondScene->autorelease();
		return secondScene;
	}
	else{
		CC_SAFE_DELETE(secondScene);
		return NULL;
	}
}

Scene* SecondScene::createScene(int pattern){
	auto scene = Scene::create();
	auto layer = SecondScene::create(pattern);
	scene->addChild(layer);
	return scene;
}

bool SecondScene::init(int pattern){
	_visibleSize = Director::getInstance()->getVisibleSize();
	srand(time(0));
	_cnt = 1;
	_velocityX = _velocityY = 0.0f;
	_pattern = pattern;
	// 创建游戏第一次开始按钮
	auto beginItem = MenuItemImage::create("begin.png", "begin.png", this, menu_selector(SecondScene::begin));
	beginItem->setOpacity(150);
	auto beginMenu = CCMenu::createWithItem(beginItem);
	beginMenu->setPosition(Point(_visibleSize.width / 2, _visibleSize.height / 2));
	beginMenu->setName("beginMenu");
	this->addChild(beginMenu, 200);
	// 创建暂停按钮
	auto pauseItem = MenuItemImage::create("pause.png", "pause.png", this, menu_selector(SecondScene::pause));
	pauseItem->setName("pauseItem");
	auto pauseMenu = CCMenu::createWithItem(pauseItem);
	pauseMenu->setPosition(Point(_visibleSize.width*0.95, _visibleSize.height*0.97));
	pauseMenu->setName("pauseMenu");
	this->addChild(pauseMenu, 200);
	// 加载精灵帧
	SpriteFrameCache* spriteFrameCache = SpriteFrameCache::getInstance();
	spriteFrameCache->addSpriteFramesWithFile("resource2.plist", "resource2.png");
	// 创建池塘背景
	_pool = Sprite::createWithSpriteFrameName("pool.png");
	_pool->setPosition(_visibleSize / 2);
	this->addChild(_pool,-1);
	// 创建得分标签
	_score = 0.0f;
	_label = LabelTTF::create();
	_label->setString("0.0");
	_label->setFontSize(25);
	_label->setPosition(_visibleSize.width*0.95, _visibleSize.height*0.04);
	_label->setColor(Color3B::YELLOW);
	this->addChild(_label, 10);
	// 创建青蛙
	_frog = Frog::create();
	_frog->bindSprite(Sprite::createWithSpriteFrameName("frog1.png"));
	_frog->setPosition(_visibleSize/2);
	_frog->setScale(0.40);
	this->addChild(_frog,100);
	//_frog->jump2(1.0f, 0.65f);
	_curfrogScale = _frog->getScale();
	// 创建水纹
	for (int i = 0; i <= 1; i++){
		LeafOrWave* wave = createWave();
		wave->setTag(i);
		_leafOrWave[i] = wave;
		if (i==0)
			wave->setPosition(_visibleSize.width*0.45, _visibleSize.height*0.5);
		if (i==1)
			wave->setPosition(_visibleSize.width*0.8, _visibleSize.height*1.92);
	}
	// 创建第一片荷叶
	//LeafOrWave* leafWillBeTouched1 = createLeaf(true); // 将被踩踏的第一片荷叶
	//leafWillBeTouched1->setPosition(_visibleSize.width/2,1150);
	//leafWillBeTouched1->move(2.0, -750, true);// true为2次运动并自动remove
	//char ch[20];
	//sprintf(ch, "%d", _cnt);
	//leafWillBeTouched1->setName(ch);
	// 3片broken 荷叶
	LeafOrWave* leaf1 = createLeaf(false); 
	leaf1->setTag(2);
	_leafOrWave[2] = leaf1;
	leaf1->setPosition(_visibleSize.width*0.3, _visibleSize.height*0.3);
	LeafOrWave* leaf2 = createLeaf(false);
	leaf2->setTag(3);
	_leafOrWave[3] = leaf2;
	leaf2->setPosition(_visibleSize.width*0.75, _visibleSize.height*1.75);
	LeafOrWave* leaf3 = createLeaf(false);
	leaf3->setTag(4);
	_leafOrWave[4] = leaf3;
	leaf3->setPosition(_visibleSize.width*0.35, _visibleSize.height*0.9);
	// 使之移动
	for (int i = 0; i < 5; i++){
		//_leafOrWave[i]->move(2.0, -750,false);
	}
	// 注册调度器
	//this->scheduleUpdate();
	// 注册每2秒一次的调度器
	//this->schedule(schedule_selector(SecondScene::selectStatus), 2.0f);
	//重力感应注册
	//this->setAccelerometerEnabled(true);//开启硬件设备 
	auto listener = EventListenerAcceleration::create([&](Acceleration* acc, Event* event){
		float deceleration = 0.65f;
		float sensitivity = 10.0f;
		float maxVelocity = 50.0f;
		_velocityX = _velocityX*deceleration + acc->x*sensitivity;
		_velocityY = _velocityY*deceleration + acc->y*sensitivity;
		// 限制最大速度为 ±maxVelocity之间
		if (_velocityX >= maxVelocity)
		{
			_velocityX = maxVelocity;
		}
		else if (_velocityX <= -maxVelocity)
		{
			_velocityX = -maxVelocity;
		}
		if (_velocityY >= maxVelocity)
		{
			_velocityY = maxVelocity;
		}
		else if (_velocityY <= -maxVelocity)
		{
			_velocityY = -maxVelocity;
		}
	});
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);  // 注册到层上
	return true;
}

void SecondScene::update(float dt){
	// 计算得分
	_score += _curfrogScale / 60;
	char ch[20];
	sprintf(ch, "%.2lf", _score);
	_label->setString(ch);
	_curfrogScale = _frog->getScale();
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
	// 重力感应改变青蛙Y坐标
	auto frogheight = _frog->getContentSize().height;
	auto downBorderLimit = frogheight / 2;
	auto upBorderLimit = visibleSize.height - frogheight / 2;
	float posY = _frog->getPositionY() + _velocityY;
	if (posY <= downBorderLimit){
		posY = downBorderLimit;
		_velocityY = 0.0f;
	}
	else if (posY >= upBorderLimit){
		posY = upBorderLimit;
		_velocityY = 0.0f;
	}
	_frog->setPositionY(posY);
	//
	for (int i = 0; i < 5; i++){
		if (_leafOrWave[i]->getPositionY() <= -_leafOrWave[i]->getContentSize().height / 2){
			std::string name = _leafOrWave[i]->getName();
			int tag = _leafOrWave[i]->getTag();
			this->removeChild(_leafOrWave[i], true);
			_leafOrWave[i] = NULL;
			LeafOrWave* waveOrLeaf;
			if (name == "wave"){
				waveOrLeaf = createWave();
				waveOrLeaf->setTag(tag);
				_leafOrWave[tag] = waveOrLeaf;
			}
			else{
				waveOrLeaf = createLeaf(false); // broken 荷叶
				waveOrLeaf->setTag(tag);
				_leafOrWave[tag] = waveOrLeaf;
			}
			int flag;
			int cnt = 0;
			while (1){
				flag = false;
				float x = _visibleSize.width*CCRANDOM_0_1();
				float y = _visibleSize.height*(CCRANDOM_0_1() + 1) + waveOrLeaf->getContentSize().height / 2;
				Rect size = Rect(x - waveOrLeaf->getContentSize().width / 2, y - waveOrLeaf->getContentSize().height / 2, waveOrLeaf->getContentSize().width, waveOrLeaf->getContentSize().height);
				for (int j = 0; j < 5; j++){
					if (j != tag){
						if (size.intersectsRect(_leafOrWave[j]->getBoundingBox())){
							flag = true;
							break;
						}
					}
				}
				if (flag){
					cnt++;
					if (cnt == 5){
						waveOrLeaf->setPosition(_visibleSize.width*CCRANDOM_0_1(),_visibleSize.height*2);
						break;
					}
					else{
						continue;
					}
				}
				else{
					waveOrLeaf->setPosition(x, y);
					break;
				}
			}
		}
	}
}

void SecondScene::selectStatus(float ft){
	char ch[20];
	sprintf(ch, "%d", _cnt);
	// 添加碰撞检测
	LeafOrWave* leaf = static_cast<LeafOrWave*>(this->getChildByName(ch));
	//Size leafsize = leaf->getContentSize();
	//Size frogsize = _frog->getContentSize();
	Point frogPos = _frog->getPosition();
	Point leafPos = leaf->getPosition();
	Rect rect = Rect(leafPos.x - 20.3, leafPos.y - 20.6, 40.6, 41.2);
	if (!rect.containsPoint(frogPos)){
		this->unscheduleUpdate();
		leaf->stopAllActions();
		_frog->getSprite()->stopAllActions();
		for (int i = 0; i < 5; i++){
			if (_leafOrWave[i] != NULL)
				_leafOrWave[i]->stopAllActions();
		}
		ActionInterval * fadeout = FadeOut::create(1);
		auto callfunc = CallFunc::create([](){Director::getInstance()->pause(); });
		auto sequence = Sequence::create(fadeout, callfunc, NULL);
		_frog->getSprite()->runAction(sequence);
		return;
	}
	_cnt++;

	_status = rand() % 2;
	_frog->jump2(1.0f, scale[_status]);
	for (int i = 0; i < 5; i++){
		if (_leafOrWave[i] != NULL)
		    _leafOrWave[i]->move(2.0, -dis[_status],false);
	}

	leaf = createLeaf(true); // 将被踩踏的荷叶
	sprintf(ch, "%d", _cnt);
	leaf->setName(ch);
	bool flag;
	int cnt = 0;
	float x, y;
	Rect size;
	while (1){
		flag = false;
		x = 19 * (rand() % 21) + 50; // 50<=x<=430
		y = 50 * (rand() % 11) + 950;// 950<=y<=1450
		size = Rect(x - 50, y - 51, 100, 103);
		for (int j = 0; j < 5; j++){
			if (size.intersectsRect(_leafOrWave[j]->getBoundingBox())){
				flag = true;
				break;
			}
		}
		if (flag){
			cnt++;
			if (cnt == 5){
				leaf->setPosition(x, y);
				break;
			}
			else{
				continue;
			}
		}
		else{
			leaf->setPosition(x, y);
			break;
		}
    }
	leaf->move(2.0, -dis[_status], true);// true为2次运动并自动remove
}

LeafOrWave* SecondScene::createWave(){
	int a = rand() % 2;
	LeafOrWave* wave = LeafOrWave::create();
	if (a == 0){
		wave->bindSprite(Sprite::createWithSpriteFrameName("plumpwave.png"));
		wave->setScale(1.5);
		this->addChild(wave,0);
	}
	if (a == 1){
		wave->bindSprite(Sprite::createWithSpriteFrameName("plumpwave.png"));
		wave->setScale(1.35);
		this->addChild(wave, 0);
	}
	wave->setName("wave");
	return wave;
}

LeafOrWave* SecondScene::createLeaf(bool flag){
	LeafOrWave* leaf;
	if (flag){ // 创建将被踩踏的荷叶
		leaf = LeafOrWave::create();
		leaf->bindSprite(Sprite::createWithSpriteFrameName("unbroken.png"));
		leaf->setScale(0.7);
		leaf->setRotation((rand() % 4) * 88);// "willBeTouched"角度随机
		this->addChild(leaf, 10);
		return leaf;
	}
	int a = rand() % 3;
	if (a == 0){
		leaf = LeafOrWave::create();
		leaf->bindSprite(Sprite::createWithSpriteFrameName("brokenWithWave.png"));
		leaf->setScale(0.85);
		leaf->setRotation((rand() % 4) * 77);
		this->addChild(leaf, 0);
	}
	if (a == 1){
		leaf = LeafOrWave::create();
		leaf->bindSprite(Sprite::createWithSpriteFrameName("broken.png"));
		leaf->setScale(0.55);
		leaf->setRotation((rand() % 4) * 66);
		this->addChild(leaf, 0);
	}
	if (a == 2){
		leaf = LeafOrWave::create();
		leaf->bindSprite(Sprite::createWithSpriteFrameName("broken.png"));
		leaf->setScale(0.70);
		leaf->setRotation((rand() % 4) * 55);
		this->addChild(leaf, 0);
	}
	leaf->setName("normalLeaf");
	return leaf;
}

void SecondScene::start(Ref *sender){
	auto visibleSize = Director::getInstance()->getVisibleSize();
	auto pauseItem = MenuItemImage::create("pause.png", "pause.png", this, menu_selector(SecondScene::pause));
	pauseItem->setName("pauseItem");
	auto pauseMenu = CCMenu::createWithItem(pauseItem);
	pauseMenu->setPosition(Point(visibleSize.width*0.95, visibleSize.height*0.97));
	pauseMenu->setName("pauseMenu");
	this->addChild(pauseMenu, 200);
	Director::getInstance()->resume();
}
void SecondScene::pause(Ref *sender){
	auto visibleSize = Director::getInstance()->getVisibleSize();
	this->removeChildByName("pauseMenu");
	auto startItem = MenuItemImage::create("start.png", "start.png", this, menu_selector(SecondScene::start));
	startItem->setName("startItem");
	auto startMenu = CCMenu::createWithItem(startItem);
	startMenu->setPosition(Point(visibleSize.width*0.95, visibleSize.height*0.97));
	startMenu->setName("pauseMenu");
	this->addChild(startMenu, 200);
	Director::getInstance()->pause();
}
void SecondScene::begin(Ref *sender){
	_frog->jump2(1.0f, 0.65f);
	// 创建第一片荷叶
	LeafOrWave* leafWillBeTouched1 = createLeaf(true); // 将被踩踏的第一片荷叶
	leafWillBeTouched1->setPosition(_visibleSize.width/2,1220);
	leafWillBeTouched1->move(2.0, -750, true);// true为2次运动并自动remove
	char ch[20];
	sprintf(ch, "%d", _cnt);
	leafWillBeTouched1->setName(ch);
	for (int i = 0; i < 5; i++){
		_leafOrWave[i]->move(2.0, -750,false);
	}
	// 注册调度器
	this->scheduleUpdate();
	// 注册每2秒一次的调度器
	this->schedule(schedule_selector(SecondScene::selectStatus), 2.0f);
	//重力感应注册
	this->setAccelerometerEnabled(true);//开启硬件设备 
	this->removeChildByName("beginMenu");
}
void SecondScene::restart(Ref *sender){
	Director::getInstance()->replaceScene(SecondScene::createScene(_pattern));
}
void SecondScene::backGameScene(Ref *sender){
	Director::getInstance()->replaceScene(TransitionFade::create(2.0, GameScene::createScene()));
}
