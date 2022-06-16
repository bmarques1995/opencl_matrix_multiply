#ifndef CL_BUFFER_HH
#define CL_BUFFER_HH

#include <CL/cl.h>
#include "Utils/FileHandler.hh"

class CLBuffer
{
public:
	CLBuffer(const cl_context& context, cl_mem_flags flags, size_t size, void* hostPtr);
	~CLBuffer();

	void ExportToCPU(const cl_command_queue& command, void* buffer, size_t size);
	const cl_mem& GetBuffer();
private:
	cl_mem m_Buffer;
};


#endif // CL_BUFFER_HH
