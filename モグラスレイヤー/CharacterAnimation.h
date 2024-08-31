#pragma once

enum class PlayerAnimation : int
{
	kNone = -1,
	kIdle = 0,
	kWalk,
	kDash,
	kAttack,
	kJump,
};

enum class EnemyAnimation : int
{
	kNone = -1,
	kIdle = 0,
	kWalk,
	kAttack,
	kChargeAttack,
	kGard,
};

enum class GimickAnimation : int
{
	kNone = -1,
	kIdle = 0,
	kWalk,
	kDash,
	kAttack,
	kJump,
};