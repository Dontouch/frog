#ifndef _FIRSTSCENE_H_
#define _FIRSTSCENE_H_
#include "Insect.h"
#include "Mushroom.h"
#include "Frog.h"
#include "Mosquito.h"

class FirstScene :public Layer{
public:
	static Scene* createScene(int pattern);
	static FirstScene* create(int pattern);
	bool init(int pattern);
	void bgMove(float uptime, int upY, float downtime, int downY);
	virtual void update(float dt);
	void createBG();
    
    //监听中的更新
	void insectsFunctionInUpdate();
	void mushroomFunctionInUpdate();
	void mosquitoFunctionInUpdate();
    
private:
	Frog* _frog;
	Mushroom* _mushroom;
	Vector<Mosquito*> _mosquitoes;
	Vector<Insect*> _insects;
	Sprite* _bg1;
	Sprite* _bg2;
    
	LabelTTF* _label; //显示分数
	float _curfrogY;// 计算得分所有
	float _score; // 得分，即是跳跃的高度
	float _velocityX; // 青蛙x轴速度
	int _pattern; // 模式区分   剧情模式 无尽模式
	
    void start(Ref *sender);
	void pause(Ref *sender);
	void begin(Ref *sender);
	void restart(Ref *sender);
	void backGameScene(Ref *sender);
	void next(Ref* sender);
	bool _isCollideValid;
};

#endif
