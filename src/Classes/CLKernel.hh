#ifndef CL_KERNEL_HH
#define CL_KERNEL_HH

#include <CL/cl.h>
#include <string>

class CLKernel
{
public:
	CLKernel(std::string_view kernelSource, const cl_context& context, const cl_device_id& deviceID);
	~CLKernel();

	cl_int PushArg(cl_uint index, size_t size, const void* data);
	cl_int ExecuteQueue(const cl_command_queue& command);

private:
	cl_kernel m_Kernel;
};

#endif // CL_KERNEL_HH
