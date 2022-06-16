#include "CLBuffer.hh"
#include <cstdlib>
#include <iostream>

CLBuffer::CLBuffer(const cl_context& context, cl_mem_flags flags, size_t size, void * hostPtr)
{
	int status;
	m_Buffer = clCreateBuffer(context, flags, size, hostPtr, &status);
	if (status != CL_SUCCESS)
	{
		exit(EXIT_FAILURE);
	}
}

CLBuffer::~CLBuffer()
{
	clReleaseMemObject(m_Buffer);
}

void CLBuffer::ExportToCPU(const cl_command_queue& command, void* buffer, size_t size)
{
	float* h_C = new float[size / sizeof(float)];

	int status = clEnqueueReadBuffer(command, m_Buffer, CL_TRUE, 0, size, h_C, 0, NULL, NULL);

	if (status != CL_SUCCESS)
	{
		printf("Error: Failed to read output array! %d\n", status);
		exit(1);
	}

	memcpy(buffer, h_C, size);

	delete[] h_C;
}

const cl_mem& CLBuffer::GetBuffer()
{
	return m_Buffer;
}
