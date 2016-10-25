#ifndef ActionInitialization_h
#define ActionInitialization_h 1

#include "G4VUserActionInitialization.hh"

#include "DetectorConstruction.hh"

class ActionInitialization : public G4VUserActionInitialization
{
public:
  ActionInitialization(const DetectorConstruction* detConstruction);
  virtual ~ActionInitialization();

  virtual void BuildForMaster() const;
  virtual void Build() const;

protected:

  const DetectorConstruction* fDetConstruction;
};

#endif

    
