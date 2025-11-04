#include "SequenceManager.h"
#include "Sequence.h"

void SequenceManager::queueAttach(shared_ptr<Sequence> sequence)
{
	attachQ.push_back(move(sequence));
}

void SequenceManager::updateAll(double deltaTime)
{
	flushAttach();

	for (auto& sp : active)
	{
		if (sp) sp->update(deltaTime);
	}

	flushDetach();
}

void SequenceManager::whenAllDone(std::function<void()> callBack)
{
	if (active.empty() && attachQ.empty())
	{
		if (callBack)
		{
			callBack();
			return;
		}
	}
	allDoneCallBacks << move(callBack);
}

void SequenceManager::flushAttach()
{
	if (attachQ.empty()) return;

	// まとめて move
	for (auto& queue : attachQ)
	{
		// Sequence 完了時に detach を予約（このラムダ内で active を触らない）
		if (queue) {
			queue->onAllDone([this, wp = std::weak_ptr<Sequence>(queue)]()
			{
				if (auto sp = wp.lock())
				{
					detachQ.push_back(sp.get()); // 予約のみ
				}
			});
			active.push_back(std::move(queue));
		}
	}
	attachQ.clear();
}

void SequenceManager::flushDetach()
{
	if (detachQ.empty()) return;

	// 消す対象をハッシュセット化
	s3d::HashSet<const Sequence*> toRemove(detachQ.begin(), detachQ.end());
	detachQ.clear();

	// 後ろから消す（インデックスで安全に）
	for (ptrdiff_t i = static_cast<ptrdiff_t>(active.size()) - 1; i >= 0; i--)
	{
		auto& sp = active[static_cast<size_t>(i)];
		if (!sp || toRemove.contains(sp.get()) || !sp->getIsRunning())
		{
			active.erase(active.begin() + i);
		}
	}

	if (active.empty())
	{
		auto cbs = std::move(allDoneCallBacks);
		for (auto& cb : cbs) if (cb) cb();
	}
}
