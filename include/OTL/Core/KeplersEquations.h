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
#include <OTL/Core/Base.h>

namespace otl
{

namespace keplerian
{

//double SolveKeplersEquation(double eccentricity, double meanAnomaly, int maxIterations = 1000, double tolerance = MATH_TOLERANCE);
//double SolveKeplersEllipticalEquation(double eccentricity, double meanAnomaly, int maxIterations = 1000, double tolerance = MATH_TOLERANCE);
//double SolveKeplersHyperbolicEquation(double eccentricity, double meanAnomaly, int maxIterations = 1000, double tolerance = MATH_TOLERANCE);

//class OTL_CORE_API KeplersEquation
//{
//public:
//   KeplersEquation(int maxIterations = 1000, double tolerance = MATH_TOLERANCE);
//   double Solve(double eccentricity, double meanAnomaly);
//   double CalculateEccentricAnomaly(double eccentricity, double meanAnomaly);
//   double CalculateHyperbolicAnomaly(double eccentricity, double meanAnomaly);
//private:
//};

class OTL_CORE_API IKeplersEquation
{
public:
   ////////////////////////////////////////////////////////////
   /// \brief Default constructor
   ///
   /// Creates an instance of Kepler's Equation and optionally
   /// sets the iteration limits.
   ///
   /// \param maxIterations Maximum number of iteration attempts
   /// \param tolerance Tolerance for convergence
   ///
   ////////////////////////////////////////////////////////////
   IKeplersEquation(int maxIterations = 1000, double tolerance = MATH_TOLERANCE);

   ////////////////////////////////////////////////////////////
   /// \brief Destructor
   ////////////////////////////////////////////////////////////
   virtual ~IKeplersEquation() {}

   ////////////////////////////////////////////////////////////
   /// \brief Set the max allowable iterations.
   ///
   /// The iteration will give up after this many attempts and
   /// return the last quess.
   ///
   /// \param maxIterations Maximum number of iteration attempts
   ///
   ////////////////////////////////////////////////////////////
   void SetMaxIterations(int maxIterations);

   ////////////////////////////////////////////////////////////
   /// \brief Set the required tolerance.
   ///
   /// The iteration will stop after the absolute difference
   /// between two consequative guesses is below this value.
   ///
   /// \param tolerance Tolerance for convergence
   ///
   ////////////////////////////////////////////////////////////
   void SetTolerance(double tolerance);

   ////////////////////////////////////////////////////////////
   /// \brief Evaluate Kepler's Equation for a type of orbit.
   ///
   /// Calculates the orbit-type anomaly of an orbit given the
   /// the eccentricity and mean anomaly by solving
   /// Kepler's Equation. This is a transcedental operation
   /// and is solved by iteration using the Newton-Raphson method.
   /// 
   /// This is a pure virtual function that must be re-implemented
   /// by the derived class.
   ///
   /// \param eccentricity The eccentricity of the orbit
   /// \param meanAnomaly The mean anomaly of the orbit
   /// \returns The anomaly depending on the orbit type
   ///
   ////////////////////////////////////////////////////////////
   virtual double Evaluate(double eccentricity,
                           double meanAnomaly);

protected:
   ////////////////////////////////////////////////////////////
   /// \brief Calculates the initial guess for the orbit-type anomaly.
   ///
   /// The initial guess is used as a starting point for the
   /// Newton-Raphson iteration.
   ///
   /// This is a pure virtual function that must be re-implemented
   /// by the derived class.
   ///
   /// \param eccentricity The eccentricity of the orbit
   /// \param meanAnomaly The mean anomaly of the orbit
   /// \returns The initial guess for the orbit-type anomaly of the orbit
   ///
   ////////////////////////////////////////////////////////////
   virtual double CalculateInitialGuess(double eccentricity, double meanAnomaly) = 0;

   ////////////////////////////////////////////////////////////
   /// \brief Solves the inverse Kepler's Equation for a type of orbit.
   ///
   /// Calculates the mean anomaly of an orbit by solving
   /// the inverse Kepler's Equation. This equation is not
   /// transcendental and does not require iteration.
   ///
   /// This is a pure virtual function that must be re-implemented
   /// by the derived class.
   ///
   /// \param eccentricity The eccentricity of the orbit
   /// \param anomaly The orbit-type anomaly of the orbit
   /// \returns The mean anomaly of the orbit
   ///
   ////////////////////////////////////////////////////////////
   virtual double SolveInverseEquation(double eccentricity, double anomaly) = 0;

   ////////////////////////////////////////////////////////////
   /// \brief Solves the derivative of the inverse Kepler's Equation for a type of orbit. 
   ///
   /// Calculates the derivative of the mean anomaly of an
   /// orbit by solving the derivative of the inverse Kepler's
   /// Equation. This equation is not transcendental and
   /// does not require iteration.
   ///
   /// \param eccentricity The eccentricity of the orbit
   /// \param anomaly The orbit-type anomaly of the orbit
   /// \returns The derivative of the mean anomaly of the orbit
   ///
   ////////////////////////////////////////////////////////////
   virtual double SolveInverseDerivative(double eccentricity, double anomaly) = 0;

protected:
   int m_maxIterations; ///< Maximum number of iteration attempts
   double m_tolerance;  ///< Tolerance for convergence
};

////////////////////////////////////////////////////////////
/// \class otl::keplerian::IKeplersEquation
/// \ingroup keplerian
///
/// Interface class for all Kepler's Equations.  
///
/// This class is an abstract base class and cannot be instantiated.
/// 
////////////////////////////////////////////////////////////

class OTL_CORE_API KeplersEquationElliptical : public IKeplersEquation
{
public:
   ////////////////////////////////////////////////////////////
   /// \brief Constructor
   ///
   /// Creates an instance of Kepler's Equation for elliptical
   /// orbits and optionally sets the iteration limits.
   ///
   /// \param maxIterations Maximum number of iteration attempts
   /// \param tolerance Tolerance for convergence
   ///
   ////////////////////////////////////////////////////////////
   KeplersEquationElliptical(int maxIterations = 1000, double tolerance = MATH_TOLERANCE);

   ////////////////////////////////////////////////////////////
   /// \brief Evaluate Kepler's Equation for elliptical orbits.
   ///
   /// Calculates the eccentric anomaly of an orbit given the
   /// the eccentricity and mean anomaly by solving
   /// Kepler's Equation. This is a transcedental operation
   /// and is solved by iteration using the Newton-Raphson method.
   ///
   /// Kepler's Equation for elliptical orbits is defined as:
   ///
   /// \f$ M = E - e\sin(E) \f$
   ///
   /// where M is the mean anomaly, E is the eccentric anomaly,
   /// and e is the eccentricity.
   ///
   /// \param eccentricity The eccentricity of the orbit
   /// \param meanAnomaly The mean anomaly of the orbit
   /// \returns The eccentric anomaly of the orbit
   ///
   ////////////////////////////////////////////////////////////
   inline virtual double Evaluate(double eccentricity,
                                  double meanAnomaly);

protected:
   ////////////////////////////////////////////////////////////
   /// \brief Calculates the initial guess for the eccentric anomaly.
   ///
   /// The initial guess is used as a starting point for the
   /// Newton-Raphson iteration.
   ///
   /// \param eccentricity The eccentricity of the orbit
   /// \param meanAnomaly The mean anomaly of the orbit
   /// \returns The initial guess for the eccentric anomaly of the orbit
   ///
   ////////////////////////////////////////////////////////////
   inline virtual double CalculateInitialGuess(double eccentricity, double meanAnomaly);

   ////////////////////////////////////////////////////////////
   /// \brief Solves the inverse Kepler's Equation for elliptical orbits.
   ///
   /// Calculates the mean anomaly of an orbit by solving
   /// the inverse Kepler's Equation. This equation is not
   /// transcendental and does not require iteration.
   ///
   /// The inverse Kepler's Equation for elliptical orbits is defined as:
   ///
   /// \f$ M = E - e\sin(E) \f$
   ///
   /// where M is the mean anomaly, E is the eccentric anomaly,
   /// and e is the eccentricity.
   ///
   /// \param eccentricity The eccentricity of the orbit
   /// \param eccentricAnomaly The eccentric anomaly of the orbit
   /// \returns The mean anomaly of the orbit
   ///
   ////////////////////////////////////////////////////////////
   inline virtual double SolveInverseEquation(double eccentricity, double eccentricAnomaly);

   ////////////////////////////////////////////////////////////
   /// \brief Solves the derivative of the inverse Kepler's Equation for elliptical orbits.
   ///
   /// Calculates the derivative of the mean anomaly of an
   /// orbit by solving the derivative of the inverse Kepler's
   /// Equation. This equation is not transcendental and
   /// does not require iteration
   ///
   /// The derivative of the inverse Kepler's Equation for elliptical
   /// orbits is defined as:
   ///
   /// \f$ dM = 1 - e\cos(E) \f$
   ///
   /// where dM is the derivative of the mean anomaly,
   /// E is the eccentric anomaly, and e is the eccentricity.
   ///
   /// \param eccentricity The eccentricity of the orbit
   /// \param eccentricAnomaly The eccentric anomaly of the orbit
   /// \returns The derivative of the mean anomaly of the orbit
   ///
   ////////////////////////////////////////////////////////////
   inline virtual double SolveInverseDerivative(double eccentricity, double eccentricAnomaly);
};

////////////////////////////////////////////////////////////
/// \class otl::keplerian::KeplersEquationElliptical
/// \ingroup keplerian
///
/// Implements Kepler's Equation for elliptical orbits.  
/// 
////////////////////////////////////////////////////////////

class OTL_CORE_API KeplersEquationHyperbolic : public IKeplersEquation
{
public:
   ////////////////////////////////////////////////////////////
   /// \brief Constructor
   ///
   /// Creates an instance of Kepler's Equation for hyperbolic
   /// orbits and optionally sets the iteration limits.
   ///
   /// \param maxIterations Maximum number of iteration attempts
   /// \param tolerance Tolerance for convergence
   ///
   ////////////////////////////////////////////////////////////
   KeplersEquationHyperbolic(int maxIterations = 1000, double tolerance = MATH_TOLERANCE);

   ////////////////////////////////////////////////////////////
   /// \brief Evaluate Kepler's Equation for hyperbolic orbits.
   ///
   /// Calculates the hyperbolic anomaly of an orbit given the
   /// the eccentricity and mean anomaly by solving
   /// Kepler's Equation. This is a transcedental operation
   /// and is solved by iteration using the Newton-Raphson method.
   ///
   /// Kepler's Equation for hyperbolic orbits is defined as:
   ///
   /// \f$ M = e\sinh(H) - H \f$
   ///
   /// where M is the mean anomaly, H is the hyperbolic anomaly,
   /// and e is the eccentricity.
   ///
   /// \param eccentricity The eccentricity of the orbit
   /// \param meanAnomaly The mean anomaly of the orbit
   /// \returns The eccentric anomaly of the orbit
   ///
   ////////////////////////////////////////////////////////////
   inline virtual double Evaluate(double eccentricity,
                                  double meanAnomaly);

protected:
   ////////////////////////////////////////////////////////////
   /// \brief Calculates the initial guess for the hyperbolic anomaly.
   ///
   /// The initial guess is used as a starting point for the
   /// Newton-Raphson iteration.
   ///
   /// \param eccentricity The eccentricity of the orbit
   /// \param meanAnomaly The mean anomaly of the orbit
   /// \returns The initial guess for the hyperbolic anomaly of the orbit
   ///
   ////////////////////////////////////////////////////////////
   inline virtual double CalculateInitialGuess(double eccentricity, double meanAnomaly);

   ////////////////////////////////////////////////////////////
   /// \brief Solves the inverse Kepler's Equation for hyperbolic orbits.
   ///
   /// Calculates the mean anomaly of an orbit by solving
   /// the inverse Kepler's Equation. This equation is not
   /// transcendental and does not require iteration.
   ///
   /// The inverse Kepler's Equation for hyperbolic orbits is defined as:
   ///
   /// \f$ M = e\sinh(H) - H \f$
   ///
   /// where M is the mean anomaly, H is the hyperbolic anomaly,
   /// and e is the eccentricity.
   ///
   /// \param eccentricity The eccentricity of the orbit
   /// \param hyperbolicAnomaly The hyperbolic anomaly of the orbit
   /// \returns The mean anomaly of the orbit
   ///
   ////////////////////////////////////////////////////////////
   inline virtual double SolveInverseEquation(double eccentricity, double hyperbolicAnomaly);

   ////////////////////////////////////////////////////////////
   /// \brief Solves the derivative of the inverse Kepler's Equation for hyperbolic orbits.
   ///
   /// Calculates the derivative of the mean anomaly of an
   /// orbit by solving the derivative of the inverse Kepler's
   /// Equation. This equation is not transcendental and
   /// does not require iteration
   ///
   /// The derivative of the inverse Kepler's Equation for hyperbolic
   /// orbits is defined as:
   ///
   /// \f$ dM = e\cosh(H) - 1 \f$
   ///
   /// where dM is the derivative of the mean anomaly,
   /// E is the hyperbolic anomaly, and e is the eccentricity.
   ///
   /// \param eccentricity The eccentricity of the orbit
   /// \param hyperbolicAnomaly The hyperbolic anomaly of the orbit
   /// \returns The derivative of the mean anomaly of the orbit
   ///
   ////////////////////////////////////////////////////////////
   inline virtual double SolveInverseDerivative(double eccentricity, double hyperbolicAnomaly);
};

////////////////////////////////////////////////////////////
/// \class otl::keplerian::KeplersEquationHyperbolic
/// \ingroup keplerian
///
/// Implements Kepler's Equation for hyperbolic orbits.  
/// 
////////////////////////////////////////////////////////////

#include <OTL/Core/KeplersEquations.inl>

} // namespace keplerian

} // namespace otl