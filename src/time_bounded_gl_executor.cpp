#include "time_bounded_gl_executor.h"

void TimeBoundedGLExecutor::add_task(std::function<void()>&& task)
{
	tasks.put(std::move(task));
}

void TimeBoundedGLExecutor::run()
{
	auto start = std::chrono::steady_clock::now();
	while ((std::chrono::steady_clock::now() - start) < max_execution_time) {
		std::optional task_opt = tasks.take();
		if (!task_opt)
			break;

		std::function task = *task_opt;
		task();
	}
}
