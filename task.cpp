//
// Created by jan on 2/8/18.
//

#include "task.h"

namespace tp {
	task_status task::get_status() {
		std::lock_guard lock(m);
		return status;
	}

	void task::cancel() {
		std::unique_lock lock(m);

		if (status == task_status::queued) {
			status = task_status::canceled;
		}

		lock.unlock();

		cv.notify_all();
	}

	void task::run() {
		std::unique_lock lock(m);
		status = task_status::running;
		lock.unlock();

		execute();

		lock.lock();
		status = task_status::finished;
		lock.unlock();

		cv.notify_all();
	}

	bool task::wait() {
		std::unique_lock lock(m);
		cv.wait(lock, [this]() {
			return status == task_status::canceled || status == task_status::finished;
		});

		if (status == task_status::finished) {
			return true;
		} else {
			return false;
		}
	}
}