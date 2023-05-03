#ifndef VOXELGAME_OPENGL_QT_SYNCHRONIZED_QUEUE_H
#define VOXELGAME_OPENGL_QT_SYNCHRONIZED_QUEUE_H


#include <optional>
#include <queue>
#include <mutex>

template<typename T>
class SynchronizedQueue {
public:
	std::optional<T> take()
	{
		std::lock_guard lock(queue_mutex);
		if (queue.empty())
			return {};

		std::optional res(std::move(queue.front()));
		queue.pop();
		return res;
	}

	void put(T&& value)
	{
		std::lock_guard lock(queue_mutex);
		queue.push(std::move(value));
	}

private:
	std::queue<T> queue;
	std::mutex queue_mutex;
};


#endif //VOXELGAME_OPENGL_QT_SYNCHRONIZED_QUEUE_H
