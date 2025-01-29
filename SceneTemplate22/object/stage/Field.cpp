#include "Field.h"
#include "DxLib.h"

namespace 
{

	const char* const kFieldModelFilename = "Data/Model/Stage/Field12.mv1";
	const char* const kTestModelFilename = "Data/Model/Weapon/Boss_Shield.mv1";

	const char* const kShadowFileName[static_cast<int>(Game::e_StageKind::kStageNum)] =
	{
		"Data/Model/Stage/TitleStage.mv1",
		"Data/Model/Stage/Stage.mv1",
		"Data/Model/Stage/Field07.mv1",
	};

	//ステージのスケール値
	constexpr VECTOR kTitleModelScale = { 0.2f , 0.2f , 0.2f };
	constexpr VECTOR kSelectModelScale = { 1.0f , 1.0f , 1.0f };
	constexpr VECTOR kGamePlayModelScale = { 0.4f , 0.4f , 0.4f };

	//ステージの座標
	constexpr VECTOR kTitleModelPosition = { 0, 60, 0 };
	constexpr VECTOR kSelectModelPosition = { 70.0f, -1600.0f, 1200.0f };
	constexpr VECTOR kGamePlayModelPosition = { 0.0f, 150.0f, 0.0f };
}

Field::Field(Game::e_StageKind stageKind):
	m_modelH(-1),
	m_pos(VGet(0,0,0))
{
	m_modelH = MV1LoadModel(kShadowFileName[static_cast<int>(stageKind)]);

	if (stageKind == Game::e_StageKind::kTitle)
	{
		MV1SetScale(m_modelH, kTitleModelScale);
	}
	else if (stageKind == Game::e_StageKind::kSelect)
	{
		MV1SetScale(m_modelH, kSelectModelScale);
	}
	else if (stageKind == Game::e_StageKind::kGamePlay)
	{
		MV1SetScale(m_modelH, kGamePlayModelScale);
	}

	if (stageKind == Game::e_StageKind::kTitle)
	{
		m_pos = kTitleModelPosition;
		MV1SetRotationXYZ(m_modelH, VGet(0, 14.1, 0));
	}
	else if (stageKind == Game::e_StageKind::kSelect)
	{
		m_pos = kSelectModelPosition;
	}
	else if (stageKind == Game::e_StageKind::kGamePlay)
	{
		m_pos = kGamePlayModelPosition;
	}

	//モデルに座標をセットする
	MV1SetPosition(m_modelH, m_pos);
}

Field::~Field()
{
	MV1DeleteModel(m_modelH);
	m_modelH = -1;
}

void Field::Initialize()
{
}

//void Field::Update()
//{
//
//}

void Field::Draw()
{
	MV1DrawModel(m_modelH);
}
