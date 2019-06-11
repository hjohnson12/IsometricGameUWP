#pragma once

#include <ppl.h>
#include <ppltasks.h>
#include <wrl\wrappers\corewrappers.h>

using namespace concurrency;
using namespace Windows::ApplicationModel::Core;
using namespace Windows::Foundation::Numerics;
using namespace Windows::Foundation;
using namespace Windows::UI::Core;

namespace GameEngine2 {
	namespace Utilities {

		template<typename T>
		inline T WaitExecution(IAsyncOperation<T>^ asyncOperation)
		{
			using namespace Microsoft::WRL::Wrappers;

			Event emptyEvent(CreateEventEx(NULL, NULL, CREATE_EVENT_MANUAL_RESET, EVENT_ALL_ACCESS));
			if (!emptyEvent.IsValid())
				throw std::bad_alloc();

			task_options options;
			options.set_continuation_context(task_continuation_context::use_arbitrary());

			task<T> asyncTask(asyncOperation);

			asyncTask.then([&](task<T>)
			{
				SetEvent(emptyEvent.Get());
			}, options);

			// waiting before event executed
			auto timeout = 1000 * 5;
			auto waitResult = WaitForSingleObjectEx(emptyEvent.Get(), timeout, true);

			return asyncTask.get();
		};

		inline void WaitExecution(IAsyncAction^ ayncAction)
		{
			using namespace Microsoft::WRL::Wrappers;

			Event emptyEvent(CreateEventEx(NULL, NULL, CREATE_EVENT_MANUAL_RESET, EVENT_ALL_ACCESS));
			if (!emptyEvent.IsValid())
				throw std::bad_alloc();

			task_options options;
			options.set_continuation_context(task_continuation_context::use_arbitrary());

			task<void> asyncTask(ayncAction);

			asyncTask.then([&](task<void>)
			{
				SetEvent(emptyEvent.Get());
			}, options);

			// waiting before event executed
			auto timeout = 1000 * 5;
			auto waitResult = WaitForSingleObjectEx(emptyEvent.Get(), timeout, true);

			asyncTask.get();
		};

	}
}