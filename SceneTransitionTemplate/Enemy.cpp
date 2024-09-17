#include "Enemy.h"
#include "Player.h"
#include <cmath>

/// <summary>
/// 定数
/// </summary>
namespace
{
	//モデルのファイル名
	const char* const kEnemyModelFilename = "data/model/enemy/Enemy.mv1";
	const char* const kWeponModelFilename = "data/model/enemy/Skeleton_Blade.mv1";
	const char* const kHpFlameFilename = "data/image/EnemyHpFlame.png";

	//シェーダのファイル名
	const char* const kOutlinePsFilename = "./OutlinePs.pso";
	const char* const kOutlineVsFilename = "./OutlineVs.vso";

	//アニメーション
	constexpr int kIdleAnimIndex = 42;		//待機
	constexpr int kDashAnimIndex = 55;		//走り
	constexpr int kAttackAnimIndex = 5;	//攻撃
	constexpr int kSkillAnimIndex = 12;	//スキル
	constexpr int kDamageAnimIndex = 25;	//ダメージ
	constexpr int kFallAnimIndex = 27;		//倒れる
	constexpr int kFallingAnimIndex = 28;	//倒れ中

	//アニメーションの切り替えにかかるフレーム数
	constexpr float kAnimChangeFrame = 8.0f;
	constexpr float kAnimChangeRateSpeed = 1.0f / kAnimChangeFrame;

	//当たり判定
	constexpr float kAddPosY = 8.0f;

	//ダメージ
	constexpr int kDamageCount = 60;

	//スピード
	constexpr float kSpeed = 0.8f;

	constexpr float kWall = 475;

	constexpr int kMaxHp = 500;

	constexpr int kMax = 2;
	constexpr int kHalf = 5;
}

/// <summary>
/// コンストラクタ
/// </summary>
Enemy::Enemy() :
	m_modelH(-1),
	m_modelWeponH(-1),
	m_hpFlameH(LoadGraph(kHpFlameFilename)),
	m_outlinePsH(-1),
	m_outlineVsH(-1),
	m_radius(15.0f),
	m_skillRadius(25.0f),
	m_searchRadius(300.0f),
	m_stopRadius(40.0f),
	m_hp(kMaxHp),
	m_pos(VGet(0, 0, 0)),
	m_attackPos(VGet(0,0,0)),
	m_velocity(VGet(0, 0, 0)),
	m_direction(VGet(0, 0, 0)),
	m_attackDir(VGet(0, 0, 0)),
	m_dirPos(VGet(0, 0, 0)),
	m_damageFrame(0),
	m_isDamage(false),
	m_isIdleAnim(false),
	m_isRunAnim(false),
	m_isAttackAnim(false),
	m_isSkillAnim(false),
	m_isDamageAnim(false),
	m_isDeadAnim(false),
	m_isDeadingAnim(false),
	m_isAttackGeneration(false),
	m_isSkillGeneration(false),
	m_isRand(false),
	m_isMove(false),
	m_state(kIdle),
	m_angle(0.0f),
	m_frame(0),
	m_attackFrame(0),
	m_currentAnimNo(-1),
	m_prevAnimNo(-1),
	m_animBlendRate(0.0f),
	m_random(0),
	m_rand(0),
	m_workFrame(0),
	m_attachFramePosition(VGet(0,0,0))
{

}

/// <summary>
/// デストラクタ
/// </summary>
Enemy::~Enemy()
{
	Delete();
}

/// <summary>
/// ロード
/// </summary>
void Enemy::Load()
{
	//モデルのロード
	//m_modelHandle[0] = MV1LoadModel(kEnemyModelFilename);
	m_modelH = MV1LoadModel(kEnemyModelFilename);
	m_modelWeponH = MV1LoadModel(kWeponModelFilename);
	//ShaderLoad();
}

/// <summary>
/// デリート
/// </summary>
void Enemy::Delete()
{
	MV1DeleteModel(m_modelH);
	MV1DeleteModel(m_modelWeponH);
	m_modelH = -1;
	m_modelWeponH = -1;
}

/// <summary>
/// 初期化
/// </summary>
void Enemy::Init()
{
	m_pos = VGet(-60.0f, 0.0f, 0.0f);
	m_attackPos = VGet(m_pos.x, m_pos.y, m_pos.z - 10);

	MV1SetPosition(m_modelH, m_pos);
	//敵のスケール
	MV1SetScale(m_modelH, VGet(20, 20, 20));
	MV1SetScale(m_modelWeponH, VGet(0.1, 0.1, 0.1));
}

/// <summary>
/// アップデート
/// </summary>
void Enemy::Update(std::shared_ptr<Player> pPlayer)
{

	MATRIX transMat;
	MATRIX attachFrameMat;
	MATRIX mixMatrix;

	//アタッチするモデルのMV1SetMatrixの設定を無効にする
	MV1SetMatrix(m_modelWeponH, MGetIdent());

	m_attachFramePosition = MV1GetFramePosition(m_modelWeponH, 0);

	transMat = MGetTranslate(VScale(m_attachFramePosition, -1.0f));

	attachFrameMat = MV1GetFrameLocalWorldMatrix(m_modelH, 14);

	mixMatrix = MMult(transMat, attachFrameMat);

	MV1SetMatrix(m_modelWeponH, mixMatrix);


	//アニメーション
	if (m_prevAnimNo != -1)
	{
		// test 8フレーム切り替え
		m_animBlendRate += kAnimChangeRateSpeed;
		if (m_animBlendRate >= 1.0f) m_animBlendRate = 1.0f;

		//変更後のアニメーション割合を設定する
		MV1SetAttachAnimBlendRate(m_modelH, m_prevAnimNo, 1.0f - m_animBlendRate);
		MV1SetAttachAnimBlendRate(m_modelH, m_currentAnimNo, m_animBlendRate);
	}

	//アニメーションを進める
	bool isLoop = UpdateAnim(m_currentAnimNo, m_isDeadAnim);
	if (isLoop)
	{
		ChangeAnim(m_animIndex);
	}
	UpdateAnim(m_prevAnimNo);

	if (m_state == kDeath)
	{
		if (!m_isDeadAnim)
		{
			ChangeAnim(kFallAnimIndex);
			m_animIndex = kFallingAnimIndex;
			m_isDeadAnim = true;
		}
		
	}
	else
	{

		//プレイヤーへの向きを取得
		m_direction = VSub(pPlayer->GetPos(), m_pos);
		m_direction = VNorm(m_direction);


		if (m_state == kIdle)			//止まっている状態
		{
			if (!m_isIdleAnim)
			{
				ChangeAnim(kIdleAnimIndex);
			}
			m_isIdleAnim = true;

			m_animIndex = kIdleAnimIndex;
		}
		else
		{
			m_isIdleAnim = false;
		}

		if (!m_isAttackAnim && !m_isSkillAnim)
		{

			if (m_state == kRun)		//プレイヤーを追っている状態
			{

				m_frame++;

				m_angle = atan2f(m_direction.x, m_direction.z);

				m_animIndex = kDashAnimIndex;

				//ベクトルを、正規化し、向きだけを保存させる
				m_velocity = VScale(m_direction, kSpeed);

				//敵の移動
				m_pos = VAdd(m_pos, m_velocity);

				if (!m_isRunAnim)
				{
					ChangeAnim(kDashAnimIndex);
				}
				m_isRunAnim = true;
			}
		}


		//else
		//{
		//	if (m_isIdleAnim && m_isRunAnim)
		//	{
		//		ChangeAnim(kIdleAnimIndex);
		//	}
		//	m_isRunAnim = false;
		//	m_animIndex = kIdleAnimIndex;
		//}

		

		if (m_state == kAttack)	//攻撃の状態
		{

			if (!m_isRand)
			{
				m_rand = GetRand(2);
				m_isRand = true;
			}

			


			if (!m_isAttackAnim && !m_isSkillAnim)
			{

				m_attackFrame++;

				if (m_attackFrame > 150)
				{
					if (m_rand <= 1)
					{
						ChangeAnim(kAttackAnimIndex);
						m_isAttackAnim = true;
						m_isRand = false;
						m_isMove = true;
					}
					else
					{
						ChangeAnim(kSkillAnimIndex);
						m_isSkillAnim = true;
						m_isRand = false;
						m_isMove = true;
					}

					m_attackFrame = 0;
				}
				


			}
			else
			{
				if (isLoop)
				{

					m_isAttackAnim = false;
					m_isSkillAnim = false;

					m_isAttackGeneration = false;
					m_isSkillGeneration = false;
				}
			}


		}
		else
		{
			m_isIdleAnim = false;
			m_isMove = false;
		}

		//アニメーションが終わったら当たり判定の生成をやめる
		if (m_isAttackAnim && !m_isSkillAnim)
		{
			m_isAttackGeneration = true;
		}
		else
		{
			m_isAttackGeneration = false;
		}
		//アニメーションが終わったら当たり判定の生成をやめる
		if (m_isSkillAnim && !m_isAttackAnim)
		{
			m_isSkillGeneration = true;
		}
		else
		{
			m_isSkillGeneration = false;
		}
	}

	//移動範囲
	if (m_pos.x >= kWall) m_pos.x = kWall;
	if (m_pos.x <= -kWall) m_pos.x = -kWall;
	if (m_pos.z >= kWall) m_pos.z = kWall;
	if (m_pos.z <= -kWall) m_pos.z = -kWall;

	//ダメージ点滅時間
	if (m_isDamage)
	{
		m_damageFrame++;
		if (m_damageFrame > kDamageCount)
		{
			m_isDamage = false;
			m_damageFrame = 0;
		}
	}


	if (!m_isAttackAnim && !m_isSkillAnim)
	{
		m_angle = atan2f(m_direction.x, m_direction.z);

		VECTOR m_attackDir = VScale(m_direction, 20.0f);
		m_attackPos = VAdd(m_pos, m_attackDir);
	}

	if (m_hp <= 0)
	{
		m_hp = 0;
	}

	MV1SetPosition(m_modelH, m_pos);
	MV1SetRotationXYZ(m_modelH, VGet(0.0f, m_angle + DX_PI_F, 0.0f));
}

/// <summary>
/// 描画
/// </summary>
void Enemy::Draw()
{

	DrawBox(700, 13, 700 + kMaxHp, 50, 0xdc143c, true);
	DrawBox(700, 13, 700 + m_hp, 50, 0x3cb371, true);
	DrawExtendGraph(620, 10,1205, 50, m_hpFlameH, true);

#ifdef _DEBUG

	DrawSphere3D(VAdd(m_pos, VGet(0, 8, 0)), m_radius, 8, 0xffffff, 0xffffff, false);
	DrawSphere3D(VAdd(m_pos, VGet(0, 8, 0)), m_searchRadius, 8, 0xffffff, 0xffffff, false);
	DrawSphere3D(VAdd(m_pos, VGet(0, 8, 0)), m_stopRadius, 8, 0xffffff, 0xffffff, false);
	DrawSphere3D(VAdd(m_attackPos, VGet(0, 8, 0)), m_radius, 8, 0xf00fff, 0xffffff, false);
	DrawFormatString(0, 32, 0xffffff, "Enemy(x:%f,y:%f,z:%f)", m_pos.x, m_pos.y, m_pos.z);
	DrawFormatString(400, 32, 0xffffff, "EnemyHp:%d", m_hp);
	DrawFormatString(400, 332, 0xffffff, "EnemyState:%d", m_state);
	DrawFormatString(400, 352, 0xffffff, "EnemyRand:%d", m_rand);

	DrawFormatString(400, 382, 0xffffff, "EnemyAttack:%d", m_isAttackAnim);
	DrawFormatString(400, 412, 0xffffff, "EnemySkill:%d", m_isSkillAnim);


	if (!m_isAttackAnim)
	{
		DrawSphere3D(VAdd(m_attackPos, VGet(0, 8, 0)), m_radius, 8, 0xff00ff, 0xff00ff, false);
	}
	else
	{
		DrawSphere3D(VAdd(m_attackPos, VGet(0, 8, 0)), m_radius, 8, 0x0000ff, 0x0000ff, false);
	}

	if (!m_isSkillAnim)
	{
		DrawSphere3D(VAdd(m_pos, VGet(0, 8, 0)), m_skillRadius, 8, 0xff00ff, 0xff00ff, false);
	}
	else
	{
		DrawSphere3D(VAdd(m_pos, VGet(0, 8, 0)), m_skillRadius, 8, 0x0000ff, 0x0000ff, false);
	}

	if (m_state == kDeath)
	{

	}

#endif

	// ダメージ演出  2フレーム間隔で表示非表示切り替え
// 0: 表示される
// 1: 表示される
// 2: 非表示
// 3: 非表示
// 4: 表示される	...
// % 4 することで012301230123... に変換する
	if (m_damageFrame % 8 >= 4) return;

	MV1DrawModel(m_modelWeponH);
	MV1DrawModel(m_modelH);
	
}

/// <summary>
/// 球の当たり判定
/// </summary>
bool Enemy::SphereHitFlag(std::shared_ptr<Player> pPlayer)
{

	//X,Y,Zの距離の成分を取得
	float delX = (m_pos.x - pPlayer->GetPos().x) * (m_pos.x - pPlayer->GetPos().x);
	float delY = ((m_pos.y + kAddPosY) - (pPlayer->GetPos().y + kAddPosY)) *
		((m_pos.y + kAddPosY) - (pPlayer->GetPos().y + kAddPosY));
	float delZ = (m_pos.z - pPlayer->GetPos().z) * (m_pos.z - pPlayer->GetPos().z);

	//球と球の距離
	float Distance = sqrt(delX + delY + delZ);

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
bool Enemy::PlayerAttackSphereHitFlag(std::shared_ptr<Player> pPlayer)
{
	//X,Y,Zの距離の成分を取得
	float delX = (m_pos.x - pPlayer->GetAttackPos().x) * (m_pos.x - pPlayer->GetAttackPos().x);
	float delY = ((m_pos.y + kAddPosY) - (pPlayer->GetAttackPos().y + kAddPosY)) *
		((m_pos.y + kAddPosY) - (pPlayer->GetAttackPos().y + kAddPosY));
	float delZ = (m_pos.z - pPlayer->GetAttackPos().z) * (m_pos.z - pPlayer->GetAttackPos().z);

	//球と球の距離
	float Distance = sqrt(delX + delY + delZ);

	//球と球の距離がプレイヤとエネミーの半径よりも小さい場合
	if (Distance < m_radius + pPlayer->GetAttackRadius())
	{
		return true;
	}

	return false;
}

bool Enemy::PlayerSkillSphereHitFlag(std::shared_ptr<Player> pPlayer)
{
	//X,Y,Zの距離の成分を取得
	float delX = (m_pos.x - pPlayer->GetAttackPos().x) * (m_pos.x - pPlayer->GetAttackPos().x);
	float delY = ((m_pos.y + kAddPosY) - (pPlayer->GetAttackPos().y + kAddPosY)) *
		((m_pos.y + kAddPosY) - (pPlayer->GetAttackPos().y + kAddPosY));
	float delZ = (m_pos.z - pPlayer->GetAttackPos().z) * (m_pos.z - pPlayer->GetAttackPos().z);

	//球と球の距離
	float Distance = sqrt(delX + delY + delZ);

	//球と球の距離がプレイヤとエネミーの半径よりも小さい場合
	if (Distance < m_radius + pPlayer->GetSkillRadius())
	{
		return true;
	}

	return false;

}

bool Enemy::EnemyAttackSphereHitFlag(std::shared_ptr<Player> pPlayer)
{
	//X,Y,Zの距離の成分を取得
	float delX = (m_attackPos.x - pPlayer->GetPos().x) * (m_attackPos.x - pPlayer->GetPos().x);
	float delY = ((m_attackPos.y + kAddPosY) - (pPlayer->GetPos().y + kAddPosY)) *
		((m_attackPos.y + kAddPosY) - (pPlayer->GetPos().y + kAddPosY));
	float delZ = (m_attackPos.z - pPlayer->GetPos().z) * (m_attackPos.z - pPlayer->GetPos().z);

	//球と球の距離
	float Distance = sqrt(delX + delY + delZ);

	//球と球の距離がプレイヤとエネミーの半径よりも小さい場合
	if (Distance < m_radius + pPlayer->GetRadius())
	{
		return true;
	}

	return false;
}

bool Enemy::EnemySkillSphereHitFlag(std::shared_ptr<Player> pPlayer)
{
	//X,Y,Zの距離の成分を取得
	float delX = (m_pos.x - pPlayer->GetPos().x) * (m_pos.x - pPlayer->GetPos().x);
	float delY = ((m_pos.y + kAddPosY) - (pPlayer->GetPos().y + kAddPosY)) *
		((m_pos.y + kAddPosY) - (pPlayer->GetPos().y + kAddPosY));
	float delZ = (m_pos.z - pPlayer->GetPos().z) * (m_pos.z - pPlayer->GetPos().z);

	//球と球の距離
	float Distance = sqrt(delX + delY + delZ);

	//球と球の距離がプレイヤとエネミーの半径よりも小さい場合
	if (Distance < m_skillRadius + pPlayer->GetRadius())
	{
		return true;
	}

	return false;
}

bool Enemy::SearchSphereFlag(std::shared_ptr<Player> pPlayer)
{

	//X,Y,Zの距離の成分を取得
	float delX = (m_pos.x - pPlayer->GetPos().x) * (m_pos.x - pPlayer->GetPos().x);
	float delY = ((m_pos.y + kAddPosY) - (pPlayer->GetPos().y + kAddPosY)) *
		((m_pos.y + kAddPosY) - (pPlayer->GetPos().y + kAddPosY));
	float delZ = (m_pos.z - pPlayer->GetPos().z) * (m_pos.z - pPlayer->GetPos().z);

	//球と球の距離
	float Distance = sqrt(delX + delY + delZ);

	//球と球の距離がプレイヤとエネミーの半径よりも小さい場合
	if (Distance < m_searchRadius + pPlayer->GetRadius())
	{
		return true;
	}

	return false;
}

bool Enemy::StopSphereFlag(std::shared_ptr<Player> pPlayer)
{
	//X,Y,Zの距離の成分を取得
	float delX = (m_pos.x - pPlayer->GetPos().x) * (m_pos.x - pPlayer->GetPos().x);
	float delY = ((m_pos.y + kAddPosY) - (pPlayer->GetPos().y + kAddPosY)) *
		((m_pos.y + kAddPosY) - (pPlayer->GetPos().y + kAddPosY));
	float delZ = (m_pos.z - pPlayer->GetPos().z) * (m_pos.z - pPlayer->GetPos().z);

	//球と球の距離
	float Distance = sqrt(delX + delY + delZ);

	//球と球の距離がプレイヤとエネミーの半径よりも小さい場合
	if (Distance < m_stopRadius + pPlayer->GetRadius())
	{
		return true;
	}

	return false;
}

/// <summary>
/// アニメーションのアップデート処理
/// </summary>
/// <param name="attachNo"></param>
/// <returns></returns>
bool Enemy::UpdateAnim(int attachNo, bool isStayEndAnim)
{
	//アニメーションが設定されていないので終了
	if (attachNo == -1) return false;

	//アニメーションを進行させる
	float now = MV1GetAttachAnimTime(m_modelH, attachNo);	//現在の再生カウントを取得
	bool isLoop = false;
	now += 0.5f;	//アニメーションを進める

	//現在再生中のアニメーションの総カウントを取得する
	float total = MV1GetAttachAnimTotalTime(m_modelH, attachNo);

	while (now > total)
	{
		if (isStayEndAnim)	now = total;
		else				now -= total;
		isLoop = true;
	}

	//進めた時間に設定
	MV1SetAttachAnimTime(m_modelH, attachNo, now);

	return isLoop;
}

/// <summary>
/// アニメーションを変更する関数
/// </summary>
/// <param name="animIndex"></param>
void Enemy::ChangeAnim(int animIndex)
{
	//さらに古いアニメーションがアタッチされている場合はこの時点で削除しておく
	if (m_prevAnimNo != -1)
	{
		MV1DetachAnim(m_modelH, m_prevAnimNo);
	}


	//現在再生中の待機アニメーションは変更前のアニメーション扱いに
	m_prevAnimNo = m_currentAnimNo;

	//変更後のアニメーションとして攻撃アニメーションを改めて設定する
	m_currentAnimNo = MV1AttachAnim(m_modelH, animIndex, -1, false);

	//切り替えの瞬間は変更前のアニメーションが再生される状態にする
	m_animBlendRate = 0.0f;

	//変更前のアニメーション100%
	MV1SetAttachAnimBlendRate(m_modelH, m_prevAnimNo, 1.0f - m_animBlendRate);

	//変更後のアニメーション0%
	MV1SetAttachAnimBlendRate(m_modelH, m_currentAnimNo, m_animBlendRate);
}

void Enemy::Move()
{

}

