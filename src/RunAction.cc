#include "RunAction.hh"

#include "G4Run.hh"
#include "G4RunManager.hh"
 
RunAction::RunAction(){
   // set printing event number per each 100 events
   //G4RunManager::GetRunManager()->SetPrintProgress(1000);
}

void RunAction::BeginOfRunAction(const G4Run*){
   //inform the runManager to save random number seed
   //G4RunManager::GetRunManager()->SetRandomNumberStore(false);
}

void RunAction::EndOfRunAction(const G4Run* )
{}

