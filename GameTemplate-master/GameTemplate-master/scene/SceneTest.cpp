#include "game.h"
#include "SceneTest.h"
#include "SceneDebug.h"

#include "Pad.h"

#include "FontManager.h"

#include <string>
#include <cassert>

namespace
{
}

SceneTest::SceneTest():
	m_frameCount(0),
	m_objPos(VGet(0.0f, 0.0f, 0.0f))
{
	// シーン設定
	setDispLoading(false);	// ロード中表示を行わない
}

void SceneTest::startLoad()
{
	// 非同期読み込みを開始する
	SetUseASyncLoadFlag(true);


	// デフォルトに戻す
	SetUseASyncLoadFlag(false);
}

bool SceneTest::isLoaded() const
{
	// 全てのリソースのロードが完了したかを確認する
	//if (CheckHandleASyncLoad(m_handle))	return false;

	return true;
}

void SceneTest::init()
{
	/* 3D関連の初期化 */

	//Zバッファを有効にする
	SetUseZBuffer3D(true);
	SetWriteZBuffer3D(true);

	//背面カリングを有効にする
	SetUseBackCulling(true);	

	//カメラの設定
	SetCameraNearFar(5.0f, 2800.0f);
	//カメラの視野角を設定(ラジアン)
	SetupCamera_Perspective(60.0f * DX_PI_F / 180.0f);
	//カメラの位置、どこを見ているかを設定する
	SetCameraPositionAndTarget_UpVecY(VGet(0.0f, 300.0f, -800.0f), VGet(0.0f, 0.0f, 0.0f));

	//地形データの作成
	//あらかじめ各頂点の位置情報のみを作成する
	std::vector<VECTOR> fieldPos;
	for (int z = -500; z <= 500; z += 100)
	{
	
		for (int x = -500; x <= 500; x += 100)
		{
			VECTOR pos = VGet(x, GetRand(128) - 64, z);
			fieldPos.push_back(pos);
		}

	}

	//1ラインの頂点数
	constexpr int kLineVtxNum = 11;

	//200個のポリゴン表示に使用する頂点データを作成する
	//1ポリゴン3頂点なので、200ポリゴンなら600頂点

	for (int indexZ = 0; indexZ < kLineVtxNum - 1; indexZ++)
	{
		for (int indexX = 0; indexX < kLineVtxNum - 1; indexX++)
		{
			VERTEX3D vtx[3];
			//1ポリゴン3頂点のデータを作成

			//indexZ,indexXで指定された頂点番号をfieldPosのIndexに変換する
			vtx[0].pos = fieldPos[indexZ * kLineVtxNum + indexX];
			vtx[1].pos = fieldPos[indexZ * kLineVtxNum + indexX + kLineVtxNum];
			vtx[2].pos = fieldPos[indexZ * kLineVtxNum + indexX + 1];

			for (int i = 0; i < 3; i++)
			{
				VECTOR tempVec1 = VSub(vtx[1].pos, vtx[0].pos);
				VECTOR tempVec2 = VSub(vtx[2].pos, vtx[1].pos);

				vtx[i].norm = VNorm(VCross(tempVec1, tempVec2));
				vtx[i].dif = GetColorU8(255, 255, 0, 255);
				vtx[i].spc = GetColorU8(0, 0, 0, 0);
				vtx[i].u = 0.0f;
				vtx[i].v = 0.0f;
				vtx[i].su = 0.0f;
				vtx[i].sv = 0.0f;
			}
			//1ポリゴン分のデータを追加
			m_fieldVtx.push_back(vtx[0]);
			m_fieldVtx.push_back(vtx[1]);
			m_fieldVtx.push_back(vtx[2]);
		}

	}

	for (int indexZ = 0; indexZ < kLineVtxNum - 1; indexZ++)
	{
		for (int indexX = 0; indexX < kLineVtxNum - 1; indexX++)
		{
			VERTEX3D vtx[3];
			//1ポリゴン3頂点のデータを作成

			//indexZ,indexXで指定された頂点番号をfieldPosのIndexに変換する
			vtx[0].pos = fieldPos[indexZ * kLineVtxNum + indexX + 1];
			vtx[1].pos = fieldPos[indexZ * kLineVtxNum + indexX + kLineVtxNum];
			vtx[2].pos = fieldPos[indexZ * kLineVtxNum + indexX + kLineVtxNum + 1];

			for (int i = 0; i < 3; i++)
			{
				VECTOR tempVec1 = VSub(vtx[1].pos, vtx[0].pos);
				VECTOR tempVec2 = VSub(vtx[2].pos, vtx[1].pos);

				vtx[i].norm = VNorm(VCross(tempVec1, tempVec2));
				vtx[i].dif = GetColorU8(255, 255, 0, 255);
				vtx[i].spc = GetColorU8(0, 0, 0, 0);
				vtx[i].u = 0.0f;
				vtx[i].v = 0.0f;
				vtx[i].su = 0.0f;
				vtx[i].sv = 0.0f;
			}
			//1ポリゴン分のデータを追加
			m_fieldVtx.push_back(vtx[0]);
			m_fieldVtx.push_back(vtx[1]);
			m_fieldVtx.push_back(vtx[2]);
		}

	}

}

void SceneTest::end()
{

}

void SceneTest::update()
{
	if (getFadeProgress() > 0.0f)
	{
		return;
	}
	m_frameCount++;

	int mouseX, mouseY;
	GetMousePoint(&mouseX, &mouseY);

	//2D座標であるマウスポインタの位置を3D座標に変換する必要がある

	//2D座標を3D座標に変換するとき
	//ConvScreenPosToWorldPos(VECTOR ScreenPos);
	//を使用する
	//引数のScreenPosのx,yはスクリーン座標を指定する(今回はマウスポインタの位置)
	//zには0.0f～1.0fの値を指定する
	//z=0.0fの場合はnearの位置にある3D座標を取得できる
	//z=1.0fの場合はfarの位置にある3D座標を取得できる

	//視線方向の開始位置、終了位置を取得して
	// それを結ぶ直線と各ポリゴンの交差判定を行い、最終的に地形のどこに置くかを決定する
	VECTOR eyeStart = ConvScreenPosToWorldPos(VGet(mouseX, mouseY, 0.0f));
	VECTOR eyeEnd = ConvScreenPosToWorldPos(VGet(mouseX, mouseY, 1.0f));

	//視線方向の直線と各地形ポリゴンを構成する三角形との交差判定を行い、最終的な位置にを決定する


	if( Pad::isTrigger(PAD_INPUT_1) )
	{
		// 一度呼んだらもう一度呼ばないように！
		// 呼んだらフェードアウトが始まるのでこのソースではisFadingではじいている
		setNextScene(new SceneDebug);
	}
}

void SceneTest::draw()
{
	// リソースのロードが終わるまでは描画しないのがよさそう
	// (どちらにしろフェード仕切っているので何も見えないはず)
	if (!isLoaded())	return;

	//マウスカーソルのあっている位置に物体を表示する
	DrawCone3D(VAdd(m_objPos, VGet(0, 64, 0)), VGet(0, 0, 0), 16.0f, 64, 0x00ffff, 0x00ffff, true);

	DrawPolygon3D(m_fieldVtx.data(), m_fieldVtx.size() / 3, DX_NONE_GRAPH, false);

	drawGrid();
	DrawFormatString(0, 0, 0xffffff, "SceneTest FRAME:%d", m_frameCount);

}

void SceneTest::drawGrid()
{

	for (float x = -500.0f; x <= 500.0f; x += 100.0f)
	{
		VECTOR start = VGet(x, 0.0f, -500.0f);
		VECTOR end = VGet(x, 0.0f, 500.0f);
		DrawLine3D(start, end, 0xff00ff);
	}

	for (float z = -500.0f; z <= 500.0f; z += 100.0f)
	{
		VECTOR start = VGet(-500.0f, 0.0f, z);
		VECTOR end = VGet(500.0f, 0.0f, z);
		DrawLine3D(start, end, 0x0000ff);
	}
}
