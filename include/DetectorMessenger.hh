#ifndef DetectorMessenger_h
#define DetectorMessenger_h 1

#include "G4UImessenger.hh"
#include "DetConstruct.hh"

class DetConstruct;
class G4UIcmdWithAString;

class DetectorMessenger: public G4UImessenger
{
public:
  DetectorMessenger(DetConstruct*);
  ~DetectorMessenger();

  void SetNewValue(G4UIcommand*, G4String);

private:

  DetConstruct* detector;
  G4UIdirectory* changeTargetDir;
  G4UIcmdWithAString* changeMaterial;

};
#endif
