#include "../include/OTL/Core/PropagateLagrangian.hpp"
#include <algorithm>
#include <math.h>
#include "mex.h"

void Propagate(double r1[],
               double v1[],
               double dt,
               double mu,
               double r2[],
               double v2[])
{
   otl::keplerian::PropagateLagrangian propagator;

   // Setup inputs
   otl::StateVector initialStateVector(otl::Vector3d(r1[0], r1[1], r1[2]), otl::Vector3d(v1[0], v1[1], v1[2]));
   otl::Time timeDelta = otl::Time::Seconds(dt);

   // Setup outputs
   otl::StateVector finalStateVector;

   // Evaluate Lambert's problem
   propagator.Propagate(initialStateVector,
                        mu,
                        timeDelta,
                        finalStateVector);

   // Convert outputs
   r2[0] = finalStateVector.position.x;
   r2[1] = finalStateVector.position.y;
   r2[2] = finalStateVector.position.z;
   v2[0] = finalStateVector.velocity.x;
   v2[1] = finalStateVector.velocity.y;
   v2[2] = finalStateVector.velocity.z;
}



void mexFunction(int numOutputs, mxArray *outputs[], int numInputs, const mxArray *inputs[] )
{
   if (numInputs != 4)
   { 
      mexErrMsgIdAndTxt( "MATLAB:Propagate:InvalidNumInputs","Four input arguments required.");
   }
   else if (numOutputs > 2)
   {
      mexErrMsgIdAndTxt( "MATLAB:Propagate:MaxNumOutputs", "Too many output arguments.");
   }

   size_t mR1 = mxGetM(inputs[0]);
   size_t nR1 = mxGetN(inputs[0]);
   if (!mxIsDouble(inputs[0]) || mxIsComplex(inputs[0]) || std::max(mR1,nR1) != 3 || std::min(mR1,nR1) != 1)
   {
      mexErrMsgIdAndTxt( "MATLAB:Propagate:InvalidR1", "Propagate requires that R1 be a 3 x 1 vector of real numbers.");
   }

   size_t mV1 = mxGetM(inputs[1]);
   size_t nV1 = mxGetN(inputs[1]);
   if (!mxIsDouble(inputs[1]) || mxIsComplex(inputs[1]) || std::max(mV1,nV1) != 3 || std::min(mV1,nV1) != 1)
   {
      mexErrMsgIdAndTxt( "MATLAB:Propagate:InvalidV1", "Propagate requires that V1 be a 3 x 1 vector of real numbers.");
   }

   size_t mDt = mxGetM(inputs[2]);
   size_t nDt = mxGetN(inputs[2]);
   if (!mxIsDouble(inputs[2]) || mxIsComplex(inputs[2]) || std::max(mDt,nDt) != 1 || std::min(mDt,nDt) != 1)
   {
      mexErrMsgIdAndTxt( "MATLAB:Propagate:InvalidDt", "Propagate requires that dt be a real number scalar.");
   }

   size_t mMu = mxGetM(inputs[3]);
   size_t nMu = mxGetN(inputs[3]);
   if (!mxIsDouble(inputs[3]) || mxIsComplex(inputs[3]) || std::max(mMu,nMu) != 1 || std::min(mMu,nMu) != 1)
   {
      mexErrMsgIdAndTxt( "MATLAB:Propagate:InvalidMu", "Propagate requires that mu be a scalar.");
   }

   // Create a matrix for the return arguement
   outputs[0] = mxCreateDoubleMatrix((mwSize)mR1, (mwSize)nR1, mxREAL);
   outputs[1] = mxCreateDoubleMatrix((mwSize)mV1, (mwSize)nV1, mxREAL);

   double *r1 = mxGetPr(inputs[0]);
   double *v1 = mxGetPr(inputs[1]);
   double *dt = mxGetPr(inputs[2]);
   double *mu = mxGetPr(inputs[3]);

   double *r2 = mxGetPr(outputs[0]);
   double *v2 = mxGetPr(outputs[1]);
   
   // Do the actual computation
   Propagate(r1, v1, *dt, *mu, r2, v2);
}