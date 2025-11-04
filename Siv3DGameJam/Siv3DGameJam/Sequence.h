#pragma once
#include <Siv3D.hpp>
#include <functional>
#include <queue>
#include "SequenceManager.h"
using namespace std;
using namespace s3d;

class Sequence : public enable_shared_from_this<Sequence>
{
public:
	using Done = function<void()>;
	using Step = function<void(const Done&)>;
	using TickStep = function<bool(double deltaTime)>;

	using AfterEach = function<void(size_t i)>;
	using AllDone = function<void()>;

	void add(const Step& step)
	{
		steps.push(StepHolder{ step });
	}

	void addTick(const function<void()>& begin, const TickStep& update)
	{
		steps.push(StepHolder{ begin, update });
	}

	void addAction(const function<void()>& action)
	{
		add([=](const Done& done) { action(); done(); });
	}

	void addWait(double seconds)
	{
		auto acc = make_shared<double>(0.0);
		addTick
		(
			[acc]()
			{
				*acc = 0.0;
			},
			[acc, seconds](double deltaTime) -> bool
			{
				*acc += deltaTime;
				return (*acc >= seconds);
			}
		);
	}

	void addWaitForAllSequences()
	{
		addTick([]() {},
			[](double) {return SequenceManager::getInstance().activeCount() == 0; }
		);
	}

	void addWaitForAllSequencesArmed() {
		// フレームをまたいで状態を保持するため shared_ptr<bool> をキャプチャ
		auto armed = std::make_shared<bool>(false);

		addTick
		(
			// begin
			[armed]()
			{
				*armed = false;
			},
			// update
			[armed](double) -> bool
			{
					auto& manager = SequenceManager::getInstance();
					const auto n = manager.activeCount();
					if (!*armed) {
						if (n > 0) *armed = true;
						return false;
					}
					return (n == 0);
			}
		);
	}

	void onAfterEach(AfterEach callBack)
	{
		afterEach = callBack;
	}

	void onAllDone(AllDone callBack)
	{
		allDone = move(callBack);
	}

	void start()
	{
		if (isRunning) return;
		isRunning = true;
		index = 0;
		next();
	}

	// Waitを使うsequenceはこれを使う
	void startAutoShared()
	{
		if (getIsRunning()) return;
		start();
		SequenceManager::getInstance().queueAttach(shared_from_this());
	}

	void update(double deltaTime)
	{
		if (!isRunning || !activeTick) return;

		if (activeTick(deltaTime))
		{
			activeTick = nullptr;
			onStepDone();
			next();
		}
	}

	bool getIsRunning() const { return isRunning; }
	size_t getIndex() const { return index; }
	bool attached;

private:

	struct StepHolder
	{
		optional<Step> instant;
		optional<TickStep> tick;
		optional<function<void()>> begin;

		StepHolder(const Step& step) : instant(step) {}
		StepHolder(const function<void()>& begin, const TickStep& tick) : instant(nullopt), tick(tick), begin(begin) {}
	};

	void next()
	{
		if (steps.empty())
		{
			isRunning = false;
			if (allDone) allDone();
			return;
		}

		auto stepHolder = move(steps.front());
		steps.pop();

		if (stepHolder.instant)
		{
			(*stepHolder.instant)([this]()
			{
				onStepDone();
				next();
			});
		}
		else
		{
			if (stepHolder.begin) (*stepHolder.begin)();
			activeTick = *stepHolder.tick;
		}
	}

	void onStepDone()
	{
		if (afterEach) afterEach(index);
		index++;
	}

	queue<StepHolder> steps;
	TickStep activeTick = nullptr;
	bool isRunning = false;
	size_t index = 0;

	AfterEach afterEach = nullptr;
	AllDone allDone = nullptr;

	Stopwatch stopWatch { StartImmediately::No };
	double waitFor = 0;
};
