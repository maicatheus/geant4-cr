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

class DetConstruct : public G4VUserDetectorConstruction
{
public:
    DetConstruct();
    virtual ~DetConstruct();

    // Getters
    G4LogicalVolume *GetScoringVolume() const { return fScoringVolume; }

    // Overridden Methods
    virtual G4VPhysicalVolume *Construct();
    virtual void ConstructSDandField();

    // Set Material
    G4bool SetMaterial(const G4String &value);

private:
    // Geometries
    G4Box *solidWorld;
    G4Box *solidDetector;
    G4LogicalVolume *logicWorld;
    G4LogicalVolume *logicDetector;
    G4VPhysicalVolume *physWorld;
    G4VPhysicalVolume *physDetector[20]; // Suporte para até 20 detectores

    // Materials
    G4Material *worldMat;       // Material padrão do mundo
    G4Material *matCH4Global;   // CH4 global
    G4Material *matCO2Global;   // CO2 global
    G4Material *matO2;          // Oxigênio
    G4Material *matN2;          // Nitrogênio

    // Elements
    G4Element *C;               // Carbono
    G4Element *H;               // Hidrogênio
    G4Element *O;               // Oxigênio
    G4Element *N;               // Nitrogênio

    // Scoring Volume
    G4LogicalVolume *fScoringVolume;

    // Utilities
    G4GenericMessenger *fMessenger; // Messenger para parâmetros
    DetectorMessenger *detectorMessenger; // Messenger personalizado

    // Variables
    G4double zw;               // Variável para coordenada Z
    G4int nCols;               // Número de colunas (se necessário)
    G4int nRows;               // Número de linhas (se necessário)

    // Private Methods
    void DefineMaterials();    // Define os materiais do detector
};

#endif
