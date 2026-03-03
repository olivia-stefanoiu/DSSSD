#include <TFile.h>
#include <TTree.h>
#include <TChain.h>
#include <TH1F.h>
#include <TH1D.h>
#include <TCanvas.h>
#include <TDirectory.h>
#include <TF1.h>
#include <TMath.h>

#include <vector>
#include <iostream>
#include <fstream>
using namespace std;

void dsssd_calibration()
{
  TString runTag = "r1081";
  TString filePattern = Form("/media/olivia/Partition1/CATS/%s_*.root", runTag.Data());
  TString outFileName = Form("dsssd_calib_%s.root", runTag.Data());
  TString outTextName = outFileName;
  outTextName.ReplaceAll(".root", ".txt");

  std::ofstream outTextFile(outTextName.Data());

  TChain* AD = new TChain("AD");
  //TODO calibrare pe un run de sulf si unul pe siliciu
  //TODO runuri mai tarzii dupa schimbarea cu numexo 1159
  AD->Add(filePattern);

  std::vector<TCanvas*> canvasVector;
  std::vector<TH1D*> histogramVector;
  std::vector<TF1*> fitVector;

  canvasVector.reserve(65);
  histogramVector.reserve(65);
  fitVector.reserve(65);

  for (int stripNumber = 31; stripNumber <= 63; ++stripNumber)
  {
    TString histogramName = Form("h_%s_strip_%d", runTag.Data(), stripNumber);
    TString canvasName = Form("c_%s_strip_%d", runTag.Data(), stripNumber);
    TString functionName = Form("f_%s_strip_%d", runTag.Data(), stripNumber);

    TObject* existingHistogramObject = gDirectory->Get(histogramName);
    if (existingHistogramObject) { delete existingHistogramObject; }

    //990
    // AD->Draw(Form("S1_StrRaw>>%s(800,8000,12000)", histogramName.Data()),
    //          Form("S1_StrRawNr==%d && Id_6>4.68 && Id_6<5.45 && Id_11>222.2 && Id_11<225", stripNumber),
    //          "goff");
    //944
    // AD->Draw(Form("S1_StrRaw>>%s(2000,12000,18000)", histogramName.Data()),
    //          Form("S1_StrRawNr==%d && Id_6>6.56 && Id_6<7.53 && Id_11>226.87 && Id_11<229.02", stripNumber),
    //          "goff");
    //1081
      AD->Draw(Form("S1_StrRaw>>%s(2000,6000,12000)", histogramName.Data()),
             Form("S1_StrRawNr==%d && Id_6>4.47 && Id_6<5.2 && Id_11>221.69 && Id_11<224.78", stripNumber),
             "goff");

    TH1D* histogramPointer = (TH1D*)gDirectory->Get(histogramName);
    if (!histogramPointer)
    {
      std::cout << stripNumber << ",0,0,0,0\n";
      outTextFile << stripNumber << ",0,0,0,0\n";
      continue;
    }

    TCanvas* canvasPointer = new TCanvas(canvasName, canvasName, 900, 650);
    histogramPointer->Draw();

    int maximumBinIndex = histogramPointer->GetMaximumBin();
    double peakCenter = histogramPointer->GetBinCenter(maximumBinIndex);

    // double fitLeft = peakCenter - 500;
    // double fitRight = peakCenter + 800;

    //  double fitLeft = peakCenter - 1000;
    // double fitRight = peakCenter + 1500;

       double fitLeft = peakCenter - 750;
    double fitRight = peakCenter + 800;

    // TF1* compositeFunction = new TF1(functionName, "gaus(0)+gaus(3)+pol1(6)", fitLeft, fitRight);
    // compositeFunction->SetParameters(
    //   40, 15000, 100.0,
    //    10, 15750.0, 150.0,
    //      0.0,     0.0
    // );

      TF1* compositeFunction = new TF1(functionName, "gaus(0)+gaus(3)+pol1(6)", fitLeft, fitRight);
    compositeFunction->SetParameters(
      150, 9250, 100.0,
       45, 9400, 150.0,
         0.0,     0.0
    );

    histogramPointer->Fit(compositeFunction, "RQ");
    compositeFunction->Draw("SAME");

    double amplitude = compositeFunction->GetParameter(0);
    double mean = compositeFunction->GetParameter(1);
    double sigma = compositeFunction->GetParameter(2);
    double area = amplitude * sigma * TMath::Sqrt(2.0 * TMath::Pi());

    std::cout << stripNumber << "," << amplitude << "," << mean << "," << sigma << "," << area << "\n";
    outTextFile << stripNumber << "," << amplitude << "," << mean << "," << sigma << "," << area << "\n";

    canvasPointer->Update();

    canvasVector.push_back(canvasPointer);
    histogramVector.push_back(histogramPointer);
    fitVector.push_back(compositeFunction);
  }

  outTextFile.close();

  std::cout << "Press Enter to keep canvases open...\n";
  std::cin.get();

  TFile* outFile = new TFile(outFileName, "RECREATE");
  outFile->cd();

  for (size_t histogramIndex = 0; histogramIndex < histogramVector.size(); ++histogramIndex)
  {
    if (histogramVector[histogramIndex]) histogramVector[histogramIndex]->Write();
  }

  outFile->Close();
}