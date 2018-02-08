//
// Created by jan on 2/8/18.
//

#include "thread_pool.h"

namespace tp {
	thread_pool::thread_pool(size_t num_threads)
			: num_threads(num_threads),
			  quit(false),
			  paused(false) {
		init_threads();
	}

	thread_pool::~thread_pool() {
		std::unique_lock lock(task_mutex);
		quit = true;
		lock.unlock();
		task_cv.notify_all();

		join_threads();

		clear_tasks();
	}

	void thread_pool::pause() {
		std::lock_guard lock(task_mutex);
		paused = true;
	}

	void thread_pool::resume() {
		std::unique_lock lock(task_mutex);
		paused = false;
		lock.unlock();

		task_cv.notify_all();
	}

	void thread_pool::cancel() {
		std::unique_lock lock(task_mutex);
		quit = true;
		lock.unlock();

		join_threads();

		clear_tasks();

		lock.lock();
		quit = false;
		lock.unlock();

		init_threads();
	}

	void thread_pool::wait() {
		std::unique_lock lock(task_mutex);

		if (tasks.empty()) {
			return;
		}

		task_cv.wait(lock, [this]() {
			return tasks.empty();
		});
	}

	void thread_pool::clear_tasks() {
		std::lock_guard lock(task_mutex);

		while (!tasks.empty()) {
			tasks.front()->cancel();
			tasks.pop();
		}
	}

	void thread_pool::init_threads() {
		for (size_t i = 0; i < num_threads; i++) {
			threads.emplace_back(std::bind(&thread_pool::task_worker, this));
		}
	}

	void thread_pool::join_threads() {
		task_cv.notify_all();

		for (auto &t : threads) {
			if (t.joinable()) {
				t.join();
			}
		}
	}

	void thread_pool::task_worker() {
		while (!quit) {
			std::unique_lock lock(task_mutex);
			task_cv.wait(lock, [this]() {
				return quit || (!tasks.empty() && !paused);
			});

			while (!quit && !tasks.empty() && !paused) {
				auto task = tasks.front();
				tasks.pop();

				lock.unlock();
				task->run();
				lock.lock();
			}


			if (tasks.empty()) {
				lock.unlock();
				task_cv.notify_all();
			}
		}
	}
}