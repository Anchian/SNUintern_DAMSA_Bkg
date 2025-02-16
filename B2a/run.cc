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
// $Id: example.cc 70284 2013-05-28 17:26:43Z perl $
//
/// \file example.cc
/// \brief Main program of the analysis/ example

#include "DetectorConstruction.hh"
#include "ActionInitialization.hh"
#include "RunAction.hh"
#include  "SteppingActions.hh"
#include "G4RunManagerFactory.hh"

#include "G4UImanager.hh"
#include "FTFP_BERT.hh"
#include "G4StepLimiterPhysics.hh"
#include "G4HadronPhysicsFTFP_BERT_HP.hh"
#include "G4IonElasticPhysics.hh"
#include "G4IonPhysicsXS.hh"
#include "G4DecayPhysics.hh"
#include "G4EmStandardPhysics.hh"
#include "G4EmExtraPhysics.hh"
#include "G4RadioactiveDecayPhysics.hh"


#include "G4VisExecutive.hh"
#include "G4UIExecutive.hh"

#include "G4ScoringManager.hh"
#include <chrono> // 시간 측정 라이브러리 포함
#include <iostream> // 입출력 라이브러리 포함
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

int main(int argc,char** argv)
{
    //Detect interactive mode (if no argument) and define UI session
    G4UIExecutive* ui = 0;
    if ( argc == 1 ) { //No commands line argument
      //Let G4UIExecutive guess what is the best available UI
      ui = new G4UIExecutive(argc,argv);
    }

    auto start = std::chrono::high_resolution_clock::now();

    // Construct the default run manager
    auto* runManager = G4RunManagerFactory::CreateRunManager();
    runManager->SetNumberOfThreads(1);


    

    // =============================================
    // Exercise 3a
    // Activate UI-command base scorer.
    // Add here the lines to activate command line
    // based scorer.
    // Remember to add the correct "include" statement
    // =============================================

    // Activate UI-command base scorer
    G4ScoringManager * scManager = G4ScoringManager::GetScoringManager();
    scManager->SetVerboseLevel(1);
    
    // Mandatory user initialization classes
    

    //====================
    //The Geometry
    
    // 1. DetectorConstruction 객체 생성 (detector)
    

    // 2. RunManager에 DetectorConstruction 객체를 설정
    // 이 설정을 통해 시뮬레이션의 기하학적 구조를 정의
    // DetectorConstruction 객체 생성 및 RunManager에 설정
    DetectorConstruction* detector = new DetectorConstruction();
    runManager->SetUserInitialization(detector);

    
    //====================
    //The Physics
    G4VModularPhysicsList* physicsList = new FTFP_BERT;

    physicsList->RegisterPhysics(new G4StepLimiterPhysics());
    physicsList->RegisterPhysics( new G4HadronPhysicsFTFP_BERT_HP());
    physicsList->RegisterPhysics( new G4IonElasticPhysics());
    physicsList->RegisterPhysics( new G4IonPhysicsXS());
    physicsList->RegisterPhysics(new G4DecayPhysics());
    physicsList->RegisterPhysics(new G4EmStandardPhysics());
    physicsList->RegisterPhysics(new G4EmExtraPhysics());
    physicsList->RegisterPhysics(new G4RadioactiveDecayPhysics());
    runManager->SetUserInitialization(physicsList);
    
    //====================
    // User action initialization
    runManager->SetUserInitialization(new ActionInitialization());
    
    // Visualization manager construction
    G4VisManager* visManager = new G4VisExecutive;
    // G4VisExecutive can take a verbosity argument - see /vis/verbose guidance.
    // G4VisManager* visManager = new G4VisExecutive("Quiet");
    
    visManager->Initialize();
    
    // Get the pointer to the User Interface manager
    G4UImanager* UImanager = G4UImanager::GetUIpointer();

    if (argc>1) {
        // execute an argument macro file if exist
        G4String command = "/control/execute ";
        G4String fileName = argv[1];
        UImanager->ApplyCommand(command+fileName);
    }
    else {
      //We have visualization, initialize defaults: look in init_vis.mac macro
      UImanager->ApplyCommand("/control/execute init_vis.mac");
      if (ui->IsGUI() ) {
         UImanager->ApplyCommand("/control/execute gui.mac");
      }
      ui->SessionStart();
      delete ui;
    }
    // Job termination
    // Free the store: user actions, physics_list and detector_description are
    // owned and deleted by the run manager, so they should not be deleted 
    // in the main() program !
  
    delete visManager;
    delete runManager;
    auto end = std::chrono::high_resolution_clock::now();

    // 소요된 시간 계산 (밀리초 단위)
    auto duration_sec = std::chrono::duration_cast<std::chrono::seconds>(end - start);
    std::cout << "Simulation run time: " << duration_sec.count() << " seconds " << std::endl;

    return 0;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
