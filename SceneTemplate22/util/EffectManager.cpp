﻿#include "EffectManager.h"
#include "EffekseerForDXLib.h"

#include <cassert>

EffectManager* EffectManager::m_instance = nullptr;

EffectManager::~EffectManager()
{
	//まずすでに同じパスのエフェクトがロードされていないか確認する
	for (auto& effect : m_effect)
	{
		DeleteEffekseerEffect(effect.second->emitterHandle);
	}
}

void EffectManager::Load(std::string name, const char* path, int endFrame, float scale)
{
	//まずすでに同じパスのエフェクトがロードされていないか確認する
	for (auto& effect : m_effect)
	{
		if (effect.second->effectPath == path)
		{
			return;
		}
	}

	//ここまで来たらエフェクトをロードする
	std::shared_ptr<EffectEmitter> add = std::make_shared<EffectEmitter>();
	add->emitterHandle = LoadEffekseerEffect(path, scale);
	assert(add->emitterHandle != -1 && "エフェクトロード失敗");
	add->endFrame = endFrame;

	m_effect[name] = add;

}

void EffectManager::Update()
{
	UpdateEffekseer3D();

	//エフェクトの更新
	for (auto& effects : m_effect)
	{
		for (auto& ef : effects.second->effects)
		{
			if (ef.frame > effects.second->endFrame)
			{
				StopEffekseer3DEffect(ef.handle);
				ef.isExist = false;
			}

			ef.frame++;
		}

		//isExistがfalseのオブジェクトを削除
		auto it = std::remove_if(effects.second->effects.begin(), effects.second->effects.end(), [](auto& v)
			{
				return v.isExist == false;
			});
		effects.second->effects.erase(it, effects.second->effects.end());
	}
}

void EffectManager::Draw()
{
	DrawEffekseer3D();
}

void EffectManager::CreateEffect(std::string name, VECTOR pos, VECTOR rot)
{
	int handle = -1;
	for (auto& effects : m_effect)
	{
		if (effects.first == name)
		{
			handle = effects.second->emitterHandle;
		}
	}

	//もし追加しようとしているエフェクトの名前のハンドルがなければ終わる
	if (handle == -1)
	{
		return;
	}

	Effect add;
	add.handle = PlayEffekseer3DEffect(handle);
	add.frame = 0;
	add.x = pos.x;
	add.y = pos.y;
	add.z = pos.z;
	add.isExist = true;

	SetPosPlayingEffekseer3DEffect(add.handle, add.x, add.y, add.z);
	SetRotationPlayingEffekseer3DEffect(add.handle, rot.x, rot.y, rot.z);

	m_effect[name]->effects.emplace_back(add);

	return;
}

void EffectManager::SetPos(std::string name, VECTOR pos)
{
	std::shared_ptr<EffectEmitter> emit;

	for (auto& effects : m_effect)
	{
		if (effects.first == name)
		{
			for (auto& ef : effects.second->effects)
			{
				ef.x = pos.x;
				ef.y = pos.y;
				ef.z = pos.z;

				SetPosPlayingEffekseer3DEffect(ef.handle, ef.x, ef.y, ef.z);

			}
		}
	}

	return;
}

void EffectManager::SetRotation(std::string name, VECTOR rot)
{
	for (auto& effects : m_effect)
	{
		if (effects.first == name)
		{
			for (auto& ef : effects.second->effects)
			{
				SetRotationPlayingEffekseer3DEffect(ef.handle, rot.x, rot.y, rot.z);
			}
		}
	}
}
