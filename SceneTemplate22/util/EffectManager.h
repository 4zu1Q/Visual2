﻿#pragma once

#include <vector>
#include <string>
#include <map>
#include <memory>

#include "DxLib.h"


class EffectManager
{
private:
	//エフェクト構造体
	struct Effect
	{
		int handle;
		float x, y, z;
		int frame;
		bool isExist;
	};

	//エフェクト生成構造体
	struct EffectEmitter
	{
		int emitterHandle = -1;
		const char* effectPath = "";
		std::vector<Effect> effects;
		int endFrame;
	};


private:

	// シングルトンパターンなのでコンストラクタはprivateに置く
	EffectManager() {};

public:
	virtual ~EffectManager();

	//コピーコンストラクタから実体の生成ができてしまうため
	//コピーコンストラクタを禁止する
	EffectManager(const EffectManager&) = delete;
	EffectManager& operator=(const EffectManager&) = delete;
	EffectManager(EffectManager&&) = delete;
	EffectManager& operator= (const EffectManager&&) = delete;

	/// <summary>
	/// EffectManagerはGetInstance()を通した参照からしか利用できない
	/// </summary>
	/// <returns></returns>
	static EffectManager& GetInstance()
	{
		if (!m_instance)
		{
			m_instance = new EffectManager;
		}

		return *m_instance;
	}

	//これをし忘れると普通にメモリリーク
	static void Destroy()
	{
		delete m_instance;
		m_instance = nullptr;
	}

public:
	void Load(std::string name, const char* path, int endFrame, float scale = 1.0f);
	void Update();
	void Draw();

	void CreateEffect(std::string name, VECTOR pos, VECTOR rot = VECTOR());

	/// <summary>
	/// 指定した名前のすべてのエフェクトの座標を移動させる
	/// </summary>
	/// <param name="name">移動させたいエフェクトの名前</param>
	/// <param name="pos">移動先座標</param>
	void SetPos(std::string name, VECTOR pos);

	/// <summary>
	/// 指定した名前のすべてのエフェクトの座標を回転させる
	/// </summary>
	/// <param name="name">回転させたいエフェクトの名前</param>
	/// <param name="pos">回転先座標</param>
	void SetRotation(std::string name, VECTOR rot);

private:

	//staticにすることで
	//Singletonのポインタがプログラム起動時に一つ作られるようにする
	static EffectManager* m_instance;

private:

	std::map<std::string, std::shared_ptr<EffectEmitter>> m_effect;
};

