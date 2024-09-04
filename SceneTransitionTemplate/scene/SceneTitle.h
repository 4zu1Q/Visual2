#pragma once
#include "SceneBase.h"

class SceneManager;
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
	std::shared_ptr<SceneManager> m_pManager = std::make_shared<SceneManager>();


	//カーソル選択
	enum Select
	{
		kStart,		//スタート
		kOption,	//オプション
		kGameEnd,	//終了
	};

	//カーソル選択
	enum Option
	{
		kOperator,		//操作説明
		kSound,			//サウンド
		kFullScreen,	//フルスクリーン
	};

	//enum変数
	Select m_select;
	Option m_option;
	int m_selectPosY;

	float m_selectAnimation;

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
	bool m_isInterval;	//フェード用
	bool m_isOption;	//オプション画面用
	bool m_isCommand;	//決定した後に動かさない用
	bool m_isFullScreen;//フルスクリーン用
	bool m_isSound;		//サウンド設定用
	bool m_isOperator;	//説明画面表示用

};

