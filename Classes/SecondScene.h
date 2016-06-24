#ifndef _SECONDSCENE_H_
#define _SECONDSCENE_H_

#include "Frog.h"
#include "LeafOrWave.h"

const float scale[2] = { 0.85, 0.65};
const float dis[2] = { 1000*0.85, 1000*0.75};
class SecondScene :public Layer{
public:
	static Scene* createScene(int pattern);
	static SecondScene* create(int pattern);
	bool init(int pattern);
	virtual void update(float dt);
	void selectStatus(float ft);
	LeafOrWave* createLeaf(bool flag); // if flag == true ,create unbroken leaf
	LeafOrWave* createWave();
private:
	Frog* _frog;
	LeafOrWave* _leafOrWave[5];
	Sprite* _pool;
	int _status; // 
	Size _visibleSize;
	int _cnt; // ��Ҫ����̤�ĺ�Ҷ�ı��
	float _velocityX; // x���ٶ�
	float _velocityY; // y���ٶ�
	LabelTTF* _label;
	float _score; // �÷�
	float _curfrogScale;// ����÷�����
	int _pattern; // ģʽ
	void start(Ref *sender);
	void pause(Ref *sender);
	void begin(Ref *sender);
	void restart(Ref *sender);
	void backGameScene(Ref *sender);
};

#endif
