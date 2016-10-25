#ifndef StackingActionAction_h
#define StackingActionAction_h

#include "G4UserStackingAction.hh"

class G4Track;

class StackingAction : public G4UserStackingAction
{
public:

  StackingAction();
  ~StackingAction();

  G4ClassificationOfNewTrack ClassifyNewTrack(const G4Track* aTrack);
};

#endif 
