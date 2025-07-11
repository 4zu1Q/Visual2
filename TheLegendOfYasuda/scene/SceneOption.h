#pragma once
#include "scene/SceneBase.h"
#include <vector>
#include <memory>

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

	//BGM
	void BgmUpdate();
	//SE
	void SeUpdate();
	//Sensitivity
	void SensitivityUpdate();
	//FlipSideUp
	void FlipSideUpUpdate();
	//FullScreen
	void FullScreenUpdate();

	void DrawCursor();
	void UpdateCursorUp();
	void UpdateCursorDown();

private:

	//更新メンバ関数ポインタ
	using UpdateFunc_t = void(SceneOption::*)(/*引数書く*/);
	UpdateFunc_t m_updateFunc;

	//オプションの種類
	enum e_Item : int
	{
		kBgm,
		kSe,
		kSensitivity,
		kFlipSideUp,
		kFullScreen
	};

	//enum変数
	e_Item m_nowItem;

	//セッティングする変数
	int m_bgmScale;
	int m_seScale;
	int m_sensitivityScale;
	bool m_isFullScreen;
	bool m_isFlipSideUp;

	//フォントハンドル
	int m_fontHandle;

	int m_pushCount;			//連続で押せる
	float m_selectAnimation;	//カーソルのアニメーション

	//画像ハンドルこれで全ての画像をロードする
	std::vector<int> m_handles;

	//カーソル変数
	Vec2 m_cursorPos;
	Vec2 m_targetCursorUpPos;
	Vec2 m_targetCursorDownPos;

};

