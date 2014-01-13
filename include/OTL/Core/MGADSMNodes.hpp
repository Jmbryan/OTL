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
#include <OTL/Core/Base.hpp>
#include <OTL/Core/Epoch.hpp>

namespace otl
{

struct TrajectoryNode
{
   enum class Type
   {
      Invalid = -1,
      Departure,
      DSM,
      Flyby,
      Rendezvous,
      Insertion,
      Count
   };

   TrajectoryNode();
   virtual ~TrajectoryNode();  

   Type GetType() const;

protected:
   void SetType(Type _type);

private:
   Type type;
};
typedef std::shared_ptr<TrajectoryNode> TrajectoryNodePtr;

struct DepartureNode : public TrajectoryNode
{
   std::string orbitalBody;
   Epoch epoch;
   Vector3d deltaV;

   DepartureNode();
   DepartureNode(const std::string& _orbitalBody,
                 Epoch _epoch = 0.0, const Vector3d& _deltaV = Vector3d());
};

struct FlybyNode : public TrajectoryNode
{
   std::string orbitalBody;
   double timeOfFlight;
   double altitude;
   double bInclinationAngle;

   FlybyNode();
   FlybyNode(const std::string& _orbitalBody,
             double _timeOfFlight = 0.0, double _altitude = 0.0, double _bInclinationAngle = 0.0);
};

struct DSMNode : public TrajectoryNode
{
   double alpha;
   Vector3d deltaV;

   DSMNode();
   DSMNode(double _alpha,
           const Vector3d& _deltaV = Vector3d());
};

struct RendezvousNode : public TrajectoryNode
{
   std::string orbitalBody;
   double timeOfFlight;

   RendezvousNode();
   RendezvousNode(const std::string& _orbitalBody,
                  double _timeOfFlight = 0.0);
};

struct InsertionNode : public TrajectoryNode
{
   std::string orbitalBody;
   OrbitalElements orbitalElements;
   double timeOfFlight;
   double timeOfOrbit;
   Vector3d deltaV;

   InsertionNode();
   InsertionNode(const std::string& _orbitalBody, const OrbitalElements& _orbitalElements,
                 double _timeOfFlight = 0.0, double _timeOfOrbit = 0.0, const Vector3d& _deltaV = Vector3d());
};

#include <OTL/Core/MGADSMNodes.inl>

} // namespace otl