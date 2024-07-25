#include "GimmickObj.h"
#include "Player.h"

namespace
{

	//float DegreeToRadian(float deg)
	//{
	//	return
	//}
}



GimmickObj::GimmickObj():
	m_radius(16)
{
	m_pos = VGet(0, 0, 0);

}

GimmickObj::~GimmickObj()
{

}

void GimmickObj::Init()
{

}

void GimmickObj::Update()
{

}

void GimmickObj::Draw()
{
	DrawSphere3D(m_pos , m_radius, 8, 0xffffff, 0xffffff, false);
	DrawFormatString(0, 48, 0xffffff, "Gimmick(x:%f,y:%f,z:%f)", m_pos.x, m_pos.y, m_pos.z);



}

bool GimmickObj::SphereHitFlag(std::shared_ptr<Player> pPlayer)
{
	//X,Y,Zの距離の成分を取得
	float delX = (m_pos.x - pPlayer->GetPos().x) * (m_pos.x - pPlayer->GetPos().x);
	float delY = (m_pos.y  - (pPlayer->GetPos().y + 8.0f)) * (m_pos.y  - (pPlayer->GetPos().y + 8.0f));
	float delZ = (m_pos.z - pPlayer->GetPos().z) * (m_pos.z - pPlayer->GetPos().z);

	//球と球の距離
	float Distance = sqrt( delX + delY + delZ);

	//球と球の距離がプレイヤとエネミーの半径よりも小さい場合
	if (Distance < m_radius + pPlayer->GetRadius())
	{
		return true;
	}

	return false;
}

//m_handle = FileRead_open("data/data.loc");
//int dataCnt = 0;
//FileRead_read(&dataCnt, sizeof(dataCnt), m_handle);
//locationData_.resize(dataCnt);
//for (auto& loc : locationData)
//{
   // //オブジェクト名ロード
   // byte nameCnt = 0;
   // FileRead_read(&nameCnt, sizeof(nameCnt), m_handle);
   // loc.name.resize(nameCnt);
   // FileRead_read(loc.name.data(), sizeof(nameCnt)*loc.name.size(), m_handle);



   // //座標情報
   // FileRead_read(loc.pos, sizeof(loc.pos) , m_handle);
   // //回転情報
   // FileRead_read(loc.rot, sizeof(loc.rot), m_handle);
   // //スケーリング情報
   // FileRead_read(loc.scale, sizeof(loc.scale), m_handle);

//}
//FileRead_close(m_handle);







	//int y = 50;
	//for (const auto& loc : locationData)
	//{
	//	DrawFormatString(50, y, 0xffffff, "name=%s, pos=%2.2f,%2.2f,%2.2f, rot=%2.2f,%2.2f,%2.2f, scale=%2.2f,%2.2f,%2.2f,"
	//		,loc.name.c_str(),
	//		loc.pos.x, loc.pos.y, loc.pos.z,
	//		loc.rot.x, loc.rot.y, loc.rot.z,
	//		loc.scale.x, loc.scale.y, loc.scale.z);
	//	y += 20;
	//}