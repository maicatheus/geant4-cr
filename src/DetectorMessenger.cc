#include "DetectorMessenger.hh"
#include "DetConstruct.hh"
#include "G4UIcmdWithADoubleAndUnit.hh"
#include "G4UIcmdWithAString.hh"
#include "G4UIdirectory.hh"
#include "G4SystemOfUnits.hh"

DetectorMessenger::DetectorMessenger(DetConstruct* _detector)
:detector(_detector)
{

  changeTargetDir = new G4UIdirectory("/setTarget/");
  changeTargetDir -> SetGuidance("Command to set the Target parameters");

  // Change sensitive volume material
  changeMaterial = new G4UIcmdWithAString("/setTarget/material", this);
  changeMaterial->SetGuidance("Set material");
  changeMaterial->SetParameterName("Material",false);
  changeMaterial->SetCandidates("Air CH4");
  changeMaterial->SetDefaultValue("Air");
  changeMaterial->AvailableForStates(G4State_PreInit, G4State_Idle);
}

DetectorMessenger::~DetectorMessenger()
{
  delete changeMaterial;
}

void DetectorMessenger::SetNewValue(G4UIcommand* command,G4String newValue)
{
  if (command == changeMaterial) {
    detector->SetMaterial(newValue);
  }
}
