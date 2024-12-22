#include <iostream>
#include <TFile.h>
#include <TTree.h>
#include <TH1F.h>
#include <TCanvas.h>
#include <TLatex.h>

void rootmacro4_test() {
    // ROOT 파일 열기
    TFile *file = TFile::Open("stage_output_t0.root");
    if (!file || file->IsZombie()) {
        std::cerr << "Error: Cannot open the file!" << std::endl;
        return;
    }

    // Stage2 트리 가져오기
    TTree *tree2 = (TTree*)file->Get("Stage2");

    // 히스토그램 생성
    TH1F *hist_gamma_20_60 = new TH1F("hist_gamma_20_60", "Stage2 Gamma Energy (20-60 MeV);Energy (MeV);Number of Particles", 40, 20, 60);
    TH1F *hist_gamma_30_60 = new TH1F("hist_gamma_30_60", "Stage2 Gamma Energy (30-60 MeV);Energy (MeV);Number of Particles", 30, 30, 60);
    TH1F *hist_gamma_40_60 = new TH1F("hist_gamma_40_60", "Stage2 Gamma Energy (40-60 MeV);Energy (MeV);Number of Particles", 20, 40, 60);

    // 데이터 채우기
    tree2->Draw("gamma_Energy >> hist_gamma_20_60", "gamma_Energy >= 20 && gamma_Energy <= 60");
    tree2->Draw("gamma_Energy >> hist_gamma_30_60", "gamma_Energy >= 30 && gamma_Energy <= 60");
    tree2->Draw("gamma_Energy >> hist_gamma_40_60", "gamma_Energy >= 40 && gamma_Energy <= 60");

    // 캔버스 생성
    TCanvas *canvas = new TCanvas("canvas", "Stage2 Gamma Energy Ranges", 1800, 600);
    canvas->Divide(3, 1);

    // 그래프 그리기 및 입자 수 표시
    canvas->cd(1);
    hist_gamma_20_60->SetLineColor(kBlue);
    hist_gamma_20_60->Draw();
    TLatex *latex1 = new TLatex();
    latex1->SetNDC();
    latex1->SetTextSize(0.04);
    latex1->DrawLatex(0.2, 0.85, Form("Particles: %d", (int)hist_gamma_20_60->GetEntries()));

    canvas->cd(2);
    hist_gamma_30_60->SetLineColor(kRed);
    hist_gamma_30_60->Draw();
    TLatex *latex2 = new TLatex();
    latex2->SetNDC();
    latex2->SetTextSize(0.04);
    latex2->DrawLatex(0.2, 0.85, Form("Particles: %d", (int)hist_gamma_30_60->GetEntries()));

    canvas->cd(3);
    hist_gamma_40_60->SetLineColor(kGreen);
    hist_gamma_40_60->Draw();
    TLatex *latex3 = new TLatex();
    latex3->SetNDC();
    latex3->SetTextSize(0.04);
    latex3->DrawLatex(0.2, 0.85, Form("Particles: %d", (int)hist_gamma_40_60->GetEntries()));

    // 이미지로 저장
    canvas->SaveAs("stage2_gamma_energy_ranges.png");

    // 파일 닫기
    file->Close();
}
