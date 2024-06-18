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

#include "EventAction.hh"
#include "RunAction.hh"

#include "G4Event.hh"
#include "G4RunManager.hh"
#include "G4AnalysisManager.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

EventAction::EventAction(RunAction* runAction)
: fRunAction(runAction)
{
  fEdep = new G4double[ndet];
  //Initialize the array
  for(int i = 0 ; i < ndet ; i++){
    fEdep[i] = 0.;
  }
  fInie = 0.;
  fImom.setX(0.); fImom.setY(0.); fImom.setZ(0.);
  fIpos.setX(0.); fIpos.setY(0.); fIpos.setZ(0.);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

EventAction::~EventAction()
{
  delete [] fEdep;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void EventAction::BeginOfEventAction(const G4Event* aEvt)
{
  fEvtId = aEvt->GetEventID();
  for(int i = 0 ; i < ndet ; i++){
    fEdep[i] = 0.;
  }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void EventAction::EndOfEventAction(const G4Event*)
{
  auto analysisManager = G4AnalysisManager::Instance();
  analysisManager->FillH1(0,fEdep[0]);
  fInie = 0.;
  fImom.setX(0.); fImom.setY(0.); fImom.setZ(0.);
  fIpos.setX(0.); fIpos.setY(0.); fIpos.setZ(0.);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void EventAction::AddEdep(G4int id, G4double edep){
  fEdep[id] += edep;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void EventAction::SetEdep(G4double edep){
  auto analysisManager = G4AnalysisManager::Instance();
  analysisManager->FillNtupleDColumn(0, 3, edep);//edep
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void EventAction::FillKine(G4double kine){
  // get analysis manager
  auto analysisManager = G4AnalysisManager::Instance();
  analysisManager->FillNtupleDColumn(0, 2, kine);//kinetic energy
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void EventAction::SetPidPosTime(G4int pid, G4String pid_str, G4ThreeVector pos, G4double gtime){
  // get analysis manager
  auto analysisManager = G4AnalysisManager::Instance();
  analysisManager->FillNtupleSColumn(0, 0, pid_str);
  analysisManager->FillNtupleIColumn(0, 1, pid);
  analysisManager->FillNtupleDColumn(0, 4, pos.getX());
  analysisManager->FillNtupleDColumn(0, 5, pos.getY());
  analysisManager->FillNtupleDColumn(0, 6, pos.getZ());
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void EventAction::SetInitialEnergyMom(G4double ene, G4ThreeVector mom, G4ThreeVector pos){
  fInie = ene;
  fImom = mom;
  fIpos = pos;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void EventAction::FillNtuple(G4int trackid, G4int parenid){
  auto analysisManager = G4AnalysisManager::Instance();
    analysisManager->FillNtupleDColumn(0, 7, fImom.getX());
    analysisManager->FillNtupleDColumn(0, 8, fImom.getY());
    analysisManager->FillNtupleDColumn(0, 9, fImom.getZ());
    analysisManager->FillNtupleDColumn(0, 10, fIpos.getX());
    analysisManager->FillNtupleDColumn(0, 11, fIpos.getY());
    analysisManager->FillNtupleDColumn(0, 12, fIpos.getZ()); 
    analysisManager->FillNtupleIColumn(0, 13, fEvtId);  
    analysisManager->FillNtupleIColumn(0, 14, trackid);  
    analysisManager->FillNtupleIColumn(0, 15, parenid);  

    analysisManager->AddNtupleRow(0);

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
