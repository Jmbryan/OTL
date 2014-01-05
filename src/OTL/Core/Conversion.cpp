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

#include <OTL/Core/Conversion.hpp>
#include <OTL/Core/Transformation.hpp>

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
   double r = R.Magnitude();
   double v = V.Magnitude();

   double rDotv = Vector3d::Dot(R, V);

   // Specific Angular Momentum
   Vector3d H;
   Vector3d::Cross(R, V, H);
   double h = H.Magnitude();

   // Node Vector
   Vector3d N;
   Vector3d::Cross(MATH_UNIT_VEC_K, H, N);
   double n = N.Magnitude();

   // Eccentricity
   Vector3d Ecc = (SQR(v) / mu - 1.0 / r) * R - (rDotv / mu) * V;
   double ecc = Ecc.Magnitude();

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
      incl = acos(H.z / h);
   }

   // Longitude of the ascending node
   double lan = 0.0;
   if (n > 0.0)
   {
      lan = acos(N.x / n);
      if (N.y < 0.0)
      {
         lan = MATH_2_PI - lan;
      }
   }

   // Argument of Perigee
   double aop = 0.0;
   if (ecc != ASTRO_ECC_CIRCULAR && incl != ASTRO_INCL_EQUATORIAL) // non-circular non-equatorial orbit
   {
      double nDotecc = Vector3d::Dot(N, Ecc);
      aop = acos(nDotecc / n / ecc);
      if (Ecc.z < 0.0)
      {
         aop = MATH_2_PI - aop;
      }
   }
   else if (ecc != ASTRO_ECC_CIRCULAR) // non-circular equatorial orbit (line of nodes is undefined)
   {
      aop = acos(Ecc.x / ecc);
   }

   // True Anomaly
   double ta;
   if (ecc != ASTRO_ECC_CIRCULAR) // non-circular orbit
   {
      double eccDotr = Vector3d::Dot(Ecc, R);
      ta = acos(eccDotr / ecc / r);
      if (rDotv < 0.0)
      {
         ta = MATH_2_PI - ta;
      }
   }
   else if (incl != ASTRO_INCL_EQUATORIAL) // circular non-equatorial orbit
   {
      double nDotr = Vector3d::Dot(N, R);
      double nDotv = Vector3d::Dot(N, V);
      ta = acos(nDotr / n / r);
      if (nDotv > 0.0)
      {
         ta = MATH_2_PI - ta;
      }
   }
   else // circular equatorial orbit (line of nodes is undefined)
   {
      ta = acos(R.x / r); // true longitude
      if (V.x > 0.0)
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
   Rp.x = p * cosTa / (1.0 + ecc * cosTa);
   Rp.y = p * sinTa / (1.0 + ecc * cosTa);
   Rp.z = 0.0;
   Vp.x = -sqrt(mu / p) * sinTa;
   Vp.y =  sqrt(mu / p) * (ecc + cosTa);
   Vp.z = 0.0;

   // Transform the state vectors into inertial coordinates.
   TransformPerifocal2Inertial(Rp, incl, aop, lan, stateVector.position);
   TransformPerifocal2Inertial(Vp, incl, aop, lan, stateVector.velocity);
}

////////////////////////////////////////////////////////////
Vector3d ConvertSpherical2Cartesian(double magnitude, double normTheta, double normPhi)
{
    double theta = MATH_2_PI * normTheta;
    double phi = acos(2.0 * normPhi - 1.0);
    Vector3d vec(-sin(phi) * cos(theta), sin(phi) * sin(theta), -cos(phi));
    return vec * magnitude;
}

} // namespace otl