#pragma once
#include "SceneBase.h"
class SceneTitle :
    public SceneBase
{
public:
	SceneTitle();
	virtual ~SceneTitle();

	virtual void Init();	//シーンに入るときの初期化処理
	virtual std::shared_ptr<SceneBase> Update();	//毎フレーム行う更新処理
	virtual void Draw();	//毎フレーム行う描画処理
	virtual void End();	//シーンを抜けるときの処理

private:
	//シーンを管理するポインタ
	//std::shared_ptr<SceneManager> m_pManager = std::make_shared<SceneManager>();


	//カーソル選択
	enum Select
	{
		kStart,		//スタート
		kOption,	//オプション
		kGameEnd,	//終了
	};

	//enum変数
	Select m_select;
	int m_selectPosY;

	//グラフィックハンドル
	int m_logoH;
	int m_startH;
	int m_optionH;
	int m_endH;
	int m_selectH;
	int m_backgroundH;

	//フレーム変数
	int m_frameScene;
	int m_frameCount;

	//フラグ変数
	bool m_isInterval;
	bool m_isCommand;
};

