#include "stdafx.h"
#include "extcode.h"
#include <string>
#include <math.h>
#include "mesh_utilities.h"
#include "octave_transport.h"
#pragma pack(1)


typedef struct {
 int32 dimSize;
 float64 data[1];
} TD1;
typedef TD1 **TD1Hdl;
 
typedef struct {
 TD1Hdl elt1;
 TD1Hdl elt2;
 LStrHandle elt3;
} TD2;

BOOL APIENTRY DllMain( HANDLE hModule,  DWORD  ul_reason_for_call, LPVOID lpReserved)
{
    return TRUE;
}

float64 func(float64* parameters, int32 length, float64 x) 
{
	float64 sum = 0;
	for (int32 i = 0 ; i < length/2 ; i ++)
		sum += parameters[i] * exp(parameters[i+1] * x)
	return sum;
}

float64 approximationError(float64* parameters, int32 length, float64* x, float64* y, int32 dataLength) 
{
	float64 sum = 0;
	
	for (int32 i = 0; i < dataLength ; i ++) 
	{
		sum += pow(func(parameters, length, x[i])-y[i]/min(y[i],x[i]), 2);
	}
	return sum * 10000/dataLength;
}
 
_declspec(dllexport) void CombinedAlgorithm(TD2 *XYData, TD2 *parameters, float64 *error, TD2 *XY, LStrHandle LVString)
{
	int32 paramsLen = parameters->elt1.dimSize * 2;
	
	float64* meshValues = new float64[paramsLen]; //Stores return values;
	float64* leisGValues = new float64[paramsLen]; 
	float64* leisGMValues = new float64[paramsLen]; 
	float64* leisMValues = new float64[paramsLen]; 
	float64* leisMMValues = new float64[paramsLen]; 
	float64* greedValues = new float64[paramsLen];
	Mesh(meshValues, paramsLen, XY->elt1.dimSize, XY->elt1.data, XY->elt2.data);

	for (int32 = 0 ; i < paramsLen / 2 ; i ++ )
	{
		greedValues[i*2] = parameters->elt1.data[i];
		greedValues[i*2+1] = parameters->elt2.data[i];
	}
	std::string filename("octave.m");
	std::string data_filename("data.dat");

	octaveGenerateLevenberg(filename, meshValues, paramsLen);
	octaveLaunchInterpreter(filename, data_filename);
	octaveParseResults(leisGValues, paramsLen);

	octaveGenerateLevenbergModified(filename, meshValues, paramsLen);
	octaveLaunchInterpreter(filename, data_filename);
	octaveParseResults(leisGMValues, paramsLen);

	octaveGenerateLevenberg(filename, greedValues, paramsLen);
	octaveLaunchInterpreter(filename, data_filename);
	octaveParseResults(leisMValues, paramsLen);
	
	octaveGenerateLevenbergModified(filename, greedValues, paramsLen);
	octaveLaunchInterpreter(filename, data_filename);
	octaveParseResults(leisMMValues, paramsLen);

	float64* minimum = approximationErrorMinimum(meshValues,greedValues,leisGValues,leisGMValues,leisMValues,leisMMValues, paramsLen);

	for (int32 = 0 ; i < paramsLen / 2 ; i ++ )
	{
		parameters->elt1.data[i] = minimum[i*2];
		parameters->elt2.data[i] = minimum[i*2+1];
	}
	
	*error=approximationError(minimum, paramsLen, XY->elt1.data, XY->elt2.data,XY->elt1.dimSize);

	delete[] meshValues;
	delete[] greedValues;
	delete[] leisGValues;
	delete[] leisGMValues;
	delete[] leisMValues;
	delete[] leisMMValues;

}