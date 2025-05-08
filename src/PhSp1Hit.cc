//-------------------------------------------------------------------------------------------------------
// Simulation of the Varian2100CD head
// Based on the confidential material givend by Varian
// Application developed by Jessica Thauany under the advisement of Gustavo Luis Pereira
// and Gabriela Hoff, conclusion monography of the Physycs Course of the Federal
// University of the Rio Grande do Sul, Brazil
//-------------------------------------------------------------------------------------------------------
//$ID: PhSp1Hit
//-------------------------------------------------------------------------------------------------------
#include "PhSp1Hit.hh"
#include "G4UnitsTable.hh"
#include "G4VVisManager.hh"
#include "G4Circle.hh"
#include "G4Colour.hh"
#include "G4VisAttributes.hh"
#include "G4SystemOfUnits.hh"

#include "DetConstruct.hh"

#include <fstream>
#include <iomanip>

extern G4String outputFileName;

G4ThreadLocal G4Allocator<PhSp1Hit> *PhSp1HitAllocator = nullptr;

PhSp1Hit::PhSp1Hit()
    : G4VHit(),
      nameHit1(outputFileName),
      trackID(-1),
      parentID(-1),
      eventID(-1),
      position(G4ThreeVector()),
      momDirection(G4ThreeVector()),
      eDep(0.),
      eKin(0.),
      particle("none"),
      process("none")
{
}

PhSp1Hit::~PhSp1Hit()
{
}

PhSp1Hit::PhSp1Hit(const PhSp1Hit &right)
    : G4VHit()
{
   trackID = right.trackID;
   parentID = right.parentID;
   eventID = right.eventID;
   position = right.position;
   momDirection = right.momDirection;
   eDep = right.eDep;
   eKin = right.eKin;
   particle = right.particle;
   process = right.process;
   copyNmb = right.copyNmb;
}

const PhSp1Hit &PhSp1Hit::operator=(const PhSp1Hit &right)
{
   trackID = right.trackID;
   parentID = right.parentID;
   eventID = right.eventID;
   position = right.position;
   momDirection = right.momDirection;
   eDep = right.eDep;
   eKin = right.eKin;
   particle = right.particle;
   process = right.process;
   copyNmb = right.copyNmb;

   return *this;
}

G4bool PhSp1Hit::operator==(const PhSp1Hit &right) const
{
   return (this == &right) ? true : false;
}

void PhSp1Hit::Draw()
{
}

//Colocar bi slide 13
void PhSp1Hit::Print(){
   std::fstream outPhSp1;
   outPhSp1.open(nameHit1, std::fstream::in | std::fstream::out | std::fstream::app);

   if (outPhSp1.is_open())   {
      outPhSp1 << trackID << " " << parentID << " " << eventID
               << " " << position.getX() / cm << " " << position.getY() / cm << " " << position.getZ() / cm
               << " " << momDirection.getX() << " " << momDirection.getY() << " " << momDirection.getZ()
               << " " << eDep / MeV << " " << eKin / MeV
               << " " << particle << " " << process << " " << copyNmb
               << G4endl;
      outPhSp1.close();
   }
}
