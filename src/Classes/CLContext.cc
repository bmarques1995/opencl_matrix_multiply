#include "CLContext.hh"
#include <cstdlib>

CLContext::CLContext(unsigned device_type)
{
	cl_uint dev_cnt;
	cl_platform_id platform_id;

	clGetPlatformIDs(0, 0, &dev_cnt);
	clGetPlatformIDs(dev_cnt, &platform_id, nullptr);

	cl_int status = clGetDeviceIDs(platform_id, device_type, 1, &m_DeviceID, nullptr);

#if 0
	cl_device_fp_config cfg;
	clGetDeviceInfo(device_id, CL_DEVICE_DOUBLE_FP_CONFIG, sizeof(cfg), &cfg, nullptr);
#endif

	if (status != CL_SUCCESS)
	{
		exit(EXIT_FAILURE);
	}

	m_Context = clCreateContext(0, 1, &m_DeviceID, nullptr, nullptr, &status);
	if (status != CL_SUCCESS)
	{
		exit(EXIT_FAILURE);
	}

	m_Commands = clCreateCommandQueue(m_Context, m_DeviceID, 0, &status);
	if (status != CL_SUCCESS)
	{
		exit(EXIT_FAILURE);
	}
}

CLContext::~CLContext()
{
	clReleaseCommandQueue(m_Commands);
	clReleaseContext(m_Context);
}

const cl_context& CLContext::GetContext()
{
	return m_Context;
}

const cl_device_id& CLContext::GetDeviceID()
{
	return m_DeviceID;
}

const cl_command_queue& CLContext::GetCommands()
{
	return m_Commands;
}
