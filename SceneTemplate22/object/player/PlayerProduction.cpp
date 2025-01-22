#include "PlayerProduction.h"

#include "util/Pad.h"
#include "util/AnimController.h"
#include "util/EffectManager.h"
#include "util/SoundManager.h"
#include "util/Game.h"

#include <cmath>
#include <cassert>

namespace
{

	//プレイヤーのモデルファイル名
	const char* const kModelFilename = "Data/Model/Player/Player.mv1";

	const char* const kSwordModelFileName = "Data/Model/Weapon/Player_Sword.mv1";

	const char* const kShadowFileName = "Data/Image/Shadow.png";

	//モデルのスケール値
	constexpr float kModelScale = 5.0f;
	constexpr float kWeaponScale = 1.0f;

	/*　プレイヤーのアニメーションの種類　*/
	const char* const kAnimInfoFilename = "Data/Master/AnimPlayerProductionMaster.csv";

	//他シーンに使うアニメーション
	const char* const kAnimIdle = "Idle";
	const char* const kAnimDash = "Dash";
	const char* const kAnimSit = "Sit";
	const char* const kAnimSitDown = "SitDown";
	const char* const kAnimSitUp = "SitUp";
	const char* const kAnimGameOver = "GameOver";
	const char* const kAnimRetry = "Retry";
	const char* const kAnimGameClear = "GameClear";

	constexpr float kShadowSize = 5.0f;
	constexpr float kShadowHeight = 50.0f;


	//プレイヤーの座標
	constexpr float kPosX = 2.0;
	constexpr float kPosY = -1.0;
	constexpr float kPosZ = -60.0;

	constexpr float kRotationY = 20.0f;
}

PlayerProduction::PlayerProduction():
	m_pos(VGet(0,0,0)),
	m_cameraPos(VGet(0,0,0)),
	m_isStart(false),
	m_isTitle(false)
{
	m_modelH = MV1LoadModel(kModelFilename);
	m_modelWeaponH = MV1LoadModel(kSwordModelFileName);
	m_shadowH = LoadGraph(kShadowFileName);

	assert(m_modelH > -1);
	assert(m_modelWeaponH > -1);
	assert(m_shadowH > -1);

	m_pAnim = std::make_shared<AnimController>();

}

PlayerProduction::~PlayerProduction()
{
	MV1DrawModel(m_modelH);
	m_modelH = -1;

	DeleteGraph(m_shadowH);
}

void PlayerProduction::Initialize(Game::e_PlayerProduction playerScene)
{
	MV1SetScale(m_modelH, VGet(kModelScale, kModelScale, kModelScale));
	MV1SetPosition(m_modelH, m_pos);

	if (playerScene == Game::e_PlayerProduction::kTitle)
	{
		m_pAnim->Initialize(kAnimInfoFilename, m_modelH, kAnimSit);
		MV1SetRotationXYZ(m_modelH, VGet(0.0f, 100, 0.0f));

	}
	else if (playerScene == Game::e_PlayerProduction::kGameOver)
	{
		m_pAnim->Initialize(kAnimInfoFilename, m_modelH, kAnimGameOver);
		MV1SetRotationXYZ(m_modelH, VGet(0.0f, 30.0f, 0.0f));

	}
	else if (playerScene == Game::e_PlayerProduction::kGameClear)
	{
		m_pAnim->Initialize(kAnimInfoFilename, m_modelH, kAnimGameClear);
		MV1SetRotationXYZ(m_modelH, VGet(0.0f, 30.0f, 0.0f));

	}

	m_playerProduction = playerScene;
	m_updateFunc = &PlayerProduction::IdleUpdate;
}

void PlayerProduction::Update(bool isStart, bool isTitle)
{
	//アップデート
	(this->*m_updateFunc)();

	//武器のアタッチ
	MATRIX transMat;
	MATRIX attachFrameMat;
	MATRIX mixMatrix;

	//アタッチするモデルのMV1SetMatrixの設定を無効にする
	MV1SetMatrix(m_modelWeaponH, MGetIdent());

	m_attachFramePosition = MV1GetFramePosition(m_modelWeaponH, 0);

	transMat = MGetTranslate(VScale(m_attachFramePosition, -1.0f));

	attachFrameMat = MV1GetFrameLocalWorldMatrix(m_modelH, 14);

	mixMatrix = MMult(transMat, attachFrameMat);

	MV1SetMatrix(m_modelWeaponH, mixMatrix);

	m_isStart = isStart;
	m_isTitle = isTitle;

	//アニメーションの更新処理
	m_pAnim->UpdateAnim();

	MV1SetPosition(m_modelH, m_pos);
}

void PlayerProduction::Draw()
{
	MV1DrawModel(m_modelH);
	MV1DrawModel(m_modelWeaponH);

}

void PlayerProduction::ShadowRender(int stageH)
{
	int i;
	MV1_COLL_RESULT_POLY_DIM HitResDim;
	MV1_COLL_RESULT_POLY* HitRes;
	VERTEX3D Vertex[3];
	VECTOR SlideVec;

	// ライティングを無効にする
	SetUseLighting(FALSE);

	// Ｚバッファを有効にする
	SetUseZBuffer3D(TRUE);

	// テクスチャアドレスモードを CLAMP にする( テクスチャの端より先は端のドットが延々続く )
	SetTextureAddressMode(DX_TEXADDRESS_CLAMP);

	// プレイヤーの直下に存在する地面のポリゴンを取得
	HitResDim = MV1CollCheck_Capsule(stageH, -1, m_pos, VAdd(m_pos, VGet(0.0f, -kShadowHeight, 0.0f)), kShadowSize);

	// 頂点データで変化が無い部分をセット
	Vertex[0].dif = GetColorU8(255, 255, 255, 255);
	Vertex[0].spc = GetColorU8(0, 0, 0, 0);
	Vertex[0].su = 0.0f;
	Vertex[0].sv = 0.0f;
	Vertex[1] = Vertex[0];
	Vertex[2] = Vertex[0];

	// 球の直下に存在するポリゴンの数だけ繰り返し
	HitRes = HitResDim.Dim;
	for (i = 0; i < HitResDim.HitNum; i++, HitRes++)
	{
		// ポリゴンの座標は地面ポリゴンの座標
		Vertex[0].pos = HitRes->Position[0];
		Vertex[1].pos = HitRes->Position[1];
		Vertex[2].pos = HitRes->Position[2];

		// ちょっと持ち上げて重ならないようにする
		SlideVec = VScale(HitRes->Normal, 0.1f);
		Vertex[0].pos = VAdd(Vertex[0].pos, SlideVec);
		Vertex[1].pos = VAdd(Vertex[1].pos, SlideVec);
		Vertex[2].pos = VAdd(Vertex[2].pos, SlideVec);

		// ポリゴンの不透明度を設定する
		Vertex[0].dif.a = 0;
		Vertex[1].dif.a = 0;
		Vertex[2].dif.a = 0;
		if (HitRes->Position[0].y > m_pos.y - kShadowHeight)
			Vertex[0].dif.a = 128 * (1.0f - fabs(HitRes->Position[0].y - m_pos.y) / kShadowHeight);

		if (HitRes->Position[1].y > m_pos.y - kShadowHeight)
			Vertex[1].dif.a = 128 * (1.0f - fabs(HitRes->Position[1].y - m_pos.y) / kShadowHeight);

		if (HitRes->Position[2].y > m_pos.y - kShadowHeight)
			Vertex[2].dif.a = 128 * (1.0f - fabs(HitRes->Position[2].y - m_pos.y) / kShadowHeight);

		// ＵＶ値は地面ポリゴンとプレイヤーの相対座標から割り出す
		Vertex[0].u = (HitRes->Position[0].x - m_pos.x) / (kShadowSize * 2.0f) + 0.5f;
		Vertex[0].v = (HitRes->Position[0].z - m_pos.z) / (kShadowSize * 2.0f) + 0.5f;
		Vertex[1].u = (HitRes->Position[1].x - m_pos.x) / (kShadowSize * 2.0f) + 0.5f;
		Vertex[1].v = (HitRes->Position[1].z - m_pos.z) / (kShadowSize * 2.0f) + 0.5f;
		Vertex[2].u = (HitRes->Position[2].x - m_pos.x) / (kShadowSize * 2.0f) + 0.5f;
		Vertex[2].v = (HitRes->Position[2].z - m_pos.z) / (kShadowSize * 2.0f) + 0.5f;

		// 影ポリゴンを描画
		DrawPolygon3D(Vertex, 1, m_shadowH, TRUE);
	}

	// 検出した地面ポリゴン情報の後始末
	MV1CollResultPolyDimTerminate(HitResDim);

	// ライティングを有効にする
	SetUseLighting(TRUE);

	// Ｚバッファを無効にする
	SetUseZBuffer3D(FALSE);
}

void PlayerProduction::IdleUpdate()
{
	if (m_playerProduction == Game::e_PlayerProduction::kTitle)
	{
		OnSit();
	}
	else if (m_playerProduction == Game::e_PlayerProduction::kGameOver)
	{
		OnGameOver();
	}
	else if (m_playerProduction == Game::e_PlayerProduction::kGameClear)
	{
		OnGameClear();
	}
}

void PlayerProduction::TitleIdleUpdate()
{
	if (m_isStart && m_isTitle)
	{
		//後で名前とか変える
		OnPlayStart();
	}



	if (!m_isStart && Pad::IsTrigger(PAD_INPUT_2))
	{
		OnSitDown();
	}
}

void PlayerProduction::DashUpdate()
{
	m_pos.x += 0.5f;
	m_pos.z -= 0.5f;
}

void PlayerProduction::SitUpdate()
{
	if (m_isTitle && Pad::IsTrigger(PAD_INPUT_1))
	{
		OnSitUp();
	}
}

void PlayerProduction::SitUpUpdate()
{
	if (m_pAnim->IsLoop())
	{
		OnTitleIdle();
	}
}

void PlayerProduction::SitDownUpdate()
{
	if (m_pAnim->IsLoop())
	{
		OnSit();
	}
}

void PlayerProduction::PlayStartUpdate()
{

}

void PlayerProduction::GameOverUpdate()
{
	if (m_isStart)
	{
		OnRetry();
	}
}

void PlayerProduction::RetryUpdate()
{

}

void PlayerProduction::GameClearUpdate()
{

}

void PlayerProduction::OnIdle()
{
	//タイプによってアニメーションを変える
	m_pAnim->ChangeAnim(kAnimIdle);
	m_updateFunc = &PlayerProduction::IdleUpdate;
}

void PlayerProduction::OnTitleIdle()
{
	//タイプによってアニメーションを変える
	m_pAnim->ChangeAnim(kAnimIdle);
	m_updateFunc = &PlayerProduction::TitleIdleUpdate;
}

void PlayerProduction::OnDash()
{
	//タイプによってアニメーションを変える
	m_pAnim->ChangeAnim(kAnimDash);
	m_updateFunc = &PlayerProduction::DashUpdate;
}

void PlayerProduction::OnSit()
{
	//タイプによってアニメーションを変える
	m_pAnim->ChangeAnim(kAnimSit);
	m_updateFunc = &PlayerProduction::SitUpdate;
}

void PlayerProduction::OnSitDown()
{
	//タイプによってアニメーションを変える
	m_pAnim->ChangeAnim(kAnimSitDown, false, true, true);
	m_updateFunc = &PlayerProduction::SitDownUpdate;
}

void PlayerProduction::OnSitUp()
{
	//タイプによってアニメーションを変える
	m_pAnim->ChangeAnim(kAnimSitUp, false, true, true);
	m_updateFunc = &PlayerProduction::SitUpUpdate;
}

void PlayerProduction::OnPlayStart()
{
	//タイプによってアニメーションを変える
	m_pAnim->ChangeAnim(kAnimGameClear);
	m_updateFunc = &PlayerProduction::PlayStartUpdate;
}

void PlayerProduction::OnGameOver()
{
	//タイプによってアニメーションを変える
	m_pAnim->ChangeAnim(kAnimGameOver, false, true, true);
	m_updateFunc = &PlayerProduction::GameOverUpdate;
}

void PlayerProduction::OnRetry()
{
	//タイプによってアニメーションを変える
	m_pAnim->ChangeAnim(kAnimRetry, false, true, true);
	m_updateFunc = &PlayerProduction::RetryUpdate;
}

void PlayerProduction::OnGameClear()
{
	//タイプによってアニメーションを変える
	m_pAnim->ChangeAnim(kAnimGameClear);
	m_updateFunc = &PlayerProduction::GameClearUpdate;
}
