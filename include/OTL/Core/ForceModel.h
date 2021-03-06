#pragma once
#include <OTL/Core/Base.h>

#include <OTL/Core/OrbitalBody.h>

namespace otl
{

//class OrbitalBody;
//typedef std::shared_ptr<OrbitalBody> OrbitalBodyPointer;


class OTL_CORE_API Spacecraft
{
public:
	Spacecraft();

    double GetRadiationPressureArea() { return m_RadiationPressureArea; }
    double GetRadiationPressureCoefficient() { return m_RadiationPressureCoefficient; }
    const Vector3d& GetPosition() const { return m_StateVector.position; }

private:
	double m_Mass;
	CartesianStateVector m_StateVector;

	double m_RadiationPressureArea;
    double m_RadiationPressureCoefficient;
};
typedef std::shared_ptr<Spacecraft> SpacecraftPointer;

class OTL_CORE_API IForceModel
{
public:
   IForceModel();

   virtual void Update(const Time& deltaTime) = 0;
   Vector3d GetForces() const;

protected:
   Vector3d m_Forces;
};

class OTL_CORE_API GravityModel : public IForceModel
{
public:
	GravityModel(const OrbitalBodyPointer& orbitalBody);

	void AddExternalBody(const OrbitalBodyPointer& orbitalBody);

   virtual void Update(const Time& deltaTime);

private:
   double m_GravitationalParameter;
   OrbitalBodyPointer m_OrbitalBody;
   std::vector<OrbitalBodyPointer> m_ExternalBodies;
};

class OTL_CORE_API RadiationSource : public OrbitalBody
{
public:
    //RadiationSource(double radiationPressure) : OrbitalBody("RadiationSource", 1.0) { m_RadiationPressure = radiationPressure; }
    virtual ~RadiationSource() {}

    virtual void Update(const Time& deltaTime) {}

    double GetRadiationPressure() const { return m_RadiationPressure; }

private:
    double m_RadiationPressure;
};
typedef std::shared_ptr<RadiationSource> RadiationSourcePointer;

class OTL_CORE_API Sun : public RadiationSource
{
public:
    //Sun() : RadiationSource(1.0) {}

    virtual void Update(const Time& deltaTime) {}
};

class OTL_CORE_API SolarPressureModel : public IForceModel
{
public:
    SolarPressureModel(const RadiationSourcePointer& radiationSource, const SpacecraftPointer& spacecraft);

   virtual void Update(const Time& deltaTime);

private:
   RadiationSourcePointer m_RadiationSource;
	SpacecraftPointer m_Spacecraft;
};

}