//
//  JUMachThreads.h
//  JUMach
//
//  Copyright (c) 2016 by Sidney Just
//  Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated
//  documentation files (the "Software"), to deal in the Software without restriction, including without limitation
//  the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software,
//  and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
//  The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
//  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
//  INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR
//  PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE
//  FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
//  ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
//


#include <dispatch/dispatch.h>
#include <stdio.h>

#include "JUMachThreads.h"
#include "JUMachHelper.h"

thread_t jumach_thread_self()
{
	return mach_thread_self();
}




bool jumach_fill_state(thread_t thread, thread_state_t state, thread_state_flavor_t flavor, mach_msg_type_number_t stateCount)
{
	if(thread == MACH_PORT_NULL)
		thread = jumach_thread_self();

	mach_msg_type_number_t stateCountBuff = stateCount;
	kern_return_t result = thread_get_state(thread, flavor, state, &stateCountBuff);

	if(result != KERN_SUCCESS)
	{
		JUMachError("thread_get_state: %s", mach_error_string(result));
		return false;
	}

	return true;
}





static inline bool __jumach_is_thread_in_list(thread_t thread, thread_t *threads, size_t count)
{
	for(size_t i = 0; i < count; i ++)
	{
		if(thread == threads[i])
			return true;
	}

	return false;
}

kern_return_t jumach_suspend_threads(thread_t *threads, size_t count)
{
	kern_return_t result = KERN_SUCCESS;

	for(size_t i = 0; i < count; i ++)
	{
		thread_t thread = threads[i];

		if((result = thread_suspend(thread)) != KERN_SUCCESS)
			JUMachError("thread_suspend(%08x): %s", thread, mach_error_string(result));
	}

	return result;
}

kern_return_t jumach_suspend_threads_excluding(thread_t *excludedThreads, size_t count)
{
	const task_t task = mach_task_self();
	const thread_t currentThread = jumach_thread_self();

	kern_return_t result;

	thread_act_array_t threads;
	mach_msg_type_number_t threadCount;

	if((result = task_threads(task, &threads, &threadCount)) != KERN_SUCCESS)
	{
		JUMachError("task_threads: %s", mach_error_string(result));
		return result;
	}

	for(mach_msg_type_number_t i = 0; i < threadCount; i++)
	{
		thread_t thread = threads[i];

		if(thread != currentThread && !__jumach_is_thread_in_list(thread, excludedThreads, count))
		{
			if((result = thread_suspend(thread)) != KERN_SUCCESS)
				JUMachError("thread_suspend(%08x): %s", thread, mach_error_string(result));
		}
	}

	for(mach_msg_type_number_t i = 0; i < threadCount; i++)
		mach_port_deallocate(task, threads[i]);

	vm_deallocate(task, (vm_address_t)threads, threadCount * sizeof(thread_t));
	return KERN_SUCCESS;
}
kern_return_t jumach_suspend_all_threads()
{
	return jumach_suspend_threads_excluding(NULL, 0);
}



kern_return_t jumach_resume_threads(thread_t *threads, size_t count)
{
	kern_return_t result = KERN_SUCCESS;

	for(size_t i = 0; i < count; i++)
	{
		thread_t thread = threads[i];

		if((result = thread_resume(thread)) != KERN_SUCCESS)
			JUMachError("thread_resume(%08x): %s", thread, mach_error_string(result));
	}

	return result;
}

kern_return_t jumach_resume_threads_excluding(thread_t *excludedThreads, size_t count)
{
	const task_t task = mach_task_self();
	const thread_t currentThread = jumach_thread_self();

	kern_return_t result;

	thread_act_array_t threads;
	mach_msg_type_number_t threadCount;

	if((result = task_threads(task, &threads, &threadCount)) != KERN_SUCCESS)
	{
		JUMachError("task_threads: %s", mach_error_string(result));
		return result;
	}

	for(mach_msg_type_number_t i = 0; i < threadCount; i++)
	{
		thread_t thread = threads[i];

		if(thread != currentThread && !__jumach_is_thread_in_list(thread, excludedThreads, count))
		{
			if((result = thread_resume(thread)) != KERN_SUCCESS)
				JUMachError("thread_resume(%08x): %s", thread, mach_error_string(result));
		}
	}

	for(mach_msg_type_number_t i = 0; i < threadCount; i++)
		mach_port_deallocate(task, threads[i]);

	vm_deallocate(task, (vm_address_t)threads, threadCount * sizeof(thread_t));
	return KERN_SUCCESS;
}
kern_return_t jumach_resume_all_threads()
{
	return jumach_resume_threads_excluding(NULL, 0);
}
