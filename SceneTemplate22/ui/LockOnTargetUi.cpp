#include "LockOnTargetUi.h"
#include <math.h>

namespace
{
	//画像ファイル名
	const char* const kFileName = "Data/Image/Target.png";
	//最大サイズ
	constexpr float kMaxScale = 1.0f;
	//通常サイズ　
	constexpr float kNormalScale = 0.4f;
	//サイズ縮小スピード
	constexpr float kSmallScaleSpeed = 0.3f;
	//上下移動速度(1フレーム当たりのsin変化量)
	constexpr float kSinRateSpeed = 0.1f;
	//上下移動最大
	constexpr float kUpMax = 3.0f;
	//高さ
	constexpr float kHeight = 25.0f;
}

LockOnTargetUi::LockOnTargetUi() :
	m_pos(VGet(0.0f, 0.0f, 0.0f)),
	m_height(kHeight),
	m_handle(-1),
	m_scale(kMaxScale),
	m_sinRate(0.0f),
	m_upVec(0.0f),
	m_updateFunc(&LockOnTargetUi::FirstUpdate)
{
	//画像ロード
	m_handle = LoadGraph(kFileName);
	GetGraphSize(m_handle, &m_graphX, &m_graphY);
}

LockOnTargetUi::~LockOnTargetUi()
{
	//画像の解放
	DeleteGraph(m_handle);
}

void LockOnTargetUi::Update(VECTOR pos)
{
	m_pos = pos;
	(this->*m_updateFunc)();
}

void LockOnTargetUi::Draw()
{
	VECTOR pos = VGet(m_pos.x, m_pos.y + m_height + m_upVec, m_pos.z);

	int m_graphSizeX = static_cast<int>(m_graphX * m_scale);
	int m_graphSizeY = static_cast<int>(m_graphY * m_scale);

	/// <summary>
	/// 第一引数			画像描画座標
	/// 第二、三引数		画像の左上座標
	/// 第四、五引数		画像の右上座標
	/// 第六、七引数		画像の右下座標
	/// 第八、九引数		画像の左下座標
	/// 第十引数			画像ハンドル
	/// 第十一引数			反転するかどうか
	/// </summary>
	DrawModiBillboard3D(pos, -m_graphSizeX * 0.5f, m_graphSizeY * 0.5f, m_graphSizeX * 0.5f, m_graphSizeY * 0.5f,
		m_graphSizeX * 0.5f, -m_graphSizeY * 0.5f, -m_graphSizeX * 0.5f, -m_graphSizeY * 0.5f, m_handle, true);
}

void LockOnTargetUi::CancelLockOn()
{
	//初期化
	m_scale = kMaxScale;
	//更新状態を初期状態に戻す
	m_updateFunc = &LockOnTargetUi::FirstUpdate;
}

void LockOnTargetUi::FirstUpdate()
{
	//サイズを縮小
	m_scale = max(m_scale - kSmallScaleSpeed, kNormalScale);

	//サイズが通常サイズ以下になったら通常の更新処理に移行
	if (m_scale <= kNormalScale)
	{
		m_scale = kNormalScale;
		m_updateFunc = &LockOnTargetUi::NormalUpdate;
	}
}

void LockOnTargetUi::NormalUpdate()
{
	//sinの値を変化させる
	m_sinRate += kSinRateSpeed;
	//sinの値を上下移動のための変数に代入
	m_upVec = static_cast<float>(sin(m_sinRate) * kUpMax);
}
