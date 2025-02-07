#include "BossShot.h"
#include "object/player/Player.h"

#include "util/AnimController.h"
#include "util/ActionTime.h"
#include "util/Pad.h"
#include "util/EffectManager.h"
#include "util/SoundManager.h"

#include <cmath>
#include <cassert>

namespace
{
	//プレイヤーのモデルファイル名
	const char* const kModelFilename = "Data/Model/Boss/BossShot.mv1";
	const char* const kWeaponModelFilename = "Data/Model/Weapon/Player_MagicWand.mv1";

	//モデルのスケール値
	constexpr float kModelScale = 10.0f;

	constexpr float kWalkSpeed = 0.4f;
	constexpr float kDashSpeed = 0.7f;

	constexpr float kAvoidSpeed = 3.0f;

	//初期位置
	constexpr VECTOR kInitPos = { 0.0f,20.0f,200.0f };

	//カプセルの上の座標
	constexpr VECTOR kUpPos = { 0.0f,18.0f,0.0f };

	/*ボスのアニメーションの種類*/
	const char* const kAnimBossInfoFilename = "Data/Master/AnimBossShotMaster.csv";

	const char* const kAnimIdle = "Idle";
	const char* const kAnimWalk = "Walk";
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

	//次の状態に遷移するまでの時間
	constexpr float kIdleToAttackTime = 30.0f;
	constexpr float kIdleToDashTime = 70.0f;
	constexpr float kCoolTimeToIdleTime = 60.0f;
	constexpr float kAvoidToIdleTime = 29.0f;

	//次の状態に遷移するまでのプレイヤーとの長さ
	constexpr float kIdleToAttackLength = 100.0f;
	constexpr float kIdleToAvoidLength = 100.0f;
	constexpr float kWalkToIdleLength = 20.0f;

	//攻撃の種類
	constexpr int kActionKind = 3;
	constexpr int kAttackKind = 3;
	constexpr int kAvoidKind = 2;

}

BossShot::BossShot() :
	BossBase(Collidable::e_Priority::kStatic, Game::e_GameObjectTag::kBoss, MyLib::ColliderData::e_Kind::kSphere, false),
	m_pos(kInitPos),
	m_posUp(kInitPos),
	m_direction(VGet(0, 0, 0)),
	m_velocity(VGet(0, 0, 0)),
	m_playerPos(VGet(0, 0, 0)),
	m_angle(0.0f),
	m_nextAngle(0.0f),
	m_length(0.0f),
	m_actionTime(0),
	m_isAttack(false),
	m_isAvoid(false),
	m_actionKind(0),
	m_dashFrontLength(0.0f),
	m_dashBackLength(0.0f),
	m_dashRightLength(0.0f),
	m_avoidLeftLength(0.0f),
	m_hp(400.0f),
	m_attachFramePos({0,0,0}),
	m_modelRightFrame(54)
{
	m_isClear = false;

	m_avoidKind = e_AvoidKind::kFront;

	m_playerAttackKind = Game::e_PlayerAttackKind::kPlayerAttackNone;
	m_attackKind = Game::e_BossAttackKind::kBossAttackNone;
	m_playerKind = e_PlayerKind::kNormalPlayer;

	m_isAttack = false;
	m_isShock = false;
	m_isHit = false;
	m_isPlayerFace = false;

	m_preliminaryActionFrame = 0;
	m_downCountDown = 0;
	m_damageFrame = 0;
	m_attackFrame = 0;

	m_hitRadius = 8.0f;
	m_normalAttackRadius = 3.0f;
	m_weaponAttackRadius = 3.0f;
	m_shockRadius = 5.0f;

	m_hitPos = VGet(0, 0, 0);
	m_attackPos = VGet(0, 0, 0);
	m_shockAttackPos = VGet(0, 0, 0);
	m_attackDir = VGet(0, 0, 0);

	//回避先の座標
	m_dashFrontPos = { 0.0f, 0.0f, 200.0f };
	m_dashBackPos = { 0.0f, 0.0f, -200.0f };
	m_dashRightPos = { 200.0f, 0.0f, 0.0f };
	m_dashLeftPos = { -200.0f, 0.0f, 0.0f };

	m_modelH = MV1LoadModel(kModelFilename);
	m_weaponModelH = MV1LoadModel(kWeaponModelFilename);

	m_pAnim = std::make_shared<AnimController>();

	m_pPlayer = std::make_shared<Player>();

	m_pColliderData = std::make_shared<MyLib::ColliderDataSphere>(false);

	auto circleColliderData = std::dynamic_pointer_cast<MyLib::ColliderDataSphere>(m_pColliderData);
	circleColliderData->m_radius = 6.5f;

}

BossShot::~BossShot()
{
	MV1DeleteModel(m_modelH);
	MV1DeleteModel(m_weaponModelH);
	m_modelH = -1;
	m_weaponModelH = -1;
}

void BossShot::Initialize(std::shared_ptr<MyLib::Physics> physics)
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
	m_pAnim->Initialize(kAnimBossInfoFilename, m_modelH, kAnimIdle);

	// メンバ関数ポインタの初期化
	m_updateFunc = &BossShot::IdleUpdate;

}

void BossShot::Finalize(std::shared_ptr<MyLib::Physics> physics)
{
	Collidable::Finalize(physics);
}

void BossShot::Update(std::shared_ptr<MyLib::Physics> physics, Player& player, Game::e_PlayerAttackKind playerAttackKind)
{
	//アニメーションの更新処理
	m_pAnim->UpdateAnim();

	//アップデート
	(this->*m_updateFunc)();

	/*武器のアタッチ*/
	MATRIX transMat;
	MATRIX attachFrameMat;
	MATRIX mixMatrix;

	//アタッチするモデルのMV1SetMatrixの設定を無効にする
	MV1SetMatrix(m_weaponModelH, MGetIdent());

	m_attachFramePos = MV1GetFramePosition(m_weaponModelH, 0);

	transMat = MGetTranslate(VScale(m_attachFramePos, -1.0f));

	attachFrameMat = MV1GetFrameLocalWorldMatrix(m_modelH, m_modelRightFrame);

	mixMatrix = MMult(transMat, attachFrameMat);

	MV1SetMatrix(m_weaponModelH, mixMatrix);


	/*座標の取得*/
	m_playerPos = player.GetPos();
	m_pos = m_rigidbody.GetPos();
	m_hitPos = VGet(m_pos.x, m_pos.y + 6.0f, m_pos.z);
	
	//VECTOR attackMove = VScale(m_attackDir, 12.0f);
	//VECTOR shockAttackMove = VScale(m_attackDir, 20.0f);
	//m_attackPos = VAdd(m_hitPos, attackMove);
	//m_shockAttackPos = VAdd(m_hitPos, shockAttackMove);

	m_playerAttackKind = playerAttackKind;
	m_playerKind = player.GetFaceKind();
	m_isPlayerFace = player.GetIsFaceUse();

	m_dashFrontPos.y = m_pos.y;
	m_dashBackPos.y = m_pos.y;
	m_dashRightPos.y = m_pos.y;
	m_dashLeftPos.y = m_pos.y;

	//プレイヤーとボスの距離を求める
	VECTOR toPlayer = VSub(m_playerPos, m_pos);
	m_length = VSize(toPlayer);

	//ボスとボスが移動する場所の距離を求める
	VECTOR toAvoidFront = VSub(m_dashFrontPos, m_pos);
	m_dashFrontLength = VSize(toAvoidFront);

	VECTOR toAvoidBack = VSub(m_dashBackPos, m_pos);
	m_dashBackLength = VSize(toAvoidBack);

	VECTOR toAvoidRight = VSub(m_dashRightPos, m_pos);
	m_dashRightLength = VSize(toAvoidRight);

	VECTOR toAvoidLeft = VSub(m_dashLeftPos, m_pos);
	m_avoidLeftLength = VSize(toAvoidLeft);

	if (m_isHit)
	{
		m_damageFrame++;
	}
	else if(!m_isHit)
	{
		m_damageFrame = 0;
	}

	if (m_damageFrame >= 140)
	{
		m_isHit = false;
	}

	if (m_length > 60)
	{
		m_isAvoid = true;
	}
	else
	{
		m_isAvoid = false;
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

void BossShot::Draw()
{
	MV1DrawModel(m_modelH);
	MV1DrawModel(m_weaponModelH);

#ifdef _DEBUG

	DrawFormatString(0, 280, 0xff0fff, "ShotBossPos:%f,%f,%f", m_pos.x, m_pos.y, m_pos.z);
	DrawFormatString(0, 388, 0xff0fff, "ShotBossToPlayer:%f", m_length);

	DrawFormatString(0, 408, 0xff0fff, "F:%f", m_dashFrontLength);
	DrawFormatString(0, 428, 0xff0fff, "B:%f", m_dashBackLength);
	DrawFormatString(0, 448, 0xff0fff, "R:%f", m_dashRightLength);
	DrawFormatString(0, 468, 0xff0fff, "L:%f", m_avoidLeftLength);

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

	//DrawCapsule3D(m_posDown, m_posUp, m_radius, 32, 0xffffff, 0xffffff, false);
}

const VECTOR& BossShot::GetPosUp() const
{
	auto pos = VAdd(m_rigidbody.GetPos(), VGet(0.0f, 10.0f, 0.0f));
	return pos;
}

const VECTOR& BossShot::GetPosDown() const
{
	return m_rigidbody.GetPos();
}

void BossShot::SetPosDown(const VECTOR pos)
{
	m_rigidbody.SetPos(pos);
}

void BossShot::Hit()
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

void BossShot::IdleUpdate()
{
	Hit();
	m_actionTime++;

	//auto pos = m_rigidbody.GetPos();

	//プレイヤーへの向きを取得
	m_direction = VSub(m_playerPos, m_pos);
	m_direction = VNorm(m_direction);

	//m_angle = atan2f(m_direction.x, m_direction.z);

	//プレイヤーと離れていた場合歩き状態に移動 && タイマー
	if (m_actionTime > kIdleToAttackTime && m_length > kIdleToAttackLength)
	{
		m_actionKind = GetRand(kAttackKind);

		//ランダムで攻撃を行う
		switch (m_actionKind)
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
			OnPreliminaryAttack1();
			break;
		default:
			break;
		}
	}
	//プレイヤーと十分な距離の場合 && タイマー
	else if (m_actionTime > kIdleToDashTime && m_length < kIdleToAttackLength )
	{
		m_actionKind = GetRand(kAvoidKind);



		if (m_avoidKind == e_AvoidKind::kFront)
		{

			//ランダムで回避を行う
			switch (m_actionKind)
			{
			case 0:
				OnDashBack();
				break;
			case 1:
				OnDashRight();
				break;
			case 2:
				OnDashLeft();
				break;
			default:
				break;
			}
		}
		else if (m_avoidKind == e_AvoidKind::kBack)
		{

			//ランダムで回避を行う
			switch (m_actionKind)
			{
			case 0:
				OnDashFront();
				break;
			case 1:
				OnDashRight();
				break;
			case 2:
				OnDashLeft();
				break;
			default:
				break;
			}
		}
		else if (m_avoidKind == e_AvoidKind::kRight)
		{

			//ランダムで回避を行う
			switch (m_actionKind)
			{
			case 0:
				OnDashFront();
				break;
			case 1:
				OnDashBack();
				break;
			case 2:
				OnDashLeft();
				break;
			default:
				break;
			}
		}
		else if (m_avoidKind == e_AvoidKind::kLeft)
		{

			//ランダムで回避を行う
			switch (m_actionKind)
			{
			case 0:
				OnDashFront();
				break;
			case 1:
				OnDashBack();
				break;
			case 2:
				OnDashRight();
				break;
			default:
				break;
			}
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

void BossShot::PreliminaryAttack1Update()
{
	Hit();
	m_rigidbody.SetVelocity(VGet(0, 0, 0));

	m_attackPos = VScale(VGet(m_playerPos.x, m_playerPos.y + 6.0f, m_playerPos.z),0.89f);

	m_preliminaryActionFrame++;

	if (m_preliminaryActionFrame > 25)
	{
		OnAttack1();
	}
}

void BossShot::PreliminaryAttack2Update()
{
	Hit();
	m_rigidbody.SetVelocity(VGet(0, 0, 0));

	m_attackPos = VScale(VGet(m_playerPos.x, m_playerPos.y + 6.0f, m_playerPos.z), 0.89f);

	m_preliminaryActionFrame++;

	if (m_preliminaryActionFrame > 25)
	{
		OnAttack2();
	}
}

void BossShot::PreliminaryAttack3Update()
{
	Hit();
	m_rigidbody.SetVelocity(VGet(0, 0, 0));

	m_shockAttackPos = VScale(VGet(m_playerPos.x, m_playerPos.y + 6.0f, m_playerPos.z), 0.89f);

	m_preliminaryActionFrame++;

	if (m_preliminaryActionFrame > 45)
	{
		OnAttack3();
	}
}

void BossShot::Attack1Update()
{
	Hit();

	m_attackFrame++;

	if (m_attackFrame > 20)
	{
		m_isAttack = true;
	}

	//アニメーションが終わったらアイドル状態に戻る
	if (m_pAnim->IsLoop())
	{
		OnAttackCoolTime();
	}

	m_rigidbody.SetVelocity(VGet(0.0f, 0.0f, 0.0f));
}

void BossShot::Attack2Update()
{
	Hit();

	m_attackFrame++;

	if (m_attackFrame > 30)
	{
		m_isAttack = true;
	}

	//アニメーションが終わったらアイドル状態に戻る
	if (m_pAnim->IsLoop())
	{
		OnAttackCoolTime();
	}
	m_rigidbody.SetVelocity(VGet(0.0f, 0.0f, 0.0f));
}

void BossShot::Attack3Update()
{
	Hit();

	m_attackFrame++;

	if (m_attackFrame > 40)
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

void BossShot::DashFrontUpdate()
{
	Hit();
	m_actionTime++;

	//プレイヤーへの向きを取得
	m_direction = VSub(m_dashFrontPos, m_pos);
	//正規化
	m_direction = VNorm(m_direction);
	//モデルの角度
	m_angle = atan2f(m_direction.x, m_direction.z);

	//移動処理
	VECTOR length = VSub(m_dashFrontPos, m_pos);
	length = VNorm(length);
	m_velocity = VScale(length, kAvoidSpeed);

	//敵の移動
	m_rigidbody.SetVelocity(m_velocity);

	//ちゃんと座標までいけたら
	if (m_dashFrontLength <= 40 && m_actionTime > kAvoidToIdleTime)
	{
		OnIdle();
	}
	//else if (m_dashFrontLength > 40 && m_actionTime > kAvoidToIdleTime)
	//{
	//	//OnAvoidCoolTime();
	//	OnDashFront();
	//}
}

void BossShot::DashBackUpdate()
{
	Hit();
	m_actionTime++;

	//プレイヤーへの向きを取得
	m_direction = VSub(m_dashBackPos, m_pos);
	//正規化
	m_direction = VNorm(m_direction);
	//モデルの角度
	m_angle = atan2f(m_direction.x, m_direction.z);
	
	//移動処理
	VECTOR length = VSub(m_dashBackPos, m_pos);
	length = VNorm(length);
	m_velocity = VScale(length, kAvoidSpeed);

	//敵の移動
	m_rigidbody.SetVelocity(m_velocity);

	//ちゃんと座標までいけたら
	if (m_dashBackLength <= 40 && m_actionTime > kAvoidToIdleTime)
	{
		OnIdle();
	}

}

void BossShot::DashRightUpdate()
{
	Hit();
	m_actionTime++;

	//移動先の向きを取得
	m_direction = VSub(m_dashRightPos, m_pos);
	//正規化
	m_direction = VNorm(m_direction);
	//モデルの角度
	m_angle = atan2f(m_direction.x, m_direction.z);

	//移動処理
	VECTOR length = VSub(m_dashRightPos, m_pos);
	length = VNorm(length);
	m_velocity = VScale(length, kAvoidSpeed);

	//敵の移動
	m_rigidbody.SetVelocity(m_velocity);

	//ちゃんと座標までいけたら
	if (m_dashRightLength <= 40 && m_actionTime > kAvoidToIdleTime)
	{
		OnIdle();
	}

}

void BossShot::DashLeftUpdate()
{
	Hit();
	m_actionTime++;

	//プレイヤーへの向きを取得
	m_direction = VSub(m_dashLeftPos, m_pos);
	//正規化
	m_direction = VNorm(m_direction);
	//モデルの角度
	m_angle = atan2f(m_direction.x, m_direction.z);

	//移動処理
	VECTOR length = VSub(m_dashLeftPos, m_pos);
	length = VNorm(length);
	m_velocity = VScale(length, kAvoidSpeed);

	//敵の移動
	m_rigidbody.SetVelocity(m_velocity);

	//ちゃんと座標までいけたら
	if (m_avoidLeftLength <= 40 && m_actionTime > kAvoidToIdleTime)
	{
		OnIdle();
	}

}

void BossShot::AttackCoolTimeUpdate()
{
	Hit();
	m_actionTime++;

	m_rigidbody.SetVelocity(VGet(0.0f, 0.0f, 0.0f));

	if (m_actionTime > kCoolTimeToIdleTime)
	{
		OnIdle();
	}
}

void BossShot::HitOneDamageUpdate()
{
	m_rigidbody.SetVelocity(VGet(0.0f, 0.0f, 0.0f));
	//アニメーションが終わったらアイドル状態に戻る
	m_rigidbody.SetVelocity(VGet(0, 0, 0));
	if (m_damageFrame > 34)
	{
		OnIdle();
	}
}

void BossShot::HitTwoDamageUpdate()
{
	m_rigidbody.SetVelocity(VGet(0.0f, 0.0f, 0.0f));
	//アニメーションが終わったらアイドル状態に戻る
	m_rigidbody.SetVelocity(VGet(0, 0, 0));
	if (m_damageFrame > 34)
	{
		OnIdle();
	}
}

void BossShot::DownUpdate()
{
	Hit();
	m_rigidbody.SetVelocity(VGet(0.0f, 0.0f, 0.0f));
	//アニメーションが終わったらアイドル状態に戻る
	if (m_pAnim->IsLoop())
	{
		OnIdle();
	}
}

void BossShot::DeadUpdate()
{
	m_rigidbody.SetVelocity(VGet(0.0f, 0.0f, 0.0f));
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


void BossShot::OnIdle()
{
	m_attackKind = Game::e_BossAttackKind::kBossAttackNone;
	m_rigidbody.SetVelocity(VGet(0.0f, 0.0f, 0.0f));
	m_isAttack = false;
	m_attackFrame = 0;
	m_actionKind = 0;
	m_actionTime = 0;
	m_pAnim->ChangeAnim(kAnimIdle);
	m_updateFunc = &BossShot::IdleUpdate;
}

void BossShot::OnPreliminaryAttack1()
{
	m_attackKind = Game::e_BossAttackKind::kBossAttackNone;
	m_rigidbody.SetVelocity(VGet(0, 0, 0));
	m_attackFrame = 0;
	m_isAttack = false;
	auto pos = m_rigidbody.GetPos();
	EffectManager::GetInstance().CreateEffect("shotPreliminaryActionEffect", VGet(pos.x, pos.y + 6.0f, pos.z));
	EffectManager::GetInstance().CreateEffect("shotPreliminaryActionEffect", VGet(m_attackPos.x, m_attackPos.y + 6.0f, m_attackPos.z));
	m_pAnim->ChangeAnim(kAnimIdle);
	m_updateFunc = &BossShot::PreliminaryAttack1Update;
}

void BossShot::OnPreliminaryAttack2()
{
	m_attackKind = Game::e_BossAttackKind::kBossAttackNone;
	m_rigidbody.SetVelocity(VGet(0, 0, 0));
	m_attackFrame = 0;
	m_isAttack = false;
	auto pos = m_rigidbody.GetPos();
	EffectManager::GetInstance().CreateEffect("shotPreliminaryActionEffect", VGet(pos.x, pos.y + 6.0f, pos.z));
	EffectManager::GetInstance().CreateEffect("shotPreliminaryActionEffect", VGet(m_attackPos.x, m_attackPos.y + 6.0f, m_attackPos.z));
	m_pAnim->ChangeAnim(kAnimIdle);
	m_updateFunc = &BossShot::PreliminaryAttack2Update;
}

void BossShot::OnPreliminaryAttack3()
{
	m_rigidbody.SetVelocity(VGet(0, 0, 0));
	m_attackFrame = 0;
	m_isAttack = false;
	auto pos = m_rigidbody.GetPos();
	EffectManager::GetInstance().CreateEffect("shotPreliminaryActionEffect", VGet(pos.x, pos.y + 6.0f, pos.z));
	EffectManager::GetInstance().CreateEffect("shotPreliminaryActionEffect", VGet(m_shockAttackPos.x, m_shockAttackPos.y + 6.0f, m_shockAttackPos.z));
	m_pAnim->ChangeAnim(kAnimIdle);
	m_updateFunc = &BossShot::PreliminaryAttack3Update;
}

void BossShot::OnAttack1()
{
	m_rigidbody.SetVelocity(VGet(0.0f, 0.0f, 0.0f));

	auto pos = m_rigidbody.GetPos();
	EffectManager::GetInstance().CreateEffect("shotBossAttackEffect", VGet(m_attackPos.x, m_attackPos.y, m_attackPos.z));

	m_attackKind = Game::e_BossAttackKind::kBossWeapon;
	m_attackFrame = 0;
	m_isAttack = false;
	m_actionKind = 0;
	m_actionTime = 0;
	m_pAnim->ChangeAnim(kAnimAttack1, true, true, false);
	m_updateFunc = &BossShot::Attack1Update;
}

void BossShot::OnAttack2()
{
	m_rigidbody.SetVelocity(VGet(0.0f, 0.0f, 0.0f));

	auto pos = m_rigidbody.GetPos();
	EffectManager::GetInstance().CreateEffect("shotBossAttackEffect", VGet(m_attackPos.x, m_attackPos.y, m_attackPos.z));

	m_attackKind = Game::e_BossAttackKind::kBossAttack;
	m_attackFrame = 0;
	m_actionKind = 0;
	m_actionTime = 0;
	m_pAnim->ChangeAnim(kAnimAttack2, true, true, false);
	m_updateFunc = &BossShot::Attack2Update;
}

void BossShot::OnAttack3()
{
	m_rigidbody.SetVelocity(VGet(0.0f, 0.0f, 0.0f));

	auto pos = m_rigidbody.GetPos();
	EffectManager::GetInstance().CreateEffect("bossShockEffect", VGet(m_shockAttackPos.x, m_shockAttackPos.y, m_shockAttackPos.z));

	m_attackKind = Game::e_BossAttackKind::kBossShock;
	m_attackFrame = 0;
	m_actionKind = 0;
	m_actionTime = 0;
	m_pAnim->ChangeAnim(kAnimAttack3, true, true, false);
	m_updateFunc = &BossShot::Attack3Update;
}

void BossShot::OnDashFront()
{
	m_avoidKind = e_AvoidKind::kFront;
	m_attackKind = Game::e_BossAttackKind::kBossAttackNone;
	m_attackFrame = 0;
	m_actionTime = 0;
	m_pAnim->ChangeAnim(kAnimDash);
	m_updateFunc = &BossShot::DashFrontUpdate;
}

void BossShot::OnDashBack()
{
	m_avoidKind = e_AvoidKind::kBack;
	m_attackKind = Game::e_BossAttackKind::kBossAttackNone;
	m_attackFrame = 0;
	m_actionTime = 0;
	m_pAnim->ChangeAnim(kAnimDash);
	m_updateFunc = &BossShot::DashBackUpdate;
}

void BossShot::OnDashRight()
{
	m_avoidKind = e_AvoidKind::kRight;
	m_attackKind = Game::e_BossAttackKind::kBossAttackNone;
	m_attackFrame = 0;
	m_actionTime = 0;
	m_pAnim->ChangeAnim(kAnimDash);
	m_updateFunc = &BossShot::DashRightUpdate;
}

void BossShot::OnDashLeft()
{
	m_avoidKind = e_AvoidKind::kLeft;
	m_attackKind = Game::e_BossAttackKind::kBossAttackNone;
	m_attackFrame = 0;
	m_actionTime = 0;
	m_pAnim->ChangeAnim(kAnimDash);
	m_updateFunc = &BossShot::DashLeftUpdate;
}

void BossShot::OnAttackCoolTime()
{
	m_rigidbody.SetVelocity(VGet(0.0f, 0.0f, 0.0f));
	m_attackKind = Game::e_BossAttackKind::kBossAttackNone;
	m_attackFrame = 0;
	m_actionKind = 0;
	m_actionTime = 0;
	m_pAnim->ChangeAnim(kAnimCoolTime);
	m_updateFunc = &BossShot::AttackCoolTimeUpdate;
}

void BossShot::OnHitOneDamage()
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

	m_hp -= 10.0f;
	m_isHit = true;
	m_isAttack = false;
	m_attackFrame = 0;

	auto pos = m_rigidbody.GetPos();
	EffectManager::GetInstance().CreateEffect("bossHitEffect", VGet(pos.x, pos.y + 6.0f, pos.z));
	m_pAnim->ChangeAnim(kAnimCoolTime);
	m_updateFunc = &BossShot::HitOneDamageUpdate;

	//攻撃判定がバグらなければこっちの方がボスの難易度が上がってよい
	//m_updateFunc = &BossShot::IdleUpdate;
}

void BossShot::OnHitTwoDamage()
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

	m_hp -= 30.0f;
	m_isHit = true;
	m_isAttack = false;
	m_attackFrame = 0;

	auto pos = m_rigidbody.GetPos();
	EffectManager::GetInstance().CreateEffect("bossHitEffect", VGet(pos.x, pos.y + 6.0f, pos.z));
	m_pAnim->ChangeAnim(kAnimCoolTime);
	m_updateFunc = &BossShot::HitTwoDamageUpdate;

	//攻撃判定がバグらなければこっちの方がボスの難易度が上がってよい
	//m_updateFunc = &BossShot::IdleUpdate;

}

void BossShot::OnDown()
{
	m_rigidbody.SetVelocity(VGet(0, 0, 0));
	m_attackKind = Game::e_BossAttackKind::kBossAttackNone;

	m_attackFrame = 0;
	m_actionKind = 0;
	m_actionTime = 0;
	m_pAnim->ChangeAnim(kAnimDown);
	m_updateFunc = &BossShot::DownUpdate;
}

void BossShot::OnDead()
{
	m_rigidbody.SetVelocity(VGet(0, 0, 0));
	m_attackKind = Game::e_BossAttackKind::kBossAttackNone;
	m_attackFrame = 0;
	m_actionKind = 0;
	m_actionTime = 0;
	m_pAnim->ChangeAnim(kAnimDead, false, true, true);
	m_updateFunc = &BossShot::DeadUpdate;
}
