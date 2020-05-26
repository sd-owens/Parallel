
// opencl vendor ids:
#define ID_AMD          0x1002
#define ID_INTEL        0x8086
#define ID_NVIDIA       0x10de

#include <stdio.h>
#include <string.h>
#include "CL/cl.h"
#include "CL/cl_platform.h"

cl_platform_id Platform {};
cl_device_id Device {};

void
PrintOpenclInfo()
{
		cl_int status;		// returned status from opencl calls
							// test against CL_SUCCESS
		fprintf(stderr, "PrintInfo:\n");

		// find out how many platforms are attached here and get their ids:

		cl_uint numPlatforms;
		status = clGetPlatformIDs(0, NULL, &numPlatforms);
		if (status != CL_SUCCESS)
			fprintf(stderr, "clGetPlatformIDs failed (1)\n");

		fprintf(stderr, "Number of Platforms = %d\n", numPlatforms);

		cl_platform_id* platforms = new cl_platform_id[numPlatforms];

		status = clGetPlatformIDs(numPlatforms, platforms, NULL);

		if (status != CL_SUCCESS)
			fprintf(stderr, "clGetPlatformIDs failed (2)\n");

		for (int p = 0; p < (int)numPlatforms; p++)
		{
			fprintf(stderr, "Platform #%d:\n", p);
			size_t size;
			char* str;

			clGetPlatformInfo(platforms[p], CL_PLATFORM_NAME, 0, NULL, &size);
			str = new char[size];
			clGetPlatformInfo(platforms[p], CL_PLATFORM_NAME, size, str, NULL);
			fprintf(stderr, "\tName    = '%s'\n", str);
			delete[] str;

			clGetPlatformInfo(platforms[p], CL_PLATFORM_VENDOR, 0, NULL, &size);
			str = new char[size];
			clGetPlatformInfo(platforms[p], CL_PLATFORM_VENDOR, size, str, NULL);
			fprintf(stderr, "\tVendor  = '%s'\n", str);
			delete[] str;

			clGetPlatformInfo(platforms[p], CL_PLATFORM_VERSION, 0, NULL, &size);
			str = new char[size];
			clGetPlatformInfo(platforms[p], CL_PLATFORM_VERSION, size, str, NULL);
			fprintf(stderr, "\tVersion = '%s'\n", str);
			delete[] str;

			clGetPlatformInfo(platforms[p], CL_PLATFORM_PROFILE, 0, NULL, &size);
			str = new char[size];
			clGetPlatformInfo(platforms[p], CL_PLATFORM_PROFILE, size, str, NULL);
			fprintf(stderr, "\tProfile = '%s'\n", str);
			delete[] str;


			// find out how many devices are attached to each platform and get their ids:

			cl_uint numDevices;

			status = clGetDeviceIDs(platforms[p], CL_DEVICE_TYPE_ALL, 0, NULL, &numDevices);
			if (status != CL_SUCCESS)
				fprintf(stderr, "clGetDeviceIDs failed (2)\n");

			fprintf(stderr, "\tNumber of Devices = %d\n", numDevices);

			cl_device_id * devices = new cl_device_id[numDevices];
			status = clGetDeviceIDs(platforms[p], CL_DEVICE_TYPE_ALL, numDevices, devices, NULL);
			if (status != CL_SUCCESS)
				fprintf(stderr, "clGetDeviceIDs failed (2)\n");

			for (int d = 0; d < (int)numDevices; d++)
			{
				fprintf(stderr, "\tDevice #%d:\n", d);
				size_t size;
				cl_device_type type;
				cl_uint ui;
				size_t sizes[3] = { 0, 0, 0 };

				clGetDeviceInfo(devices[d], CL_DEVICE_TYPE, sizeof(type), &type, NULL);
				fprintf(stderr, "\t\tType = 0x%04x = ", (unsigned int)type);
				switch (type)
				{
				case CL_DEVICE_TYPE_CPU:
					fprintf(stderr, "CL_DEVICE_TYPE_CPU\n");
					break;
				case CL_DEVICE_TYPE_GPU:
					fprintf(stderr, "CL_DEVICE_TYPE_GPU\n");
					break;
				case CL_DEVICE_TYPE_ACCELERATOR:
					fprintf(stderr, "CL_DEVICE_TYPE_ACCELERATOR\n");
					break;
				default:
					fprintf(stderr, "Other...\n");
					break;
				}

				clGetDeviceInfo(devices[d], CL_DEVICE_VENDOR_ID, sizeof(ui), &ui, NULL);
				fprintf(stderr, "\t\tDevice Vendor ID = 0x%04x ", ui);
				switch (ui)
				{
				case ID_AMD:
					fprintf(stderr, "(AMD)\n");
					break;
				case ID_INTEL:
					fprintf(stderr, "(Intel)\n");
					break;
				case ID_NVIDIA:
					fprintf(stderr, "(NVIDIA)\n");
					break;
				default:
					fprintf(stderr, "(?)\n");
				}

				clGetDeviceInfo(devices[d], CL_DEVICE_MAX_COMPUTE_UNITS, sizeof(ui), &ui, NULL);
				fprintf(stderr, "\t\tDevice Maximum Compute Units = %d\n", ui);

				clGetDeviceInfo(devices[d], CL_DEVICE_MAX_WORK_ITEM_DIMENSIONS, sizeof(ui), &ui, NULL);
				fprintf(stderr, "\t\tDevice Maximum Work Item Dimensions = %d\n", ui);

				clGetDeviceInfo(devices[d], CL_DEVICE_MAX_WORK_ITEM_SIZES, sizeof(sizes), sizes, NULL);
				fprintf(stderr, "\t\tDevice Maximum Work Item Sizes = %d x %d x %d\n", sizes[0], sizes[1], sizes[2]);

				clGetDeviceInfo(devices[d], CL_DEVICE_MAX_WORK_GROUP_SIZE, sizeof(size), &size, NULL);
				fprintf(stderr, "\t\tDevice Maximum Work Group Size = %d\n", size);

				clGetDeviceInfo(devices[d], CL_DEVICE_MAX_CLOCK_FREQUENCY, sizeof(ui), &ui, NULL);
				fprintf(stderr, "\t\tDevice Maximum Clock Frequency = %d MHz\n", ui);

				size_t extensionSize;
				clGetDeviceInfo(devices[d], CL_DEVICE_EXTENSIONS, 0, NULL, &extensionSize);
				char* extensions = new char[extensionSize];
				clGetDeviceInfo(devices[d], CL_DEVICE_EXTENSIONS, extensionSize, extensions, NULL);
				fprintf(stderr, "\nDevice #%d's Extensions:\n", d );
				for (int e = 0; e < (int)strlen(extensions); e++)
				{
					if (extensions[e] == ' ')
						extensions[e] = '\n';
				}
				fprintf(stderr, "%s\n", extensions);
				delete[] extensions;
			}
			delete[] devices;
		}
		delete[] platforms;
		fprintf(stderr, "\n\n");
}

void
SelectOpenclDevice()
{
		// select which opencl device to use:
		// priority order:
		//	1. a gpu
		//	2. an nvidia or amd gpu

	int iplatform = -1;
	int idevice = -1;
	cl_device_type deviceType;
	cl_uint deviceVendor;
	cl_int status;		// returned status from opencl calls
						// test against CL_SUCCESS

	fprintf(stderr, "\nSelecting the OpenCL Platform and Device:\n");

	// find out how many platforms are attached here and get their ids:

	cl_uint numPlatforms;
	status = clGetPlatformIDs(0, NULL, &numPlatforms);
	if (status != CL_SUCCESS)
		fprintf(stderr, "clGetPlatformIDs failed (1)\n");

	cl_platform_id* platforms = new cl_platform_id[numPlatforms];
	status = clGetPlatformIDs(numPlatforms, platforms, NULL);
	if (status != CL_SUCCESS)
		fprintf(stderr, "clGetPlatformIDs failed (2)\n");

	for (int p = 0; p < (int)numPlatforms; p++)
	{
		// find out how many devices are attached to each platform and get their ids:

		cl_uint numDevices;

		status = clGetDeviceIDs(platforms[p], CL_DEVICE_TYPE_ALL, 0, NULL, &numDevices);
		if (status != CL_SUCCESS)
			fprintf(stderr, "clGetDeviceIDs failed (2)\n");

		cl_device_id* devices = new cl_device_id[numDevices];
		status = clGetDeviceIDs(platforms[p], CL_DEVICE_TYPE_ALL, numDevices, devices, NULL);
		if (status != CL_SUCCESS)
			fprintf(stderr, "clGetDeviceIDs failed (2)\n");

		for (int d = 0; d < (int)numDevices; d++)
		{
			cl_device_type type;
			cl_uint vendor;
			size_t sizes[3] = { 0, 0, 0 };

			clGetDeviceInfo(devices[d], CL_DEVICE_TYPE, sizeof(type), &type, NULL);

			clGetDeviceInfo(devices[d], CL_DEVICE_VENDOR_ID, sizeof(vendor), &vendor, NULL);

			// select:

			if (iplatform < 0)		// not yet holding anything -- we'll accept anything
			{
				iplatform = p;
				idevice = d;
				Platform = platforms[iplatform];
				Device = devices[idevice];
				deviceType = type;
				deviceVendor = vendor;
			}
			else					// holding something already -- can we do better?
			{
				if (deviceType == CL_DEVICE_TYPE_CPU)		// holding a cpu already -- switch to a gpu if possible
				{
					if (type == CL_DEVICE_TYPE_GPU)			// found a gpu
					{										// switch to the gpu we just found
						iplatform = p;
						idevice = d;
						Platform = platforms[iplatform];
						Device = devices[idevice];
						deviceType = type;
						deviceVendor = vendor;
					}
				}
				else										// holding a gpu -- is a better gpu available?
				{
					if (deviceVendor == ID_INTEL)			// currently holding an intel gpu
					{										// we are assuming we just found a bigger, badder nvidia or amd gpu
						iplatform = p;
						idevice = d;
						Platform = platforms[iplatform];
						Device = devices[idevice];
						deviceType = type;
						deviceVendor = vendor;
					}
				}
			}
		}
	}

	if (iplatform < 0)
		fprintf(stderr, "Found no OpenCL devices!\n");
	else
		fprintf(stderr, "I have selected Platform #%d, Device #%d\n", iplatform, idevice);
}

int main () {

	PrintOpenclInfo();
	SelectOpenclDevice();


	return 0;
}