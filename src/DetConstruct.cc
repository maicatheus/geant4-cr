#include "DetConstruct.hh"
#include "G4MTRunManager.hh"
#include "PhSp1SD.hh"
#include "G4SDManager.hh"

DetConstruct::DetConstruct(const G4String& material)
    : G4VUserDetectorConstruction(),
      solidWorld(nullptr), solidDetector(nullptr),
      logicWorld(nullptr), logicDetector(nullptr),
      physWorld(nullptr), fScoringVolume(nullptr),
      detectorMessenger(new DetectorMessenger(this))
{
    DefineMaterials();
    SetMaterial(material); // Set initial material
}

DetConstruct::~DetConstruct()
{
    delete detectorMessenger;
}

void DetConstruct::DefineMaterials()
{
    G4NistManager* nist = G4NistManager::Instance();
    G4double energy[2] = {1.239841939*eV/0.9, 1.239841939*eV/0.2};

    // Air
    matAir = nist->FindOrBuildMaterial("G4_AIR");
    G4double rindexAir[2] = {1.0, 1.0};
    G4MaterialPropertiesTable* mptAir = new G4MaterialPropertiesTable();
    mptAir->AddProperty("RINDEX", energy, rindexAir, 2);
    matAir->SetMaterialPropertiesTable(mptAir);

    // CH4
    G4Element* elH = new G4Element("Hydrogen", "H", 1., 1.00794*g/mole);
    G4Element* elC = new G4Element("Carbon", "C", 6., 12.0107*g/mole);
    G4double densityCH4 = 0.000667151*g/cm3;
    matCH4 = new G4Material("CH4", densityCH4, 2, kStateGas, CLHEP::STP_Temperature, 101325*pascal);
    matCH4->AddElement(elC, 1);
    matCH4->AddElement(elH, 4);
    matCH4->GetIonisation()->SetMeanExcitationEnergy(41.7*eV);
    G4double rindexCH4[2] = {1.004, 1.004};
    G4MaterialPropertiesTable* mptCH4 = new G4MaterialPropertiesTable();
    mptCH4->AddProperty("RINDEX", energy, rindexCH4, 2);
    matCH4->SetMaterialPropertiesTable(mptCH4);

    // CO2
    G4Element* elO = new G4Element("Oxygen", "O", 8., 15.9994*g/mole);
    G4double densityCO2 = 0.00184212*g/cm3;
    matCO2 = new G4Material("CO2", densityCO2, 2, kStateGas, 273.15*kelvin, 1.*atmosphere);
    matCO2->AddElement(elC, 1);
    matCO2->AddElement(elO, 2);
    matCO2->GetIonisation()->SetMeanExcitationEnergy(85*eV);
    G4double rindexCO2[2] = {1.0, 1.0};
    G4MaterialPropertiesTable* mptCO2 = new G4MaterialPropertiesTable();
    mptCO2->AddProperty("RINDEX", energy, rindexCO2, 2);
    matCO2->SetMaterialPropertiesTable(mptCO2);

    // Default material
    worldMat = matCO2;
}

G4VPhysicalVolume *DetConstruct::Construct()
{
    G4double xWorld = 10 * km;
    G4double yWorld = 10 * km;
    G4double zWorld = 10 * km;

    solidWorld = new G4Box("solidWorld", xWorld, yWorld, zWorld);

    logicWorld = new G4LogicalVolume(solidWorld, worldMat, "logicWorld");

    physWorld = new G4PVPlacement(0, G4ThreeVector(0., 0., 0.), logicWorld, "physWorld", nullptr, false, 0, true);

    solidDetector = new G4Box("solidDetector", xWorld - xWorld / 100.0, yWorld - yWorld / 100.0, 1.0 * m);

    logicDetector = new G4LogicalVolume(solidDetector, worldMat, "logicDetector");

    for (G4int k = 0; k < 20; k++)
    {
        G4double zw = zWorld / 20. * 2 * k - zWorld + zWorld / 20.;

        physDetector[k] = new G4PVPlacement(0, G4ThreeVector(0., 0., zw), logicDetector, "physDetector", logicWorld, false, k, true);
    }

    return physWorld;
}

void DetConstruct::ConstructSDandField()
{
    G4String trackerSDname = "/hitFiles/hit1";
    if (G4SDManager::GetSDMpointer()->FindSensitiveDetector(trackerSDname, 0))
        delete G4SDManager::GetSDMpointer()->FindSensitiveDetector(trackerSDname);

    PhSp1SD *phSp1SD = new PhSp1SD(trackerSDname, "Hit1");
    G4SDManager::GetSDMpointer()->AddNewDetector(phSp1SD);
    SetSensitiveDetector(logicDetector, phSp1SD);
}

G4bool DetConstruct::SetMaterial(const G4String &value)
{
    G4Material* newMaterial = nullptr;
    
    if (value == "Air") {
        newMaterial = matAir;
        G4cout << "Material set to Air" << G4endl;
    }
    else if (value == "CH4") {
        newMaterial = matCH4;
        G4cout << "Material set to CH4" << G4endl;
    }
    else if (value == "CO2") {
        newMaterial = matCO2;
        G4cout << "Material set to CO2" << G4endl;
    }
    else {
        G4cerr << "Unknown material: " << value << G4endl;
        return false;
    }

    if (newMaterial && newMaterial != worldMat) {
        worldMat = newMaterial;
        
        if (logicWorld) {
            logicWorld->SetMaterial(worldMat);
            logicDetector->SetMaterial(worldMat);
            G4MTRunManager::GetRunManager()->PhysicsHasBeenModified();
            G4MTRunManager::GetRunManager()->GeometryHasBeenModified();
        }
    }

    return true;
}