#ifndef DETCONSTRUCT_HH
#define DETCONSTRUCT_HH

#include "G4VUserDetectorConstruction.hh"
#include "G4VPhysicalVolume.hh"
#include "G4LogicalVolume.hh"
#include "G4Box.hh"
#include "G4PVPlacement.hh"
#include "G4NistManager.hh"
#include "G4SystemOfUnits.hh"
#include "G4GenericMessenger.hh"
#include "DetectorMessenger.hh"

class DetectorMessenger;

class DetConstruct : public G4VUserDetectorConstruction{
public:
    DetConstruct(const G4String& material = "CO2");
    virtual ~DetConstruct();

    virtual G4VPhysicalVolume* Construct() override;
    virtual void ConstructSDandField() override;

    G4bool SetMaterial(const G4String &value);
    G4LogicalVolume* GetScoringVolume() const { return fScoringVolume; }

private:
    void DefineMaterials();
    
    
    G4Box* solidWorld;
    G4Box* solidDetector;
    G4LogicalVolume* logicWorld;
    G4LogicalVolume* logicDetector;
    G4VPhysicalVolume* physWorld;
    G4VPhysicalVolume* physDetector[20]; 

    
    G4Material* worldMat;
    G4Material* matAir;
    G4Material* matCH4;
    G4Material* matCO2;

    
    G4LogicalVolume* fScoringVolume;

    
    DetectorMessenger* detectorMessenger;
};

#endif