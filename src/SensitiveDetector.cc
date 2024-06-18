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
/// \file persistency/gdml//src/SensitiveDetector.cc
/// \brief Implementation of the SensitiveDetector class
//
//
//

#include "SensitiveDetector.hh"
#include "G4HCofThisEvent.hh"
#include "G4Step.hh"
#include "G4ThreeVector.hh"
#include "G4SDManager.hh"
#include "G4ios.hh"

#include <CLHEP/Units/SystemOfUnits.h>

#include "G4RunManager.hh"
#include "EventAction.hh"

#include <iostream>
#include <string>

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

SensitiveDetector::SensitiveDetector(const G4String& name)
  : G4VSensitiveDetector(name)
{
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

SensitiveDetector::~SensitiveDetector()
{
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void SensitiveDetector::Initialize(G4HCofThisEvent*)
{
}

//....Ooooo0ooooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4bool SensitiveDetector::ProcessHits(G4Step* aStep, G4TouchableHistory*)
{
  G4LogicalVolume* volume = aStep->GetPreStepPoint()->GetTouchableHandle()->GetVolume()->GetLogicalVolume();
  std::string volName = volume->GetName();
  G4Material* material = volume->GetMaterial();
  G4String matName = material->GetName();
  G4Track* track = aStep->GetTrack();
  G4ParticleDefinition* particle = track->GetDefinition();
  G4String particleName = particle->GetParticleName();

  // Access the EventAction
  auto eventAction = const_cast<EventAction*>(static_cast<const EventAction*>(G4RunManager::GetRunManager()->GetUserEventAction()));

  G4int pid = track->GetDynamicParticle()->GetParticleDefinition()->GetPDGEncoding();
  G4double edep = aStep->GetTotalEnergyDeposit()/CLHEP::keV;//For _HP library
  G4double kine = track->GetKineticEnergy()/CLHEP::keV;

  G4ThreeVector pos = track->GetPosition();
  G4double gtime   = track->GetGlobalTime();
  return true;
}
