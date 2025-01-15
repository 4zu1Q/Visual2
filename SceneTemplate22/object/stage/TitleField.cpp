#include "TitleField.h"

namespace
{
	const char* const kFieldModelFilename = "Data/Model/Stage/Plane.mv1";


	constexpr VECTOR kModelScale = { 1.0f , 1.0f , 1.0f };
	//constexpr VECTOR kModelScale = { 0.10f , 0.10f , 0.10f };
	constexpr VECTOR kModelPosition = { 0.0f, 0.0f, 0.0f };

}


TitleField::TitleField():
	m_modelH(-1),
	m_pos(kModelPosition)
{
	m_modelH = MV1LoadModel(kFieldModelFilename);
	MV1SetScale(m_modelH, kModelScale);

	//モデルに座標をセットする
	MV1SetPosition(m_modelH, m_pos);
}

TitleField::~TitleField()
{
	MV1DeleteModel(m_modelH);
	m_modelH = -1;
}

void TitleField::Initialize()
{
}

void TitleField::Draw()
{
	MV1DrawModel(m_modelH);
}
