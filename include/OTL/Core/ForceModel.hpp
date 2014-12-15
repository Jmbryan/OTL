#pragma once
#include <OTL/Core/Base.hpp>

namespace otl
{

class OrbitalBody;

class IForceModel
{
public:
   IForceModel();

   virtual void Update(const Time& deltaTime) = 0;
   Vector3d GetForces() const;

protected:
   Vector3d m_Forces;
};

class GravityModel : public IForceModel
{
public:
   GravityModel(const std::shared_ptr<OrbitalBody>& orbitalBody);

   void AddExternalBody(const std::shared_ptr<OrbitalBody>& orbitalBody);

   virtual void Update(const Time& deltaTime);

private:
   double m_GravitationalParameter;
   std::shared_ptr<OrbitalBody> m_OrbitalBody;
   std::vector<std::shared_ptr<OrbitalBody>> m_ExternalBodies;
};

class SolarPressureModel : public IForceModel
{
public:
   SolarPressureModel();

   virtual void Update(const Time& deltaTime);
};

}