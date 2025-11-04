#pragma once
#include "ISkill.h"
using namespace s3d;

class BruteForce : public ISkill
{
	String name() const override
	{
		return U"BruteForce.atk";

	}
	void execute() override;
private:
	void action()
	{
		Console << U"brute!!";
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

