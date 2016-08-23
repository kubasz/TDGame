#pragma once

struct CreepBuff
{
	enum class Type : unsigned int
	{
		//! Speed or slowness
		BUFF_SPEED,
		//! Armor or weakness
		BUFF_VULNERABILITY
	};
	double duration;
	Type type;
	float strength;

	CreepBuff(double duration, Type type, float strength)
			: duration(duration)
			, type(type)
			, strength(strength)
	{}
};
