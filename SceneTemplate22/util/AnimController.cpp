#include "AnimController.h"
#include <DxLib.h>
#include <cassert>
#include <fstream>
#include "util/StringUtility.h"

namespace
{
	enum class FileIndex
	{
		ID,
		NAME,
		SPEED,
	};

	// デフォルトブレンドスピード
	constexpr float kDefBlendSpeed = 1 / 16.0f;
}

AnimController::AnimController() :
	m_updateFunc(nullptr),
	m_modelH(-1),
	m_nowAnimIndex(-1),
	m_nowAttachIndex(-1),
	m_preAttachIndex(-1),
	m_blendRate(0.0f),
	m_blendSpeed(kDefBlendSpeed),
	m_isLoop(true),
	m_isStopEnd(false),
	m_isPreStopEnd(false)
{
}

AnimController::~AnimController()
{
}

void AnimController::Initialize(const char* const path, int modelH, const char* const id)
{
	LoadIdTable(path);
	m_updateFunc = &AnimController::NormalUpdate;
	m_modelH = modelH;
	auto index = MV1GetAnimIndex(m_modelH, m_idTable.at(id).name.c_str());
	assert(index != -1 && "存在しないアニメーション名です");
	m_nowId = id;
	m_nowAttachIndex = MV1AttachAnim(modelH, index);
}

void AnimController::ChangeModel(int modelH, const const char* const id, const char* const path)
{
	if (m_nowAttachIndex > -1) MV1DetachAnim(m_modelH, m_nowAttachIndex);
	if (m_preAttachIndex > -1) MV1DetachAnim(m_modelH, m_preAttachIndex);

	if (path != "") LoadIdTable(path);
	m_updateFunc = &AnimController::NormalUpdate;
	m_modelH = modelH;
	auto index = MV1GetAnimIndex(modelH, m_idTable.at(id).name.c_str());
	assert(index != -1 && "存在しないアニメーション名です");
	m_nowId = id;
	m_nowAttachIndex = MV1AttachAnim(modelH, index);
}

void AnimController::UpdateAnim(float rate)
{
	assert(m_nowAttachIndex != -1);
	(this->*m_updateFunc)(rate);
}

bool AnimController::ChangeAnim(const char* const id, bool isTopStart, bool isStopEnd, bool isBlendChange, float blendSpeed)
{
	auto index = MV1GetAnimIndex(m_modelH, m_idTable.at(id).name.c_str());
	if (index == -1)
	{
		assert(false && "存在しないアニメーション名です");
		return false;
	}

	if (m_nowAnimIndex == index) return false;
	m_nowAnimIndex = index;
	m_isLoop = false;
	m_preId = m_nowId;
	m_isPreStopEnd = m_isStopEnd;
	m_isStopEnd = isStopEnd;

	m_nowId = id;
	if (isBlendChange)
	{
		BlendChange(index, blendSpeed);
	}
	else
	{
		NoBlendChange(index);
	}

	if (isTopStart)
	{
		MV1SetAttachAnimTime(m_modelH, m_nowAttachIndex, 0.0f);
	}

	return true;
}

void AnimController::LoadIdTable(const char* const path)
{
	std::list<std::vector<std::string>> data;
	m_idTable.clear();

	// ファイル読み込み
	std::ifstream ifs(path);

	if (!ifs)
	{
		assert(false);
		return;
	}

	// バッファー
	std::string strBuf;

	// 余分な部分の読み込み
	std::getline(ifs, strBuf);
	// データすべて読み込み
	while (std::getline(ifs, strBuf))
	{
		data.push_back(StringUtility::Split(strBuf, ','));
	}

	for (auto& item : data)
	{
		// 登録
		const auto& id =item.at(static_cast<int>(FileIndex::ID));
		auto& info = m_idTable[id];
		info.name = item.at(static_cast<int>(FileIndex::NAME));
		info.speed = std::stof(item.at(static_cast<int>(FileIndex::SPEED)));
	}
}

void AnimController::NormalUpdate(float rate)
{
	AnimUpdate(m_nowId, m_nowAttachIndex, rate, true, m_isStopEnd);
}

void AnimController::BlendUpdate(float rate)
{
	// それぞれのアニメーションの更新
	AnimUpdate(m_nowId, m_nowAttachIndex, rate, true, m_isStopEnd);
	AnimUpdate(m_preId, m_preAttachIndex, rate, false, m_isPreStopEnd);

	// ブレンドを進める
	m_blendRate += m_blendSpeed;
	// ブレンド率が1.0を超えたらアニメーションブレンドの終了
	if (m_blendRate > 1.0f)
	{
		m_blendRate = 1.0f;
		// 古いアニメーションの削除
		MV1DetachAnim(m_modelH, m_preAttachIndex);
		m_preAttachIndex = -1;
		// アップデート先の変更
		m_updateFunc = &AnimController::NormalUpdate;
	}
	// ブレンド率の適用
	MV1SetAttachAnimBlendRate(m_modelH, m_preAttachIndex, 1.0f - m_blendRate);
	MV1SetAttachAnimBlendRate(m_modelH, m_nowAttachIndex, m_blendRate);
}

void AnimController::AnimUpdate(const std::string& id, int attachIndex, float rate, bool isLoopCheck, bool isStopEnd)
{
	// アニメーションの総再生時間の取得
	auto totalTime = MV1GetAttachAnimTotalTime(m_modelH, attachIndex);
	// アニメーションの再生時間を取得
	auto time = MV1GetAttachAnimTime(m_modelH, attachIndex);
	// スピード分アニメを進める
	rate = std::max<float>(0.0f ,rate);		//アニメーション速度のリミッター解除
	//rate = std::max<float>(0.0f, std::min<float>(1.0f, rate));
	float speed = m_idTable.at(id).speed * rate;
	time += speed;
	// 総裁時間を超えたら再生時間から総再生時間分へらす
	while (time >= totalTime)
	{
		time -= totalTime;
		// ループチェックする場合
		if (isLoopCheck) m_isLoop = true;
		// 最後で終了させる場合は時間を総再生時間にして抜ける
		if (isStopEnd)
		{
			time = totalTime;
			break;
		}
	}
	// 時間の適用
	MV1SetAttachAnimTime(m_modelH, attachIndex, time);
}

void AnimController::BlendChange(int animIndex, float blendSpeed)
{
	// 古い情報が残っていれば削除する
	if (m_preAttachIndex > -1)
	{
		MV1DetachAnim(m_modelH, m_preAttachIndex);
	}
	// 現在の情報を古い情報として保存する
	m_preAttachIndex = m_nowAttachIndex;
	// 新しくアニメーションをアタッチする
	m_nowAttachIndex = MV1AttachAnim(m_modelH, animIndex);

	// ブレンドの変更
	if (blendSpeed < 0)
	{
		blendSpeed = kDefBlendSpeed;
	}
	m_blendSpeed = blendSpeed;
	m_blendRate = 0.0f;
	MV1SetAttachAnimBlendRate(m_modelH, m_preAttachIndex, 1.0f);
	MV1SetAttachAnimBlendRate(m_modelH, m_nowAttachIndex, 0.0f);

	// Update先の変更
	m_updateFunc = &AnimController::BlendUpdate;
}

void AnimController::NoBlendChange(int animIndex)
{
	// 古い情報が残っていれば削除
	if (m_preAttachIndex > -1)
	{
		MV1DetachAnim(m_modelH, m_preAttachIndex);
		m_preAttachIndex = -1;
	}
	// アニメーションの削除
	MV1DetachAnim(m_modelH, m_nowAttachIndex);
	// 新しいアニメーションをアタッチ
	m_nowAttachIndex = MV1AttachAnim(m_modelH, animIndex);

	// Update先の変更
	m_updateFunc = &AnimController::NormalUpdate;
}
