#ifndef VOXELGAME_OPENGL_QT_TIME_BOUNDED_GL_EXECUTOR_H
#define VOXELGAME_OPENGL_QT_TIME_BOUNDED_GL_EXECUTOR_H

#include <functional>
#include "synchronized_queue.h"

class TimeBoundedGLExecutor {
public:
	static constexpr std::chrono::duration<unsigned int, std::ratio<1, 60 * 2>> max_execution_time{1}; // half a frame

	void add_task(std::function<void()>&& task);
	void run();

private:
	SynchronizedQueue<std::function<void()>> tasks;
};


#endif //VOXELGAME_OPENGL_QT_TIME_BOUNDED_GL_EXECUTOR_H
