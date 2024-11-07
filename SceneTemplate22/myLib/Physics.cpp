// 2024 Takeru Yui All Rights Reserved.
#include <cassert>
#include "DxLib.h"
#include "MyLib.h"
#include "MathHelper.h"


using namespace MyLib;

namespace
{
	constexpr int kBeforeFixInfoColor = 0x0000ff;	// 補正前情報色
	constexpr int kAimInfoColor = 0x0000aa;			// 補正前予定情報色
	constexpr int kAfterFixInfoColor = 0xff00ff;		// 補正後情報色


	//最大当たり判定ポリゴン数
	constexpr int kMaxColHitPolyNum = 2000;
	//壁押し出し処理の最大試行回数
	constexpr int kMaxColHitTryNum = 16;
	//壁押し出し時にスライドさせる距離
	constexpr float kColHitSlideLength = 1.0f;

	//壁ポリゴンか床ポリゴンかを判断するための変数
	constexpr float kWallPolyBorder = 0.4f;
	//壁ポリゴンと判断するための高さ変数
	constexpr float kWallPolyHeight = 5.0f;

	//頭の高さを設定
	constexpr float kHeadHeight = 40.0f;
}

MyLib::Physics::Physics()
{

	m_pField = std::make_shared<Field>();

}

/// <summary>
/// 衝突物の登録
/// </summary>
void Physics::Entry(Collidable* collidable)
{
	// 登録
	bool found = (std::find(m_collidables.begin(), m_collidables.end(), collidable) != m_collidables.end());
	if(!found)
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
void Physics::Exit(Collidable* collidable)
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
		assert(0 && "指定のcollidableが登録されていません。");
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

		// もともとの情報、予定情報をデバッグ表示
#if _DEBUG

		auto kind = item->m_colliderData->GetKind();

		if (kind == ColliderData::e_Kind::kSphere)
		{
			ColliderDataSphere* sphereData;
			sphereData = dynamic_cast<ColliderDataSphere*>(item->m_colliderData);
			float radius = sphereData->m_radius;
			DebugDraw::DrawSphere(pos, radius, kBeforeFixInfoColor);
			DebugDraw::DrawSphere(nextPos, radius, kAimInfoColor);

		}
		else if (kind == ColliderData::e_Kind::kCapsule)
		{
			ColliderDataCapsule* capsuleData;
			capsuleData = dynamic_cast<ColliderDataCapsule*>(item->m_colliderData);
			float radius = capsuleData->m_radius;
			VECTOR upPos = capsuleData->m_posUp;
			

			DebugDraw::DrawCapsule(pos, upPos, radius, kBeforeFixInfoColor);
			DebugDraw::DrawCapsule(nextPos, upPos, radius, kAimInfoColor);
		}
		else if (kind == ColliderData::e_Kind::kLine)
		{
			// lineはstaticなので確定色で描く
			auto lineData = dynamic_cast<ColliderDataLine*>(item->m_colliderData);
			DebugDraw::DrawLine(lineData->m_startPos, lineData->m_endPos, kAfterFixInfoColor);
		}
#endif

		// 予定ポジション設定
		item->m_nextPos = nextPos;

		auto sphere = dynamic_cast<ColliderDataSphere*>(item->m_colliderData);


		// プレイヤーの周囲にあるステージポリゴンを取得する
		// ( 検出する範囲は移動距離も考慮する )
		m_hitDim = MV1CollCheck_Sphere(m_pField->GetModelHandle(), -1, item->m_rigidbody.GetPos() , sphere->m_radius);

		//壁と床の当たり判定を行う
		CheckWallAndFloor(item);
		//壁との当たり判定処理
		FixPositionWithWall();
		//床との当たり判定処理
		FixNowPositionWithFloor();
	}

	// 当たり判定チェック（nextPos指定）
	std::vector<OnCollideInfo> onCollideInfo = CheckColide();



	// 位置確定
	FixPosition();

	// 当たり通知
	for (auto& item : onCollideInfo)
	{
		item.owner_->OnCollide(*item.colider_);
	}
}

/// <summary>
/// 当たり判定チェック
/// </summary>
std::vector<Physics::OnCollideInfo> Physics::CheckColide() const
{
	std::vector<OnCollideInfo> onCollideInfo;
	// 衝突通知、ポジション補正
	bool	doCheck		= true;
	int		checkCount	= 0;	// チェック回数
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
					if (IsCollide(objA, objB))
					{
						auto priorityA = objA->GetPriority();
						auto priorityB = objB->GetPriority();

						Collidable* primary = objA;
						Collidable* secondary = objB;

						// どちらもトリガーでなければ次目標位置修正
						bool isTriggerAorB = objA->m_colliderData->IsTrigger() || objB->m_colliderData->IsTrigger();
						if (!isTriggerAorB)
						{
							// プライオリティの低いほうを移動
							if (priorityA < priorityB)
							{
								primary = objB;
								secondary = objA;
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


/// <summary>
/// 指定ラインがオブジェクトとぶつかっているかどうか判定し、ぶつかっているオブジェクトを返す
/// </summary>
std::list<Collidable*> Physics::IsCollideLine(const VECTOR& start, const VECTOR& end) const
{
	std::list<Collidable*> ret;

	for (auto& obj : m_collidables)
	{
		// collidableの種類によって、当たり判定を分ける
		auto kind = obj->m_colliderData->GetKind();
		// ラインとライン
		if (kind == ColliderData::e_Kind::kLine)
		{
			auto lineColliderData = dynamic_cast<ColliderDataLine*>(obj->m_colliderData);
			// 2つのライン同士の最短距離が0だと当たっている
			float minLength = Segment_Segment_MinLength(start, end, lineColliderData->m_startPos, lineColliderData->m_endPos);
			if (minLength < 0.001f)	// float誤差を許容
			{
 				ret.emplace_back(obj);
			}
		}
		// ラインと球体
		else if (kind == ColliderData::e_Kind::kSphere)
		{
			auto sphereColliderData = dynamic_cast<ColliderDataSphere*>(obj->m_colliderData);
			bool isHit = (Segment_Point_MinLength(start, end, obj->m_rigidbody.GetPos()) < sphereColliderData->m_radius);
			if (isHit)
			{
				ret.emplace_back(obj);
			}
		}
		//ラインとカプセル
		else if (kind == ColliderData::e_Kind::kCapsule)
		{

		}

	}
	return ret;
}

/// <summary>
/// 当たっているかどうかだけ判定
/// </summary>
bool Physics::IsCollide(const Collidable* objA, const Collidable* objB) const
{
	bool isHit = false;

	// どちらかのスルー対象としてタグが入っていたら無視
	if (objA->IsThroughTarget(objB) || objB->IsThroughTarget(objA))
	{
		return false;
	}

	// collidableの種類によって、当たり判定を分ける
	auto aKind = objA->m_colliderData->GetKind();
	auto bKind = objB->m_colliderData->GetKind();

	// 当たり判定を分ける
	// 円同士
	if (aKind == ColliderData::e_Kind::kSphere && bKind == ColliderData::e_Kind::kSphere)
	{
		auto atob = VSub(objB->m_nextPos, objA->m_nextPos);
		auto atobLength = VSize(atob);

		// お互いの距離が、それぞれの半径を足したものより小さければ当たる
		auto objAColliderData = dynamic_cast<ColliderDataSphere*>(objA->m_colliderData);
		auto objBColliderData = dynamic_cast<ColliderDataSphere*>(objB->m_colliderData);
		isHit = (atobLength < objAColliderData->m_radius + objBColliderData->m_radius);
	}
	//カプセル同士
	else if (aKind == ColliderData::e_Kind::kCapsule && bKind == ColliderData::e_Kind::kCapsule)
	{

	}
	//円とカプセル
	else if (aKind == ColliderData::e_Kind::kCapsule && bKind == ColliderData::e_Kind::kSphere
				||
			 aKind == ColliderData::e_Kind::kSphere && bKind == ColliderData::e_Kind::kCapsule)
	{

	}
	
	

	//// 円とライン
	//else if (
	//	(aKind == ColliderData::Kind::Line2D && bKind == ColliderData::Kind::Circle3D)
	//	 ||
	//	(aKind == ColliderData::Kind::Circle3D && bKind == ColliderData::Kind::Line2D)
	//	)
	//{
	//	ColliderData* lineDataBase = objA->colliderData;
	//	ColliderData* circleDataBase = objB->colliderData;
	//	VECTOR circleCenter = objB->nextPos;
	//	if (bKind == ColliderData::Kind::Line2D)
	//	{
	//		lineDataBase = objB->colliderData;
	//		circleDataBase = objA->colliderData;
	//		circleCenter = objA->nextPos;
	//	}
	//	auto lineColliderData = dynamic_cast<ColliderDataLine2D*>(lineDataBase);
	//	auto circleColliderData = dynamic_cast<ColliderDataCircle3D*>(circleDataBase);
	//	isHit = (Segment_Point_MinLength(lineColliderData->startPoint, lineColliderData->endPoint, circleCenter) < circleColliderData->radius);
	//}
	//// 円と一方通行ライン
	//else if (
	//	(aKind == ColliderData::Kind::OneWayLine2D && bKind == ColliderData::Kind::Circle3D)
	//		||
	//	(aKind == ColliderData::Kind::Circle3D && bKind == ColliderData::Kind::OneWayLine2D)
	//	)
	//{
	//	ColliderData* lineDataBase = objA->colliderData;
	//	ColliderData* circleDataBase = objB->colliderData;
	//	VECTOR circleCenter = objB->nextPos;
	//	VECTOR circlePrevCenter = objB->rigidbody.GetPos();
	//	if (bKind == ColliderData::Kind::OneWayLine2D)
	//	{
	//		lineDataBase = objB->colliderData;
	//		circleDataBase = objA->colliderData;
	//		circleCenter = objA->nextPos;
	//		circlePrevCenter = objA->rigidbody.GetPos();
	//	}
	//	auto lineColliderData = dynamic_cast<ColliderDataOneWayLine2D*>(lineDataBase);
	//	auto circleColliderData = dynamic_cast<ColliderDataCircle3D*>(circleDataBase);

	//	// 一方通行ラインの場合は、円の移動前のポジション→次のポジションのベクトルが
	//	// 一方通行ラインの通行可能方向かどうかの判定が必要
	//	// 通行不可能な方向なら通常のライン当たり判定を行う
	//	VECTOR prevToNext = VSub(circleCenter, circlePrevCenter);
	//	VECTOR startToEnd = VSub(lineColliderData->endPoint, lineColliderData->startPoint);

	//	// 「円の移動前のポジション→次のポジション」ベクトルと、「通っていい方向」の内積をとることで、
	//	// それぞれのベクトルの交差方向を見る
	//	VECTOR throughWay;	// 通っていい方向
	//	{
	//		// 法線方向が始点→終点ラインの上方向なフラグが立っている
	//		if (lineColliderData->isStartToEndUp)
	//		{
	//			// 通っていい方向は始点→終点ラインとZ=1.0fなライン始点との外積
	//			throughWay = VCross(startToEnd, VGet(startToEnd.x, startToEnd.y, 1.0f));
	//		}
	//		// 法線方向が始点→終点ラインの下方向なフラグが立っている
	//		else
	//		{
	//			// 通っていい方向は始点→終点ラインとZ=-1.0fなライン始点との外積
	//			throughWay = VCross(startToEnd, VGet(startToEnd.x, startToEnd.y, -1.0f));
	//		}
	//	}
	//	// 「円の移動前のポジション→次のポジション」ベクトルと、「通っていい方向」の内積がプラスなら当たる
	//	if (VDot(prevToNext, throughWay) > 0)
	//	{
	//		auto minLength = Segment_Point_MinLength(lineColliderData->startPoint, lineColliderData->endPoint, circleCenter);
	//		isHit = (minLength < circleColliderData->radius);

	//		// 例外処理：移動前に既に当たっている場合
	//		//（移動前のポジションと線分の距離が円の半径より小さい場合）は無視
	//		auto prevMinLength = Segment_Point_MinLength(lineColliderData->startPoint, lineColliderData->endPoint, circlePrevCenter);
	//		if (isHit && (prevMinLength < circleColliderData->radius))
	//		{
	//			isHit = false;
	//		}
	//	}
	//}

	return isHit;
}

/// <summary>
/// 次位置補正
/// </summary>
void Physics::FixNextPosition(Collidable* primary, Collidable* secondary) const
{
	// 当たり判定の種別ごとに補正方法を変える
	auto primaryKind = primary->m_colliderData->GetKind();
	auto secondaryKind = secondary->m_colliderData->GetKind();

	// 円同士の位置補正
	if (primaryKind == ColliderData::e_Kind::kSphere && secondaryKind == ColliderData::e_Kind::kSphere)
	{
		VECTOR primaryToSecondary = VSub(secondary->m_nextPos, primary->m_nextPos);
		VECTOR primaryToSecondaryN = VNorm(primaryToSecondary);

		auto primaryColliderData = dynamic_cast<ColliderDataSphere*>(primary->m_colliderData);
		auto secondaryColliderData = dynamic_cast<ColliderDataSphere*>(secondary->m_colliderData);
		float  awayDist = primaryColliderData->m_radius + secondaryColliderData->m_radius + 0.0001f;	// そのままだとちょうど当たる位置になるので少し余分に離す

		VECTOR primaryToNewSecondaryPos = VScale(primaryToSecondaryN, awayDist);
		VECTOR fixedPos = VAdd(primary->m_nextPos, primaryToNewSecondaryPos);
		secondary->m_nextPos = fixedPos;
	}
	// ラインと円の位置補正
	// HACK:現状lineは必ずstaticなのでprimaryにline,secondaryにcircleがくる。そうでないときはエラーを吐く
	//else if (
	//	(primaryKind == ColliderData::e_Kind::Line2D || primaryKind == ColliderData::e_Kind::OneWayLine2D)
	//	&& secondaryKind == ColliderData::e_Kind::Circle3D)
	{
		//auto lineData = dynamic_cast<ColliderDataLine2D*>(primary->colliderData);
		//auto circleData = dynamic_cast<ColliderDataCircle3D*>(secondary->colliderData);
		//// secondaryが移動する円として処理

		//// 1.過去の位置→未来の位置の線分と、壁線分が交わるかどうかを調べる
		//float bothSegmentLength = (
		//	Segment_Segment_MinLength(secondary->nextPos, secondary->rigidbody.GetPos(),
		//		lineData->startPoint, lineData->endPoint)
		//	);
		//bool isCross = (bothSegmentLength == 0);	// ２つの線分の最近接点までの距離が0なら交差している

		//// 後の計算で結局「壁線分上の、未来の位置（ベクトルの終端の点）との最近接点」が必要になるので
		//// 先に出しておく
		//VECTOR closestPtOnWallAndNextPos 
		//	= GetClosestPtOnSegment(secondary->nextPos, lineData->startPoint, lineData->endPoint);

		//// 2.交わる場合
		//if (isCross)
		//{
		//	// 未来の位置の点から壁線分に対しておろした垂線との交点を調べる
		//	// →垂線の交点＝「壁線分上の、未来の位置（ベクトルの終端の点）との最近接点」
		//	// 最終的な位置は、垂線の交点から「未来の位置→垂線との交点 ベクトル方向」に円の半径分動かした位置
		//	VECTOR nextToClosest = VSub(closestPtOnWallAndNextPos, secondary->nextPos);
		//	VECTOR nextToClosestN = VNorm(nextToClosest);
		//	VECTOR fixedPos = VAdd(closestPtOnWallAndNextPos, VScale(nextToClosestN, circleData->radius + 0.0001f));
		//	secondary->nextPos = fixedPos;
		//}
		//// 3.交わらない場合
		//else
		//{
		//	// 過去の位置→未来の位置の線分と、壁線分の最近接点を調べる
		//	// その最近接点から壁線分に対しておろした垂線との交点を調べる
		//	// →垂線の交点＝「壁線分上の、未来の位置（ベクトルの終端の点）との最近接点」
		//	// 最終的な位置は、垂線の交点から「垂線の交点→未来の位置 ベクトル方向」に円の半径分動かした位置
		//	VECTOR closestToNext = VSub(secondary->nextPos, closestPtOnWallAndNextPos);
		//	VECTOR closestToNextN = VNorm(closestToNext);
		//	VECTOR fixedPos = VAdd(closestPtOnWallAndNextPos, VScale(closestToNextN, circleData->radius + 0.0001f));
		//	secondary->nextPos = fixedPos;
		//}
	}
	//else
	//{
	//	assert(0 && "許可されていない当たり判定の位置補正です");
	//}
}

/// <summary>
/// 位置確定
/// </summary>
void Physics::FixPosition()
{
	for (auto& item : m_collidables)
	{
#if _DEBUG
		// 補正後の位置をデバッグ表示
		//DebugDraw::DrawLine(item->rigidbody.GetPos(), item->nextPos, AfterFixInfoColor);

		if (item->m_colliderData->GetKind() == ColliderData::e_Kind::kSphere)
		{
			auto itemCircleData = dynamic_cast<ColliderDataSphere*>(item->m_colliderData);
			DebugDraw::DrawSphere(item->m_nextPos, itemCircleData->m_radius, kAfterFixInfoColor);
		}


#endif
		// Posを更新するので、velocityもそこに移動するvelocityに修正
		VECTOR toFixedPos = VSub(item->m_nextPos, item->m_rigidbody.GetPos());
		item->m_rigidbody.SetVelocity(toFixedPos);

		// 位置確定
		item->m_rigidbody.SetPos(item->m_nextPos);
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

	auto sphere = dynamic_cast<ColliderDataSphere*>(col->m_colliderData);

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
			if (!HitCheck_Capsule_Triangle(col->m_nextPos, VAdd(col->m_nextPos,
				VGet(0.0f, sphere->m_radius, 0.0f)),
				sphere->m_radius,
				m_pPoly->Position[0], m_pPoly->Position[1], m_pPoly->Position[2])) continue;

			// ここにきたらポリゴンとプレイヤーが当たっているということなので、ポリゴンに当たったフラグを立てる
			m_isHitFlag = true;

			//壁を考慮した移動を外積を使って算出
			VECTOR SlideVec;

			// 進行方向ベクトルと壁ポリゴンの法線ベクトルに垂直なベクトルを算出
			SlideVec = VCross(col->m_rigidbody.GetDir() , m_pPoly->Normal);

			// 算出したベクトルと壁ポリゴンの法線ベクトルに垂直なベクトルを算出、これが
			// 元の移動成分から壁方向の移動成分を抜いたベクトル
			SlideVec = VCross(m_pPoly->Normal, SlideVec);

			// それを移動前の座標に足したものを新たな座標とする
			col->m_nextPos = VAdd(col->m_rigidbody.GetPos(), SlideVec);


			// 新たな移動座標で壁ポリゴンと当たっていないかどうかを判定する
			bool isHitWallPolygon = false;
			for (int j = 0; j < m_wallNum; j++)
			{
				// j番目の壁ポリゴンのアドレスを壁ポリゴンポインタ配列から取得
				m_pPoly = m_pWallPoly[j];

				// 当たっていたらループから抜ける
				if (HitCheck_Capsule_Triangle(col->m_nextPos, VAdd(col->m_nextPos,
					VGet(0.0f, sphere->m_radius, 0.0f)),
					sphere->m_radius,
					m_pPoly->Position[0], m_pPoly->Position[1], m_pPoly->Position[2]))
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
			if (HitCheck_Sphere_Triangle(
				col->m_nextPos,		//球体の座標 
				sphere->m_radius,	//球体の半径
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

			// プレイヤーと当たっているかを判定
			auto sphere = dynamic_cast<ColliderDataSphere*>(col->m_colliderData);
			
			if (!HitCheck_Sphere_Triangle(
				col->m_nextPos,			//球体の座標
				sphere->m_radius,		//球体の半径
				m_pPoly->Position[0],	//ポリゴンの三頂点の一つ
				m_pPoly->Position[1],	//ポリゴンの三頂点の一つ
				m_pPoly->Position[2]))	//ポリゴンの三頂点の一つ
				continue;

			// 当たっていたら規定距離分プレイヤーを壁の法線方向に移動させる
			col->m_nextPos = VAdd(col->m_nextPos, VScale(m_pPoly->Normal, kColHitSlideLength));

			// 移動した上で壁ポリゴンと接触しているかどうかを判定
			for (int k = 0; k < m_wallNum; k++)
			{
				// 当たっていたらループを抜ける
				m_pPoly = m_pWallPoly[k];
				if (HitCheck_Sphere_Triangle(col->m_nextPos,
					sphere->m_radius,
					m_pPoly->Position[0], m_pPoly->Position[1], m_pPoly->Position[2]))
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
	if (m_floorNum == 0) return;

	// 床ポリゴンとの当たり判定処理
	//あたったかどうかのフラグ初期化
	bool IsHitFlag = false;

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
		m_lineRes = HitCheck_Line_Triangle(VAdd(col->m_nextPos, VGet(0.0f, kHeadHeight, 0.0f)),
			col->m_nextPos, m_pPoly->Position[0], m_pPoly->Position[1], m_pPoly->Position[2]);

		//当たっていなかったら何もしない
		if (!m_lineRes.HitFlag) continue;

		//既に当たったポリゴンがあり、且つ今まで検出した床ポリゴンより低い場合は何もしない
		if (m_isHitFlag && PolyMaxPosY > m_lineRes.Position.y) continue;

		//ポリゴンに当たったフラグを立てる
		m_isHitFlag = true;

		//接触したＹ座標を保存する
		PolyMaxPosY = m_lineRes.Position.y;
	}

	//床ポリゴンの当たり判定かつ、ジャンプ力が0よりも小さい(下降中の場合)どうかで処理を分岐
	if (m_isHitFlag)
	{
		// 接触したポリゴンで一番高いＹ座標をプレイヤーのＹ座標にする
		col->m_nextPos.y = PolyMaxPosY;
		//col->m_rigidbody.SetVelocity(col->m_nextPos);

	}
	
}
