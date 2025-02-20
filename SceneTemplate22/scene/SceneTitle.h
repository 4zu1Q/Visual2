#pragma once
#include "scene/SceneBase.h"
#include <vector>
#include <memory>

class PlayerProduction;
class CameraProduction;
class SkyDome;
class TitleField;
class Field;

class SceneTitle : public SceneBase
{
public:
    SceneTitle(SceneManager& manager);
    virtual ~SceneTitle();

	/// <summary>
	/// 更新処理
	/// </summary>
	virtual void Update() override final;	//毎フレーム行う更新処理

	/// <summary>
	/// 描画処理
	/// </summary>
	virtual void Draw() override final;	//毎フレーム行う描画処理
    
	const bool& GetIsPlayer() const { return m_isPlayer; }

private:

	void DrawCursor();
	void UpdateCursorUp();
	void UpdateCursorDown();

private:

	//スマートポインタ
	std::shared_ptr<PlayerProduction> m_pPlayerProduction;
	std::shared_ptr<CameraProduction> m_pCameraProduction;
	std::shared_ptr<SkyDome> m_pSkyDome;
	std::shared_ptr<TitleField> m_pTitleField;
	std::shared_ptr<Field> m_pField;

	//遷移先
	enum class e_SceneTrans : int
	{
		kNewGame,	//ニューゲーム
		kLoadGame,	//ロードゲーム
		kOption,	//オプション
		kQuit,		//ゲーム終了
	};

	//enum変数
	e_SceneTrans m_sceneTrans;

	//画像ハンドルこれで全ての画像をロードする
	std::vector<int> m_handles;

	bool m_isPlayer;	//プレイヤーのタイトルバージョンのアニメーションを行う

	//イベントのフラグ
	bool m_isPreesAnyButton;
	bool m_isStart;
	
	//タイム変数
	int m_startTime;
	int m_titleLogoTime;
	int m_fadeGraphTime;

	//カーソルアニメーション
	float m_selectAnimation;

	//プレイヤーの座標
	VECTOR m_PlayerPos;

	//行動するかしないかの場合
	bool m_isActionStart;
	bool m_isActionBack;

	//タイトルロゴの座標
	Vec2 m_titleLogoPos;

	// カーソルの現在位置と目標位置
	Vec2 m_cursorPos;
	Vec2 m_targetCursorUpPos;
	Vec2 m_targetCursorDownPos;

};

