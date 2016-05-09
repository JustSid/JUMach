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


#ifndef JUMACH_JUMACHTHREADS_H
#define JUMACH_JUMACHTHREADS_H

#ifdef __cplusplus
extern "C" {
#endif

#include <mach/mach.h>
#include <pthread.h>
#include <stdbool.h>

#include "JUMachHelper.h"

thread_t jumach_thread_self();

bool jumach_fill_state(thread_t thread, thread_state_t state, thread_state_flavor_t flavor, mach_msg_type_number_t stateCount);

bool jumach_fill_thread_state(thread_t thread, _STRUCT_MCONTEXT *machineContext);
bool jumach_fill_float_state(thread_t thread, _STRUCT_MCONTEXT *machineContext);
bool jumach_fill_exception_state(thread_t thread, _STRUCT_MCONTEXT *machineContext);

uintptr_t jumach_thread_get_frame_pointer(thread_t thread);
uintptr_t jumach_context_get_frame_pointer(_STRUCT_MCONTEXT *machineContext);

uintptr_t jumach_thread_get_stack_pointer(thread_t thread);
uintptr_t jumach_context_get_stack_pointer(_STRUCT_MCONTEXT *machineContext);

uintptr_t jumach_thread_get_instruction_pointer(thread_t thread);
uintptr_t jumach_context_get_instruction_pointer(_STRUCT_MCONTEXT *machineContext);

#if defined (__arm64__) || defined(__arm__)
uintptr_t jumach_thread_get_link_register_pointer(thread_t thread);
uintptr_t jumach_context_get_link_register_pointer(_STRUCT_MCONTEXT *machineContext);
#endif

kern_return_t jumach_suspend_threads(thread_t *threads, size_t count);
kern_return_t jumach_suspend_threads_excluding(thread_t *excludedThreads, size_t count);
kern_return_t jumach_suspend_all_threads();

kern_return_t jumach_resume_threads(thread_t *threads, size_t count);
kern_return_t jumach_resume_threads_excluding(thread_t *excludedThreads, size_t count);
kern_return_t jumach_resume_all_threads();

#ifdef __cplusplus
}
#endif

#endif //JUMACH_JUMACHTHREADS_H
