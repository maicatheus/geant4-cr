//-------------------------------------------------------------------------------------------------------
//Simulation of the Varian2100CD head
//Based on the confidential material givend by Varian
//Application developed by Jessica Thauany under the advisement of Gustavo Luis Pereira
//and Gabriela Hoff, conclusion monography of the Physycs Course of the Federal 
//University of the Rio Grande do Sul, Brazil 
//-------------------------------------------------------------------------------------------------------
//$ID: PhSp1Hit
//-------------------------------------------------------------------------------------------------------
#ifndef PhSp1Hit_h
#define PhSp1Hit_h 1

#include "G4VHit.hh"
#include "G4THitsCollection.hh"
#include "G4Allocator.hh"
#include "G4ThreeVector.hh"
#include "tls.hh"

class PhSp1Hit : public G4VHit{
  public:
    PhSp1Hit();
    PhSp1Hit(const PhSp1Hit&);
    virtual ~PhSp1Hit();

    // operators
    const PhSp1Hit& operator=(const PhSp1Hit&);
    G4bool operator==(const PhSp1Hit&) const;

    inline void* operator new(size_t);
    inline void  operator delete(void*);

    // methods from base class
    virtual void Draw();
    virtual void Print();

    // Set methods
    void SetTrackID(G4int track){ trackID = track; };
    void SetParentID(G4int parent){ parentID = parent; };
    void SetEventID(G4int event){ eventID = event; };
    void SetPosition(G4ThreeVector posXYZ){ position = posXYZ; };
    void SetMomDirection(G4ThreeVector momXYZ){ momDirection = momXYZ; };
    void SetEdep(G4double de){ eDep = de; };
    void SetEkin(G4double kin){ eKin = kin; };
    void SetParticleName(G4String particleName){ particle = particleName; };
    void SetProcess(G4String processName){ process = processName; };
    void SetCopyNmb(G4int copynmb){ copyNmb = copynmb; };

    // Get methods
    G4int GetTrackID() const { return trackID; };
    G4int GetParentID() const { return parentID; };
    G4int GetEventID() const { return eventID; };
    G4ThreeVector GetPosition() const { return position; };
    G4ThreeVector GetMomDirection() const { return momDirection; };
    G4double GetEdep() const { return eDep; };
    G4double GetEkin() const { return eKin; };
    G4String GetParticleName() const { return particle; };
    G4String GetProcessName() const { return process; };
    G4int GetCopyNmb() const { return copyNmb; };
    
  private:
      G4String 		nameHit1;
      G4int         	trackID;
      G4int         	parentID;
      G4int         	eventID;
      G4ThreeVector 	position;
      G4ThreeVector 	momDirection;
      G4double      	eDep;
      G4double      	eKin;
      G4String      	particle;
      G4String      	process;
      G4int		copyNmb;

};

typedef G4THitsCollection<PhSp1Hit> PhSp1HitsCollection;

extern G4ThreadLocal G4Allocator<PhSp1Hit>* PhSp1HitAllocator;

inline void* PhSp1Hit::operator new(size_t){
  if(!PhSp1HitAllocator)
      PhSp1HitAllocator = new G4Allocator<PhSp1Hit>;
  return (void *) PhSp1HitAllocator->MallocSingle();
}

inline void PhSp1Hit::operator delete(void *hit){
  PhSp1HitAllocator->FreeSingle((PhSp1Hit*) hit);
}

#endif
