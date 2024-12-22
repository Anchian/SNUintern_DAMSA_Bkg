#include <cmath> // 수학 함수 사용을 위해 포함
#include <TFile.h>
#include <TTree.h>
#include <TCanvas.h>
#include <TH1F.h>
#include <TLegend.h>
#include <iostream>
#include <vector>
#include <string>

void rootmacro() {
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

    // 캔버스 생성
    TCanvas *canvas = new TCanvas("canvas", "Particle Counts by Energy Threshold", 1600, 1200);
    canvas->cd();
    canvas->SetLogy(); // y축 로그 스케일 설정
    // 히스토그램 생성
    TH1F *histogram = new TH1F("particle_counts", "Particle Counts by Energy Threshold", 16, 0, 16);

    // 빈에 값을 설정
    for (size_t i = 0; i < energyThresholds.size(); ++i) {
        histogram->SetBinContent(i + 1, counts[0][i]);       // Gamma counts at bins 1, 2, 3, 4
        histogram->SetBinContent(i + 5, counts[1][i]);       // Electron counts at bins 5, 6, 7, 8
        histogram->SetBinContent(i + 9, counts[2][i]);       // Proton counts at bins 9, 10, 11, 12
        histogram->SetBinContent(i + 13, counts[3][i]);      // Neutron counts at bins 13, 14, 15, 16

        // 각 빈에 대한 라벨 설정
        histogram->GetXaxis()->SetBinLabel(i + 1, ("Gamma > " + std::to_string(energyThresholds[i]) + " MeV").c_str());
        histogram->GetXaxis()->SetBinLabel(i + 5, ("Electron > " + std::to_string(energyThresholds[i]) + " MeV").c_str());
        histogram->GetXaxis()->SetBinLabel(i + 9, ("Proton > " + std::to_string(energyThresholds[i]) + " MeV").c_str());
        histogram->GetXaxis()->SetBinLabel(i + 13, ("Neutron > " + std::to_string(energyThresholds[i]) + " MeV").c_str());
    }

    // 스타일 설정 및 그리기
    histogram->SetFillColor(4); // 파란색 막대 설정
    histogram->SetBarWidth(0.8); // 막대 폭 설정
    histogram->SetStats(0); // 통계창 비활성화
    histogram->Draw("bar"); // 막대 그래프로 그리기

    // 범례 추가
    TLegend *legend = new TLegend(0.7, 0.7, 0.9, 0.9);
    legend->AddEntry(histogram, "Energy Thresholds for Particles", "f");
    legend->Draw();

    // 이미지로 저장
    canvas->SaveAs("particle_counts_by_energy_threshold.png");

    // 파일 닫기
    file->Close();
}