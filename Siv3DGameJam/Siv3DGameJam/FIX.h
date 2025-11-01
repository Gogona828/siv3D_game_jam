#pragma once
#include "ISkill.h"

class FIX : public ISkill
{
	s3d::String name() const override { return U"FIX.atk"; }
	void execute() override {
		Console << U"execute!!!";
	}
};

