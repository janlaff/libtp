//
// Created by jan on 2/8/18.
//

#include "lambda_task.h"

namespace tp {
	lambda_task::lambda_task(std::function<void()> fn)
			: fn(std::move(fn)) {}

	void lambda_task::execute() {
		fn();
	}
}