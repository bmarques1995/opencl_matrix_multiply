#include <iostream>
#include <Eigen/Dense>
#include <CL/cl.h>
#include <chrono>
#include <OpenCLWrapperLog.hpp>

#include "Utils/FileHandler.hh"
#include "Utils/EigenBenchmark.hh"
#include "Utils/Timer.hh"
#include "Classes/CLContext.hh"
#include "Classes/CLKernel.hh"
#include "Classes/CLBuffer.hh"

using std::chrono::steady_clock;

//Eigen armazena matrizes no modo column major por padr�o

int main()
{
	
	Eigen::MatrixXf matrixA;
	Eigen::MatrixXf matrixB;

	EigenBenchmark::PushElements(600, &matrixA);
	EigenBenchmark::PushElements(600, &matrixB);

	Eigen::MatrixXf matrixC;
	
	Timer::Start();
	matrixC = matrixA * matrixB;
	Timer::End();
	Timer::Log();

	Eigen::MatrixXf matrixCOpenCL;

	matrixCOpenCL.resize(matrixA.cols(), matrixB.rows());

	//std::cout << matrixC << "\n";

	std::cout << "Initializing OpenCL device...\n";
	
	int status;

	CLContext* context = new CLContext();

	std::string kernelSource;
	if (!FileHandler::ReadTextFile("matrix_multiply.cl", &kernelSource)) 
	{
		std::cout << "Error: Failed to read the file!\n";
		return EXIT_FAILURE;
	}

	CLKernel* kernel = new CLKernel(kernelSource,context->GetContext(), context->GetDeviceID());

	CLBuffer* matrix_C = new CLBuffer(context->GetContext(), CL_MEM_READ_WRITE, sizeof(float) * matrixCOpenCL.size(), nullptr);
	CLBuffer* matrix_A = new CLBuffer(context->GetContext(), CL_MEM_READ_WRITE | CL_MEM_COPY_HOST_PTR, sizeof(float) * matrixA.size(), matrixA.data());
	CLBuffer* matrix_B = new CLBuffer(context->GetContext(), CL_MEM_READ_WRITE | CL_MEM_COPY_HOST_PTR, sizeof(float) * matrixB.size(), matrixB.data());
	
	std::cout << "Running matrix multiplication for matrices A (" << matrixA.rows() << "x" << matrixA.cols() << ") and B (" << matrixB.rows() << "x" << matrixB.cols() << ") ...\n";

	//Launch OpenCL kernel
	
	int wA = matrixA.rows();
	int wC = matrixCOpenCL.rows();

	status = CL_SUCCESS;

	Timer::Start();

	status |= kernel->PushArg(0, sizeof(cl_mem), (void*)&(matrix_C->GetBuffer()));
	status |= kernel->PushArg(1, sizeof(cl_mem), (void*)&(matrix_B->GetBuffer()));
	status |= kernel->PushArg(2, sizeof(cl_mem), (void*)&(matrix_A->GetBuffer()));
	status |= kernel->PushArg(3, sizeof(int), (void*)&wA);
	status |= kernel->PushArg(4, sizeof(int), (void*)&wC);

	Timer::End();
	Timer::Log();

	if (status != CL_SUCCESS)
	{
		std::cout << "Error: Failed to set kernel arguments! " << status << "\n";
		return EXIT_FAILURE;
	}

	Timer::Start();
	status = kernel->ExecuteQueue(context->GetCommands());

	Timer::End();
	Timer::Log();

	if (status != CL_SUCCESS)
	{
		std::cout << "Error: Failed to execute kernel! " << status << "\n";
		return EXIT_FAILURE;
	}

	Timer::Start();
	matrix_C->ExportToCPU(context->GetCommands(), matrixCOpenCL.data(), matrixCOpenCL.size() * sizeof(float));
	Timer::End();
	Timer::Log();

	//std::cout << matrixCOpenCL << "\n";

	std::cout << matrixC.isApprox(matrixCOpenCL, .0001f) << "\n";

	delete matrix_A;
	delete matrix_B;
	delete matrix_C;
	delete kernel;
	delete context;

	return 0;
}