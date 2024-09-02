#include "SceneBase.h"
#include "DxLib.h"

namespace
{
	//�t�F�[�h���x
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

