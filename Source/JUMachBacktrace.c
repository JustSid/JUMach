//
//  JUMachBacktrace.c
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


#include <stdlib.h>
#include "JUMachBacktrace.h"
#include "JUMachThreads.h"
#include "JUMachMemory.h"

#if defined(__arm__)
#define _REMOVE_TAG(A) ((A) & ~(1UL))
#elif defined(__arm64__)
#define _REMOVE_TAG(A) ((A) & ~(3UL))
#else
#define _REMOVE_TAG(A) (A)
#endif

typedef struct frame_s
{
	const struct frame_s *const previous;
	const uintptr_t return_address;
} frame_t;

size_t jumach_backtrace_context(_STRUCT_MCONTEXT *machineContext, uintptr_t *buffer, size_t skipEntries, size_t maxEntries)
{
	if(maxEntries == 0)
		return 0;

	size_t i = 0;

	if(skipEntries == 0)
	{
		buffer[i] = jumach_context_get_instruction_pointer(machineContext);

		if((++i) == maxEntries)
			return i;
	}

#if defined (__arm64__) || defined(__arm__)
	if(skipEntries <= 1)
	{
		buffer[i] = jumach_context_get_link_register_pointer(machineContext);

		if((++ i) == maxEntries)
			return i;
	}
#endif

	frame_t frame = {0};

	const uintptr_t framePtr = jumach_context_get_frame_pointer(machineContext);
	if(framePtr == 0 || jumach_memcpy(&frame, (void *)framePtr, sizeof(frame)) != KERN_SUCCESS)
		return 0;


	for(size_t j = 1; j < skipEntries; j++)
	{
		if(frame.previous == 0 || jumach_memcpy(&frame, frame.previous, sizeof(frame)) != KERN_SUCCESS)
			return 0;
	}

	for(; i < maxEntries; i++)
	{
		buffer[i] = frame.return_address;

		if(buffer[i] == 0 || frame.previous == 0 || jumach_memcpy(&frame, frame.previous, sizeof(frame)) != KERN_SUCCESS)
			break;
	}

	return i;
}

size_t jumach_backtrace(thread_t thread, uintptr_t *buffer, size_t maxEntries)
{
	_STRUCT_MCONTEXT context;

	if(!jumach_fill_thread_state(thread, &context))
		return 0;

	return jumach_backtrace_context(&context, buffer, 0, maxEntries);
}
