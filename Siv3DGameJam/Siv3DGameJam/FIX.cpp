#include "FIX.h"
#include "Sequence.h"
#include "GameData.h"
using namespace s3d;

void FIX::execute()
{
	auto sequence = std::make_shared<Sequence>();
	sequence->addAction([this]()
	{
		action();
	});
	sequence->onAfterEach([this](size_t i)
	{
		after();
	});
	sequence->addWait(2.0);
	sequence->addAction([this, number = int{ 2 }]()
	{
		action2(number);
	});

	sequence->startAutoShared();
}

void FIX::action(){
	GameData::getInstance().applyBossDamage(1000000);
}
