#include "generator.hh"

#include "G4ParticleGun.hh"

MyPrimaryGenerator::MyPrimaryGenerator()
{
	fParticleGun = new G4ParticleGun(1);
	G4ParticleTable *particleTable = G4ParticleTable::GetParticleTable();
	G4String particleName="proton";
	G4ParticleDefinition *particle = particleTable->FindParticle(particleName);
        fParticleGun->SetParticleDefinition(particle);	

	G4ThreeVector pos(0.,0.,-10. *km);
	G4ThreeVector mom(0.,0.,1.);
	
	fParticleGun->SetParticlePosition(pos);
	fParticleGun->SetParticleMomentumDirection(mom);
	fParticleGun->SetParticleMomentum(2.*GeV);
}

MyPrimaryGenerator::~MyPrimaryGenerator()
{
	delete fParticleGun;
}

void MyPrimaryGenerator::GeneratePrimaries(G4Event *anEvent)
{	
	fParticleGun->GeneratePrimaryVertex(anEvent);
}

