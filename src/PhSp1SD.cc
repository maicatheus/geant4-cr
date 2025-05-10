//-------------------------------------------------------------------------------------------------------
//Simulation of the Varian2100CD head
//Based on the confidential material givend by Varian
//Application developed by Jessica Thauany under the advisement of Gustavo Luis Pereira
//and Gabriela Hoff, conclusion monography of the Physycs Course of the Federal 
//University of the Rio Grande do Sul, Brazil 
//-------------------------------------------------------------------------------------------------------
//$ID: PhSp1SD
//-------------------------------------------------------------------------------------------------------
#include "PhSp1SD.hh"
#include "G4HCofThisEvent.hh"
#include "G4Step.hh"
#include "G4ThreeVector.hh"
#include "G4SDManager.hh"
#include "G4ios.hh"
#include "G4RunManager.hh" 
#include "G4Gamma.hh"

PhSp1SD::PhSp1SD(const G4String& name1, const G4String& hitsCollectionName1) 
 : G4VSensitiveDetector(name1),
   hitsCollection1(NULL){
   collectionName.insert(hitsCollectionName1);
}

PhSp1SD::~PhSp1SD(){
}

void PhSp1SD::Initialize(G4HCofThisEvent* hce1){
// Create hits collection
hitsCollection1 = new PhSp1HitsCollection(SensitiveDetectorName, collectionName[0]); 

// Add this collection in hce
G4int hcID1 = G4SDManager::GetSDMpointer()->GetCollectionID(collectionName[0]);
hce1->AddHitsCollection(hcID1, hitsCollection1); 
}

G4bool PhSp1SD::ProcessHits(G4Step* aStep, G4TouchableHistory*)
{  
   
   if (aStep->GetTrack()->GetDefinition() != G4Gamma::GammaDefinition())
   {
      return false;  
   }

   PhSp1Hit* phsp1Hit = new PhSp1Hit();
  
   phsp1Hit->SetTrackID(aStep->GetTrack()->GetTrackID());
   phsp1Hit->SetParentID(aStep->GetTrack()->GetParentID());
   phsp1Hit->SetEventID(G4RunManager::GetRunManager()->GetCurrentEvent()->GetEventID());
   phsp1Hit->SetPosition(aStep->GetPostStepPoint()->GetPosition());
   phsp1Hit->SetMomDirection(aStep->GetTrack()->GetMomentumDirection());
   phsp1Hit->SetEdep(aStep->GetTotalEnergyDeposit());
   phsp1Hit->SetEkin(aStep->GetTrack()->GetKineticEnergy());
   phsp1Hit->SetParticleName(aStep->GetTrack()->GetParticleDefinition()->GetParticleName());
   phsp1Hit->SetProcess(aStep->GetPostStepPoint()->GetProcessDefinedStep()->GetProcessName());
   phsp1Hit->SetCopyNmb(aStep->GetPreStepPoint()->GetTouchableHandle()->GetCopyNumber());

   hitsCollection1->insert(phsp1Hit);

   phsp1Hit->Print();

   return true;
}

void PhSp1SD::EndOfEvent(G4HCofThisEvent*) {
}

