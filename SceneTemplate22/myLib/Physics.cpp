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

}

/// <summary>
/// 衝突物の登録
/// </summary>
void Physics::Entry(Collidable* collidable)
{
	// 登録
	bool found = (std::find(collidables.begin(), collidables.end(), collidable) != collidables.end());
	if(!found)
	{
		collidables.emplace_back(collidable);
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
	bool found = (std::find(collidables.begin(), collidables.end(), collidable) != collidables.end());
	if (found)
	{
		collidables.remove(collidable);
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
	for (auto& item : collidables)
	{
		// ポジションに移動力を足す
		VECTOR pos = item->m_rigidbody.GetPos();
		VECTOR velocity = item->m_rigidbody.GetVelocity();

		// 重力を利用する設定なら、重力を追加する
		if (item->m_rigidbody.UseGravity())
		{
			velocity = VAdd(velocity, VGet(0, m_kGravity, 0));

			// 最大重力加速度より大きかったらクランプ
			if (velocity.y < m_kMaxGravityAccel)
			{
				velocity = VGet(velocity.x, m_kMaxGravityAccel, velocity.z);
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
		for (auto& objA : collidables)
		{
			for (auto& objB : collidables)
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
	for (auto& obj : collidables)
	{
		// collidableの種類によって、当たり判定を分ける
		auto kind = obj->m_colliderData->GetKind();
		// ラインとラインor一方通行ライン
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
		// ラインと円
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
	for (auto& item : collidables)
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