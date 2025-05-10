#include <iostream>

#include "G4MTRunManager.hh"
#include "G4UImanager.hh"
#include "G4VisManager.hh"
#include "G4UIExecutive.hh"
#include "G4VisExecutive.hh"
#include "G4Timer.hh"
#include "G4GeometryManager.hh"  
#include "G4VUserPhysicsList"
#include "QGSP_BIC_HP.hh"
#include "QGSP_BERT_HP.hh"

#include "DetConstruct.hh"
#include "action.hh"


G4String outputFileName;

namespace {
    void PrintUsage() {
        G4cerr << " Usage: " << G4endl;
        G4cerr << " ./sim [-m macro] [-p physicsName] [-t nThreads] [-s seed] [-o outputFilePatternName]" << G4endl;
        G4cerr << "   note: this application was defined to run on multi-threaded mode only." << G4endl;
    }
}

int main(int argc, char** argv) {
    G4Timer timer;
    timer.Start();

    
    G4int nThreads = 4;
    G4String macroName = "vis.mac";
    G4String outputFile = "atm";
    G4String physicsName = "QGSP_BIC_HP";
    G4String nSeed = "1";
    G4long seed = G4UIcommand::ConvertToLongInt(nSeed);
    G4VUserPhysicsList* pl = nullptr;

    
    for (G4int i = 1; i < argc; i += 2) {
        G4String arg = argv[i];
        if (i+1 >= argc) {
            PrintUsage();
            return 1;
        }

        if (arg == "-m") {
            macroName = argv[i+1];
        } 
        else if (arg == "-o") {
            outputFile = argv[i+1];
        } 
        else if (arg == "-s") {
            nSeed = argv[i+1];
        } 
        else if (arg == "-p") {
            physicsName = argv[i+1];
        } 
        else if (arg == "-t") {
            nThreads = G4UIcommand::ConvertToInt(argv[i+1]);
        } 
        else {
            PrintUsage();
            return 1;
        }
    }

    
    if (physicsName == "QGSP_BIC_HP") {
        pl = new QGSP_BIC_HP();
    } 
    else if (physicsName == "QGSP_BERT_HP") {
        pl = new QGSP_BERT_HP();
    } 
    else {
        G4cerr << "Invalid physics list specified!" << G4endl;
        G4cerr << "Available options: QGSP_BIC_HP, QGSP_BERT_HP" << G4endl;
        return 1;
    }

    
    G4MTRunManager* runManager = new G4MTRunManager();
    runManager->SetNumberOfThreads(nThreads);
    runManager->SetUserInitialization(new DetConstruct());
    runManager->SetUserInitialization(pl);
    runManager->SetUserInitialization(new MyActionInitialization());

    
    seed = G4UIcommand::ConvertToLongInt(nSeed);
    G4Random::setTheSeed(seed);
    CLHEP::HepRandom::showEngineStatus();

    
    outputFileName = outputFile + "_" + physicsName + "_s" + nSeed + "_t" + 
                    G4UIcommand::ConvertToString(nThreads) + ".hit";
    G4cout << "Output hits will be saved to: " << outputFileName << G4endl;

    
    G4UIExecutive* ui = nullptr;
    if (macroName == "vis.mac") {
        ui = new G4UIExecutive(argc, argv);
    }

    G4VisManager* visManager = new G4VisExecutive();
    visManager->Initialize();

    
    G4UImanager* UImanager = G4UImanager::GetUIpointer();
    UImanager->ApplyCommand("/control/execute " + macroName);

    
    if (ui) {
        ui->SessionStart();
        delete ui;
    }

    
    timer.Stop();
    
    
    G4GeometryManager::GetInstance()->OpenGeometry();
    
    
    delete visManager;
    delete runManager;

    
    G4cout << G4endl << "******************************************" << G4endl;
    G4cout << "Total Real Elapsed Time: " << timer.GetRealElapsed() << G4endl;
    G4cout << "Total System Elapsed Time: " << timer.GetSystemElapsed() << G4endl;
    G4cout << "Total User Elapsed Time: " << timer.GetUserElapsed() << G4endl;
    G4cout << "******************************************" << G4endl;

    return 0;
}