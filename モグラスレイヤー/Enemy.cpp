#include "Enemy.h"
#include "Player.h"
#include <cmath>

/// <summary>
/// 定数
/// </summary>
namespace
{
	//モデルのファイル名
	const char* const kEnemyModelFilename = "data/model/enemy/knight.mv1";

	//シェーダのファイル名
	const char* const kOutlinePsFilename = "./OutlinePs.pso";
	const char* const kOutlineVsFilename = "./OutlineVs.vso";

}

/// <summary>
/// コンストラクタ
/// </summary>
Enemy::Enemy():
	m_modelHandle(),
	m_outlinePsH(-1),
	m_outlineVsH(-1),
	m_radius(6.0f),
	m_pos(VGet(0,0,0))
{

}

/// <summary>
/// デストラクタ
/// </summary>
Enemy::~Enemy()
{
}

/// <summary>
/// ロード
/// </summary>
void Enemy::Load()
{
	//モデルのロード
	//m_modelHandle[0] = MV1LoadModel(kEnemyModelFilename);
	m_modelHandle = MV1LoadModel(kEnemyModelFilename);

	//for (int i = 1; i < 9; i++)
	//{
	//	m_modelHandle[i] = MV1DuplicateModel(m_modelHandle[0]);
	//}

	//ShaderLoad();
}

/// <summary>
/// デリート
/// </summary>
void Enemy::Delete()
{
	//for (int i = 0; i < 9; i++)
	//{
	//	MV1DeleteModel(m_modelHandle[i]);
	//	m_modelHandle[i] = -1;
	//}
		MV1DeleteModel(m_modelHandle);
		m_modelHandle = -1;
}

/// <summary>
/// 初期化
/// </summary>
void Enemy::Init()
{
	m_pos = VGet(-60.0f , 0.0f, 0.0f);
	//for (int i = 0; i < 9; i++)
	//{
	//	//敵の初期位置設定
	//	MV1SetPosition(m_modelHandle[i], m_pos);
	//	//敵のスケール
	//	MV1SetScale(m_modelHandle[i], VGet(10, 10, 10));
	//}
	MV1SetPosition(m_modelHandle, m_pos);
	//敵のスケール
	MV1SetScale(m_modelHandle, VGet(10, 10, 10));
}

/// <summary>
/// アップデート
/// </summary>
void Enemy::Update()
{
	//for (int i = 0; i < 9; i++)
	//{
	//	MV1SetPosition(m_modelHandle[i], m_pos);
	//}

	MV1SetPosition(m_modelHandle, m_pos);
}

/// <summary>
/// 描画
/// </summary>
void Enemy::Draw()
{
	//for (int i = 0; i < 9; i++)
	//{
	//	MV1DrawModel(m_modelHandle[i]);
	//}
	MV1DrawModel(m_modelHandle);
	DrawSphere3D(m_pos, m_radius, 16, 0xffffff, 0xffffff, false);
}

/// <summary>
/// 球の当たり判定
/// </summary>
bool Enemy::SphereHitFlag(std::shared_ptr<Player> pPlayer)
{

	//X,Y,Zの距離の成分を取得
	float delX = (m_pos.x - pPlayer->GetPos().x) * (m_pos.x - pPlayer->GetPos().x);
	float delY = (m_pos.y - pPlayer->GetPos().y) * (m_pos.y - pPlayer->GetPos().y);
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

/// <summary>
/// 
/// </summary>
/// <param name="pPlayer"></param>
/// <returns></returns>
bool Enemy::AttackSphereHitFlag(std::shared_ptr<Player> pPlayer)
{


	return false;
}



