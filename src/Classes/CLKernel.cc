#include "CLKernel.hh"
#include <iostream>

CLKernel::CLKernel(std::string_view kernelSource, const cl_context& context, const cl_device_id& deviceID)
{

	int status;

	const char* kernel_source = kernelSource.data();
	cl_program program = clCreateProgramWithSource(context, 1, &kernel_source, nullptr, &status);

	if (!program)
	{
		exit(EXIT_FAILURE);
	}

	status = clBuildProgram(program, 0, nullptr, nullptr, nullptr, nullptr);

	if (status != CL_SUCCESS)
	{
		size_t len;
		char buffer[2048];
		std::cout << "Error: Failed to build program executable!\n";
		clGetProgramBuildInfo(program, deviceID, CL_PROGRAM_BUILD_LOG, sizeof(buffer), buffer, &len);
		std::cout << buffer << "\n";
		exit(EXIT_FAILURE);
	}

	m_Kernel = clCreateKernel(program, "matrixMul", &status);
	if (!m_Kernel || status != CL_SUCCESS)
	{
		std::cout << "Error: Failed to create compute kernel!\n";
		exit(1);
	}

	clReleaseProgram(program);
}

CLKernel::~CLKernel()
{
	clReleaseKernel(m_Kernel);
}

cl_int CLKernel::PushArg(cl_uint index, size_t size, const void* data)
{
	return clSetKernelArg(m_Kernel, index, size, data);
}

cl_int CLKernel::ExecuteQueue(const cl_command_queue& command)
{
	size_t localWorkSize[2], globalWorkSize[2];

	localWorkSize[0] = 16;
	localWorkSize[1] = 16;
	globalWorkSize[0] = 1024;
	globalWorkSize[1] = 1024;

	return clEnqueueNDRangeKernel(command, m_Kernel, 2, NULL, globalWorkSize, localWorkSize, 0, NULL, NULL);;
}
