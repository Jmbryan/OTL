#pragma once
#include <OTL/Core/Base.hpp>
#include <mutex>

// temporary - should be in .cpp
#include <OTL/Core/Epoch.hpp>
#include <OTL/Core/Conversion.hpp>
#include <niek-ephem/DE405Ephemeris.h>

typedef DE405Ephemeris JplEphemerisDB;

namespace otl
{

class IEphemerisDatabase;
typedef std::shared_ptr<IEphemerisDatabase> EphemerisDatabasePointer;

class IEphemerisDatabase
{
public:
   virtual ~IEphemerisDatabase() {}

   void QueryDatabase(const std::string& name, const Epoch& epoch, StateVector& stateVector)
   {
      std::lock_guard<std::mutex> lock(m_mutex);

      if (!m_initialized)
      {
         Initialize();
      }

      if (IsNameValid(name))
      {
         VQueryDatabase(name, epoch, stateVector);
      }
      else
      {
         //throw InvalidArgumentException("Name not found");
      }
   }

   void QueryDatabase(const std::string& name, const Epoch& epoch, OrbitalElements& orbitalElements)
   {
      std::lock_guard<std::mutex> lock(m_mutex);

      if (!m_initialized)
      {
         Initialize();
      }

      if (IsNameValid(name))
      {
         VQueryDatabase(name, epoch, orbitalElements);
      }
      else
      {
         //throw InvalidArgumentException("Name not found");
      }
   }

   static EphemerisDatabasePointer GetInstance()
   {
      return instance;
   }

protected:
   virtual void VInitialize() = 0;
   virtual bool VIsNameValid(const std::string& name) = 0;
   virtual void VQueryDatabase(const std::string& name, const Epoch& epoch, StateVector& stateVector) = 0;
   virtual void VQueryDatabase(const std::string& name, const Epoch& epoch, OrbitalElements& orbitalElements) = 0;
   
   IEphemerisDatabase() : m_initialized(false) {}
   IEphemerisDatabase(const IEphemerisDatabase& other) = delete;
   IEphemerisDatabase& operator=(const IEphemerisDatabase&) = delete;

   void Initialize()
   {
      VInitialize();
      m_initialized = true;
   }

   bool IsNameValid(const std::string& name)
   {
      return VIsNameValid(name);
   }

private:
   bool m_initialized;
   std::mutex m_mutex;

   static EphemerisDatabasePointer instance;
};

class JplEphemerisDatabase : public IEphemerisDatabase
{
public:
   virtual ~JplEphemerisDatabase() {}

protected:
   JplEphemerisDatabase() : IEphemerisDatabase() {}

   virtual void VInitialize()
   {
      m_ephemeris.reset(new DE405Ephemeris("E:/Dev/OTL/data/jpl_eph/de405/de405.data"));

      m_entityDictionary["Mercury"] = DE405Ephemeris::Mercury;
      m_entityDictionary["Venus"] = DE405Ephemeris::Venus;
      m_entityDictionary["Earth"] = DE405Ephemeris::EarthMoonBarycenter;
      m_entityDictionary["Mars"] = DE405Ephemeris::Mars;
      m_entityDictionary["Jupiter"] = DE405Ephemeris::JupiterBarycenter;
      m_entityDictionary["Saturn"] = DE405Ephemeris::SaturnBarycenter;
      m_entityDictionary["Uranus"] = DE405Ephemeris::UranusBarycenter;
      m_entityDictionary["Neptune"] = DE405Ephemeris::NeptuneBarycenter;
      m_entityDictionary["Pluto"] = DE405Ephemeris::PlutoBarycenter;
      m_entityDictionary["Sun"] = DE405Ephemeris::Sun;
      m_entityDictionary["Moon"] = DE405Ephemeris::Moon;
   }

   virtual bool VIsNameValid(const std::string& name)
   {
      std::map<std::string, DE405Ephemeris::AstroEntity>::const_iterator it = m_entityDictionary.find(name);
      return (it != m_entityDictionary.end());
   }

   virtual void VQueryDatabase(const std::string& name, const Epoch& epoch, StateVector& stateVector)
   {
      DE405Ephemeris::AstroEntity entity = m_entityDictionary[name];

      double pos[3], vel[3];
      m_ephemeris->getPosVel(epoch.GetJD(), entity, pos, vel);

      for (int i = 0; i < 3; ++i)
      {
         stateVector.position[i] = pos[i];
         stateVector.velocity[i] = vel[i] / (24.0 * 60.0 * 60.0);
      }
   }

   virtual void VQueryDatabase(const std::string& name, const Epoch& epoch, OrbitalElements& orbitalElements)
   {
      StateVector stateVector;
      QueryDatabase(name, epoch, stateVector);
      ConvertStateVector2OrbitalElements(stateVector, orbitalElements, ASTRO_MU_SUN);
   }

private:
   std::unique_ptr<JplEphemerisDB> m_ephemeris;
   std::map<std::string, DE405Ephemeris::AstroEntity> m_entityDictionary;
};

} // namespace otl