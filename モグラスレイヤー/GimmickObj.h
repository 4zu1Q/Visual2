#pragma once
#include "DxLib.h"

#include<memory>
#include <string>
#include <vector>

//struct LocationData
//{
//	std::string name;
//	std::string tag;
//	VECTOR pos;
//	VECTOR rot;
//	VECTOR scale;
//};

class Player;
class GimmickObj
{
public:

	GimmickObj();
	virtual ~GimmickObj();

	void Init();
	void Update();
	void Draw();

	//プレイヤーとギミックの当たり判定の当たり判定
	bool SphereHitFlag(std::shared_ptr<Player> pPlayer);

private:
	int m_handle;
	int m_radius;
	VECTOR m_pos;


};

