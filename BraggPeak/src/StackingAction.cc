#include "G4Track.hh"
#include "G4Gamma.hh"

#include "StackingAction.hh"

StackingAction::StackingAction()
{

}

StackingAction::~StackingAction()
{

}

G4ClassificationOfNewTrack StackingAction::ClassifyNewTrack(const G4Track* aTrack)
{
  // const G4ParticleDefinition* particleDefinition = aTrack->GetParticleDefinition();

  //if (particleDefinition == G4Gamma::Definition()) return fKill;
  //if (particleDefinition->GetPDGCharge()==0.) return fKill;

  if (aTrack->GetParentID()==0) return fUrgent;
  return fWaiting;
}
