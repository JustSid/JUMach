//
//  JUMachAarch64.c
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


#if defined (__arm64__)

#include "JUMachHelper.h"
#include "JUMachThreads.h"

bool jumach_fill_thread_state(thread_t thread, _STRUCT_MCONTEXT *machineContext)
{
	return jumach_fill_state(thread, (thread_state_t)&machineContext->__ss, ARM_THREAD_STATE64, ARM_THREAD_STATE64_COUNT);
}

bool jumach_fill_float_state(thread_t thread, _STRUCT_MCONTEXT *machineContext)
{
	return jumach_fill_state(thread, (thread_state_t)&machineContext->__ns, ARM_VFP_STATE, ARM_VFP_STATE_COUNT);
}

bool jumach_fill_exception_state(thread_t thread, _STRUCT_MCONTEXT *machineContext)
{
	return jumach_fill_state(thread, (thread_state_t)&machineContext->__es, ARM_EXCEPTION_STATE64, ARM_EXCEPTION_STATE64_COUNT);
}


uintptr_t jumach_context_get_frame_pointer(_STRUCT_MCONTEXT *context)
{
	return context->__ss.__fp;
}

uintptr_t jumach_context_get_stack_pointer(_STRUCT_MCONTEXT *context)
{
	return context->__ss.__sp;
}

uintptr_t jumach_context_get_instruction_pointer(_STRUCT_MCONTEXT *context)
{
	return context->__ss.__pc;
}

uintptr_t jumach_context_get_link_register_pointer(_STRUCT_MCONTEXT *context)
{
	return context->__ss.__lr;
}

uintptr_t jumach_thread_get_frame_pointer(thread_t thread)
{
	_STRUCT_MCONTEXT context;
	jumach_fill_thread_state(thread, &context);

	return context.__ss.__fp;
}
uintptr_t jumach_thread_get_stack_pointer(thread_t thread)
{
	_STRUCT_MCONTEXT context;
	jumach_fill_thread_state(thread, &context);

	return context.__ss.__sp;
}
uintptr_t jumach_thread_get_instruction_pointer(thread_t thread)
{
	_STRUCT_MCONTEXT context;
	jumach_fill_thread_state(thread, &context);

	return context.__ss.__pc;
}

uintptr_t jumach_thread_get_link_register_pointer(thread_t thread)
{
	_STRUCT_MCONTEXT context;
	jumach_fill_thread_state(thread, &context);

	return context.__ss.__lr;
}

#endif

void __aarch64_dummy()
{}
