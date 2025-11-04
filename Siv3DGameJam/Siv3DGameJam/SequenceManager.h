#pragma once
#include <Siv3D.hpp>
class Sequence;
using namespace std;
using namespace s3d;

class SequenceManager
{
public:
	static SequenceManager& getInstance()
	{
		static SequenceManager instance;
		return instance;
	}

	void queueAttach(shared_ptr<Sequence> sequence);

	void updateAll(double deltaTime); // 毎フレ1回だけ呼ぶ

	void whenAllDone(std::function<void()> callBack);

	size_t activeCount() const { return active.size(); }

private:
	// attachキューを反映
	void flushAttach();
	// detachキューを反映
	void flushDetach();

	// 実行中
	Array<shared_ptr<Sequence>> active;
	// 追加待ち
	Array<shared_ptr<Sequence>> attachQ;
	// 削除待ち
	Array<const Sequence*> detachQ;

	Array<function<void()>> allDoneCallBacks;
};
