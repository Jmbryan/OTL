#include <OTL/Core/LambertExponentialSinusoid.hpp>
#include <algorithm>
#include <math.h>
#include "mex.h"

void Lambert(double r1[],
             double r2[],
             double dt,
             double dir,
             double maxRev,
             double mu,
             double v1[],
             double v2[])
{
   otl::keplerian::LambertExponentialSinusoid lambert;

   // Setup inputs
   otl::Vector3d initialPosition(r1[0], r1[1], r1[2]);
   otl::Vector3d finalPosition(r2[0], r2[1], r2[2]);
   otl::Time timeDelta = otl::Time::Seconds(dt);
   int maxRevolutions = static_cast<int>(maxRev);
   otl::keplerian::Orbit::Direction orbitDirection = (dir > 0.0 ? otl::keplerian::Orbit::Direction::Prograde : otl::keplerian::Orbit::Direction::Retrograde);

   // Setup outputs
   otl::Vector3d initialVelocity(v1[0], v1[1], v1[2]);
   otl::Vector3d finalVelocity(v2[0], v2[1], v2[2]);

   // Evaluate Lambert's problem
   lambert.Evaluate(initialPosition,
                    finalPosition,
                    timeDelta,
                    orbitDirection,
                    maxRevolutions,
                    mu,
                    initialVelocity,
                    finalVelocity);

   // Convert outputs
   v1[0] = initialVelocity.x;
   v1[1] = initialVelocity.y;
   v1[2] = initialVelocity.z;
   v2[0] = finalVelocity.x;
   v2[1] = finalVelocity.y;
   v2[2] = finalVelocity.z;
}



void mexFunction(int numOutputs, mxArray *outputs[], int numInputs, const mxArray *inputs[] )
{
   if (numInputs != 6)
   { 
      mexErrMsgIdAndTxt( "MATLAB:Lambert:InvalidNumInputs","Six input arguments required.");
   }
   else if (numOutputs > 2)
   {
      mexErrMsgIdAndTxt( "MATLAB:Lambert:MaxNumOutputs", "Too many output arguments.");
   }

   size_t mR1 = 3;
   size_t nR1 = 1;
   size_t mR2 = 3;
   size_t nR2 = 1;

   //size_t mR1 = mxGetM(outputs[0]);
   //size_t nR1 = mxGetN(outputs[0]);
   //if (!mxIsDouble(outputs[0]) || mxIsComplex(outputs[0]) || std::max(mR1,nR1) != 3 || std::min(mR1,nR1) != 1)
   //{
   //   mexErrMsgIdAndTxt( "MATLAB:Lambert:InvalidR1", "Lambert requires that R1 be a 3 x 1 vector.");
   //}

   //size_t mR2 = mxGetM(outputs[1]);
   //size_t nR2 = mxGetN(outputs[1]);
   //if (!mxIsDouble(outputs[1]) || mxIsComplex(outputs[1]) || std::max(mR2,nR2) != 3 || std::min(mR2,nR2) != 1)
   //{
   //   mexErrMsgIdAndTxt( "MATLAB:Lambert:InvalidR2", "Lambert requires that R2 be a 3 x 1 vector.");
   //}

   //size_t mDt = mxGetM(outputs[2]);
   //size_t nDt = mxGetN(outputs[2]);
   //if (!mxIsDouble(outputs[2]) || mxIsComplex(outputs[2]) || std::max(mDt,nDt) != 1 || std::min(mDt,nDt) != 1)
   //{
   //   mexErrMsgIdAndTxt( "MATLAB:Lambert:InvalidDt", "Lambert requires that dt be a scalar.");
   //}

   // Create a matrix for the return arguement
   outputs[0] = mxCreateDoubleMatrix((mwSize)mR1, (mwSize)nR1, mxREAL);
   outputs[1] = mxCreateDoubleMatrix((mwSize)mR2, (mwSize)nR2, mxREAL);

   double *r1 = mxGetPr(inputs[0]);
   double *r2 = mxGetPr(inputs[1]);
   double *dt = mxGetPr(inputs[2]);
   double *dir = mxGetPr(inputs[3]);
   double *maxRev = mxGetPr(inputs[4]);
   double *mu = mxGetPr(inputs[5]);

   double *v1 = mxGetPr(outputs[0]);
   double *v2 = mxGetPr(outputs[1]);
   
   // Do the actual computation
   Lambert(r1, r2, *dt, *dir, *maxRev, *mu, v1, v2);
}