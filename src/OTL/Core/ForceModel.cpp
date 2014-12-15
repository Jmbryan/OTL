#include <OTL/Core/ForceModel.hpp>
#include <OTL/Core/OrbitalBody.hpp>

namespace otl
{

IForceModel::IForceModel()
{

}

Vector3d IForceModel::GetForces() const
{
   return m_Forces;
}

GravityModel::GravityModel(const std::shared_ptr<OrbitalBody>& orbitalBody)
{
   m_OrbitalBody = orbitalBody;
}

void GravityModel::AddExternalBody(const std::shared_ptr<OrbitalBody>& orbitalBody)
{
   m_ExternalBodies.push_back(orbitalBody);
}

void GravityModel::Update(const Time& deltaTime)
{
   m_Forces.Zero();

   double myMass = m_OrbitalBody->GetMass();
   const auto& myPosition = m_OrbitalBody->GetPosition();

   for (auto& externalBody : m_ExternalBodies)
   {
      double otherMass = externalBody->GetMass();
      const auto& otherPosition = externalBody->GetPosition();

      Vector3d distanceVector = (otherPosition - myPosition);
      double distanceSquared = std::pow(distanceVector.Magnitude(), 2.0);
      distanceVector.Normalize();

      m_Forces += otherMass / distanceSquared * distanceVector;
   }

   m_Forces *= m_GravitationalParameter * myMass;
}


SolarPressureModel::SolarPressureModel()
{

}

void SolarPressureModel::Update(const Time& deltaTime)
{

}

}