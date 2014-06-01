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

namespace keplerian
{

////////////////////////////////////////////////////////////
/// \brief Basic component of a MGADSMTrajectory itinerary.
/// \ingroup keplerian
///
/// Interface class for all trajectory nodes
/// 
////////////////////////////////////////////////////////////
struct TrajectoryNode
{
   ////////////////////////////////////////////////////////////
   /// \brief Types of TrajectoryNodes
   ////////////////////////////////////////////////////////////
   enum class Type
   {
      Invalid = -1,  ///< Invalid node type
      Departure,     ///< DepartureNode
      DSM,           ///< DSMNode
      Flyby,         ///< FlybyNode
      Rendezvous,    ///< RendezvousNode
      Insertion,     ///< InsertionNode
      Count          ///< Number of node types
   };

   ////////////////////////////////////////////////////////////
   /// \brief Default constructor
   ////////////////////////////////////////////////////////////
   TrajectoryNode();

   ////////////////////////////////////////////////////////////
   /// \brief Default destructor
   ////////////////////////////////////////////////////////////
   virtual ~TrajectoryNode();  

   ////////////////////////////////////////////////////////////
   /// \brief Get the type of TrajectoryNode
   ///
   /// \returns A TrajectoryNode::Type that characterizes this node
   ///
   ////////////////////////////////////////////////////////////
   Type GetType() const;

protected:
   ////////////////////////////////////////////////////////////
   /// \brief Set the type of TrajectoryNode
   ///
   /// This function should only be called by the derived nodes
   /// during their creation.
   ///
   /// \param _type TrajectoryNode::Type that characterizes this node
   ///
   ////////////////////////////////////////////////////////////
   void SetType(Type _type);

private:
   Type type;  ///< Type of node
};
typedef std::shared_ptr<TrajectoryNode> TrajectoryNodePtr; ///< Convenience alias for TrajectoryNode smart pointer

////////////////////////////////////////////////////////////
/// \brief Contains the departure conditions of the trajectory.
/// \ingroup keplerian
///
/// A DepartureNode must be and can only be the first node of
/// a trajectory.
/// 
////////////////////////////////////////////////////////////
struct DepartureNode : public TrajectoryNode
{
   std::string orbitalBody;   ///< Name of departure orbital body
   Epoch epoch;               ///< Epoch representing the departure date
   Vector3d deltaV;           ///< Relative deltaV vector for escaping departure orbital body 

   ////////////////////////////////////////////////////////////
   /// \brief Default constructor
   ////////////////////////////////////////////////////////////
   DepartureNode();

   ////////////////////////////////////////////////////////////
   /// \brief Constructor
   ///
   /// \param _orbitalBody Name of departure orbital body
   /// \param _epoch Epoch representing the departure date
   /// \param _deltaV Relative deltaV vector for escaping departure orbital body
   ///
   ////////////////////////////////////////////////////////////
   DepartureNode(const std::string& _orbitalBody,
                 Epoch _epoch = 0.0, const Vector3d& _deltaV = Vector3d());
};

////////////////////////////////////////////////////////////
/// \brief Contains the flyby conditions for a particular flyby of the trajectory.
/// \ingroup keplerian
///
/// There may be multiple FlybyNodes in the trajectory.
///
////////////////////////////////////////////////////////////
struct FlybyNode : public TrajectoryNode
{
   std::string orbitalBody;   ///< Name of departure orbital body
   double timeOfFlight;       ///< Total time of flight between previous TrajectoryNode and the flyby
   double altitude;           ///< Minimum altitude of the flyby hyperbola
   double bInclinationAngle;  ///< Defines the orientation of the flyby hyperbola

   ////////////////////////////////////////////////////////////
   /// \brief Default constructor
   ////////////////////////////////////////////////////////////
   FlybyNode();

   ////////////////////////////////////////////////////////////
   /// \brief Constructor
   ///
   /// \param _orbitalBody Name of flyby orbital body
   /// \param _timeOfFlight Total time of flight between previous TrajectoryNode and the flyby
   /// \param _altitude Minimum altitude of the flyby hyperbola
   /// \param _bInclinationAngle Defines the orientation of the flyby hyperbola
   ///
   ////////////////////////////////////////////////////////////
   FlybyNode(const std::string& _orbitalBody,
             double _timeOfFlight = 0.0, double _altitude = 0.0, double _bInclinationAngle = 0.0);
};

////////////////////////////////////////////////////////////
/// \brief Contains the DSM conditions for a particular flyby of the trajectory.
/// \ingroup keplerian
///
/// There may be multiple DSMNodes in the trajectory.
///
////////////////////////////////////////////////////////////
struct DSMNode : public TrajectoryNode
{
   double alpha;     ///< Fractional time at which the DSM occurs 
   Vector3d deltaV;  ///< Vector3d which represents the relative change deltaV at the DSM

   ////////////////////////////////////////////////////////////
   /// \brief Default constructor
   ////////////////////////////////////////////////////////////
   DSMNode();

   ////////////////////////////////////////////////////////////
   /// \brief Constructor
   ///
   /// \param _alpha Fractional time at which the DSM occurs 
   /// \param _deltaV Vector3d which represents the relative change deltaV at the DSM
   ///
   ////////////////////////////////////////////////////////////
   DSMNode(double _alpha,
           const Vector3d& _deltaV = Vector3d());
};

////////////////////////////////////////////////////////////
/// \brief Contains the rendezvous conditions of the trajectory.
/// \ingroup keplerian
///
/// A RendezvousNode can only be the last node of a trajectory.
///
/// A rendezvous requires only that the spacecraft achieves
/// the same absolute position as the orbital body. The
/// difference in absolute velocity between the spacecraft
/// and the orbital body (relative deltaV) may or may not
/// be important.
/// 
///////////////////////////////////////////////////////////
struct RendezvousNode : public TrajectoryNode
{
   std::string orbitalBody;   ///< Name of rendezvous orbital body
   double timeOfFlight;       ///< Total time of flight between previous TrajectoryNode and the rendezvous

   ////////////////////////////////////////////////////////////
   /// \brief Default constructor
   ////////////////////////////////////////////////////////////
   RendezvousNode();

   ////////////////////////////////////////////////////////////
   /// \brief Constructor
   ///
   /// \param _orbitalBody Name of rendezvous orbital body
   /// \param _timeOfFlight Total time of flight between previous TrajectoryNode and the rendezvous
   ///
   ////////////////////////////////////////////////////////////
   RendezvousNode(const std::string& _orbitalBody,
                  double _timeOfFlight = 0.0);
};

////////////////////////////////////////////////////////////
/// \brief Contains the insertion conditions of the trajectory.
/// \ingroup keplerian
///
/// An InsertionNode requires that the spacecraft achieves
/// the same absolute position as the orbital body and inserts
/// into a parking orbit about the orbital body.
/// 
///////////////////////////////////////////////////////////
struct InsertionNode : public TrajectoryNode
{
   std::string orbitalBody;         ///< Name of insertion orbital body
   OrbitalElements orbitalElements; ///< OrbitalElements that define the parking orbit
   double timeOfFlight;             ///< Total time of flight between previous TrajectoryNode and the insertion
   double timeOfOrbit;              ///< Total time to remain in parking orbit
   Vector3d deltaV;                 ///< Escape relative velocity vector after staying in parking orbit

   ////////////////////////////////////////////////////////////
   /// \brief Default constructor
   ////////////////////////////////////////////////////////////
   InsertionNode();

   ////////////////////////////////////////////////////////////
   /// \brief Constructor
   ///
   /// \param _orbitalBody Name of insertion orbital body
   /// \param _orbitalElements OrbitalElements that define the parking orbit
   /// \param _timeOfFlight Total time of flight between previous TrajectoryNode and the insertion
   /// \param _timeOfOrbit Total time to remain in parking orbit
   /// \param _deltaV Escape relative velocity vector after staying in parking orbit
   ///
   ////////////////////////////////////////////////////////////
   InsertionNode(const std::string& _orbitalBody, const OrbitalElements& _orbitalElements,
                 double _timeOfFlight = 0.0, double _timeOfOrbit = 0.0, const Vector3d& _deltaV = Vector3d());
};

#include <OTL/Core/MGADSMNodes.inl>

} // namespace keplerian

} // namespace otl