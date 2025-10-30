#pragma once
#include <Siv3D.hpp>

struct HandOverData
{
	bool gameOverFlag = false;
};

// これを全シーンで使う
using App = s3d::SceneManager<s3d::String, HandOverData>;
