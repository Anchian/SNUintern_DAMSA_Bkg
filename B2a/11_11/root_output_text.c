#include <cmath> // 수학 함수 사용을 위해 포함
#include <TFile.h>
#include <TTree.h>
#include <iostream>
#include <vector>
#include <string>

void root_output_text() {
    // ROOT 파일 열기
    TFile *file = TFile::Open("stage_output_t0.root");
    if (!file || file->IsZombie()) {
        std::cerr << "Error: Cannot open the file!" << std::endl;
        return;
    }

    // 트리 가져오기
    TTree *tree1 = (TTree*)file->Get("Target_paritcle_creation");

    // 에너지 임계값 설정
    std::vector<double> energyThresholds = {0.3, 1.0, 10.0, 100.0}; // MeV

    // 입자 종류와 위치별 카운트 저장 벡터
    std::vector<std::vector<int>> counts(4, std::vector<int>(energyThresholds.size(), 0));
    // 0: gamma in target, 1: electron in target, 2: proton in target, 3: neutron in target

    // 각 에너지 기준에 대해 트리 엔트리 수를 계산
    for (size_t i = 0; i < energyThresholds.size(); ++i) {
        std::string condition_gamma = "gamma_Energy > " + std::to_string(energyThresholds[i]);
        std::string condition_electron = "electron_Energy > " + std::to_string(energyThresholds[i]);
        std::string condition_proton = "proton_Energy > " + std::to_string(energyThresholds[i]);
        std::string condition_neutron = "neutron_Energy > " + std::to_string(energyThresholds[i]);

        counts[0][i] = tree1->GetEntries(condition_gamma.c_str());
        counts[1][i] = tree1->GetEntries(condition_electron.c_str());
        counts[2][i] = tree1->GetEntries(condition_proton.c_str());
        counts[3][i] = tree1->GetEntries(condition_neutron.c_str());
    }

    // 값 출력
    std::cout << "Gamma Threshold Counts:" << std::endl;
    for (size_t i = 0; i < energyThresholds.size(); ++i) {
        std::cout << "  Gamma > " << energyThresholds[i] << " MeV: " << counts[0][i] << std::endl;
    }

    std::cout << "\nElectron Threshold Counts:" << std::endl;
    for (size_t i = 0; i < energyThresholds.size(); ++i) {
        std::cout << "  Electron > " << energyThresholds[i] << " MeV: " << counts[1][i] << std::endl;
    }

    std::cout << "\nProton Threshold Counts:" << std::endl;
    for (size_t i = 0; i < energyThresholds.size(); ++i) {
        std::cout << "  Proton > " << energyThresholds[i] << " MeV: " << counts[2][i] << std::endl;
    }

    std::cout << "\nNeutron Threshold Counts:" << std::endl;
    for (size_t i = 0; i < energyThresholds.size(); ++i) {
        std::cout << "  Neutron > " << energyThresholds[i] << " MeV: " << counts[3][i] << std::endl;
    }

    // 파일 닫기
    file->Close();
}