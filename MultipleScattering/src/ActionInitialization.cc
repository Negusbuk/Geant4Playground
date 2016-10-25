#include "ActionInitialization.hh"
#include "PrimaryGeneratorAction.hh"
#include "RunAction.hh"
#include "EventAction.hh"
#include "SteppingAction.hh"

ActionInitialization::ActionInitialization(const DetectorConstruction* detConstruction)
 : G4VUserActionInitialization(),
   fDetConstruction(detConstruction)
{

}

ActionInitialization::~ActionInitialization()
{

}

void ActionInitialization::BuildForMaster() const
{
  EventAction* eventAction = 0;
  SetUserAction(new RunAction(eventAction));
}

void ActionInitialization::Build() const
{
  EventAction* eventAction = new EventAction;

  SetUserAction(new PrimaryGeneratorAction);
  SetUserAction(new RunAction(eventAction));
  SetUserAction(eventAction);
  SetUserAction(new SteppingAction);
}  
