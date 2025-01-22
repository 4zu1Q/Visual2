#pragma once
#include <memory>
#include "DxLib.h"
#include "util/Vec2.h"

class SceneManager;

/// <summary>
/// シーン基底クラス
/// 色んなシーンの基になるクラス
/// </summary>
class SceneBase
{
public:

	/// <summary>
	/// 生成時にSceneManagerの参照を受け取っておく
	/// </summary>
	/// <param name="manager">SceneManagerの参照</param>
	SceneBase(SceneManager& manager);
	
	/// <summary>
	/// シーンの更新
	/// </summary>										
	virtual void Update() = 0;											

	/// <summary>
	/// シーンの描画
	/// </summary>
	virtual void Draw() = 0;	//毎フレーム行う描画処理

protected:

	/// <summary>
	/// フェードのアップデート関数
	/// </summary>
	void UpdateFade();

	/// <summary>
	/// フェードの描画処理とフェードの色
	/// </summary>
	/// <param name="fadeColor"></param>
	void DrawFade(int fadeColor) const;

	/// <summary>
	/// 白いフェードの描画関数
	/// </summary>
	//void DrawWhiteFade() const;

	/// <summary>
	/// フェードアウト開始
	/// </summary>
	void StartFadeOut();

	/// <summary>
	/// フェードイン開始
	/// </summary>
	void StartFadeIn();

	/// <summary>
	/// フェードイン中
	/// </summary>
	/// <returns></returns>
	bool IsFadingIn() const;

	/// <summary>
	/// フェードアウト中
	/// </summary>
	/// <returns></returns>
	bool IsFadingOut() const;	

	/// <summary>
	/// フェードインorアウト中
	/// </summary>
	/// <returns></returns>
	bool IsFading() const { return IsFadingIn() || IsFadingOut(); }	

	/// <summary>
	/// 
	/// </summary>
	/// <returns></returns>
	bool IsFinishFadeOut()const;

	/// <summary>
	/// フェードインをスキップする関数
	/// </summary>
	void FadeInSkip();

	/// <summary>
	/// フェードアウトをスキップする関数
	/// </summary>
	void FadeOutSkip();

	/// <summary>
	/// グラフのフェードを行う関数
	/// </summary>
	void UpdateFadeSelectGraph();

	/// <summary>
	/// 特定の画像をフェード描画させる関数(DrawGraphのみ使うことができる)
	/// </summary>
	/// <param name="graphHandle">フェードさせる画像ハンドル</param>
	/// <param name="graphPos">フェードさせる画像の座標</param>
	void DrawFadeSelectGraph(int graphHandle , Vec2 graphPos);

	/// <summary>
	/// フェードの時間を0にリセットする関数
	/// </summary>
	void FadeGraphSelectReset();


	void UpdateFadeGraphTitleLogo();

	void DrawFadeGraphTitleLogo(int graphHandle, Vec2 graphPos);

	void FadeGraphTitleLogoReset();



protected:

	SceneManager& m_pManager;	//シーン管理クラスの参照
	bool m_isToNextScene;
	bool m_isFadeColor;		

private:

	//フェード関連処理
	int m_fadeColor;
	int m_fadeBright;
	int m_fadeSpeed;

	int m_fadeGraphSelectTime;
	int m_fadeGraphTitleTime;
};

