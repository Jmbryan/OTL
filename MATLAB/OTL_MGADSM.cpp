#include "../include/OTL/Core/MGADSMTrajectory.hpp"
#include <algorithm>
#include <math.h>
#include <string>
#include "mex.h"

void MGADSMTrajectory(const std::vector<std::string>& itineary,
                      double stateVector[],
                      std::vector<double>& deltaVs)
{
   otl::keplerian::trajectory::MGADSMTrajectory trajectory;

   // Setup inputs

   // Setup outputs

   // Evaluate the trajectory
   for (size_t i = 0; i < itineary.size(); ++i)
   {
      printf(itineary[i].c_str());
   }
   
   // Convert outputs
}



void mexFunction(int numOutputs, mxArray *outputs[], int numInputs, const mxArray *inputs[] )
{
   if (numInputs != 2)
   { 
      mexErrMsgIdAndTxt( "MATLAB:MGADSM:InvalidNumInputs","Six input arguments required.");
   }
   else if (numOutputs > 1)
   {
      mexErrMsgIdAndTxt( "MATLAB:MGADSM:MaxNumOutputs", "Too many output arguments.");
   }

   size_t mItineary = mxGetM(inputs[0]);
   size_t nItineary = mxGetN(inputs[0]);
   //if (!mxIsChar(inputs[0]) || std::min(mItineary,nItineary) != 1)
   //{
   //   mexErrMsgIdAndTxt( "MATLAB:MGADSM:InvalidItineary", "MGADSM requires that itineary be a vector of chars.");
   //}

   size_t mState = mxGetM(inputs[1]);
   size_t nState = mxGetN(inputs[1]);
   if (!mxIsDouble(inputs[1]) || mxIsComplex(inputs[1]) || std::min(mState,nState) != 1)
   {
      mexErrMsgIdAndTxt( "MATLAB:MGADSM:InvalidStateVector", "MGADSM requires that stateVector be a vector of real numbers.");
   }

   
   //outputs[0] = mxCreateDoubleMatrix((mwSize)mR1, (mwSize)nR1, mxREAL);
   //outputs[1] = mxCreateDoubleMatrix((mwSize)mR2, (mwSize)nR2, mxREAL);

   std::vector<std::string> itineary;

   size_t len = std::max(mItineary, nItineary);
   for (size_t i = 0; i < len; i++)
   {
      mxArray* orbitalBody = mxGetCell(inputs[0], i);
      std::string str = mxArrayToString(orbitalBody);
      itineary.push_back(str);
   }

   double *stateVector = mxGetPr(inputs[1]);

   //double *deltaVs = mxGetPr(outputs[0]);
   std::vector<double> deltaVs;
 
   // Do the actual computation
   MGADSMTrajectory(itineary, stateVector, deltaVs);

   // Create a matrix for the return arguement
   //outputs[0] = mxCreateDoubleMatrix((mwSize)deltaVs.length(), 1.0, mxREAL);
}