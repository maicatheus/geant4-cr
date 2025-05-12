#include <iostream>

#include "G4MTRunManager.hh"
#include "G4UImanager.hh"
#include "G4VisManager.hh"
#include "G4UIExecutive.hh"
#include "G4VisExecutive.hh"
#include "G4Timer.hh"

#include "QGSP_BIC_HP.hh"
#include "QGSP_BERT_HP.hh"

#include "DetConstruct.hh"
#include "action.hh"

// Definição da variável global para o nome do arquivo hit de saída
// Nnão é um aopção elegante, mas resolve no nosso caso
// Pode-se fazer uma sequancia de classes para lidar com esta variável
G4String outputFileName;

namespace
{
	void PrintUsage()
	{
		G4cerr << " Usage: " << G4endl;
		G4cerr << " ./sim [-m macro] [-p physicsName] [-t nThreads] [-s seed] [-o outputFilePatternName]" << G4endl;
		G4cerr << "   note: this application was defined to run on multi-threaded mode only." << G4endl;
	}
}

int main(int argc, char **argv)
{
	G4Timer timer;
	timer.Start();

	G4UIExecutive *ui = 0;

	if (argc > 11)
	{
		PrintUsage();
		return 1;
	}

	G4int nThreads = 4;
	G4String nThreadsComment = "The number of Threads defindes by defualt was " + G4UIcommand::ConvertToString(nThreads);
	G4String macroName = "vis.mac";
	G4String macroNameComment = "The macro name defined by default was " + macroName;
	G4String outputFile = "atm";
	G4String outputFileComment = "The ouput file name defined by default was " + outputFile;
	G4String physicsName = "QGSP_BIC_HP";
	G4String physicsNameComment = "The physics invokec by default was " + physicsName;
	G4VUserPhysicsList *pl = 0; // definido variável default para armazenamento da classe se física definida
	G4String nSeed = "1";
	G4long seed = G4UIcommand::ConvertToLongInt(nSeed);
	G4String nSeedComment = "The seed defined by default are " + nSeed;

	// Passando argumentos da linha de comando
	for (G4int i = 1; i < argc; i = i + 2)
	{
		if (G4String(argv[i]) == "-m")
		{ // usuário definindo a maro a ser utilizada
			macroName = argv[i + 1];
			macroNameComment = "The macro name defined by the user was " + macroName;
		}
		else if (G4String(argv[i]) == "-o")
		{ // usuário definido o nome d arquivo de saída
			outputFile = argv[i + 1];
			outputFileComment = "The output file name defined by the user was " + outputFile;
		}
		else if (G4String(argv[i]) == "-s")
		{ // usuário definido sementes a serem utlizadas
			nSeed = argv[i + 1];
			nSeedComment = "The seed defined by the user was " + nSeed;
		}
		else if (G4String(argv[i]) == "-p")
		{ // Usuário definindo a física invocada
			physicsName = (argv[i + 1]);
			physicsNameComment = "The physycs list invoked by user was " + physicsName;
		}
		else if (G4String(argv[i]) == "-t")
		{
			nThreads = G4UIcommand::ConvertToInt(argv[i + 1]);
		}
		else if (G4String(argv[i]) == "-mat") {
    	    materialName = argv[i+1];
    	}
		else
		{
			PrintUsage();
			return 1;
		}
	}

	// Definido run mamnager
	G4MTRunManager *runManager = new G4MTRunManager();
	runManager->SetNumberOfThreads(nThreads);
	runManager->SetUserInitialization(new DetConstruct(materialName));


	// definindo a lista física
	if (physicsName == "QGSP_BIC_HP" || physicsName == "QGSP_BERT_HP")
	{ // Verificando se lista fisica evocada pelo usuário existe
		if (physicsName == "QGSP_BIC_HP")
		{ // definido a lista física evocada
			pl = new QGSP_BIC_HP();
		}
		else if (physicsName == "QGSP_BERT_HP")
		{
			pl = new QGSP_BERT_HP();
		}
	}
	else
	{ // Mensagem de erro para list física não existente no código sendo evocada
		G4cout << "Unappropriate physics list defined by the user!" << G4endl;
		G4cout << "The user must define one of the following physicslist:" << G4endl;
		G4cout << "- QGSP_BIC_HP" << G4endl;
		G4cout << "- QGSP_BERT_HP" << G4endl;
		std::abort();
	}

	// Imprimento definições de rodagens
	G4cout << macroNameComment << G4endl;
	G4cout << outputFileComment << G4endl;
	G4cout << nThreadsComment << G4endl;
	G4cout << nSeedComment << G4endl;
	CLHEP::HepRandom::showEngineStatus();
	G4cout << physicsNameComment << G4endl;

	// outputFileName = outputFile + "_" + physicsName + "_s" + nSeed + "_t" + G4UIcommand::ConvertToString(nThreads) + ".hit";
	outputFileName = "CO2_E0_10_n_100.hit";
	G4cout << "The output file name do save hits was defined as " + outputFileName << G4endl;

	// Instantiate the seed engine
	seed = G4UIcommand::ConvertToLongInt(nSeed);
	G4Random::setTheSeed(seed);
	// G4Random::setTheEngine(new CLHEP::MTwistEngine);
	CLHEP::HepRandom::showEngineStatus();

	runManager->SetUserInitialization(pl);
	runManager->SetUserInitialization(new MyActionInitialization());

	G4VisManager *visManager = new G4VisExecutive();
	visManager->Initialize();

	G4UImanager *UImanager = G4UImanager::GetUIpointer();
	G4String command = "/control/execute ";

	if (macroName == "vis.mac")
	{
		ui = new G4UIExecutive(argc, argv);
		UImanager->ApplyCommand(command + "../" + macroName);
		ui->SessionStart();
		// delete ui;
	}
	else
	{
		UImanager->ApplyCommand(command + macroName);
	}

	// imprimindo o tempo de rodagem
	timer.Stop();
	G4cout << G4endl;
	G4cout << "******************************************";
	G4cout << G4endl;
	G4cout << "Total Real Elapsed Time is: " << timer.GetRealElapsed();
	G4cout << G4endl;
	G4cout << "Total System Elapsed Time: " << timer.GetSystemElapsed();
	G4cout << G4endl;
	G4cout << "Total GetUserElapsed Time: " << timer.GetUserElapsed();
	G4cout << G4endl;
	G4cout << "******************************************";
	G4cout << G4endl;

	return 0;
}
