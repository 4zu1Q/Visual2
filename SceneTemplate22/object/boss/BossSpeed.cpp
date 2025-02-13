#include "BossSpeed.h"
#include "object/player/Player.h"

#include "util/AnimController.h"
#include "util/ActionTime.h"
#include "util/EffectManager.h"
#include "util/SoundManager.h"

#include "util/Pad.h"

#include <cmath>
#include <cassert>

namespace
{
	//プレイヤーのモデルファイル名
	const char* const kModelFilename = "Data/Model/Boss/BossSpeed.mv1";
	//モデルのスケール値
	constexpr float kModelScale = 8.0f;

	constexpr float kWalkSpeed = 0.4f;
	constexpr float kDashSpeed = 0.7f;

	constexpr float kAvoidSpeed = 2.0f;

	//初期位置
	constexpr VECTOR kInitPos = { 0.0f,15.0f,0.0f };

	//カプセルの上の座標
	constexpr VECTOR kUpPos = { 0.0f,18.0f,0.0f };

	/*ボスのアニメーションの種類*/
	const char* const kAnimSpeedInfoFilename = "Data/Master/AnimBossSpeedMaster.csv";

	const char* const kAnimIdle = "Idle";
	const char* const kAnimDash = "Dash";

	const char* const kAnimAttack1 = "Attack1";
	const char* const kAnimAttack2 = "Attack2";
	const char* const kAnimAttack3 = "Attack3";

	const char* const kAnimAvoid = "Avoid";

	const char* const kAnimCoolTime = "CoolTime";

	const char* const kAnimHit = "Hit";

	const char* const kAnimDown = "Down";
	const char* const kAnimDead = "Dead";

	//HPの最大値
	constexpr float kMaxHp = 400.0f;

	//


	//次の状態に遷移するまでの時間
	constexpr float kIdleToAttackTime = 40.0f;
	constexpr float kIdleToAvoidTime = 40.0f;
	constexpr float kCoolTimeToAvoidTime = 80.0f;
	constexpr float kAvoidToIdleTime = 29.0f;

	//次の状態に遷移するまでのプレイヤーとの長さ
	constexpr float kIdleToAttackLength = 20.0f;
	constexpr float kIdleToDashLength = 20.0f;
	constexpr float kWalkToIdleLength = 20.0f;

	constexpr float kWalkToDashLength = 100.0f;
	constexpr float kDashToWalkLength = 80.0f;

	//攻撃の種類
	constexpr int kAttackKind = 3;

	constexpr int kAttackNum = 2;

}

BossSpeed::BossSpeed() :
	BossBase(Collidable::e_Priority::kStatic, Game::e_GameObjectTag::kBoss, MyLib::ColliderData::e_Kind::kSphere, false),
	m_pos(kInitPos),
	m_posUp(kInitPos),
	m_direction(VGet(0, 0, 0)),
	m_velocity(VGet(0, 0, 0)),
	m_playerPos(VGet(0, 0, 0)),
	m_homePos(VGet(0,0,0)),
	m_angle(0.0f),
	m_nextAngle(0.0f),
	m_bossToPlayerLength(0.0f),
	m_bossToHomePosLength(0.0f),
	m_actionTime(0),
	m_attackNum(0),
	m_isAttackAction(false),
	m_isAvoidAction(false),
	m_attackType(0),
	m_moveAngle(0.0f),
	m_hp(400.0f)
{

	m_attackKind = Game::e_BossAttackKind::kBossAttackNone;
	m_playerAttackKind = Game::e_PlayerAttackKind::kPlayerAttackNone;
	m_playerKind = e_PlayerKind::kNormalPlayer;

	m_isClear = false;

	m_isAttack = false;
	m_isShock = false;
	m_isHit = false;
	m_isPlayerFace = false;


	m_downCountDown = 0;
	m_damageFrame = 0;
	m_preliminaryActionFrame = 0;
	m_attackFrame = 0;

	m_hitRadius = 8.0f;
	m_normalAttackRadius = 3.0f;
	m_weaponAttackRadius = 6.0f;
	m_shockRadius = 15.0f;

	m_hitPos = VGet(0, 0, 0);
	m_attackPos = VGet(0, 0, 0);
	m_shockAttackPos = VGet(0, 0, 0);
	m_attackDir = VGet(0, 0, 0);

	m_isPlayerAttack = false;
	m_isPlayerFace = false;


	m_modelH = MV1LoadModel(kModelFilename);

	m_pAnim = std::make_shared<AnimController>();

	m_pPlayer = std::make_shared<Player>();

	m_pColliderData = std::make_shared<MyLib::ColliderDataSphere>(false);

	auto circleColliderData = std::dynamic_pointer_cast<MyLib::ColliderDataSphere>(m_pColliderData);
	circleColliderData->m_radius = 6.5f;

}

BossSpeed::~BossSpeed()
{
	MV1DeleteModel(m_modelH);
	m_modelH = -1;
}

void BossSpeed::Initialize(std::shared_ptr<MyLib::Physics> physics)
{
	//
	Collidable::Initialize(physics);

	// 物理挙動の初期化
	m_rigidbody.Initialize(true);
	m_rigidbody.SetPos(kInitPos);
	//m_speed = 0.1f;

	//初期位置を代入
	m_pos = m_rigidbody.GetPos();

	//モデルのスケールを決める
	MV1SetScale(m_modelH, VGet(kModelScale, kModelScale, kModelScale));

	//アニメーションの初期化
	m_pAnim->Initialize(kAnimSpeedInfoFilename, m_modelH, kAnimIdle);

	// メンバ関数ポインタの初期化
	m_updateFunc = &BossSpeed::IdleUpdate;
}

void BossSpeed::Finalize(std::shared_ptr<MyLib::Physics> physics)
{
	Collidable::Finalize(physics);
}

void BossSpeed::Update(std::shared_ptr<MyLib::Physics> physics, Player& player, Game::e_PlayerAttackKind playerAttackKind)
{
	//アップデート
	(this->*m_updateFunc)();

	//アニメーションの更新処理
	m_pAnim->UpdateAnim();

	//プレイヤーとボスの距離を距離を求める
	VECTOR toPlayer = VSub(m_playerPos, m_pos);
	m_bossToPlayerLength = VSize(toPlayer);

	VECTOR toHomePos = VSub(m_homePos, m_pos);
	m_bossToHomePosLength = VSize(toHomePos);

	m_playerPos = player.GetPos();
	m_pos = m_rigidbody.GetPos();
	m_hitPos = VGet(m_pos.x, m_pos.y + 6.0f, m_pos.z);

	VECTOR attackMove = VScale(m_attackDir, 12.0f);
	VECTOR shockAttackMove = VScale(m_attackDir, 20.0f);
	m_attackPos = VAdd(m_hitPos, attackMove);
	m_shockAttackPos = VAdd(m_hitPos, shockAttackMove);

	m_playerAttackKind = playerAttackKind;
	m_playerKind = player.GetFaceKind();
	m_isPlayerFace = player.GetIsFaceUse();



	if (m_isHit)
	{
		m_damageFrame++;
	}
	else if (!m_isHit)
	{
		m_damageFrame = 0;
	}

	if (m_damageFrame >= 140)
	{
		m_isHit = false;
	}


	if (m_attackNum > 2)
	{
		m_isAvoidAction = true;
	}
	else
	{
		m_isAvoidAction = false;
	}

	//HPがゼロより下にいった場合
	if (m_hp <= 0)
	{
		m_hp = 0;

		//死亡状態へ遷移
		OnDead();
	}

	//auto pos = m_rigidbody.GetPos();

	//モデルのポジションを合わせるよう
	//VECTOR modelPos = VGet(pos.x, pos.y, pos.z);

	m_posUp = VGet(m_pos.x, m_pos.y + kUpPos.y, m_pos.z);

	//DrawSphere3D(m_pos, 32, 16, 0xffffff, 0xffffff, false);

	//モデルに座標をセットする
	MV1SetPosition(m_modelH, m_pos);
	MV1SetRotationXYZ(m_modelH, VGet(0.0f, m_angle + DX_PI_F, 0.0f));

}

void BossSpeed::Draw()
{
#ifdef _DEBUG

	DrawFormatString(0, 262, 0xff0fff, "SpeedBossPos:%f,%f,%f", m_pos.x, m_pos.y, m_pos.z);
	DrawFormatString(0, 368, 0xff0fff, "SpeedBossToPlayer:%f", m_bossToPlayerLength);
	DrawFormatString(0, 408, 0xff0fff, "BossToHomePos:%f", m_bossToHomePosLength);

	//DrawCapsule3D(m_posDown, m_posUp, m_radius, 32, 0xffffff, 0xffffff, false);
	DrawSphere3D(m_hitPos, m_hitRadius, 16, 0xffffff, 0xffffff, false);

	DrawSphere3D(m_attackPos, m_normalAttackRadius, 16, 0xff00ff, 0xffffff, false);
	DrawSphere3D(m_attackPos, m_weaponAttackRadius, 16, 0xff00ff, 0xffffff, false);
	DrawSphere3D(m_shockAttackPos, m_shockRadius, 16, 0xff00ff, 0xffffff, false);

	if (m_isAttack)
	{
		if (m_attackKind == Game::e_BossAttackKind::kBossAttack) DrawSphere3D(m_attackPos, m_normalAttackRadius, 16, 0xffff00, 0xffffff, false);
		if (m_attackKind == Game::e_BossAttackKind::kBossWeapon) DrawSphere3D(m_attackPos, m_weaponAttackRadius, 16, 0xffff00, 0xffffff, false);
		if (m_attackKind == Game::e_BossAttackKind::kBossShock) DrawSphere3D(m_shockAttackPos, m_shockRadius, 16, 0xffff00, 0xffffff, false);
	}

#endif // DEBUG

	if (m_damageFrame % 8 >= 4) return;

	if (!m_isClear)
	{
		MV1DrawModel(m_modelH);
	}
}

const VECTOR& BossSpeed::GetPosUp() const
{
	auto pos = VAdd(m_rigidbody.GetPos(), VGet(0.0f, 10.0f, 0.0f));
	return pos;
}

const VECTOR& BossSpeed::GetPosDown() const
{
	return m_rigidbody.GetPos();
}

void BossSpeed::SetPosDown(const VECTOR pos)
{
	m_rigidbody.SetPos(pos);
}

void BossSpeed::Hit()
{
	if (!m_isClear)
	{
		if (m_isPlayerAttack)
		{

			if (!m_isHit)
			{
				if (IsAttackXHit() == true && m_playerAttackKind == Game::e_PlayerAttackKind::kPlayerAttackX)
				{
					OnHitOneDamage();
				}
				else if (IsAttackYHit() == true && m_playerAttackKind == Game::e_PlayerAttackKind::kPlayerAttackY)
				{
					OnHitTwoDamage();
				}
				else if (IsShockAttackHit() == true && m_playerAttackKind == Game::e_PlayerAttackKind::kPlayerShock)
				{
					OnHitOneDamage();
				}
			}
		}
	}
}

void BossSpeed::IdleUpdate()
{
	Hit();

	m_rigidbody.SetVelocity(VGet(0, 0, 0));
	m_actionTime++;

	//auto pos = m_rigidbody.GetPos();

	//プレイヤーへの向きを取得
	m_direction = VSub(m_playerPos, m_pos);
	m_direction = VNorm(m_direction);

	//m_angle = atan2f(m_direction.x, m_direction.z);

	//プレイヤーと離れていた場合歩き状態に移動 && タイマー
	if (m_actionTime > kIdleToAttackTime && m_bossToPlayerLength > kIdleToDashLength)
	{
		OnDash();
	}
	//プレイヤーと十分な距離の場合 && タイマー
	else if (m_actionTime > kIdleToAvoidTime && m_bossToPlayerLength < kIdleToAttackLength)
	{
		//ランダム関数かなんか使ってやる

		m_attackType = GetRand(kAttackKind);

		//ランダムで攻撃を行う
		switch (m_attackType)
		{
		case 0:
			OnPreliminaryAttack1();
			break;
		case 1:
			OnPreliminaryAttack2();
			break;
		case 2:
			OnPreliminaryAttack3();
			break;
		case 3:
			OnPreliminaryAttack3();
			break;
		default:
			break;
		}

	}
	m_nextAngle = atan2(m_direction.x, m_direction.z);

	SmoothAngle(m_angle, m_nextAngle);

	m_attackDir = VGet(m_direction.x, m_direction.y, m_direction.z);
	m_attackDir = VNorm(m_attackDir);

	VECTOR move;
	move.y = m_rigidbody.GetVelocity().y;
	m_rigidbody.SetVelocity(VGet(0, move.y, 0));

}

void BossSpeed::DashUpdate()
{
	Hit();
	m_actionTime++;

	//プレイヤーへの向きを取得
	m_direction = VSub(m_playerPos, m_pos);

	VECTOR length = VSub(m_pos, m_playerPos);
	float size = VSize(length);


	m_direction = VNorm(m_direction);

	m_angle = atan2f(m_direction.x, m_direction.z);

	//ベクトルを、正規化し、向きだけを保存させる
	m_velocity = VScale(m_direction, kDashSpeed);

	//敵の移動
	m_rigidbody.SetVelocity(m_velocity);

	m_attackDir = VGet(m_direction.x, m_direction.y, m_direction.z);
	m_attackDir = VNorm(m_attackDir);

	if (m_actionTime > kIdleToAvoidTime && m_bossToPlayerLength < kIdleToAttackLength)
	{
		//ランダム関数かなんか使ってやる
		OnIdle();
	}

}

void BossSpeed::PlayerToDashUpdate()
{
	Hit();
	m_actionTime++;

	//プレイヤーへの向きを取得
	m_direction = VSub(m_playerPos, m_pos);

	VECTOR length = VSub(m_pos, m_playerPos);
	float size = VSize(length);


	m_direction = VNorm(m_direction);

	m_angle = atan2f(m_direction.x, m_direction.z);

	//ベクトルを、正規化し、向きだけを保存させる
	m_velocity = VScale(m_direction, kDashSpeed);

	//敵の移動
	m_rigidbody.SetVelocity(m_velocity);

	if (m_actionTime > kIdleToAvoidTime && m_bossToPlayerLength < kIdleToAttackLength)
	{
		//ランダム関数かなんか使ってやる

		m_attackType = GetRand(kAttackKind);

		//ランダムで攻撃を行う
		switch (m_attackType)
		{
		case 0:
			OnPreliminaryAttack1();
			break;
		case 1:
			OnPreliminaryAttack2();
			break;
		case 2:
			OnPreliminaryAttack3();
			break;
		case 3:
			OnPreliminaryAttack3();
			break;
		default:
			break;
		}

	}
}

void BossSpeed::HomePosDashUpdate()
{
	Hit();
	//定位置の向きを取得
	m_direction = VSub(m_homePos, m_pos);

	m_direction = VNorm(m_direction);

	m_angle = atan2f(m_direction.x, m_direction.z);

	//ベクトルを、正規化し、向きだけを保存させる
	m_velocity = VScale(m_direction, kDashSpeed);

	//敵の移動
	m_rigidbody.SetVelocity(m_velocity);

	if (m_bossToHomePosLength < 120)
	{
		OnDash();
	}

}

void BossSpeed::PreliminaryAttack1Update()
{
	Hit();
	m_rigidbody.SetVelocity(VGet(0, 0, 0));


	m_preliminaryActionFrame++;

	if (m_preliminaryActionFrame > 10)
	{
		OnAttack1();
	}
}

void BossSpeed::PreliminaryAttack2Update()
{
	Hit();
	m_rigidbody.SetVelocity(VGet(0, 0, 0));


	m_preliminaryActionFrame++;

	if (m_preliminaryActionFrame > 10)
	{
		OnAttack2();
	}
}

void BossSpeed::PreliminaryAttack3Update()
{
	Hit();
	m_rigidbody.SetVelocity(VGet(0, 0, 0));


	m_preliminaryActionFrame++;

	if (m_preliminaryActionFrame > 10)
	{
		OnAttack3();
	}
}

void BossSpeed::Attack1Update()
{
	Hit();

	m_attackFrame++;

	if (m_attackFrame > 15)
	{
		m_isAttack = false;
	}

	//アニメーションが終わったらアイドル状態に戻る
	if (m_pAnim->IsLoop() && m_isAvoidAction)
	{
		OnAvoid();
	}
	else if (m_pAnim->IsLoop() && !m_isAvoidAction)
	{
		OnIdle();
	}
	m_rigidbody.SetVelocity(VGet(0.0f, 0.0f, 0.0f));
}

void BossSpeed::Attack2Update()
{
	Hit();

	m_attackFrame++;

	if (m_attackFrame > 15)
	{
		m_isAttack = false;
	}

	//アニメーションが終わったらアイドル状態に戻る
	if (m_pAnim->IsLoop() && m_isAvoidAction)
	{
		OnAvoid();
	}
	else if (m_pAnim->IsLoop() && !m_isAvoidAction)
	{
		OnIdle();
	}
	m_rigidbody.SetVelocity(VGet(0.0f, 0.0f, 0.0f));
}

void BossSpeed::Attack3Update()
{
	Hit();

	m_attackFrame++;

	if (m_attackFrame > 30)
	{
		m_isAttack = true;
	}

	//アニメーションが終わったらクールタイム状態に入る
	if (m_pAnim->IsLoop())
	{
		OnAttackCoolTime();
	}

	m_rigidbody.SetVelocity(VGet(0.0f, 0.0f, 0.0f));
}

void BossSpeed::AvoidUpdate()
{
	Hit();
	m_actionTime++;

	//プレイヤーへの向きを取得
	m_direction = VSub(m_playerPos, m_pos);
	//正規化
	m_direction = VNorm(m_direction);
	//モデルの角度
	m_angle = atan2f(m_direction.x, m_direction.z);

	//ベクトルを、正規化し、向きだけを保存させる
	m_velocity = VScale(m_direction, -kAvoidSpeed);

	//敵の移動
	m_rigidbody.SetVelocity(m_velocity);

	//アニメーションが終わったらアイドル状態に戻る
	if (m_actionTime > kAvoidToIdleTime)
	{
		OnIdle();
	}

}

void BossSpeed::AttackCoolTimeUpdate()
{
	Hit();
	m_actionTime++;

	if (m_actionTime > kCoolTimeToAvoidTime && m_isAvoidAction)
	{
		OnAvoid();
	}
	else if (m_actionTime > kCoolTimeToAvoidTime && !m_isAvoidAction)
	{
		OnIdle();
	}
}

void BossSpeed::HitOneDamageUpdate()
{
	//アニメーションが終わったらアイドル状態に戻る
	m_rigidbody.SetVelocity(VGet(0, 0, 0));
	if (m_damageFrame > 34)
	{
		OnIdle();
	}
}

void BossSpeed::HitTwoDamageUpdate()
{
	//アニメーションが終わったらアイドル状態に戻る
	m_rigidbody.SetVelocity(VGet(0, 0, 0));
	if (m_damageFrame > 34)
	{
		OnIdle();
	}
}

void BossSpeed::DownUpdate()
{
	Hit();
	//アニメーションが終わったらアイドル状態に戻る
	if (m_pAnim->IsLoop())
	{
		OnIdle();
	}
}

void BossSpeed::DeadUpdate()
{
	//ワープアイテムが出現するフラグをおいておく
	if (m_pAnim->IsLoop())
	{
		m_isClear = true;
	}

	if (!m_isClear)
	{
		auto pos = m_rigidbody.GetPos();
		EffectManager::GetInstance().CreateEffect("gameClearEffect", VGet(pos.x, pos.y, pos.z));
	}

}


void BossSpeed::OnIdle()
{
	m_attackKind = Game::e_BossAttackKind::kBossAttackNone;
	m_attackFrame = 0;
	m_rigidbody.SetVelocity(VGet(0, 0, 0));
	m_actionTime = 0;
	m_pAnim->ChangeAnim(kAnimIdle);
	m_updateFunc = &BossSpeed::IdleUpdate;
}

void BossSpeed::OnDash()
{
	m_attackKind = Game::e_BossAttackKind::kBossAttackNone;
	m_actionTime = 0;
	m_pAnim->ChangeAnim(kAnimDash);
	m_updateFunc = &BossSpeed::DashUpdate;
}

void BossSpeed::OnPlayerToDash()
{
	m_attackKind = Game::e_BossAttackKind::kBossAttackNone;
	m_actionTime = 0;
	m_pAnim->ChangeAnim(kAnimDash);
	m_updateFunc = &BossSpeed::PlayerToDashUpdate;
}

void BossSpeed::OnHomePosDash()
{
	m_attackKind = Game::e_BossAttackKind::kBossAttackNone;
	m_actionTime = 0;
	m_pAnim->ChangeAnim(kAnimDash);
	m_updateFunc = &BossSpeed::HomePosDashUpdate;
}

void BossSpeed::OnPreliminaryAttack1()
{
	m_attackKind = Game::e_BossAttackKind::kBossAttackNone;
	m_rigidbody.SetVelocity(VGet(0, 0, 0));
	auto pos = m_rigidbody.GetPos();
	EffectManager::GetInstance().CreateEffect("speedPreliminaryActionEffect", VGet(pos.x, pos.y + 6.0f, pos.z));
	m_pAnim->ChangeAnim(kAnimIdle);
	m_updateFunc = &BossSpeed::PreliminaryAttack1Update;
}

void BossSpeed::OnPreliminaryAttack2()
{
	m_attackKind = Game::e_BossAttackKind::kBossAttackNone;
	m_rigidbody.SetVelocity(VGet(0, 0, 0));
	auto pos = m_rigidbody.GetPos();
	EffectManager::GetInstance().CreateEffect("speedPreliminaryActionEffect", VGet(pos.x, pos.y + 6.0f, pos.z));
	m_pAnim->ChangeAnim(kAnimIdle);
	m_updateFunc = &BossSpeed::PreliminaryAttack2Update;
}

void BossSpeed::OnPreliminaryAttack3()
{
	m_attackKind = Game::e_BossAttackKind::kBossAttackNone;
	m_rigidbody.SetVelocity(VGet(0, 0, 0));
	auto pos = m_rigidbody.GetPos();
	EffectManager::GetInstance().CreateEffect("speedPreliminaryActionEffect", VGet(pos.x, pos.y + 6.0f, pos.z));
	m_pAnim->ChangeAnim(kAnimIdle);
	m_updateFunc = &BossSpeed::PreliminaryAttack3Update;
}

void BossSpeed::OnAttack1()
{
	m_rigidbody.SetVelocity(VGet(0, 0, 0));

	m_isAttack = true;
	m_attackNum++;
	m_attackType = 0;
	m_actionTime = 0;
	m_attackKind = Game::e_BossAttackKind::kBossAttack;
	m_pAnim->ChangeAnim(kAnimAttack1, true, true, false);
	m_updateFunc = &BossSpeed::Attack1Update;
}

void BossSpeed::OnAttack2()
{
	m_rigidbody.SetVelocity(VGet(0, 0, 0));

	m_isAttack = true;
	m_attackNum++;
	m_attackType = 0;
	m_actionTime = 0;
	m_attackKind = Game::e_BossAttackKind::kBossAttack;
	m_pAnim->ChangeAnim(kAnimAttack2, true, true, false);
	m_updateFunc = &BossSpeed::Attack2Update;
}

void BossSpeed::OnAttack3()
{
	m_rigidbody.SetVelocity(VGet(0, 0, 0));

	m_isAttack = false;
	m_attackNum++;
	m_attackType = 0;
	m_actionTime = 0;
	m_attackKind = Game::e_BossAttackKind::kBossWeapon;
	m_pAnim->ChangeAnim(kAnimAttack3, true, true, false);
	EffectManager::GetInstance().CreateEffect("bossShockEffect", m_shockAttackPos);
	m_updateFunc = &BossSpeed::Attack3Update;
}

void BossSpeed::OnAvoid()
{
	m_rigidbody.SetVelocity(VGet(0, 0, 0));
	m_attackKind = Game::e_BossAttackKind::kBossAttackNone;

	m_attackNum = 0;
	m_attackType = 0;
	m_actionTime = 0;
	m_pAnim->ChangeAnim(kAnimAvoid, true, true, false);
	m_updateFunc = &BossSpeed::AvoidUpdate;
}

void BossSpeed::OnAttackCoolTime()
{
	m_rigidbody.SetVelocity(VGet(0, 0, 0));
	m_attackKind = Game::e_BossAttackKind::kBossAttackNone;

	m_attackType = 0;
	m_actionTime = 0;
	m_pAnim->ChangeAnim(kAnimCoolTime);
	m_updateFunc = &BossSpeed::AttackCoolTimeUpdate;
}

void BossSpeed::OnHitOneDamage()
{
	m_rigidbody.SetVelocity(VGet(0, 0, 0));
	m_attackKind = Game::e_BossAttackKind::kBossAttackNone;

	if (m_playerKind == e_PlayerKind::kPowerPlayer && m_isPlayerFace)
	{
		m_hp -= 40.0f;
	}
	if (m_playerKind == e_PlayerKind::kSpeedPlayer && m_isPlayerFace)
	{
		m_hp -= 15.0f;
	}
	if (m_playerKind == e_PlayerKind::kShotPlayer && m_isPlayerFace)
	{
		m_hp -= 10.0f;
	}
	if (m_playerKind == e_PlayerKind::kRassPlayer && m_isPlayerFace)
	{
		m_hp -= 50.0f;
	}

	if (!m_isPlayerFace)
	{
		m_hp -= 20.0f;
	}

	m_attackFrame = 0;
	m_isHit = true;
	m_isAttack = false;

	auto pos = m_rigidbody.GetPos();
	EffectManager::GetInstance().CreateEffect("bossHitEffect", VGet(pos.x, pos.y + 6.0f, pos.z));
	m_pAnim->ChangeAnim(kAnimCoolTime);
	m_updateFunc = &BossSpeed::HitOneDamageUpdate;
	//攻撃判定がバグらなければこっちの方がボスの難易度が上がってよい
	//m_updateFunc = &BossSpeed::IdleUpdate;

}

void BossSpeed::OnHitTwoDamage()
{
	m_rigidbody.SetVelocity(VGet(0, 0, 0));
	m_attackKind = Game::e_BossAttackKind::kBossAttackNone;

	if (m_playerKind == e_PlayerKind::kPowerPlayer && m_isPlayerFace)
	{
		m_hp -= 80.0f;
	}
	if (m_playerKind == e_PlayerKind::kSpeedPlayer && m_isPlayerFace)
	{
		m_hp -= 40.0f;
	}
	if (m_playerKind == e_PlayerKind::kShotPlayer && m_isPlayerFace)
	{
		m_hp -= 40.0f;
	}
	if (m_playerKind == e_PlayerKind::kRassPlayer && m_isPlayerFace)
	{
		m_hp -= 100.0f;
	}

	if (!m_isPlayerFace)
	{
		m_hp -= 50.0f;
	}

	m_attackFrame = 0;
	m_isHit = true;
	m_isAttack = false;

	auto pos = m_rigidbody.GetPos();
	EffectManager::GetInstance().CreateEffect("bossHitEffect", VGet(pos.x, pos.y + 6.0f, pos.z));
	m_pAnim->ChangeAnim(kAnimCoolTime);
	m_updateFunc = &BossSpeed::HitTwoDamageUpdate;

	//攻撃判定がバグらなければこっちの方がボスの難易度が上がってよい
	//m_updateFunc = &BossSpeed::IdleUpdate;
}

void BossSpeed::OnDown()
{
	m_rigidbody.SetVelocity(VGet(0, 0, 0));
	m_attackKind = Game::e_BossAttackKind::kBossAttackNone;

	m_attackType = 0;
	m_actionTime = 0;
	m_pAnim->ChangeAnim(kAnimDown);
	m_updateFunc = &BossSpeed::DownUpdate;
}

void BossSpeed::OnDead()
{
	m_rigidbody.SetVelocity(VGet(0, 0, 0));
	m_attackKind = Game::e_BossAttackKind::kBossAttackNone;

	m_attackType = 0;
	m_actionTime = 0;

	m_pAnim->ChangeAnim(kAnimDead, false, true, true);
	m_updateFunc = &BossSpeed::DeadUpdate;
}