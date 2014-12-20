#pragma once
#include <OTL/Core/Base.hpp>

// temporary - should be in .cpp
#include <OTL/Core/EphemerisDatabase.h>

namespace otl
{

//class IEphemerisDatabase;
//typedef std::shared_ptr<IEphemerisDatabase> EphemerisDatabasePointer;

class IEphemerisDatabaseFactory;
typedef std::shared_ptr<IEphemerisDatabaseFactory> EphemerisDatabaseFactoryPointer;

class IEphemerisDatabaseFactory
{
public:
   IEphemerisDatabaseFactory() {}
   virtual ~IEphemerisDatabaseFactory() {}

   //static EphemerisDatabaseFactoryPointer GetInstance()
   //{

   //}

   virtual EphemerisDatabasePointer CreateEphemerisDatabase(const std::string& name) = 0;
};

class EphemerisDatabaseFactory : public IEphemerisDatabaseFactory
{
public:
   EphemerisDatabaseFactory() {}
   virtual ~EphemerisDatabaseFactory() {}

   virtual EphemerisDatabasePointer CreateEphemerisDatabase(const std::string& name)
   {
      EphemerisDatabasePointer ephemerisDatabase;
      if (name == "JplApproximateEphemeris")
      {
         //ephemerisDatabase.reset(new JplApproximateEphemerisDatabase());
      }
      else if (name == "JplEphemeris")
      {
         ephemerisDatabase = JplEphemerisDatabase::GetInstance();
         //ephemerisDatabase.reset(new JplEphemerisDatabase());
      }
      else if (name == "SpiceEphemeris")
      {
         //ephemerisDatabase.reset(new SpiceEphemerisDatabase());
      }
      else
      {
         //throw InvalidArgumentException("Name not valid");
      }
      return ephemerisDatabase;
   }
};

} // namespace otl