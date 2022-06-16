#ifndef CL_CONTEXT_HH
#define CL_CONTEXT_HH

#include <CL/cl.h>

class CLContext
{
public:
	CLContext(unsigned device_type = CL_DEVICE_TYPE_GPU);
	~CLContext();

	const cl_context& GetContext();
	const cl_device_id& GetDeviceID();
	const cl_command_queue& GetCommands();

private:
	cl_device_id m_DeviceID;
	cl_context m_Context;
	cl_command_queue m_Commands;

	size_t m_LocalWorkSize[2], m_GlobalWorkSize[2];
};


#endif // CL_CONTEXT_HH
