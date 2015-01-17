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

#include <OTL/Core/Conversion.h>
#include <OTL/Core/Transformation.h>

namespace otl
{

////////////////////////////////////////////////////////////
void CalculateCanonicalUnits(double radius, double mu,
                             double& DU, double& TU, double& VU)
{
   double du = radius;                  // distance unit
   double tu = sqrt(du * du * du / mu); // time unit
   double vu = du / tu;                 // velocity unit

   // Compute the conversion factors
   DU = 1.0 / du;
   TU = 1.0 / tu;
   VU = 1.0 / vu;
}

////////////////////////////////////////////////////////////
void ConvertStateVector2OrbitalElements(const StateVector& stateVector,
                                        OrbitalElements& orbitalElements,
                                        double mu)
{
   // Position and velocity
   const Vector3d& R = stateVector.position;
   const Vector3d& V = stateVector.velocity;
   double r = R.GetNorm();
   double v = V.GetNorm();
   double rDotv = R.Dot(V);

   // Specific Angular Momentum
   Vector3d H = R.Cross(V);
   double h = H.GetNorm();

   // Node Vector
   Vector3d N = MATH_UNIT_VEC_K.Cross(H);
   double n = N.GetNorm();

   // Eccentricity
   Vector3d Ecc = (SQR(v) / mu - 1.0 / r) * R - (rDotv / mu) * V;
   double ecc = Ecc.GetNorm();

   // Semimajor axis and semiparameter
   double a, p;
   if (ecc != ASTRO_ECC_PARABOLIC) // non parabolic orbit
   {
      double m = 0.5 * SQR(v) - mu / r;
      a = -0.5 * mu / m;
      p = a * (1.0 - SQR(ecc));
   }
   else
   {
      a = MATH_INFINITY;
      p = SQR(h) / mu;
   }

   // Inclination
   double incl = 0.0;
   if (h > 0.0)
   {
      incl = acos(H.Z() / h);
   }

   // Longitude of the ascending node
   double lan = 0.0;
   if (n > 0.0)
   {
      lan = acos(N.X() / n);
      if (N.Y() < 0.0)
      {
         lan = MATH_2_PI - lan;
      }
   }

   // Argument of Perigee
   double aop = 0.0;
   if (ecc != ASTRO_ECC_CIRCULAR && incl != ASTRO_INCL_EQUATORIAL) // non-circular non-equatorial orbit
   {
      double nDotecc = N.Dot(Ecc);
      aop = acos(nDotecc / n / ecc);
      if (Ecc.Z() < 0.0)
      {
         aop = MATH_2_PI - aop;
      }
   }
   else if (ecc != ASTRO_ECC_CIRCULAR) // non-circular equatorial orbit (line of nodes is undefined)
   {
      aop = acos(Ecc.X() / ecc);
   }

   // True Anomaly
   double ta;
   if (ecc != ASTRO_ECC_CIRCULAR) // non-circular orbit
   {
      double eccDotr = Ecc.Dot(R);
      ta = acos(eccDotr / ecc / r);
      if (rDotv < 0.0)
      {
         ta = MATH_2_PI - ta;
      }
   }
   else if (incl != ASTRO_INCL_EQUATORIAL) // circular non-equatorial orbit
   {
      double nDotr = N.Dot(R);
      double nDotv = N.Dot(V);
      ta = acos(nDotr / n / r);
      if (nDotv > 0.0)
      {
         ta = MATH_2_PI - ta;
      }
   }
   else // circular equatorial orbit (line of nodes is undefined)
   {
      ta = acos(R.X() / r); // true longitude
      if (V.X() > 0.0)
      {
         ta = MATH_2_PI - ta;
      }
   }

   orbitalElements.semiMajorAxis       = a;
   orbitalElements.eccentricity        = ecc;
   orbitalElements.inclination         = incl;
   orbitalElements.argOfPericenter     = aop;
   orbitalElements.lonOfAscendingNode  = lan;
   orbitalElements.trueAnomaly         = ta;
}

////////////////////////////////////////////////////////////
void ConvertOrbitalElements2StateVector(const OrbitalElements& orbitalElements,
                                        StateVector& stateVector,
                                        double mu)
{
   double a    = orbitalElements.semiMajorAxis;
   double ecc  = orbitalElements.eccentricity;
   double incl = orbitalElements.inclination;
   double aop  = orbitalElements.argOfPericenter;
   double lan  = orbitalElements.lonOfAscendingNode;
   double ta   = orbitalElements.trueAnomaly;

   // Calculate the semiparameter.
   double p = a * (1.0 - SQR(ecc));

   // Precompute common trig functions.
   double cosTa = cos(ta);
   double sinTa = sin(ta);

   // Build the state vectors in perifical coordinates.
   Vector3d Rp, Vp;
   Rp.X() = p * cosTa / (1.0 + ecc * cosTa);
   Rp.Y() = p * sinTa / (1.0 + ecc * cosTa);
   Rp.Z() = 0.0;
   Vp.X() = -sqrt(mu / p) * sinTa;
   Vp.Y() =  sqrt(mu / p) * (ecc + cosTa);
   Vp.Z() = 0.0;

   // Transform the state vectors into inertial coordinates.
   TransformPerifocal2Inertial(Rp, incl, aop, lan, stateVector.position);
   TransformPerifocal2Inertial(Vp, incl, aop, lan, stateVector.velocity);
}

////////////////////////////////////////////////////////////
Vector3d ConvertNormalizedSpherical2Cartesian(double magnitude, double normTheta, double normPhi)
{
    double theta = MATH_2_PI * normTheta;
    double phi = acos(2.0 * normPhi - 1.0);
    Vector3d vec({ -sin(phi) * cos(theta), sin(phi) * sin(theta), -cos(phi) });
    return vec * magnitude;
}

////////////////////////////////////////////////////////////
double ConvertTrueAnomaly2Anomaly(double eccentricity, double trueAnomaly)
{
   if (eccentricity < (1.0 - MATH_TOLERANCE))
   {
      return ConvertTrueAnomaly2EccentricAnomaly(eccentricity, trueAnomaly);
   }
   else if (eccentricity > (1.0 + MATH_TOLERANCE))
   {
      return ConvertTrueAnomaly2HyperbolicAnomaly(eccentricity, trueAnomaly);
   }
   else
   {
      return ConvertTrueAnomaly2ParabolicAnomaly(trueAnomaly);
   }
}

////////////////////////////////////////////////////////////
double ConvertTrueAnomaly2EccentricAnomaly(double eccentricity, double trueAnomaly)
{
   double sinE = (sin(trueAnomaly) * sqrt(1.0 - SQR(eccentricity))) /
                 (1.0 + eccentricity * cos(trueAnomaly));
   double cosE = (eccentricity + cos(trueAnomaly)) /
                 (1.0 + eccentricity * cos(trueAnomaly));
   return atan2(sinE, cosE);
   //return 2.0 * atan(sqrt((1.0 - eccentricity) / (1.0 + eccentricity)) * tan(0.5 * trueAnomaly)); // requires quadrant check?
}

////////////////////////////////////////////////////////////
double ConvertTrueAnomaly2HyperbolicAnomaly(double eccentricity, double trueAnomaly)
{
   double sinH = (sin(trueAnomaly) * sqrt(SQR(eccentricity) - 1.0)) /
                 (1.0 + eccentricity * cos(trueAnomaly));
   double cosH = (eccentricity + cos(trueAnomaly)) /
                 (1.0 + eccentricity * cos(trueAnomaly));

   return atanh(sinH / cosH); // requires quadrant check?
   //return 2.0 * atanh(sqrt((eccentricity - 1.0) / (eccentricity + 1.0)) * tan(0.5 * trueAnomaly)); // requires quadrant check?
}

////////////////////////////////////////////////////////////
double ConvertTrueAnomaly2ParabolicAnomaly(double trueAnomaly)
{
   return tan(0.5 * trueAnomaly);
}

////////////////////////////////////////////////////////////
double ConvertAnomaly2TrueAnomaly(double eccentricity, double anomaly)
{
   if (eccentricity < (1.0 - MATH_TOLERANCE))
   {
      return ConvertEccentricAnomaly2TrueAnomaly(eccentricity, anomaly);
   }
   else if (eccentricity > (1.0 + MATH_TOLERANCE))
   {
      return ConvertHyperbolicAnomaly2TrueAnomaly(eccentricity, anomaly);
   }
   else
   {
      return ConvertParabolicAnomaly2TrueAnomaly(anomaly);
   }
}

////////////////////////////////////////////////////////////
double ConvertEccentricAnomaly2TrueAnomaly(double eccentricity, double eccentricAnomaly)
{
   double sinTA = (sin(eccentricAnomaly) * sqrt(1.0 - SQR(eccentricity))) /
                  (1.0 - eccentricity * cos(eccentricAnomaly));
   double cosTA = (cos(eccentricAnomaly) - eccentricity) /
                  (1.0 - eccentricity * cos(eccentricAnomaly));
   return atan2(sinTA, cosTA);
   //return 2.0 * atan(sqrt((1.0 + eccentricity) / (1.0 - eccentricity)) * tan(0.5 * eccentricAnomaly)); // requires quadrant check?
}

////////////////////////////////////////////////////////////
double ConvertHyperbolicAnomaly2TrueAnomaly(double eccentricity, double hyperbolicAnomaly)
{
   double sinTA = (-sinh(hyperbolicAnomaly) * sqrt(SQR(eccentricity) - 1.0)) /
                  (1.0 - eccentricity * cosh(hyperbolicAnomaly));
   double cosTA = (cosh(hyperbolicAnomaly) - eccentricity) /
                  (1.0 - eccentricity * cosh(hyperbolicAnomaly));
   
   return atan2(sinTA, cosTA);
   //return 2.0 * atan(sqrt((eccentricity + 1.0) / (eccentricity - 1.0)) * tanh(0.5 * hyperbolicAnomaly)); // requires quadrant check?
}

////////////////////////////////////////////////////////////
double ConvertParabolicAnomaly2TrueAnomaly(double parabolicAnomaly)
{
   OTL_ERROR() << "ConvertParabolicAnomaly2TrueAnomaly not implemented yet";
   double p = 0, r = 0;
   double sinTA = parabolicAnomaly * (p / r);
   double cosTA = (parabolicAnomaly - r) / r;
   return atan2(sinTA, cosTA);
}

} // namespace otl