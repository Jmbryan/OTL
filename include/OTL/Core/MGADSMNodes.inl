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

///////////////////////////////////////////////////////////////////////////////////
inline TrajectoryNode::TrajectoryNode() :
type(Type::Invalid)
{
}

///////////////////////////////////////////////////////////////////////////////////
inline TrajectoryNode::~TrajectoryNode()
{
}

///////////////////////////////////////////////////////////////////////////////////
inline TrajectoryNode::Type TrajectoryNode::GetType() const
{
   return type;
}

///////////////////////////////////////////////////////////////////////////////////
inline void TrajectoryNode::SetType(Type _type)
{
   OTL_ASSERT(_type > Type::Invalid && _type < Type::Count, "Invalid type.");
   type = _type;
}

///////////////////////////////////////////////////////////////////////////////////
inline DepartureNode::DepartureNode()
{
   SetType(Type::Departure);
}

///////////////////////////////////////////////////////////////////////////////////
inline DepartureNode::DepartureNode(const std::string& _orbitalBody,
                                    Epoch _epoch, const Vector3d& _deltaV) :
orbitalBody(_orbitalBody),
epoch(_epoch),
deltaV(_deltaV)
{
   SetType(Type::Departure);
}

///////////////////////////////////////////////////////////////////////////////////
inline FlybyNode::FlybyNode()
{
   SetType(Type::Flyby);
}

///////////////////////////////////////////////////////////////////////////////////
inline FlybyNode::FlybyNode(const std::string& _orbitalBody, double _timeOfFlight,
                            double _altitude, double _bInclinationAngle) :
orbitalBody(_orbitalBody),
timeOfFlight(_timeOfFlight),
altitude(_altitude),
bInclinationAngle(_bInclinationAngle)
{
   SetType(Type::Flyby);  
}

///////////////////////////////////////////////////////////////////////////////////
inline DSMNode::DSMNode()
{
   SetType(Type::DSM);
}

///////////////////////////////////////////////////////////////////////////////////
inline DSMNode::DSMNode(double _alpha,
                        const Vector3d& _deltaV) :
alpha(_alpha),
deltaV(_deltaV)
{
   SetType(Type::DSM);
}

///////////////////////////////////////////////////////////////////////////////////
inline RendezvousNode::RendezvousNode()
{
   SetType(Type::Rendezvous);
}

///////////////////////////////////////////////////////////////////////////////////
inline RendezvousNode::RendezvousNode(const std::string& _orbitalBody,
                                      double _timeOfFlight) :
orbitalBody(_orbitalBody),
timeOfFlight(_timeOfFlight)
{
   SetType(Type::Rendezvous);   
}

///////////////////////////////////////////////////////////////////////////////////
inline InsertionNode::InsertionNode()
{
   SetType(Type::Insertion);
}

///////////////////////////////////////////////////////////////////////////////////
inline InsertionNode::InsertionNode(const std::string& _orbitalBody, const OrbitalElements& _orbitalElements,
                                    double _timeOfFlight, double _timeOfOrbit, const Vector3d& _deltaV) :
orbitalBody(_orbitalBody),
orbitalElements(_orbitalElements),
timeOfFlight(_timeOfFlight),
timeOfOrbit(_timeOfOrbit)
{
   SetType(Type::Insertion);
}