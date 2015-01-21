#include <OTL/Core/ForceModel.h>
#include <OTL/Core/OrbitalBody.h>

namespace otl
{

IForceModel::IForceModel()
{

}

Vector3d IForceModel::GetForces() const
{
   return m_Forces;
}

GravityModel::GravityModel(const OrbitalBodyPointer& orbitalBody)
{
   m_OrbitalBody = orbitalBody;
}

void GravityModel::AddExternalBody(const OrbitalBodyPointer& orbitalBody)
{
   m_ExternalBodies.push_back(orbitalBody);
}

void GravityModel::Update(const Time& deltaTime)
{
   m_Forces.Zero();

   double myMass = m_OrbitalBody->GetMass();
   const auto& myPosition = m_OrbitalBody->GetPosition();

   for (const auto& externalBody : m_ExternalBodies)
   {
      double otherMass = externalBody->GetMass();
      const auto& otherPosition = externalBody->GetPosition();

      Vector3d distanceVector = (otherPosition - myPosition);
      double distanceSquared = distanceVector.squaredNorm();
      distanceVector.normalize();

      m_Forces += otherMass / distanceSquared * distanceVector;
   }

   m_Forces *= m_GravitationalParameter * myMass;
}

SolarPressureModel::SolarPressureModel(const RadiationSourcePointer& radiationSource, const SpacecraftPointer& spacecraft)
{
    m_RadiationSource = radiationSource;
	m_Spacecraft = spacecraft;
}

void SolarPressureModel::Update(const Time& deltaTime)
{
    const auto& sourcePosition = m_RadiationSource->GetPosition();
    const auto& radiationPressure = m_RadiationSource->GetRadiationPressure();

    const auto& myPosition = m_Spacecraft->GetPosition();
    const auto& radiationPressureArea = m_Spacecraft->GetRadiationPressureArea();
    const auto& radiationPressureCoefficient = m_Spacecraft->GetRadiationPressureCoefficient();

    Vector3d incidentVector = sourcePosition - myPosition;
    incidentVector.normalize();

    m_Forces = radiationPressure *
               radiationPressureCoefficient *
               radiationPressureArea *
               incidentVector;
}

}