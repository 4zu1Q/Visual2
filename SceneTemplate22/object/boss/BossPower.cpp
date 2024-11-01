#include "BossPower.h"

#include "util/AnimController.h"
#include "util/ActionTime.h"

#include <cmath>
#include <cassert>

namespace
{
	//プレイヤーのモデルファイル名
	const char* const kModelFilename = "Data/Model/Boss/BossPower.mv1";
	//モデルのスケール値
	constexpr float kModelScale = 8.0f;

	//初期位置
	constexpr VECTOR kInitPos = { 0.0f,0.0f,200.0f };

	//カプセルの上の座標
	constexpr VECTOR kUpPos = { 0.0f,18.0f,0.0f };

	/*プレイヤーのアニメーションの種類*/
	const char* const kAnimInfoFilename = "Data/Master/AnimBossPowerMaster.csv";

	const char* const kAnimIdle = "Idle";
	const char* const kAnimWalk = "Walk";
	const char* const kAnimDash = "Dash";

	const char* const kAnimAttack1 = "Attack1";
	const char* const kAnimAttack2 = "Attack2";
	const char* const kAnimAttack3 = "Attack3";

	const char* const kAnimDown = "Down";
	const char* const kAnimDead = "Dead";

}

BossPower::BossPower():
	m_posDown(kInitPos),
	m_posUp(kInitPos),
	m_radius(5)
{

	m_modelH = MV1LoadModel(kModelFilename);

	m_pAnim = std::make_shared<AnimController>();
	//m_wlakTime = std::make_shared<ActionTime>();

}

BossPower::~BossPower()
{
	MV1DeleteModel(m_modelH);
	m_modelH = -1;


}

void BossPower::Initialize()
{
	//初期位置を代入
	m_pos = VGet(0, 0, 100);

	//モデルのスケールを決める
	MV1SetScale(m_modelH, VGet(kModelScale, kModelScale, kModelScale));

	//アニメーションの初期化
	m_pAnim->Initialize(kAnimInfoFilename, m_modelH, kAnimIdle);

	// メンバ関数ポインタの初期化
	m_updaFunc = &BossPower::IdleUpdate;

}

void BossPower::Finalize()
{

}

void BossPower::Update()
{
	//アップデート
	(this->*m_updaFunc)();

	//アニメーションの更新処理
	m_pAnim->UpdateAnim();
	
	m_posUp = VGet(m_posDown.x, m_posDown.y + kUpPos.y, m_posDown.z);

	//モデルに座標をセットする
	MV1SetPosition(m_modelH, m_posDown);
}

void BossPower::Draw()
{
	MV1DrawModel(m_modelH);

	DrawCapsule3D(m_posDown, m_posUp, m_radius, 32, 0xffffff, 0xffffff, false);
	DrawSphere3D(m_posDown, 32, 16, 0xffffff, 0xffffff, false);
}

void BossPower::IdleUpdate()
{
	if (m_pAnim->IsLoop())
	{
		OnWalk();
	}
}

void BossPower::WalkUpdate()
{
	if (0 > 120)
	{
		OnDash();
	}

}

void BossPower::DashUpdate()
{
}

void BossPower::Attack1Update()
{
	if (m_pAnim->IsLoop())
	{
		OnIdle();
	}
}

void BossPower::Attack2Update()
{
	if (m_pAnim->IsLoop())
	{
		OnIdle();
	}
}

void BossPower::Attack3Update()
{
	if (m_pAnim->IsLoop())
	{
		OnIdle();
	}
}

void BossPower::DownUpdate()
{
}

void BossPower::DeadUpdate()
{

}


void BossPower::OnIdle()
{
	m_pAnim->ChangeAnim(kAnimIdle,true,true,true,-1.0f);
	m_updaFunc = &BossPower::IdleUpdate;
}

void BossPower::OnWalk()
{
	m_pAnim->ChangeAnim(kAnimWalk);
	m_updaFunc = &BossPower::WalkUpdate;
}

void BossPower::OnDash()
{
}

void BossPower::OnAttack1()
{
	m_pAnim->ChangeAnim(kAnimDead, true, true, true);
	m_updaFunc = &BossPower::Attack1Update;
}

void BossPower::OnAttack2()
{
	m_pAnim->ChangeAnim(kAnimDead, true, true, true);
	m_updaFunc = &BossPower::Attack2Update;
}

void BossPower::OnAttack3()
{
	m_pAnim->ChangeAnim(kAnimDead, true, true, true);
	m_updaFunc = &BossPower::Attack3Update;
}

void BossPower::OnDown()
{
	m_pAnim->ChangeAnim(kAnimDown);
	m_updaFunc = &BossPower::DownUpdate;
}

void BossPower::OnDead()
{
	m_pAnim->ChangeAnim(kAnimDead, false, true, true);
	m_updaFunc = &BossPower::DeadUpdate;
}
