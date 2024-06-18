//
// ********************************************************************
// * License and Disclaimer                                           *
// *                                                                  *
// * The  Geant4 software  is  copyright of the Copyright Holders  of *
// * the Geant4 Collaboration.  It is provided  under  the terms  and *
// * conditions of the Geant4 Software License,  included in the file *
// * LICENSE and available at  http://cern.ch/geant4/license .  These *
// * include a list of copyright holders.                             *
// *                                                                  *
// * Neither the authors of this software system, nor their employing *
// * institutes,nor the agencies providing financial support for this *
// * work  make  any representation or  warranty, express or implied, *
// * regarding  this  software system or assume any liability for its *
// * use.  Please see the license in the file  LICENSE  and URL above *
// * for the full disclaimer and the limitation of liability.         *
// *                                                                  *
// * This  code  implementation is the result of  the  scientific and *
// * technical work of the GEANT4 collaboration.                      *
// * By using,  copying,  modifying or  distributing the software (or *
// * any work based  on the software)  you  agree  to acknowledge its *
// * use  in  resulting  scientific  publications,  and indicate your *
// * acceptance of all terms of the Geant4 Software license.          *
// ********************************************************************
//
//
/// \file /include/EventAction.hh
/// \brief Definition of the ::EventAction class

#ifndef EventAction_h
#define EventAction_h 1

#include "G4UserEventAction.hh"
#include "globals.hh"

#include "G4ThreeVector.hh"

class RunAction;

class EventAction : public G4UserEventAction
{
public:
  EventAction(RunAction* runAction);
  ~EventAction();

  void BeginOfEventAction(const G4Event* event) override;
  void EndOfEventAction(const G4Event* event) override;

  void AddEdep(G4int, G4double); 
  void SetEdep(G4double);

  void FillKine(G4double);

  void SetPidPosTime(G4int, G4String, G4ThreeVector, G4double);

  void SetInitialEnergyMom(G4double, G4ThreeVector, G4ThreeVector);

  void FillNtuple(G4int, G4int);

private:
  RunAction* fRunAction = nullptr;
  G4int fEvtId;
  G4double   *fEdep;
  const G4int ndet = 1;
  G4double fInie;
  G4ThreeVector fImom;
  G4ThreeVector fIpos;
};


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif


