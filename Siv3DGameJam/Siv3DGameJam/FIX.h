#pragma once
#include "ISkill.h"
#include "Sequence.h"

class FIX : public ISkill
{
	String name() const override
	{
		return U"FIX.atk";

	}
	void execute() override;

private:
	void action()
	{
		Console << U"action!";
	}
	void after()
	{
		Console << U"safe";
	}
	void action2(int num)
	{
		Console << num;
	}
};
