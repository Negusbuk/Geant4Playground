#ifndef SteppingAction_h
#define SteppingAction_h 1

#include "G4UserSteppingAction.hh"

class G4GenericMessenger;

class DetectorConstruction;

class SteppingAction : public G4UserSteppingAction
{
public:
  SteppingAction();
  virtual ~SteppingAction();

  virtual void UserSteppingAction(const G4Step* step);

private:

    void DefineCommands();

    G4GenericMessenger* fMessenger;
    bool fFillNTuple;
};

#endif
