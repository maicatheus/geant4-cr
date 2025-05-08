//-------------------------------------------------------------------------------------------------------
//Simulation of the Varian2100CD head
//Based on the confidential material givend by Varian
//Application developed by Jessica Thauany under the advisement of Gustavo Luis Pereira
//and Gabriela Hoff, conclusion monography of the Physycs Course of the Federal 
//University of the Rio Grande do Sul, Brazil 
//-------------------------------------------------------------------------------------------------------
//$ID: PhantomSD
//-------------------------------------------------------------------------------------------------------
#ifndef PhSp1SD_h
#define PhSp1SD_h 1

#include "G4VSensitiveDetector.hh"

#include "PhSp1Hit.hh"

class G4Step;
class G4HCofThisEvent;

class PhSp1SD : public G4VSensitiveDetector
{
  public:
    PhSp1SD(const G4String& SensitiveDetectorName1, const G4String& hitsCollectionName1);
    virtual ~PhSp1SD();
  
    // methods from base class
    virtual void   Initialize(G4HCofThisEvent*);
    virtual G4bool ProcessHits(G4Step*, G4TouchableHistory*);
    virtual void   EndOfEvent(G4HCofThisEvent*);

  private:
    PhSp1HitsCollection* hitsCollection1;
};

#endif
