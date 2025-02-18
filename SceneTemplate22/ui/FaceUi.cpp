#include "FaceUi.h"

#include "object/player/Player.h"
#include "util/SaveDataManager.h"

namespace
{
	//顔の画像ファイル名
	const char* const kPowerFaceFileName = "Data/Image/PowerFace.png";
	const char* const kSpeedFaceFileName = "Data/Image/SpeedFace.png";
	const char* const kShotFaceFileName = "Data/Image/ShotFace.png";
	const char* const kRastFaceFileName = "Data/Image/RassFace.png";

	//顔の座標
	const Vec2 kFacePos = { 1128.0f , 40.0f };

	//使う画像の種類
	enum e_Ui
	{
		kPowerH,
		kPowerShadowH,
		kSpeedH,
		kSpeedShadowH,
		kShotH,
		kShotShadowH,
		kRassH,
		kRassShadowH,
	};

}

FaceUi::FaceUi()
{
	m_handles.push_back(LoadGraph("Data/Image/PowerFace.png"));
	m_handles.push_back(LoadGraph("Data/Image/PowerFace01.png"));		

	m_handles.push_back(LoadGraph("Data/Image/SpeedFace.png"));
	m_handles.push_back(LoadGraph("Data/Image/SpeedFace01.png"));			

	m_handles.push_back(LoadGraph("Data/Image/ShotFace.png"));
	m_handles.push_back(LoadGraph("Data/Image/ShotFace01.png"));				

	m_handles.push_back(LoadGraph("Data/Image/RassFace.png"));
	m_handles.push_back(LoadGraph("Data/Image/RassFace01.png"));				
}

FaceUi::~FaceUi()
{
	//画像の削除
	for (int i = 0; i < m_handles.size(); i++)
	{
		DeleteGraph(m_handles[i]);
	}


	m_handles.clear();
}

void FaceUi::Update()
{

}

void FaceUi::Draw(Player& player)
{
	//プレイヤーの仮面UI
	//セーブデータをやったら使う
	if (player.GetFaceKind() == e_PlayerKind::kPowerPlayer)
	{
		if(SaveDataManager::GetInstance().IsRelease(Game::e_PlayerKind::kPowerPlayer))
		{
			DrawGraph(kFacePos.x, kFacePos.y, m_handles[kPowerH], true);
		}
		else
		{
			DrawGraph(kFacePos.x, kFacePos.y, m_handles[kPowerShadowH], true);
		}
	}
	else if (player.GetFaceKind() == e_PlayerKind::kSpeedPlayer)
	{
		if(SaveDataManager::GetInstance().IsRelease(Game::e_PlayerKind::kSpeedPlayer))
		{
			DrawGraph(kFacePos.x, kFacePos.y, m_handles[kSpeedH], true);
		}
		else
		{
			DrawGraph(kFacePos.x, kFacePos.y, m_handles[kSpeedShadowH], true);
		}
	}
	else if (player.GetFaceKind() == e_PlayerKind::kShotPlayer)
	{
		if(SaveDataManager::GetInstance().IsRelease(Game::e_PlayerKind::kShotPlayer))
		{
			DrawGraph(kFacePos.x, kFacePos.y, m_handles[kShotH], true);
		}
		else
		{
			DrawGraph(kFacePos.x, kFacePos.y, m_handles[kShotShadowH], true);
		}
	}
	else if (player.GetFaceKind() == e_PlayerKind::kRassPlayer)
	{
		if(SaveDataManager::GetInstance().IsRelease(Game::e_PlayerKind::kRassPlayer))
		{
			DrawGraph(kFacePos.x, kFacePos.y, m_handles[kRassH], true);
		}
		else
		{
			DrawGraph(kFacePos.x, kFacePos.y, m_handles[kRassShadowH], true);
		}
	}

}
