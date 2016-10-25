#include "G4Track.hh"

#include "TrackingAction.hh"

TrackingAction::TrackingAction()
{

}

TrackingAction::~TrackingAction()
{

}

void TrackingAction::PreUserTrackingAction(const G4Track* track)
{
  // const G4ParticleDefinition* particleDefinition = track->GetParticleDefinition();

}

void TrackingAction::PostUserTrackingAction(const G4Track* track)
{
  // const G4ParticleDefinition* particleDefinition = track->GetParticleDefinition();

}

