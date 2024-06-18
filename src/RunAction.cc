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

#include "RunAction.hh"
#include "PrimaryGeneratorAction.hh"
#include "DetectorConstruction.hh"

#include "G4RunManager.hh"
#include "G4Run.hh"
#include "G4AccumulableManager.hh"
#include "G4LogicalVolumeStore.hh"
#include "G4LogicalVolume.hh"
#include "G4UnitsTable.hh"
#include "G4SystemOfUnits.hh"

#include "G4AnalysisManager.hh" 

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

RunAction::RunAction(G4String outputName)
{

  fOutputFileName = outputName;
  auto analysisManager = G4AnalysisManager::Instance();
  // Create directories
  analysisManager->SetVerboseLevel(0);
  analysisManager->SetNtupleMerging(true);
  // Note: merging ntuples is available only with Root output

  // Book histograms, ntuple
  //
  // Creating histograms
  analysisManager->CreateH1("edep" ,"edep", 1e4, 0., 20*MeV);

  // Creating ntuple
  analysisManager->CreateNtuple("det", "Detector particle bank");
  analysisManager->CreateNtupleSColumn("pid_str"); //0
  analysisManager->CreateNtupleIColumn("pid");//1
  analysisManager->CreateNtupleDColumn("kine");//2
  analysisManager->CreateNtupleDColumn("edep");//3
  analysisManager->CreateNtupleDColumn("x");//4
  analysisManager->CreateNtupleDColumn("y");//5
  analysisManager->CreateNtupleDColumn("z");//6
  analysisManager->CreateNtupleDColumn("ipx");//7 // primary momentum direction x
  analysisManager->CreateNtupleDColumn("ipy");//8 // primary momentum direction y
  analysisManager->CreateNtupleDColumn("ipz");//9 // primary momentum direction z
  analysisManager->CreateNtupleDColumn("ix");//10 // primary momentum direction x
  analysisManager->CreateNtupleDColumn("iy");//11 // primary momentum direction y
  analysisManager->CreateNtupleDColumn("iz");//12 // primary momentum direction z

  analysisManager->CreateNtupleIColumn("evtid");//13
  analysisManager->CreateNtupleIColumn("trackid");//14
  analysisManager->CreateNtupleIColumn("parenid");//15

  analysisManager->FinishNtuple();
  
  
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void RunAction::BeginOfRunAction(const G4Run*)
{
  G4Random::showEngineStatus();

  // Get analysis manager
  auto analysisManager = G4AnalysisManager::Instance();
  
  analysisManager->OpenFile(fOutputFileName);

  G4cout << "Using " << analysisManager->GetType() << G4endl;

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void RunAction::EndOfRunAction(const G4Run* run)
{
  // print histogram statistics
  //
  auto analysisManager = G4AnalysisManager::Instance();
    // save histograms & ntuple
    //
    analysisManager->Write();
    analysisManager->CloseFile();
    
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

