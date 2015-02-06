////////////////////////////////////////////////////////////
//
// OTL - Orbital Trajectory Library
// Copyright (C) 2013-2018 Jason Bryan (Jmbryan10@gmail.com)
//
// This software is provided 'as-is', without any express or implied warranty.
// In no event will the authors be held liable for any damages arising from the use of this software.
//
// Permission is granted to anyone to use this software for any purpose,
// including commercial applications, and to alter it and redistribute it freely,
// subject to the following restrictions:
//
// 1. The origin of this software must not be misrepresented;
//    you must not claim that you wrote the original software.
//    If you use this software in a product, an acknowledgment
//    in the product documentation would be appreciated but is not required.
//
// 2. Altered source versions must be plainly marked as such,
//    and must not be misrepresented as being the original software.
//
// 3. This notice may not be removed or altered from any source distribution.
//
////////////////////////////////////////////////////////////

#pragma once
#include <OTL/Core/Propagator.h>

namespace otl
{

class OTL_CORE_API LagrangianPropagator : public IPropagator
{
public:
   ////////////////////////////////////////////////////////////
   /// \brief Helper object for storing the results of the Stumpff Functions
   ////////////////////////////////////////////////////////////
   struct StumpffParameters
   {
      double c2;  ///< Result of Stumpff function c2
      double c3;  ///< Result of Stumpff function c3
   };

   ////////////////////////////////////////////////////////////
   /// \brief Helper object for storing the results of the Universal Variable iteration
   ////////////////////////////////////////////////////////////
   struct UniversalVariableResult
   {
      double x;                  ///< Universal variable
      double r;                  ///< Position magnitude
      double psi;                ///< Ratio of the universal variable squared and the semimajor axis
      StumpffParameters stumpff; ///< Stumpff function results
   };

   ////////////////////////////////////////////////////////////
   /// \brief Helper object for storing the Lagrange Coefficients
   ////////////////////////////////////////////////////////////
   struct LagrangeCoefficients
   {
      double f;      ///< Lagrange coefficient F
      double g;      ///< Lagrange coefficient G
      double fDot;   ///< Derivative of Lagrange coefficient F
      double gDot;   ///< Derivative of Lagrange coefficient G
   };

   LagrangianPropagator();
   virtual ~LagrangianPropagator();

protected:
   virtual void VPropagate(const test::StateVector& initialStateVector, const Time& timeDelta, double mu, test::StateVector& finalStateVector) override;

private:
   UniversalVariableResult CalculateUniversalVariable(double r0, double v0, double rdotv, double seconds, double mu);
   double CalculateUniversableVariableInitialGuess(double r0, double v0, double rdotv, double alpha, double seconds, double mu);
   StumpffParameters CalculateStumpffParameters(double psi);
   LagrangeCoefficients CalculateLagrangeCoefficients(double r0, double seconds, double sqrtMu, const UniversalVariableResult& results);
};

} // namespace otl