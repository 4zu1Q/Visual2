#include "Player.h"
#include "Pad.h"
#include "Game.h"
#include <cmath>
#include <cassert>
#include <EffekseerForDXLib.h>

/// <summary>
/// 定数
/// </summary>
namespace
{

	//モデルのファイル名
	const char* const kPlayerModelFilename = "data/model/player/barbarian.mv1";

	//モデルの向いてる位置の初期化
	constexpr float kInitAngle = 3.143059f;

	//モデル用の定数
	constexpr float kHitEffectScale = 10.0f;

	//HPのファイル名
	const char* const kHpFilename = "data/image/Hp.png";
	const char* const kLostHpFilename = "data/image/LostHp.png";
	const char* const kHpFlameFilename = "data/image/HpFlame.png";

	//サウンドのファイル名
	const char* const kAttackFilename = "data/sound/se/AxeSlashSe.mp3";
	const char* const kSkillFilename = "data/sound/se/AxeSkillSe.mp3";
	const char* const kDamageFilename = "data/sound/se/DamageSe.mp3";

	//シェーダのファイル名
	const char* const kOutlinePsFilename = "./OutlinePs.pso";
	const char* const kOutlineVsFilename = "./OutlineVs.vso";

	//アニメーション
	constexpr int kIdleAnimIndex = 1;		//待機
	constexpr int kWalkAnimIndex = 2;		//歩き
	constexpr int kDashAnimIndex = 7;		//走り
	constexpr int kAttackAnimIndex = 31;	//攻撃
	constexpr int kSkillAnimIndex = 41;	//スキル
	constexpr int kDamageAnimIndex = 25;	//ダメージ
	constexpr int kFallAnimIndex = 26;		//倒れる
	constexpr int kFallingAnimIndex = 27;	//倒れ中
	constexpr int kGameClearAnimIndex = 10;	//倒した後のモーション
	constexpr int kGameOverAnimIndex = 10;

	//アニメーションの切り替えにかかるフレーム数
	constexpr float kAnimChangeFrame = 8.0f;
	constexpr float kAnimChangeRateSpeed = 1.0f / kAnimChangeFrame;

	//ダメージ
	constexpr int kDamageCount = 60;


	//アナログスティックによる移動関連
	constexpr float kMaxSpeed = 0.5f;		//プレイヤーの最大移動速度
	constexpr float kAnalogRangeMin = 0.1;	//アナログスティックの入力判定範囲
	constexpr float kAnalogRangeMax = 0.8;
	constexpr float kAnalogInputMax = 1000.0f;	//アナログスティックから入力されるベクトルの最大値

	//ハートのアニメーション
	constexpr float kHpSinSpeed = 0.05f;
	constexpr float kHpAnimationSize = 3.0f;

	constexpr float kMaxStamina = 400.0f;

	//壁
	constexpr float kWall = 475;

	constexpr float kSelectSpeed = 3.0f;
}

/// <summary>
/// コンストラクタ
/// </summary>
Player::Player() :
	m_modelH(-1),
	m_pos(VGet(0, 0, 0)),
	m_attackPos(VGet(0, 0, 0)),
	m_attackDir(VGet(0, 0, 0)),
	m_shadowMapH(-1),
	m_hpH(LoadGraph(kHpFilename)),
	m_losthpH(LoadGraph(kLostHpFilename)),
	m_hpFlameH(LoadGraph(kHpFlameFilename)),
	m_outlinePsH(-1),
	m_outlineVsH(-1),
	m_currentAnimNo(-1),
	m_prevAnimNo(-1),
	m_animBlendRate(0.0f),
	m_cameraAngle(0.0f),
	m_angle(kInitAngle),
	m_radius(7.0f),
	m_attackRadius(15.0f),
	m_skillRadius(10.0f),
	m_isWalk(false),
	m_isDamage(false),
	m_isDash(false),
	m_isAttack(false),
	m_isSkill(false),
	m_isDown(false),
	m_isAttackGeneration(false),
	m_isSkillGeneration(false),
	m_isAnimWalk(false),
	m_isAnimDash(false),
	m_isAnimDamage(false),
	m_isAnimDown(false),
	m_isStamina(false),
	m_isAvoid(false),
	m_isCommand(false),
	m_isAnimIdle(false),
	m_damageFrame(0),
	m_hp(10),
	m_losthp(10),
	m_stamina(kMaxStamina),
	m_hpAnimationHeight(0.0f),
	m_animKind(AnimKind::kIdle),
	m_analogX(0.0f),
	m_analogZ(0.0f),
	m_damageSeH(-1),
	m_axeAttackSeH(-1),
	m_axeSkillSeH(-1),
	m_shadowAngle(0.0f),
	m_effectPH(-1),
	m_effectSH(-1),
	m_lightDirection(VGet(0, 0, 0))
{
	m_damageSeH = LoadSoundMem(kDamageFilename);	  //ダメージ音
	m_axeAttackSeH = LoadSoundMem(kAttackFilename);	  //攻撃音
	m_axeSkillSeH = LoadSoundMem(kSkillFilename);	  //スキル音

	//m_effectSH = LoadEffekseerEffect("");

	ChangeVolumeSoundMem(128, m_damageSeH);
	ChangeVolumeSoundMem(128, m_axeAttackSeH);
	ChangeVolumeSoundMem(128, m_axeSkillSeH);

}

/// <summary>
/// デストラクタ
/// </summary>
Player::~Player()
{
	Delete();
	ShadowMapDelete(m_modelH);

	DeleteSoundMem(m_damageSeH);
	DeleteSoundMem(m_axeAttackSeH);
	DeleteSoundMem(m_axeSkillSeH);

}

/// <summary>
/// ロード
/// </summary>
void Player::Load()
{
	//モデルのロード
	m_modelH = MV1LoadModel(kPlayerModelFilename);
	assert(m_modelH > -1);

	//シャドウマップモデルをロード
	//ShadowMapLoad();

	//アニメーションを再生


	//シェーダのロード
	//ShaderLoad();
}

/// <summary>
/// デリート
/// </summary>
void Player::Delete()
{
	MV1DeleteModel(m_modelH);
	m_modelH = -1;
}

/// <summary>
/// 初期化
/// </summary>
void Player::Init()
{

	//待機アニメーションを設定
	m_currentAnimNo = MV1AttachAnim(m_modelH, kIdleAnimIndex, -1, false);
	m_prevAnimNo = -1;
	m_animBlendRate = 1.0f;

	//プレイヤーの初期位置設定
	m_pos = VGet(0.0f, 0.0f, -170.0f);

	MV1SetPosition(m_modelH, m_pos);
	MV1SetRotationXYZ(m_modelH, VGet(0, m_angle, 0));
	MV1SetScale(m_modelH, VGet(kHitEffectScale, kHitEffectScale, kHitEffectScale));

	//シャドウマップの初期化
	//ShadowMapInit();

}

/// <summary>
/// アップデート
/// </summary>
void Player::Update()
{
	Pad::Update();

	//ShadowMapUpdate();

	if (m_stamina >= kMaxStamina)
	{
		m_stamina = kMaxStamina;
	}

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
	bool isLoop = UpdateAnim(m_currentAnimNo);
	if (isLoop)
	{
		ChangeAnim(m_animIndex);
	}
	UpdateAnim(m_prevAnimNo);

	//死んだら処理
	if (m_isDown)
	{
		if (!m_isAnimDown)
		{
			ChangeAnim(kFallAnimIndex);
			m_animIndex = kFallingAnimIndex;
		}
		m_isAnimDown = true;

	}

	if (m_isCommand)
	{
		if (!m_isAnimIdle)
		{
			ChangeAnim(kGameClearAnimIndex);
			m_animIndex = kGameClearAnimIndex;
		}
		m_isAnimIdle = true;
	}


	//ボタンを押したら攻撃アニメーションを再生する
	if (!m_isAttack && !m_isSkill && !m_isDown && !m_isCommand)
	{
		//攻撃
		if (Pad::IsPress(PAD_INPUT_3) && !m_isStamina)
		{
			
			m_isAttack = true;
			ChangeAnim(kAttackAnimIndex);
			m_isMove = true;
			PlaySoundMem(m_axeAttackSeH, DX_PLAYTYPE_BACK, true);//アタック
		}
		else
		{
			if (m_isMove)
			{
				Move();
			}
		}

		//スキル攻撃
		if (Pad::IsPress(PAD_INPUT_4) && m_stamina >= 100.0f && !m_isStamina)
		{
			m_isSkill = true;
			ChangeAnim(kSkillAnimIndex);
			m_isMove = false;
			PlaySoundMem(m_axeSkillSeH, DX_PLAYTYPE_BACK, true);//スキル
			m_stamina -= 100.0f;
		}
		else
		{
			if (!m_isMove)
			{
				Move();
			}
		}

	}
	else
	{

		//攻撃アニメーションが終了したら待機アニメーションを再生する
		if (isLoop)
		{
			m_isAttack = false;
			m_isSkill = false;
			m_isDash = false;
			m_isDown = false;
		}
		
	}


	

	//ShaderUpdate();

	//Hpのアニメーション
	static float SinCount = 0;
	SinCount += kHpSinSpeed;
	m_hpAnimationHeight = sinf(SinCount) * kHpAnimationSize;


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

	//int soundAttackFrame = 0;
	//int soundSkillFrame = 0;

	if (m_isAttack)
	{
		PlaySoundMem(m_axeAttackSeH, DX_PLAYTYPE_BACK, true);//アタック

		//プレイヤーが攻撃したら生成するためのフラグがtrueになる
		m_isAttackGeneration = true;
	}
	else m_isAttackGeneration = false;

	if (m_isSkill)
	{
		PlaySoundMem(m_axeAttackSeH, DX_PLAYTYPE_BACK, true);//アタック

		//プレイヤーがスキルを使用したら生成するためのフラグがtrueになる
		m_isSkillGeneration = true;
	}
	else m_isSkillGeneration = false;

	//HPがゼロより下にいった場合
	if (m_hp <= 0)
	{
		m_hp = 0;
	}


	MV1SetPosition(m_modelH, m_pos);
	MV1SetRotationXYZ(m_modelH, VGet(0, m_angle, 0));
}

/// <summary>
/// 描画
/// </summary>
void Player::Draw()
{


	DrawExtendGraph(10, 10, 420, 50, m_hpFlameH,true);

	for (int i = 1; i <= m_losthp; i++)
	{
		DrawExtendGraph(35 * i + 20, 16 + m_hpAnimationHeight, 35 * i + 52, 48, m_losthpH, true);

	}

	for (int i = 1; i <= m_hp; i++)
	{
		DrawExtendGraph(35 * i + 20, 16 + m_hpAnimationHeight, 35 * i + 52, 48, m_hpH, true);
	}

	DrawBox(19, 54, 22 + kMaxStamina, 66, 0xdc143c, true);

	float alpha = 0;
	alpha += kSelectSpeed;


	if (!m_isStamina)
	{
		DrawBox(20, 55, 20 + m_stamina, 65, 0x3cb371, true);
	}
	else
	{
		DrawBox(20, 55, 20 + m_stamina, 65, 0xffa500, true);
	}
	


#ifdef _DEBUG

	DrawSphere3D(VAdd(m_pos, VGet(0, 8, 0)), m_radius, 8, 0xffffff, 0xffffff, false);
	//当たり判定が生成されているかどうか
	if (!m_isSkill)
	{
		DrawSphere3D(VAdd(m_attackPos, VGet(0, 8, 0)), m_skillRadius, 8, 0xff00ff, 0xff00ff, false);
	}
	else
	{
		DrawSphere3D(VAdd(m_attackPos, VGet(0, 8, 0)), m_skillRadius, 8, 0x0000ff, 0x0000ff, false);
	}

	if (!m_isAttack)
	{
		DrawSphere3D(VAdd(m_attackPos, VGet(0, 8, 0)), m_attackRadius, 8, 0xff00ff, 0xff00ff, false);
	}
	else
	{
		DrawSphere3D(VAdd(m_attackPos, VGet(0, 8, 0)), m_attackRadius, 8, 0x0000ff, 0x0000ff, false);
	}

	DrawFormatString(0, 16, 0xffffff, "Player(x:%f,y:%f,z:%f)", m_pos.x, m_pos.y, m_pos.z);
	DrawFormatString(800, 16, 0xffffff, "Player(x:%f,y:%f,z:%f)", m_attackPos.x, m_attackPos.y, m_attackPos.z);

	DrawFormatString(400, 16, 0xffffff, "PlayerHp:%d", m_hp);
	DrawFormatString(400, 48, 0xffffff, "AttackFlag:%f", m_isSkill);
	DrawFormatString(400, 64, 0xffffff, "PlayerAngle:%f", m_angle);


	DrawFormatString(400, 96, 0xffffff, "PlayerLeftAngle(x:%f,z:%f)", m_analogX, m_analogZ);


#endif
	// ダメージ演出  2フレーム間隔で表示非表示切り替え
	// 0: 表示される
	// 1: 表示される
	// 2: 非表示
	// 3: 非表示
	// 4: 表示される	...
	// % 4 することで012301230123... に変換する
	if (m_damageFrame % 8 >= 4) return;


	// 描画にキャラクターモデル用のシャドウマップと
	// ステージモデル用のシャドウマップのどちらも使用する
	//SetUseShadowMap(0, m_modelH);

	////シャドウマップへの描画の準備
	//ShadowMap_DrawSetup(m_shadowMapH);

	MV1DrawModel(m_modelH);

	////シャドウマップの設定を解除
	//SetUseShadowMap(0, -1);
}

/// <summary>
/// ギミックに当たった時のアップデート処理
/// </summary>
void Player::OnGimmickHitUpdate()
{
	Pad::Update();

	int frame = 0;


}

void Player::SetAnimDamage(const bool animDamage)
{
	m_isAnimDamage = animDamage;
	ChangeAnim(kDamageAnimIndex);
}

/// <summary>
/// アニメーションのアップデート処理
/// </summary>
/// <param name="attachNo"></param>
/// <returns></returns>
bool Player::UpdateAnim(int attachNo)
{
	//アニメーションが設定されていないので終了
	if (attachNo == -1) return false;

	//アニメーションを進行させる
	
	float now = MV1GetAttachAnimTime(m_modelH, attachNo);	//現在の再生カウントを取得
	bool isLoop = false;

	now += 0.7f;	// アニメーションを進める

	//現在再生中のアニメーションの総カウントを取得する
	float total = MV1GetAttachAnimTotalTime(m_modelH, attachNo);

	while (now >= total)
	{
		now -= total;
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
void Player::ChangeAnim(int animIndex)
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

/// <summary>
/// シェーダをロードする関数
/// </summary>
void Player::ShaderLoad()
{
	/*シェーダ*/
	//作ったシェーダを呼び出すか呼び出さないか
	MV1SetUseOrigShader(true);

	//シェーダのロード
	m_outlineVsH = LoadVertexShader(kOutlineVsFilename);
	m_outlinePsH = LoadPixelShader(kOutlinePsFilename);
}

/// <summary>
/// シェーダのアップデート処理
/// </summary>
void Player::ShaderUpdate()
{
	//メッシュの数を取得
	auto m_meshNum = MV1GetMeshNum(m_modelH);

	/*反転法*/
	SetUseVertexShader(m_outlineVsH);
	SetUsePixelShader(m_outlinePsH);
	MV1DrawModel(m_modelH);


}

/// <summary>
/// プレイヤーの移動関数
/// </summary>
void Player::Move()
{

	//アナログスティックを使って移動
	int analogX = 0;
	int analogZ = 0;

	GetJoypadAnalogInput(&analogX, &analogZ, DX_INPUT_PAD1);
	//アナログスティックの入力の10% ~ 80%の範囲を使用する

	//ベクトルの長さが最大で1000になる
	//プレイヤーの最大移動速度は0.01f / frame

	VECTOR move = VGet(analogX, 0.0f, -analogZ);	//ベクトルの長さは0～1000

	m_analogX = analogX;
	m_analogZ = analogZ;

	//0.00 ~ 0.01の長さにする
	//ベクトルの長さを取得する
	float len = VSize(move);
	//ベクトルの長さを0.0~1.0の割合に変換する
	float rate = len / kAnalogInputMax;

	//アナログスティック無効な範囲を除外する
	rate = (rate - kAnalogRangeMin) / (kAnalogRangeMax - kAnalogRangeMin);
	rate = min(rate, 1.0f);
	rate = max(rate, 0.0f);

	//速度が決定できるので移動ベクトルに反映する
	move = VNorm(move);
	float speed = kMaxSpeed * rate;
	move = VScale(move, speed);

	//カメラのいる場所(角度)から
	//コントローラーによる移動方向を決定する
	MATRIX mtx = MGetRotY(-m_cameraAngle - DX_PI_F / 2);
	move = VTransform(move, mtx);

	if (VSize(move) != 0.0f)
	{
		if (!m_isWalk)
		{
			ChangeAnim(kWalkAnimIndex);
			m_animIndex = kWalkAnimIndex;
		}
		m_isWalk = true;
	}
	else
	{
		m_isWalk = false;
	}

	//移動方向からプレイヤーの向く方向を決定する
	//移動していない場合(ゼロベクトル)の場合は変更しない
	if (VSquareSize(move) > 0.0f)
	{
		m_angle = -atan2f(move.z, move.x) - DX_PI_F / 2;
		m_attackDir = VNorm(move);

	}

	m_pos = VAdd(m_pos, move);


	//Aボタンを押している間
	if (!m_isStamina)
	{
		if (Pad::IsPress(PAD_INPUT_1) && m_stamina >= 0.0f && VSize(move) != 0.0f)
		{
			//動くスピードを1.5倍
			move = VScale(move, 2.0f);

			m_stamina--;

			if (m_stamina <= 0.0f)
			{
				m_isStamina = true;
			}

			//動きを反映
			m_pos = VAdd(m_pos, move);
			if (!m_isDash)
			{
				ChangeAnim(kDashAnimIndex);
				m_animIndex = kDashAnimIndex;
			}
			m_isDash = true;
		}
		else
		{
			m_stamina++;

			if (m_isDash && m_isWalk)
			{
				ChangeAnim(kWalkAnimIndex);
				m_animIndex = kWalkAnimIndex;
			}
			m_isDash = false;
		}
	}
	else
	{
		m_isDash = false;

		m_stamina += 0.5f;

		if (!m_isDash && !m_isWalk)
		{
			ChangeAnim(kWalkAnimIndex);
			m_animIndex = kWalkAnimIndex;
		}
		m_isWalk = true;

		if (m_stamina == kMaxStamina)
		{
			m_isStamina = false;
		}

	}


	if (!m_isDash && !m_isWalk)
	{
		ChangeAnim(kIdleAnimIndex);
		m_animIndex = kIdleAnimIndex;
	}


	VECTOR attackMove = VScale(m_attackDir, 15.0f);
	m_attackPos = VAdd(m_pos, attackMove);
}

/// <summary>
/// シャドウマップのロード
/// </summary>
void Player::ShadowMapLoad()
{
	m_shadowMapH = MakeShadowMap(1024, 1024);
}
/// <summary>
/// シャドウマップの初期化
/// </summary>
void Player::ShadowMapInit()
{
	SetShadowMapDrawArea(m_shadowMapH, VGet(-1000.0f, -1.0f, -1000.0f), VGet(1000.0f, 1000.0f, 1000.0f));
	m_shadowAngle = 0.0f;
}
/// <summary>
/// シャドウマップのアップデート
/// </summary>
void Player::ShadowMapUpdate()
{
	m_shadowAngle += 0.0125f;
	if (m_shadowAngle > DX_PI_F * 2.0f)
	{
		m_shadowAngle -= DX_PI_F * 2.0f;
	}

	//ライトの方向ベクトルの算出
	m_lightDirection.x = sin(m_shadowAngle);
	m_lightDirection.y = -1.0f;
	m_lightDirection.z = cos(m_shadowAngle);

	//ライトの方向を設定
	SetLightDirection(m_lightDirection);

	//シャドウマップが想定するライトの方向もセット
	SetShadowMapLightDirection(m_shadowMapH, m_lightDirection);



}
/// <summary>
/// シャドウマップの描画
/// </summary>
void Player::ShadowMapDraw(int handle)
{

}
/// <summary>
/// シャドウマップのデリート
/// </summary>
void Player::ShadowMapDelete(int handle)
{
	// シャドウマップの削除
	DeleteShadowMap(handle);
	m_shadowMapH = -1;
}



