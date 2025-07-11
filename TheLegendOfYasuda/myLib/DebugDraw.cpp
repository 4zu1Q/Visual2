// 2024 Takeru Yui All Rights Reserved.
#include "DxLib.h"
#include "DebugDraw.h"

using namespace MyLib;

// 実体定義
std::vector<DebugDraw::LineInfo>	DebugDraw::m_lineInfo;
std::vector<DebugDraw::SphereInfo>	DebugDraw::m_sphereInfo;
std::vector<DebugDraw::CapsuleInfo>	DebugDraw::m_capsuleInfo;


/// <summary>
/// クリア
/// </summary>
void DebugDraw::Clear()
{
	m_lineInfo.clear();
	m_sphereInfo.clear();
	m_capsuleInfo.clear();
}

/// <summary>
/// 描画
/// </summary>
void DebugDraw::Draw()
{
	for (const auto& line : m_lineInfo)
	{
		DxLib::DrawLine3D(line.startPos_,
			line.endPos_,
			line.color_);
	}

	for (const auto& sphere : m_sphereInfo)
	{
		DxLib::DrawSphere3D(sphere.center_,
			sphere.radius_,
			6,
			sphere.color_,
			sphere.color_,
			FALSE);
	}

	for (const auto& capsule : m_capsuleInfo)
	{
		DxLib::DrawCapsule3D(capsule.downPos_,
			capsule.upPos_,
			capsule.radius_,
			6,
			capsule.color_,
			capsule.color_,
			FALSE);
	}

}

void MyLib::DebugDraw::DrawLine(const VECTOR& start, const VECTOR& end, int color)
{
	LineInfo newInfo;
	newInfo.startPos_ = start;
	newInfo.endPos_ = end;
	newInfo.color_ = color;
	m_lineInfo.emplace_back(newInfo);
}

void MyLib::DebugDraw::DrawSphere(const VECTOR& center, float radius, int color)
{
	SphereInfo newInfo;
	newInfo.center_ = center;
	newInfo.radius_ = radius;
	newInfo.color_ = color;
	m_sphereInfo.emplace_back(newInfo);
}

void MyLib::DebugDraw::DrawCapsule(const VECTOR& downPos, const VECTOR& upPos, float radius, int color)
{
	CapsuleInfo newInfo;
	newInfo.downPos_ = downPos;
	newInfo.upPos_ = upPos;
	newInfo.radius_ = radius;
	newInfo.color_ = color;
	m_capsuleInfo.emplace_back(newInfo);
}