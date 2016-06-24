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
	LabelTTF* _label;
	float _curfrogY;// ����÷�����
	float _score; // �÷֣�����Ծ�ĸ߶�
	float _velocityX; // ����x���ٶ�
	int _pattern; // ģʽ����  1Ϊ����ģʽ��2Ϊ�޾�ģʽ
	void start(Ref *sender);
	void pause(Ref *sender);
	void begin(Ref *sender);
	void restart(Ref *sender);
	void backGameScene(Ref *sender);
	void next(Ref* sender);
	bool _isCollideValid;
};

#endif
