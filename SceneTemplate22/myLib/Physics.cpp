#include <cassert>
#include "DxLib.h"
#include "MyLib.h"
#include "MathHelper.h"
#include "object/player/Player.h"


using namespace MyLib;

namespace
{
	constexpr int kBeforeFixInfoColor = 0xff00ff;	// 補正前情報色
	constexpr int kAimInfoColor = 0x00ffff;			// 補正前予定情報色
	constexpr int kAfterFixInfoColor = 0xffffff;	// 補正後情報色


	//最大当たり判定ポリゴン数
	constexpr int kMaxColHitPolyNum = 2000;
	//壁押し出し処理の最大試行回数
	constexpr int kMaxColHitTryNum = 16;

	//壁押し出し時にスライドさせる距離
	constexpr float kColHitSlideLength = 0.1f;

	//壁ポリゴンか床ポリゴンかを判断するための変数
	constexpr float kWallPolyBorder = 0.4f;

	//壁ポリゴンと判断するための高さ変数
	constexpr float kWallPolyHeight = 5.0f;

	//頭の高さを設定
	constexpr float kHeadHeight = 40.0f;
}

MyLib::Physics::Physics(Game::e_StageKind stageKind)
{
	m_pField = std::make_shared<Field>(stageKind);
	m_pPlayer = std::make_shared<Player>();
}

/// <summary>
/// 衝突物の登録
/// </summary>
void Physics::Entry(std::shared_ptr<Collidable> collidable /*std::shared_ptr<Collidable*> collidable*/)
{
	// 登録
	bool found = (std::find(m_collidables.begin(), m_collidables.end(), collidable) != m_collidables.end());
	if (!found)
	{
		m_collidables.emplace_back(collidable);
	}
	// 既に登録されてたらエラー
	else
	{
		assert(0 && "指定のcollidableは登録済です。");
	}
}

/// <summary>
/// 衝突物の登録解除
/// </summary>
void Physics::Exit(std::shared_ptr<Collidable> collidable /*std::shared_ptr<Collidable*> collidable*/)
{
	// 登録解除(remove)
	bool found = (std::find(m_collidables.begin(), m_collidables.end(), collidable) != m_collidables.end());
	if (found)
	{
		m_collidables.remove(collidable);
	}
	// 登録されてなかったらエラー
	else
	{
		//これを入れてなくても動くので一応入れておかない(こいつのせいで一部のシーン遷移ができない)
		//assert(0 && "指定のcollidableが登録されていません。");
	}
}

/// <summary>
/// 更新（登録オブジェクトの物理移動、衝突通知）
/// </summary>
void Physics::Update()
{
	// 移動
	for (auto& item : m_collidables)
	{
		// ポジションに移動力を足す
		VECTOR pos = item->m_rigidbody.GetPos();
		VECTOR velocity = item->m_rigidbody.GetVelocity();

		// 重力を利用する設定なら、重力を追加する
		if (item->m_rigidbody.UseGravity())
		{
			velocity = VAdd(velocity, VGet(0, m_gravity, 0));

			// 最大重力加速度より大きかったらクランプ
			if (velocity.y < m_maxGravityAccel)
			{
				velocity = VGet(velocity.x, m_maxGravityAccel, velocity.z);
			}
		}

		VECTOR nextPos = VAdd(pos, velocity);

		item->m_rigidbody.SetVelocity(velocity);

		/* もともとの情報、予定情報をデバッグ表示 */ 
		auto kind = item->m_pColliderData->GetKind();

#if _DEBUG


		if (kind == ColliderData::e_Kind::kSphere)
		{
			auto sphereData = std::dynamic_pointer_cast<ColliderDataSphere>(item->m_pColliderData);
			float radius = sphereData->m_radius;
			DebugDraw::DrawSphere(pos, radius, kBeforeFixInfoColor);
			DebugDraw::DrawSphere(nextPos, radius, kAimInfoColor);

		}
		else if (kind == ColliderData::e_Kind::kCapsule)
		{
			auto capsuleData = std::dynamic_pointer_cast<ColliderDataCapsule>(item->m_pColliderData);
			float radius = capsuleData->m_radius;
			VECTOR upPos = capsuleData->m_posUp;


			DebugDraw::DrawCapsule(pos, upPos, radius, kBeforeFixInfoColor);
			DebugDraw::DrawCapsule(nextPos, upPos, radius, kAimInfoColor);
		}

#endif

		if (kind == ColliderData::e_Kind::kSphere)
		{


		}
		else if (kind == ColliderData::e_Kind::kCapsule)
		{
			///*フィールドとの当たり判定の処理*/

			//// 予定ポジション設定
			//item->m_rigidbody.SetNextPos(nextPos);

			//auto capsule = std::dynamic_pointer_cast<ColliderDataCapsule>(item->m_pColliderData);


			//// プレイヤーの周囲にあるステージポリゴンを取得する
			//// ( 検出する範囲は移動距離も考慮する )
			//m_hitDim = MV1CollCheck_Sphere(m_pField->GetModelHandle(), -1, item->m_rigidbody.GetPos(), sphere->m_radius);

			////壁と床の当たり判定を行う
			//CheckWallAndFloor(item);

			////壁との当たり判定処理
			//FixPositionWithWall(item);

			////床との当たり判定処理
			//FixNowPositionWithFloor(item);
		}

		/*フィールドとの当たり判定の処理*/

		// 予定ポジション設定
		item->m_rigidbody.SetNextPos(nextPos);

		if (item->m_pColliderData->GetKind() == ColliderData::e_Kind::kSphere)
		{
			auto sphere = std::dynamic_pointer_cast<ColliderDataSphere>(item->m_pColliderData);
			m_hitDim = MV1CollCheck_Sphere(m_pField->GetModelHandle(), -1, item->m_rigidbody.GetPos(), sphere->m_radius);
		}
		else if (item->m_pColliderData->GetKind() == ColliderData::e_Kind::kCapsule)
		{
			auto capsule = std::dynamic_pointer_cast<ColliderDataCapsule>(item->m_pColliderData);
			m_hitDim = MV1CollCheck_Capsule(m_pField->GetModelHandle(), -1, item->m_rigidbody.GetPos(), capsule->m_posUp, capsule->m_radius);
		}

		//auto sphere = std::dynamic_pointer_cast<ColliderDataCapsule>(item->m_pColliderData);

		// プレイヤーの周囲にあるステージポリゴンを取得する
		// ( 検出する範囲は移動距離も考慮する )
		//m_hitDim = MV1CollCheck_Capsule(m_pField->GetModelHandle(), -1, item->m_rigidbody.GetPos(), VAdd(item->m_rigidbody.GetNextPos(), VGet(0.0f, sphere->m_radius, 0.0f)), sphere->m_radius);


		//壁と床の当たり判定を行う
		CheckWallAndFloor(item);

		//壁との当たり判定処理
		FixPositionWithWall(item);

		//床との当たり判定処理
		FixNowPositionWithFloor(item);


		// 検出したプレイヤーの周囲のポリゴン情報を開放する
		MV1CollResultPolyDimTerminate(m_hitDim);
	}

	// 当たり判定チェック（nextPos指定）
	std::vector<OnCollideInfo_> onCollideInfo = CheckColide();

	// 位置確定
	FixPosition();


	// 当たり通知
	for (auto& item : onCollideInfo)
	{
		//item.owner_->OnCollide(item.colider_);
	}
}

/// <summary>
/// 当たり判定チェック
/// </summary>
std::vector<Physics::OnCollideInfo_> Physics::CheckColide() const
{
	std::vector<OnCollideInfo_> onCollideInfo;
	// 衝突通知、ポジション補正
	bool	doCheck = true;
	int		checkCount = 0;	// チェック回数

	while (doCheck)
	{
		doCheck = false;
		++checkCount;

		// 2重ループで全オブジェクト当たり判定
		// FIXME: 重いので近いオブジェクト同士のみ当たり判定するなど工夫がいる
		for (auto& objA : m_collidables)
		{
			for (auto& objB : m_collidables)
			{
				if (objA != objB)
				{
					// ぶつかっていれば
					if (IsCollide(objA.get(), objB.get()))
					{
						auto priorityA = objA->GetPriority();
						auto priorityB = objB->GetPriority();

						Collidable* primary = objA.get();
						Collidable* secondary = objB.get();

						// どちらもトリガーでなければ次目標位置修正
						bool isTriggerAorB = objA->m_pColliderData->IsTrigger() || objB->m_pColliderData->IsTrigger();
						if (!isTriggerAorB)
						{
							// プライオリティの低いほうを移動
							if (priorityA < priorityB)
							{
								primary = objB.get();
								secondary = objA.get();
							}
							FixNextPosition(primary, secondary);
						}

						// 衝突通知情報の更新
						// HACK: primaryもsecondaryも何回も呼ばれる可能性はあるので、排他遅延処理
						bool hasPrimaryInfo = false;
						bool hasSecondaryInfo = false;
						for (const auto& item : onCollideInfo)
						{
							// 既に通知リストに含まれていたら呼ばない
							if (item.owner_ == primary)
							{
								hasPrimaryInfo = true;
							}
							if (item.owner_ == secondary)
							{
								hasSecondaryInfo = true;
							}
						}
						if (!hasPrimaryInfo)
						{
							onCollideInfo.push_back({ primary, secondary });
						}
						if (!hasSecondaryInfo)
						{
							onCollideInfo.push_back({ secondary, primary });
						}

						// 一度でもヒット+補正したら衝突判定と補正やりなおし
						if (!isTriggerAorB)	// 片方がトリガーならヒットとりなおさない
						{
							doCheck = true;
						}
						break;
					}
				}
			}
			if (doCheck)
			{
				break;
			}
		}

		// 無限ループ避け
		// FIXME: 定数化
		if (checkCount > 1000 && doCheck)
		{

#if _DEBUG
			printfDx("当たり判定の繰り返しチェックが規定数を超えた\n");
#endif

			break;
		}
	}
	return onCollideInfo;
}

bool Physics::IsCollide(const Collidable* objA, const Collidable* objB) const
{
	bool isHit = false;

	// どちらかのスルー対象としてタグが入っていたら無視
	if (objA->IsThroughTarget(objB) || objB->IsThroughTarget(objA))
	{
		return false;
	}

	// collidableの種類によって、当たり判定を分ける
	auto aKind = objA->m_pColliderData->GetKind();
	auto bKind = objB->m_pColliderData->GetKind();

	// 当たり判定を分ける
	// 円同士
	if (aKind == ColliderData::e_Kind::kSphere && bKind == ColliderData::e_Kind::kSphere)
	{
		auto atob = VSub(objB->m_rigidbody.GetNextPos(), objA->m_rigidbody.GetNextPos());
		auto atobLength = VSize(atob);

		// お互いの距離が、それぞれの半径を足したものより小さければ当たる
		auto objAColliderData = std::dynamic_pointer_cast<ColliderDataSphere>(objA->m_pColliderData);
		auto objBColliderData = std::dynamic_pointer_cast<ColliderDataSphere>(objB->m_pColliderData);
		isHit = (atobLength < objAColliderData->m_radius + objBColliderData->m_radius);
	}
	//カプセル同士
	else if (aKind == ColliderData::e_Kind::kCapsule && bKind == ColliderData::e_Kind::kCapsule)
	{
		auto atob = VSub(objB->m_rigidbody.GetNextPos(), objA->m_rigidbody.GetNextPos());
		auto atobLength = VSize(atob);

		//


	}
	//円とカプセル
	else if (aKind == ColliderData::e_Kind::kCapsule && bKind == ColliderData::e_Kind::kSphere
				||
			 aKind == ColliderData::e_Kind::kSphere && bKind == ColliderData::e_Kind::kCapsule)
	{
		auto atob = VSub(objB->m_rigidbody.GetNextPos(), objA->m_rigidbody.GetNextPos());
		auto atobLength = VSize(atob);

		//


	}



	return isHit;
}

void Physics::FixNextPosition(Collidable* primary, Collidable* secondary) const
{
	// 当たり判定の種別ごとに補正方法を変える
	auto primaryKind = primary->m_pColliderData->GetKind();
	auto secondaryKind = secondary->m_pColliderData->GetKind();

	// 球体同士の位置補正
	if (primaryKind == ColliderData::e_Kind::kSphere && secondaryKind == ColliderData::e_Kind::kSphere)
	{
		VECTOR primaryToSecondary = VSub(secondary->m_rigidbody.GetNextPos(), primary->m_rigidbody.GetNextPos());
		VECTOR primaryToSecondaryN = VNorm(primaryToSecondary);

		auto primaryColliderData = std::dynamic_pointer_cast<ColliderDataSphere>(primary->m_pColliderData);
		auto secondaryColliderData = std::dynamic_pointer_cast<ColliderDataSphere>(secondary->m_pColliderData);
		float  awayDist = primaryColliderData->m_radius + secondaryColliderData->m_radius + 0.0001f;	// そのままだとちょうど当たる位置になるので少し余分に離す

		VECTOR primaryToNewSecondaryPos = VScale(primaryToSecondaryN, awayDist);
		VECTOR fixedPos = VAdd(primary->m_rigidbody.GetNextPos(), primaryToNewSecondaryPos);
		secondary->m_rigidbody.SetNextPos(fixedPos);
	}
	// カプセルとカプセルの位置補正
	else if (primaryKind == ColliderData::e_Kind::kCapsule && secondaryKind == ColliderData::e_Kind::kCapsule)
	{

	}
	//カプセルと球体の位置補正
	else if (primaryKind == ColliderData::e_Kind::kSphere && secondaryKind == ColliderData::e_Kind::kCapsule
			||
			primaryKind == ColliderData::e_Kind::kCapsule && secondaryKind == ColliderData::e_Kind::kSphere)
	{

	}
	else
	{
		assert(0 && "許可されていない当たり判定の位置補正です");
	}
}

void Physics::FixPosition()
{
	for (auto& item : m_collidables)
	{
		/* 補正後の位置をデバッグ表示 */
#if _DEBUG

		if (item->m_pColliderData->GetKind() == ColliderData::e_Kind::kSphere)
		{
			auto itemCircleData = std::dynamic_pointer_cast<ColliderDataSphere>(item->m_pColliderData);
			DebugDraw::DrawSphere(item->m_rigidbody.GetNextPos(), itemCircleData->m_radius, kAfterFixInfoColor);
		}

		if (item->m_pColliderData->GetKind() == ColliderData::e_Kind::kCapsule)
		{
			VECTOR upPos = VGet(item->m_rigidbody.GetNextPos().x, item->m_rigidbody.GetNextPos().y + 8.0f, item->m_rigidbody.GetNextPos().z);

			auto itemCircleData = std::dynamic_pointer_cast<ColliderDataCapsule>(item->m_pColliderData);
			DebugDraw::DrawCapsule(item->m_rigidbody.GetNextPos(), upPos,itemCircleData->m_radius, kAfterFixInfoColor);
		}

#endif
		// Posを更新するので、velocityもそこに移動するvelocityに修正
		VECTOR toFixedPos = VSub(item->m_rigidbody.GetNextPos(), item->m_rigidbody.GetPos());
		item->m_rigidbody.SetVelocity(toFixedPos);

		// 位置確定
		item->m_rigidbody.SetPos(item->m_rigidbody.GetNextPos());
	}
}

void MyLib::Physics::CheckWallAndFloor(std::shared_ptr<Collidable>& col)
{

	// 壁ポリゴンと床ポリゴンの数を初期化する
	m_wallNum = 0;
	m_floorNum = 0;

	// 検出されたポリゴンの数だけ繰り返し
	for (int i = 0; i < m_hitDim.HitNum; i++)
	{
		// ポリゴンの法線のＹ成分が壁ポリゴンボーダーに達っしているかどうかで壁ポリゴンか床ポリゴンかを判断する
		if (m_hitDim.Dim[i].Normal.y < kWallPolyBorder && m_hitDim.Dim[i].Normal.y > -kWallPolyBorder)
		{
			// 壁ポリゴンと判断された場合でも、プレイヤーのＹ座標より高いポリゴンのみ当たり判定を行う
			if (m_hitDim.Dim[i].Position[0].y > col->m_rigidbody.GetPos().y + kWallPolyHeight ||
				m_hitDim.Dim[i].Position[1].y > col->m_rigidbody.GetPos().y + kWallPolyHeight ||
				m_hitDim.Dim[i].Position[2].y > col->m_rigidbody.GetPos().y + kWallPolyHeight)
			{
				// ポリゴンの数が限界数に達していなかったらポリゴンを配列に追加
				if (m_wallNum < ColInfo::kMaxColHitPolyNum)
				{
					// ポリゴンの構造体のアドレスを壁ポリゴンポインタ配列に保存する
					m_pWallPoly[m_wallNum] = &m_hitDim.Dim[i];

					// 壁ポリゴンの数を加算する
					m_wallNum++;
				}
			}
		}
		else
		{
			// ポリゴンの数が限界数に達していなかったらポリゴンを配列に追加
			if (m_floorNum < ColInfo::kMaxColHitPolyNum)
			{
				// ポリゴンの構造体のアドレスを床ポリゴンポインタ配列に保存する
				m_pFloorPoly[m_floorNum] = &m_hitDim.Dim[i];

				// 床ポリゴンの数を加算する
				m_floorNum++;
			}
		}
	}
}

void MyLib::Physics::FixPositionWithWall(std::shared_ptr<Collidable>& col)
{

	//auto sphere = std::dynamic_pointer_cast<ColliderDataCapsule>(col->m_pColliderData);
	auto sphere = std::dynamic_pointer_cast<ColliderDataSphere>(col->m_pColliderData);

	// 壁ポリゴンがない場合は何もしない
	if (m_wallNum == 0) return;

	// 壁ポリゴンとの当たり判定処理
	// 壁に当たったかどうかのフラグは初期状態では「当たっていない」にしておく
	m_isHitFlag = false;

	// 移動したかどうかで処理を分岐
	if (VSize(col->m_rigidbody.GetDir()) != 0.0f)
	{
		// 壁ポリゴンの数だけ繰り返し
		for (int i = 0; i < m_wallNum; i++)
		{
			// i番目の壁ポリゴンのアドレスを壁ポリゴンポインタ配列から取得
			m_pPoly = m_pWallPoly[i];

			// ポリゴンとプレイヤーが当たっていなかったら次のカウントへ
			// 球体のチェックにすると壁がすり抜けるようになってしまう
			if (!HitCheck_Capsule_Triangle(col->m_rigidbody.GetNextPos(),				//カプセルの下座標
				VAdd(col->m_rigidbody.GetNextPos(),VGet(0.0f, sphere->m_radius, 0.0f)),	//カプセルの上座標
				sphere->m_radius,		//半径
				m_pPoly->Position[0],	//ポリゴンの三頂点の一つ
				m_pPoly->Position[1],	//ポリゴンの三頂点の一つ
				m_pPoly->Position[2]))	//ポリゴンの三頂点の一つ
				continue;

			//if (!HitCheck_Sphere_Triangle(col->m_rigidbody.GetNextPos(),
			//	sphere->m_radius,
			//	m_pPoly->Position[0],
			//	m_pPoly->Position[1], 
			//	m_pPoly->Position[2])) 
			//	continue;

			// ここにきたらポリゴンとプレイヤーが当たっているということなので、ポリゴンに当たったフラグを立てる
			m_isHitFlag = true;

			//壁を考慮した移動を外積を使って算出
			VECTOR SlideVec;

			// 進行方向ベクトルと壁ポリゴンの法線ベクトルに垂直なベクトルを算出
			SlideVec = VCross(col->m_rigidbody.GetDir(), m_pPoly->Normal);

			// 算出したベクトルと壁ポリゴンの法線ベクトルに垂直なベクトルを算出、これが
			// 元の移動成分から壁方向の移動成分を抜いたベクトル
			SlideVec = VCross(m_pPoly->Normal, SlideVec);

			// それを移動前の座標に足したものを新たな座標とする
			col->m_rigidbody.SetNextPos(VAdd(col->m_rigidbody.GetPos(), SlideVec));


			// 新たな移動座標で壁ポリゴンと当たっていないかどうかを判定する
			bool isHitWallPolygon = false;
			for (int j = 0; j < m_wallNum; j++)
			{
				// j番目の壁ポリゴンのアドレスを壁ポリゴンポインタ配列から取得
				m_pPoly = m_pWallPoly[j];

				// 当たっていたらループから抜ける
				// 球体のチェックにすると壁がすり抜けるようになってしまう
				if (HitCheck_Capsule_Triangle(col->m_rigidbody.GetNextPos(),				//カプセルの下座標
					VAdd(col->m_rigidbody.GetNextPos(),VGet(0.0f, sphere->m_radius, 0.0f)),	//カプセルの上座標
					sphere->m_radius,		//半径
					m_pPoly->Position[0],	//ポリゴンの三頂点の一つ
					m_pPoly->Position[1],	//ポリゴンの三頂点の一つ
					m_pPoly->Position[2]))	//ポリゴンの三頂点の一つ

				//if (HitCheck_Sphere_Triangle(col->m_rigidbody.GetNextPos(),
				//	sphere->m_radius,
				//	m_pPoly->Position[0], 
				//	m_pPoly->Position[1],
				//	m_pPoly->Position[2]))
				{
					//trueにする
					isHitWallPolygon = true;
					break;
				}
			}

			// 全てのポリゴンと当たっていなかったらここでループ終了
			if (!isHitWallPolygon)
			{
				//ヒットフラグを倒す
				m_isHitFlag = false;
				break;
			}
		}
	}
	else
	{
		// 移動していない場合の処理

		// 壁ポリゴンの数だけ繰り返し
		for (int i = 0; i < m_wallNum; i++)
		{
			// i番目の壁ポリゴンのアドレスを壁ポリゴンポインタ配列から取得
			m_pPoly = m_pWallPoly[i];

			// ポリゴンに当たっていたら当たったフラグを立てた上でループから抜ける
			//if (HitCheck_Sphere_Triangle(
			//	col->m_rigidbody.GetNextPos(),		//球体の座標 
			//	sphere->m_radius,	//球体の半径
			//	m_pPoly->Position[0],	//ポリゴンの三頂点の一つ
			//	m_pPoly->Position[1],	//ポリゴンの三頂点の一つ
			//	m_pPoly->Position[2]))	//ポリゴンの三頂点の一つ
			if (HitCheck_Capsule_Triangle(col->m_rigidbody.GetNextPos(),				//カプセルの下座標
				VAdd(col->m_rigidbody.GetNextPos(), VGet(0.0f, sphere->m_radius, 0.0f)),	//カプセルの上座標
				sphere->m_radius,		//半径
				m_pPoly->Position[0],	//ポリゴンの三頂点の一つ
				m_pPoly->Position[1],	//ポリゴンの三頂点の一つ
				m_pPoly->Position[2]))	//ポリゴンの三頂点の一つ
			{
				m_isHitFlag = true;
				break;
			}
		}
	}

	// 壁に当たっていたら壁から押し出す処理を行う
	if (m_isHitFlag)
	{
		FixPositionWithWallInternal(col);
	}
}

void MyLib::Physics::FixPositionWithWallInternal(std::shared_ptr<Collidable>& col)
{
	// 壁からの押し出し処理を試みる最大数だけ繰り返し
	for (int i = 0; i < ColInfo::kMaxColHitTryNum; i++)
	{
		// 当たる可能性のある壁ポリゴンを全て見る
		bool isHitWall = false;
		// 壁ポリゴンの数だけ繰り返し
		for (int j = 0; j < m_wallNum; j++)
		{
			// i番目の壁ポリゴンのアドレスを壁ポリゴンポインタ配列から取得
			m_pPoly = m_pWallPoly[j];


			//if (!HitCheck_Sphere_Triangle(
			//	col->m_rigidbody.GetNextPos(),			//球体の座標
			//	sphere->m_radius,		//球体の半径
			//	m_pPoly->Position[0],	//ポリゴンの三頂点の一つ
			//	m_pPoly->Position[1],	//ポリゴンの三頂点の一つ
			//	m_pPoly->Position[2]))	//ポリゴンの三頂点の一つ
			//	continue;


			// プレイヤーと当たっているかを判定
			auto sphere = std::dynamic_pointer_cast<ColliderDataSphere>(col->m_pColliderData);
			//auto sphere = std::dynamic_pointer_cast<ColliderDataCapsule>(col->m_pColliderData);

			if (!HitCheck_Capsule_Triangle(col->m_rigidbody.GetNextPos(),				//カプセルの下座標
				VAdd(col->m_rigidbody.GetNextPos(), VGet(0.0f, sphere->m_radius, 0.0f)),	//カプセルの上座標
				sphere->m_radius,		//半径
				m_pPoly->Position[0],	//ポリゴンの三頂点の一つ
				m_pPoly->Position[1],	//ポリゴンの三頂点の一つ
				m_pPoly->Position[2]))	//ポリゴンの三頂点の一つ
				continue;

			// 当たっていたら規定距離分プレイヤーを壁の法線方向に移動させる
			col->m_rigidbody.SetNextPos(VAdd(col->m_rigidbody.GetNextPos(), VScale(m_pPoly->Normal, kColHitSlideLength)));

			// 移動した上で壁ポリゴンと接触しているかどうかを判定
			for (int k = 0; k < m_wallNum; k++)
			{
				// 当たっていたらループを抜ける
				m_pPoly = m_pWallPoly[k];

				//if (HitCheck_Sphere_Triangle(col->m_rigidbody.GetNextPos(),
				//	sphere->m_radius,
				//	m_pPoly->Position[0],
				//	m_pPoly->Position[1],
				//	m_pPoly->Position[2]))

				if (HitCheck_Capsule_Triangle(col->m_rigidbody.GetNextPos(),				//カプセルの下座標
					VAdd(col->m_rigidbody.GetNextPos(), VGet(0.0f, sphere->m_radius, 0.0f)),	//カプセルの上座標
					sphere->m_radius,		//半径
					m_pPoly->Position[0],	//ポリゴンの三頂点の一つ
					m_pPoly->Position[1],	//ポリゴンの三頂点の一つ
					m_pPoly->Position[2]))	//ポリゴンの三頂点の一つ
				{
					isHitWall = true;
					break;
				}
			}

			// 全てのポリゴンと当たっていなかったらここでループ終了
			if (!isHitWall) break;
		}

		//ループ終了
		if (!isHitWall) break;
	}
}

void MyLib::Physics::FixNowPositionWithFloor(std::shared_ptr<Collidable>& col)
{
	//床ポリゴンがない場合は何もしない
	if (m_floorNum == 0)
	{
		//床とぶつかっているかどうかをfalseにしておく
		col->m_pColliderData->SetIsGround(false);
		return;
	}

	// 床ポリゴンとの当たり判定処理
	// あたったかどうかのフラグ初期化
	bool IsHitFlag = false;


	//auto sphere = std::dynamic_pointer_cast<ColliderDataSphere>(col->m_pColliderData);

	//床ポリゴンとの当たり判定


   //一番高い床ポリゴンにぶつける為の判定用変数を初期化
	float PolyMaxPosY = 0.0f;

	//床ポリゴンに当たったかどうかのフラグを倒しておく
	m_isHitFlag = false;

	////床ポリゴンの数だけ繰り返し
	for (int i = 0; i < m_floorNum; i++)
	{

		//i番目の床ポリゴンのアドレスを床ポリゴンポインタ配列から取得
		m_pPoly = m_pFloorPoly[i];

		//頭から足元まででポリゴンと当たっているかを判定
		m_lineRes = HitCheck_Line_Triangle(VAdd(col->m_rigidbody.GetNextPos(),
			VGet(0.0f, kHeadHeight, 0.0f)),
			col->m_rigidbody.GetNextPos(),
			m_pPoly->Position[0],
			m_pPoly->Position[1], 
			m_pPoly->Position[2]);

		//if(!HitCheck_Sphere_Triangle(VAdd(col->m_rigidbody.GetNextPos(),
		//	VGet(0.0f, kHeadHeight, 0.0f)), 
		//	sphere->m_radius,
		//	m_pPoly->Position[0],
		//	m_pPoly->Position[1],
		//	m_pPoly->Position[2])) continue;

		//float mostHeightY = m_pPoly->Position[0].y;

		//if (mostHeightY < m_pPoly->Position[1].y)
		//{
		//	mostHeightY = m_pPoly->Position[1].y;
		//}

		//if (mostHeightY < m_pPoly->Position[2].y)
		//{
		//	mostHeightY = m_pPoly->Position[2].y;
		//}

		//当たっていなかったら何もしない
		if (!m_lineRes.HitFlag) continue;


		//既に当たったポリゴンがあり、且つ今まで検出した床ポリゴンより低い場合は何もしない
		if (m_isHitFlag && PolyMaxPosY > m_lineRes.Position.y) continue;
		//if (m_isHitFlag && PolyMaxPosY > mostHeightY) continue;

		//ポリゴンに当たったフラグを立てる
		m_isHitFlag = true;

		//接触したＹ座標を保存する
		PolyMaxPosY = m_lineRes.Position.y /*+ std::dynamic_pointer_cast<ColliderDataSphere>(col->m_pColliderData)->m_radius*/;
		//PolyMaxPosY = mostHeightY; /*+ std::dynamic_pointer_cast<ColliderDataSphere>(col->m_pColliderData)->m_radius*/;
	}

	//床ポリゴンの当たり判定かつ、ジャンプ力が0よりも小さい(下降中の場合)どうかで処理を分岐
	if (m_isHitFlag)
	{

		// 接触したポリゴンで一番高いＹ座標をプレイヤーのＹ座標にする
		col->m_rigidbody.SetNextPos(VGet(col->m_rigidbody.GetNextPos().x, PolyMaxPosY/* + sphere->m_radius*/, col->m_rigidbody.GetNextPos().z));
		if (m_pPlayer->GetIsJump())
		{

			col->m_pColliderData->SetIsGround(true);

		}
		
	}

}
