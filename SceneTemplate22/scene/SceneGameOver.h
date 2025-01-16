#pragma once
#include "SceneBase.h"
#include <vector>
#include <memory>

class PlayerProduction;
class CameraProduction;

class SceneGameOver :
	public SceneBase
{
public:
	SceneGameOver(SceneManager& manager);
	virtual ~SceneGameOver();

	/// <summary>
	/// 更新処理
	/// </summary>
	virtual void Update() override final;	//毎フレーム行う更新処理

	/// <summary>
	/// 描画処理
	/// </summary>
	virtual void Draw() override final;	//毎フレーム行う描画処理

private:

	std::shared_ptr<PlayerProduction> m_pPlayerProduction;
	std::shared_ptr<CameraProduction> m_pCameraProduction;

	int m_fadeTime;	//フェード時間の引き延ばし
	float m_selectAnimation;

	//遷移先
	enum class e_SceneTrans : int
	{
		kGamePlay,	//ゲームプレイ
		kSelect,		//セレクト
	};

	//画像ハンドルこれで全ての画像をロードする
	std::vector<int> m_handles;

	bool m_isActionStart;
	bool m_isActionBack;

	e_SceneTrans m_sceneTrans;

};


