#pragma once
#include "SceneBase.h"
#include <vector>
#include <memory>

class PlayerProduction;
class CameraProduction;
class SkyDome;
//class TitleField;
class Field;

class SceneGameClear :
	public SceneBase
{
public:
	SceneGameClear(SceneManager& manager);
	virtual ~SceneGameClear();

	/// <summary>
	/// 更新処理
	/// </summary>
	virtual void Update() override final;	//毎フレーム行う更新処理

	/// <summary>
	/// 描画処理
	/// </summary>
	virtual void Draw() override final;	//毎フレーム行う描画処理

private:

	void DrawCursor();
	void UpdateCursorRight();
	void UpdateCursorLeft();

private:

	std::shared_ptr<PlayerProduction> m_pPlayerProduction;
	std::shared_ptr<CameraProduction> m_pCameraProduction;
	std::shared_ptr<SkyDome> m_pSkyDome;
	//std::shared_ptr<TitleField> m_pTitleField;
	std::shared_ptr<Field> m_pField;

	//遷移先
	enum class e_SceneTrans : int
	{
		kSelect,	//セレクト
		kTitle,		//オプション
	};

	//画像ハンドルこれで全ての画像をロードする
	std::vector<int> m_handles;
	float m_selectAnimation;

	bool m_isActionStart;
	bool m_isActionBack;

	e_SceneTrans m_sceneTrans;

	// カーソルの現在位置と目標位置
	Vec2 m_cursorPos;
	Vec2 m_targetCursorPos;

};

