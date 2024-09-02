#include "SceneBase.h"
#include "DxLib.h"

namespace
{
	//フェード速度
	constexpr int kFadeSpeed = 8;
}

void SceneBase::Init()
{

}

std::shared_ptr<SceneBase> SceneBase::Update()
{
	return shared_from_this();
}

void SceneBase::Draw()
{

}

