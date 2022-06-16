#include <iostream>
#include <Eigen/Dense>
#include <CL/cl.h>
#include <OpenCLWrapperLog.hpp>

#include "Utils/FileHandler.hh"

//Eigen armazena matrizes no modo column major por padrão

int main()
{
	Eigen::MatrixXf matrixA;
	Eigen::MatrixXf matrixB;

	matrixA.resize(2, 2);
	matrixA << 0, 1, -6, -5;

	matrixB.resize(2, 2);
	matrixB << 7, 3, - 8, - 9;

	Eigen::MatrixXf matrixC = matrixA * matrixB;
	Eigen::MatrixXf matrixCOpenCL;

	matrixCOpenCL.resize(matrixA.cols(), matrixB.rows());

	std::cout << matrixC << "\n";

	cl_device_id device_id;
	cl_context context;
	cl_command_queue commands;
	cl_program program;
	cl_kernel kernel;

	cl_mem matrix_A;
	cl_mem matrix_B;
	cl_mem matrix_C;

	std::cout << "Initializing OpenCL device...\n";
	
	cl_uint dev_cnt;
	clGetPlatformIDs(0,0, &dev_cnt);

	cl_platform_id platform_id;
	clGetPlatformIDs(dev_cnt, &platform_id, nullptr);

	unsigned device_type = CL_DEVICE_TYPE_GPU;
	cl_int status = clGetDeviceIDs(platform_id, device_type, 1, &device_id, nullptr);

	cl_device_fp_config cfg;
	clGetDeviceInfo(device_id, CL_DEVICE_DOUBLE_FP_CONFIG, sizeof(cfg), &cfg, nullptr); // 0 is for the device number I guess?

	if (status != CL_SUCCESS)
	{
		std::cout << "Error: Failed to create a device group!\n";
		return EXIT_FAILURE;
	}

	context = clCreateContext(0, 1, &device_id, nullptr, nullptr, &status);

	if (status != CL_SUCCESS)
	{
		std::cout << "Error: Failed to create a device group!\n";
		return EXIT_FAILURE;
	}

	commands = clCreateCommandQueue(context, device_id, 0, &status);

	if (status != CL_SUCCESS)
	{
		std::cout << "Error: Failed to create a command commands!\n";
		return EXIT_FAILURE;
	}

	std::string clKernel;
	if (!FileHandler::ReadTextFile("matrix_multiply.cl", &clKernel)) 
	{
		std::cout << "Error: Failed to read the file!\n";
		return EXIT_FAILURE;
	}

	const char* kernel_source = clKernel.c_str();
	program = clCreateProgramWithSource(context, 1, &kernel_source, nullptr, &status);

	if (!program)
	{
		std::cout << "Error: Failed to create compute program!\n";
		return EXIT_FAILURE;
	}

	status = clBuildProgram(program, 0, nullptr, nullptr, nullptr, nullptr);

	if (status != CL_SUCCESS)
	{
		size_t len;
		char buffer[2048];
		std::cout << "Error: Failed to build program executable!\n";
		clGetProgramBuildInfo(program, device_id, CL_PROGRAM_BUILD_LOG, sizeof(buffer), buffer, &len);
		std::cout << buffer << "\n";
		exit(EXIT_FAILURE);
	}

	kernel = clCreateKernel(program, "matrixMul", &status);
	if (!kernel || status != CL_SUCCESS)
	{
		printf("Error: Failed to create compute kernel!\n");
		exit(1);
	}

	matrix_C = clCreateBuffer(context, CL_MEM_READ_WRITE, sizeof(float) * matrixCOpenCL.rows() * matrixCOpenCL.cols(), NULL, &status);
	matrix_A = clCreateBuffer(context, CL_MEM_READ_WRITE | CL_MEM_COPY_HOST_PTR, sizeof(float) * matrixA.rows() * matrixA.cols(), matrixA.data(), &status);
	matrix_B = clCreateBuffer(context, CL_MEM_READ_WRITE | CL_MEM_COPY_HOST_PTR, sizeof(float) * matrixB.rows() * matrixB.cols(), matrixB.data(), &status);

	if (!matrix_A || !matrix_B || !matrix_C)
	{
		printf("Error: Failed to allocate device memory!\n");
		exit(1);
	}

	std::cout << "Running matrix multiplication for matrices A (" << matrixA.rows() << "x" << matrixA.cols() << ") and B" << matrixB.rows() << "x" << matrixB.cols() << " ...\n";

	//Launch OpenCL kernel
	size_t localWorkSize[2], globalWorkSize[2];

	int wA = matrixA.rows();
	int wC = matrixCOpenCL.rows();

	status = CL_SUCCESS;

	status |= clSetKernelArg(kernel, 0, sizeof(cl_mem), (void*)&matrix_C);
	status |= clSetKernelArg(kernel, 1, sizeof(cl_mem), (void*)&matrix_B);
	status |= clSetKernelArg(kernel, 2, sizeof(cl_mem), (void*)&matrix_A);
	status |= clSetKernelArg(kernel, 3, sizeof(int), (void*)&wA);
	status |= clSetKernelArg(kernel, 4, sizeof(int), (void*)&wC);

	if (status != CL_SUCCESS)
	{
		std::cout << "Error: Failed to set kernel arguments! " << status << "\n";
		return EXIT_FAILURE;
	}

	localWorkSize[0] = 16;
	localWorkSize[1] = 16;
	globalWorkSize[0] = 1024;
	globalWorkSize[1] = 1024;

	status = clEnqueueNDRangeKernel(commands, kernel, 2, NULL, globalWorkSize, localWorkSize, 0, NULL, NULL);

	if (status != CL_SUCCESS)
	{
		std::cout << "Error: Failed to execute kernel! " << status << "\n";
		return EXIT_FAILURE;
	}

	float h_C[2][2];

	status = clEnqueueReadBuffer(commands, matrix_C, CL_TRUE, 0, sizeof(float) * matrixCOpenCL.rows() * matrixCOpenCL.cols(), h_C, 0, NULL, NULL);

	if (status != CL_SUCCESS)
	{
		printf("Error: Failed to read output array! %d\n", status);
		exit(1);
	}

	memcpy(matrixCOpenCL.data(), h_C, matrixCOpenCL.size() * sizeof(float));

	std::cout << matrixCOpenCL << "\n";

	clReleaseMemObject(matrix_A);
	clReleaseMemObject(matrix_C);
	clReleaseMemObject(matrix_B);

	clReleaseProgram(program);
	clReleaseKernel(kernel);
	clReleaseCommandQueue(commands);
	clReleaseContext(context);

	return 0;
}