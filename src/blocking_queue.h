#ifndef VOXELGAME_OPENGL_QT_BLOCKING_QUEUE_H
#define VOXELGAME_OPENGL_QT_BLOCKING_QUEUE_H


#include <optional>
#include <queue>
#include <mutex>
#include <semaphore>

template<typename T>
class BlockingQueue {
public:
	std::optional<T> take();

	template<typename V>
	void put(V&& value) requires std::same_as<std::remove_cvref_t<V>, T>;

private:
	std::queue<T> queue;
	std::mutex queue_mutex;
	std::counting_semaphore<> item_count{0};
};

#endif //VOXELGAME_OPENGL_QT_BLOCKING_QUEUE_H
