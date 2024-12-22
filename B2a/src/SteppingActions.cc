#include "SteppingActions.hh"
#include "G4Step.hh"
#include "G4Track.hh"
#include "G4ParticleDefinition.hh"
#include "G4ParticleTypes.hh"  // 입자 정의
#include "G4SystemOfUnits.hh"   // 단위 정의
#include "G4EventManager.hh"
#include "G4Event.hh"
#include "G4RunManager.hh"
#include "G4AnalysisManager.hh"
#include "RunAction.hh"
#include <algorithm>
#include <cmath>


SteppingAction::SteppingAction ()
: G4UserSteppingAction()
{}

SteppingAction::~SteppingAction()
{}

void SteppingAction::UserSteppingAction(const G4Step* step)
{
    
    
    // root 기록을 위한 호출
    auto analysisManager = G4AnalysisManager::Instance();

    //stage 1을 지나는 입자를 송출함
    G4StepPoint* preStepPoint = step->GetPreStepPoint();
    G4StepPoint* postStepPoint = step->GetPostStepPoint();
    G4ThreeVector preStepPos = preStepPoint->GetPosition();
    G4ThreeVector postStepPos = postStepPoint->GetPosition();
    G4String volume = step->GetPreStepPoint()->GetTouchableHandle()->GetVolume()->GetName();

    if ( volume == "targetPhysical"){
        const G4Track* track = step->GetTrack();
        G4ParticleDefinition* particleType = track->GetDefinition();
        if(track->GetCurrentStepNumber() == 1 ){
        G4int particleID = particleType->GetPDGEncoding();
        G4double gammaEnergy = 0.0;
        G4double electronEnergy = 0.0;
        G4double protonEnergy = 0.0;
        G4double neutronEnergy = 0.0;



        switch(particleID){
            case 22:  // gamma
                gammaEnergy = postStepPoint->GetKineticEnergy()/MeV;
                analysisManager->FillNtupleFColumn(8, 0, gammaEnergy);
                analysisManager->AddNtupleRow(8);
                break;
            case 11:  // electron
                electronEnergy = postStepPoint->GetKineticEnergy()/MeV;
                analysisManager->FillNtupleFColumn(8, 1, electronEnergy);
                analysisManager->AddNtupleRow(8);
                break;
            case 2212: // proton
                protonEnergy = postStepPoint->GetKineticEnergy()/MeV;
                analysisManager->FillNtupleFColumn(8, 2, protonEnergy);
                analysisManager->AddNtupleRow(8);
                break;
            case 2112:  // neutron
                neutronEnergy = postStepPoint->GetKineticEnergy()/MeV;
                analysisManager->FillNtupleFColumn(8, 3, neutronEnergy);
                analysisManager->AddNtupleRow(8);
                break;
            
        }

        }
        





    }


    // targetphysical 안 생성된 입자의 갯수 
    

    //step 0

    if (step->IsFirstStepInVolume()) {
        // 현재 볼륨 이름 가져오기
        G4String volumeName = step->GetPreStepPoint()->GetTouchableHandle()->GetVolume()->GetName();
        const G4Track* track = step->GetTrack();
        // 타겟 볼륨 내에서 첫 번째 스텝인 경우
        if (volumeName =="targetPhysical" && track->GetParticleDefinition() == G4Gamma::GammaDefinition()) {
            // 감마 입자의 운동 에너지 가져오기
            G4double gammaEnergy = preStepPoint->GetKineticEnergy()/MeV;

            // 감지된 입자의 에너지를 저장 (switch 문 활용)

            fStage0ParticlesEnergy.push_back(preStepPoint->GetKineticEnergy());
            // 에너지 저장 (히스토그램 또는 Ntuple 등으로 저장)
            // 예시: Ntuple의 첫 번째 컬럼에 에너지를 저장
            analysisManager->FillNtupleFColumn(2, 0, gammaEnergy);
            analysisManager->AddNtupleRow(2);

            
        }
    }    



    // prestepPos의 z 좌표가 5cm 보다작고 postStepPos의 z좌표가 5cm 보다 큰 경우
    if (preStepPos.z() < 5.0*cm && postStepPos.z() > 5.0*cm) {
        const G4Track* track = step->GetTrack();
        G4ParticleDefinition* particleType = track->GetDefinition();
        // 입자의 이름과 에너지 출력
        //G4cout << " stage 1 Particle : " << particleType->GetParticleName() << "Energy" << preStepPoint->GetKineticEnergy() / MeV << "Mev" << G4endl;
        fStage1ParticlesEnergy.push_back(preStepPoint->GetKineticEnergy());
        fStage1ParticlesName.push_back(particleType->GetParticleName());




        //입자의 이름과 에너지를 rootfile에 저장
        G4int particleID = particleType->GetPDGEncoding();
        
        // Stage 1 데이터를 저장하는 경우
        
        G4double gammaEnergy = 0.0;
        G4double electronEnergy = 0.0;
        G4double neutronEnergy = 0.0;

        // 감지된 입자의 에너지를 저장 (switch 문 활용)
        switch (particleID) {
            case 22:  // gamma
                gammaEnergy = postStepPoint->GetKineticEnergy()/MeV;
                break;
            case 11:  // electron
                electronEnergy = postStepPoint->GetKineticEnergy()/MeV;
                break;
            case 2112:  // neutron
                neutronEnergy = postStepPoint->GetKineticEnergy()/MeV;
                break;
        }

        // 모든 Column에 값을 설정하고 나서 한 번에 Row를 추가
        analysisManager->FillNtupleFColumn(0, 0, gammaEnergy);
        analysisManager->FillNtupleFColumn(0, 1, electronEnergy);
        analysisManager->FillNtupleFColumn(0, 2, neutronEnergy);
        analysisManager->AddNtupleRow(0);
        
        

    }
    
    

    // stage 2 조건
    
    // PreStepPoint의 Stage 2 조건 검사
    

    //bool preStage2rule = (preStepPos.z() >= 35.0 * cm && preStepPos.z() <= 79.0 * cm &&
    //                    preStepPos.x() >= -6.0 * cm && preStepPos.x() <= 6.0 * cm &&
    //                     preStepPos.y() >= -6.0 * cm && preStepPos.y() <= 6.0 * cm);

// PostStepPoint의 Stage 2 조건 검사
    //bool postStage2rule = (postStepPos.z() >= 35.0 * cm && postStepPos.z() <= 79.0 * cm &&
    //                    postStepPos.x() >= -6.0 * cm && postStepPos.x() <= 6.0 * cm &&
    //                    postStepPos.y() >= -6.0 * cm && postStepPos.y() <= 6.0 * cm);
    //!preStage2rule && postStage2rule

//if(step->IsFirstStepInVolume() && step->GetPreStepPoint()->GetTouchable()->GetVolume()->GetName() == "Ecallogical")


//step->IsFirstStepInVolume()
// PreStepPoint가 Stage 2에 해당하지 않고, PostStepPoint가 Stage 2에 해당하는 경우에만 실행
    if(step->IsFirstStepInVolume() && step->GetPreStepPoint()->GetTouchable()->GetVolume()->GetName() == "EcalPhysical")
    {
        const G4Track* track = step->GetTrack();
        G4ParticleDefinition* particleType = track->GetDefinition();

        //G4cout << " stage 2 Particle : " << particleType->GetParticleName() << "Energy" << preStepPoint->GetKineticEnergy() / MeV << "Mev" << G4endl;
        fStage2ParticlesEnergy.push_back(preStepPoint->GetKineticEnergy());
        fStage2ParticlesName.push_back(particleType->GetParticleName()); 

         

        //입자의 이름과 에너지를 rootfile에 저장
        G4int particleID = particleType->GetPDGEncoding();
        // Stage 1 데이터를 저장하는 경우
        G4double gammaEnergy = 0.0;
        G4double electronEnergy = 0.0;
        G4double neutronEnergy = 0.0;
        std::vector<G4double> row = {0.0, 0.0, 0.0,0.0,0.0};
        std::vector<G4double> vertexrow = {0.0, 0.0, 0.0};
        G4double globalTime = 0.0;
        G4ThreeVector momentum = {0.0, 0.0, 0.0};
        
        G4ThreeVector vertexposition = {0.0, 0.0, 0.0};

        G4ThreeVector arriveposition = {0.0, 0.0, 0.0};
        
        G4String process =" ";
        const G4VProcess* creatorProcess;

        // 감지된 입자의 에너지를 저장 (switch 문 활용)
        switch (particleID) {
            case 22:  // gamma
                gammaEnergy = preStepPoint->GetKineticEnergy() / MeV;
                globalTime = preStepPoint->GetGlobalTime() / ns;  // 나노초(ns)
                momentum = preStepPoint->GetMomentumDirection() / MeV;
                vertexposition = track->GetVertexPosition();
                arriveposition = preStepPoint->GetPosition();   
                
                creatorProcess = track->GetCreatorProcess();
                process = creatorProcess->GetProcessName(); 

                row = {globalTime, gammaEnergy, momentum.x(), momentum.y(), momentum.z() ,arriveposition.x(), arriveposition.y(),arriveposition.z()  };
                fStage2TimeCut4Vec.push_back(row);

                vertexrow = {vertexposition.x(), vertexposition.y(), vertexposition.z()};
                fVertexPosition.push_back(vertexrow);
                
                fphysicalprocess.push_back(process);
                


                break;
            case 11:  // electron
                electronEnergy = preStepPoint->GetKineticEnergy() / MeV;
                break;
            case 2112:  // neutron
                neutronEnergy = preStepPoint->GetKineticEnergy() / MeV;
                break;
        }
        


        
        // gamma invariant mass distribution
        
            
        



        // 모든 Column에 값을 설정하고 나서 한 번에 Row를 추가
        analysisManager->FillNtupleFColumn(1, 0, gammaEnergy);
        analysisManager->FillNtupleFColumn(1, 1, electronEnergy);
        analysisManager->FillNtupleFColumn(1, 2, neutronEnergy);
        analysisManager->AddNtupleRow(1);

        analysisManager->FillNtupleFColumn(4, 0, globalTime);
        analysisManager->AddNtupleRow(4);

        




        
    }
}    
    
    




