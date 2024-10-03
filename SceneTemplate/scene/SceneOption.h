#pragma once
#include "SceneBase.h"

class SceneOption :
	public SceneBase
{
public:
	SceneOption(SceneManager& manager);
	virtual ~SceneOption();

	/// <summary>
	/// 更新処理
	/// </summary>
	virtual void Update() override final;	//毎フレーム行う更新処理

	/// <summary>
	/// 描画処理
	/// </summary>
	virtual void Draw() override final;	//毎フレーム行う描画処理

private:

	enum Item : int
	{
		kBgm,
		kSe,
		kSensitivity,
		kFullScreen
	};

	Item m_nowItem;

	int m_bgmScale;
	int m_seScale;
	int m_sensitivityScale;
	bool m_isFullScreen;

	//void (SceneOption::* m_updateFunc)

	//BGM
	void BgmUpdate();
	//SE
	void SeUpdate();
	//Sensitivity
	void SensitivityUpdate();
	//FullScreen
	void FullScreenUpdate();


};

