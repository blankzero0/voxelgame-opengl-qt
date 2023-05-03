#include "blocking_queue.h"
#include "geometry.h"
#include "chunk.h"

template<typename T>
template<typename V>
void BlockingQueue<T>::put(V&& value) requires std::same_as<std::remove_cvref_t<V>, T>
{
	{
		std::lock_guard lock(queue_mutex);
		queue.push(std::forward<V>(value));
	}
	item_count.release();
}

template<typename T>
std::optional<T> BlockingQueue<T>::take()
{
	if (!item_count.try_acquire_for(std::chrono::seconds(1)))
		return {};
	std::lock_guard lock(queue_mutex);

	T res(std::move(queue.front()));
	queue.pop();
	return res;
}

template class BlockingQueue<std::pair<ChunkPosition, std::reference_wrapper<const Chunk>>>;
template void BlockingQueue<std::pair<ChunkPosition, std::reference_wrapper<const Chunk>>>::put<std::pair<ChunkPosition, std::reference_wrapper<const Chunk>>>(std::pair<ChunkPosition, std::reference_wrapper<const Chunk>>&&);
template class BlockingQueue<ChunkPosition>;
template void BlockingQueue<ChunkPosition>::put<const ChunkPosition&>(const ChunkPosition&);
