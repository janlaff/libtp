//
// Created by jan on 2/8/18.
//

#ifndef LIBTP_LAMBDA_TASK_H
#define LIBTP_LAMBDA_TASK_H

#include "task.h"

namespace tp {
	class lambda_task : public task {
	public:
		explicit lambda_task(std::function<void()> fn);

		void execute() override;

	private:
		std::function<void()> fn;
	};
}

#endif //LIBTP_LAMBDA_TASK_H
