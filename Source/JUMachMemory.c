//
//  JUMachMemory.c
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

#include "JUMachMemory.h"

kern_return_t jumach_memcpy(void *dst, const void *src, size_t numBytes)
{
	vm_size_t bytesCopied = 0;
	return vm_read_overwrite(mach_task_self(), (vm_address_t)src, (vm_size_t)numBytes, (vm_address_t)dst, &bytesCopied);
}
size_t jumach_memlcpy(void *dst, const void *src, size_t numBytes)
{
	const uint8_t *pSrc = src;
	const uint8_t *pSrcMax = (uint8_t *)src + numBytes;
	const uint8_t *pSrcEnd = (uint8_t *)src + numBytes;
	uint8_t *pDst = dst;

	size_t bytesCopied = 0;

	if(jumach_memcpy(dst, src, 1) != KERN_SUCCESS)
		return 0;

	if(numBytes <= 1)
		return numBytes;

	while(1)
	{
		ssize_t copyLength = pSrcEnd - pSrc;
		if(copyLength <= 0)
			break;

		if(jumach_memcpy(pDst, pSrc, (size_t)copyLength) == KERN_SUCCESS)
		{
			bytesCopied += (size_t)copyLength;

			pSrc += copyLength;
			pDst += copyLength;
			pSrcEnd = pSrc + (pSrcMax - pSrc) / 2;
		}
		else
		{
			if(copyLength <= 1)
				break;

			pSrcMax = pSrcEnd;
			pSrcEnd = pSrc + copyLength / 2;
		}
	}
	return bytesCopied;
}
