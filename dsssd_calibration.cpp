#include <TFile.h>
#include <TTree.h>
#include <TChain.h>
#include <TH1F.h>
#include <TH1D.h>
#include <TCanvas.h>
#include <TDirectory.h>
#include <TF1.h>
#include <TMath.h>
#include <TSystem.h>

#include <vector>
#include <iostream>
#include <fstream>
using namespace std;

void dsssd_calibration()
{
  //==================== choose SEC run here ====================

  //r0990
  // TString runTag = "r0990";
  // int histogramBins = 1000;
  // double histogramMin = 0;
  // double histogramMax = 20000;
  // TString cutCondition = "Id_6>4.68 && Id_6<5.45 && Id_11>222.2 && Id_11<225";
  // TString fitFunctionExpression = "gaus(0)+gaus(3)+pol1(6)";
  // double fitLeftOffset = 900;
  // double fitRightOffset = 600;
  // double initialParameters[] = {60, 6900, 170.0, 250, 7250, 100, 0.0, 0.0};
  // int numberOfParameters = 8;

  //r0944
  // TString runTag = "r0944";
  // int histogramBins = 2000;
  // double histogramMin = 4000;
  // double histogramMax = 7000;
  // TString cutCondition = "Id_6>6.56 && Id_6<7.53 && Id_11>226.87 && Id_11<229.02";
  // TString fitFunctionExpression = "gaus(0)+gaus(3)+pol1(6)";
  // double fitLeftOffset = 1000;
  // double fitRightOffset = 600;
  // double initialParameters[] = {20, 5200, 250, 180, 5600, 150, 0.0, 0.0};
  // int numberOfParameters = 8;

  //r1081
  // TString runTag = "r1081";
  // int histogramBins = 2000;
  // double histogramMin = 0;
  // double histogramMax = 7000;
  // TString cutCondition = "Id_6>4.47 && Id_6<5.2 && Id_11>221.69 && Id_11<224.78";
  // TString fitFunctionExpression = "gaus(0)+gaus(3)+expo(6)";
  // double fitLeftOffset = 1500;
  // double fitRightOffset = 800;
  // double initialParameters[] = {28, 5232, 363, 74, 5729, 190, 0.0, 0.0};
  // int numberOfParameters = 8;

  //r1131
  // TString runTag = "r1131";
  // int histogramBins = 2000;
  // double histogramMin = 0;
  // double histogramMax = 20000;
  // TString cutCondition = "Id_6>4.45 && Id_6<5.24 && Id_11>220.7 && Id_11<224.16";
  // TString fitFunctionExpression = "gaus(0)+pol1(3)";
  // double fitLeftOffset = 300;
  // double fitRightOffset = 300;
  // double initialParameters[] = {38, 2304, 57, 0.0, 0.0};
  // int numberOfParameters = 5;

  //r1159
  // TString runTag = "r1159";
  // int histogramBins = 2000;
  // double histogramMin = 0;
  // double histogramMax = 7000;
  // TString cutCondition = "Id_6>6.14 && Id_6<7.02 && Id_11>225.7 && Id_11<226.68";
  // TString fitFunctionExpression = "gaus(0)+pol1(3)";
  // double fitLeftOffset = 700;
  // double fitRightOffset = 700;
  // double initialParameters[] = {10, 5100, 60, 0.0, 0.0};
  // int numberOfParameters = 5;

  //=========================================================

   //==================== choose STR run here ====================

   //r0990
  TString runTag = "r0990";
  int histogramBins = 1000;
  double histogramMin = 0;
  double histogramMax = 20000;
  TString cutCondition = "Id_6>4.68 && Id_6<5.45 && Id_11>222.2 && Id_11<225";
  TString fitFunctionExpression = "gaus(0)+gaus(3)+pol1(6)";
  double fitLeftOffset = 800;
  double fitRightOffset = 800;
  double initialParameters[] = {2088.77, 9546.38, 108.846, 828.432, 9697.44, 224.788, 280.134, -0.0221279};
  int numberOfParameters = 8;

  //r0944
  // TString runTag = "r0944";
  // int histogramBins = 2000;
  // double histogramMin = 4000;
  // double histogramMax = 7000;
  // TString cutCondition = "Id_6>6.56 && Id_6<7.53 && Id_11>226.87 && Id_11<229.02";
  // TString fitFunctionExpression = "gaus(0)+gaus(3)+pol1(6)";
  // double fitLeftOffset = 1000;
  // double fitRightOffset = 600;
  // double initialParameters[] = {20, 5200, 250, 180, 5600, 150, 0.0, 0.0};
  // int numberOfParameters = 8;

  //r1081
  // TString runTag = "r1081";
  // int histogramBins = 2000;
  // double histogramMin = 0;
  // double histogramMax = 7000;
  // TString cutCondition = "Id_6>4.47 && Id_6<5.2 && Id_11>221.69 && Id_11<224.78";
  // TString fitFunctionExpression = "gaus(0)+gaus(3)+expo(6)";
  // double fitLeftOffset = 1500;
  // double fitRightOffset = 800;
  // double initialParameters[] = {28, 5232, 363, 74, 5729, 190, 0.0, 0.0};
  // int numberOfParameters = 8;

  //r1131
  // TString runTag = "r1131";
  // int histogramBins = 2000;
  // double histogramMin = 0;
  // double histogramMax = 20000;
  // TString cutCondition = "Id_6>4.45 && Id_6<5.24 && Id_11>220.7 && Id_11<224.16";
  // TString fitFunctionExpression = "gaus(0)+pol1(3)";
  // double fitLeftOffset = 300;
  // double fitRightOffset = 300;
  // double initialParameters[] = {38, 2304, 57, 0.0, 0.0};
  // int numberOfParameters = 5;

  //r1159
  // TString runTag = "r1159";
  // int histogramBins = 2000;
  // double histogramMin = 0;
  // double histogramMax = 7000;
  // TString cutCondition = "Id_6>6.14 && Id_6<7.02 && Id_11>225.7 && Id_11<226.68";
  // TString fitFunctionExpression = "gaus(0)+pol1(3)";
  // double fitLeftOffset = 700;
  // double fitRightOffset = 700;
  // double initialParameters[] = {10, 5100, 60, 0.0, 0.0};
  // int numberOfParameters = 5;

  //=========================================================

  std::cout << runTag.Data() << '\n';
  TString filePattern = Form("/media/olivia/Partition1/CATS/Remerged/%s_*.root", runTag.Data());
  TString outFileName = Form("dsssd_calib_Str_%s.root", runTag.Data());
  TString outTextName = outFileName;
  outTextName.ReplaceAll(".root", ".txt");
  TString outPdfName = outFileName;
  outPdfName.ReplaceAll(".root", ".pdf");

  std::ofstream outTextFile(outTextName.Data());

  TChain* AD = new TChain("AD");
  //TODO calibrare pe un run de sulf si unul pe siliciu
  //TODO runuri mai tarzii dupa schimbarea cu numexo 1159
  AD->Add(filePattern);

  TFile* outFile = new TFile(outFileName, "RECREATE");

  std::vector<TCanvas*> canvasVector;
  std::vector<TH1D*> histogramVector;
  std::vector<TF1*> functionVector;

  canvasVector.reserve(64);
  histogramVector.reserve(64);
  functionVector.reserve(64);

  bool openedPdf = false;

  for (int stripNumber = 0; stripNumber <= 63; ++stripNumber)
  {
    outFile->cd();

    TString histogramName = Form("h_%s_Str_%d", runTag.Data(), stripNumber);
    TString functionName = Form("f_%s_Str_%d", runTag.Data(), stripNumber);

    TObject* existingHistogramObject = gDirectory->Get(histogramName);
    if (existingHistogramObject) { delete existingHistogramObject; }

    AD->Draw(Form("S1_StrRaw>>%s(%d,%g,%g)", histogramName.Data(), histogramBins, histogramMin, histogramMax),
             Form("S1_StrRawNr==%d && %s", stripNumber, cutCondition.Data()),
             "goff");

    TH1D* histogramPointer = (TH1D*)gDirectory->Get(histogramName);
    if (!histogramPointer)
    {
      std::cout << stripNumber << " 0 0 0 0\n";
      outTextFile << stripNumber << " 0 0 0 0\n";
      continue;
    }

    histogramPointer->SetDirectory(0);

    int maximumBinIndex = histogramPointer->GetMaximumBin();
    double peakCenter = histogramPointer->GetBinCenter(maximumBinIndex);

    double fitLeft = peakCenter - fitLeftOffset;
    double fitRight = peakCenter + fitRightOffset;

    TF1* compositeFunction = new TF1(functionName, fitFunctionExpression.Data(), fitLeft, fitRight);

    for (int parameterIndex = 0; parameterIndex < numberOfParameters; ++parameterIndex)
    {
      compositeFunction->SetParameter(parameterIndex, initialParameters[parameterIndex]);
    }

    histogramPointer->Fit(compositeFunction, "RQ");

    double area = 0.0;
    if (numberOfParameters >= 3)
    {
      double amplitude = compositeFunction->GetParameter(0);
      double sigma = compositeFunction->GetParameter(2);
      area = amplitude * sigma * TMath::Sqrt(2.0 * TMath::Pi());
    }

    std::cout << stripNumber;
    outTextFile << stripNumber;

    for (int parameterIndex = 0; parameterIndex < numberOfParameters; ++parameterIndex)
    {
      double parameterValue = compositeFunction->GetParameter(parameterIndex);
      double parameterError = compositeFunction->GetParError(parameterIndex);
      std::cout << " " << parameterValue << " " << parameterError;
      outTextFile << " " << parameterValue << " " << parameterError;
    }

    std::cout << " " << area << "\n";
    outTextFile << " " << area << "\n";

    outFile->cd();
    histogramPointer->Write();
    compositeFunction->Write();

    TCanvas* canvas = new TCanvas(Form("c_%s_Str_%d", runTag.Data(), stripNumber), Form("c_%s_Str_%d", runTag.Data(), stripNumber), 900, 700);
    canvas->cd();
    histogramPointer->Draw();
    compositeFunction->Draw("same");
    canvas->Modified();
    canvas->Update();

    if (!openedPdf)
    {
      canvas->Print(outPdfName + "[");
      openedPdf = true;
    }
    canvas->Print(outPdfName);

    canvasVector.push_back(canvas);
    histogramVector.push_back(histogramPointer);
    functionVector.push_back(compositeFunction);

    gSystem->ProcessEvents();
  }

  if (openedPdf && !canvasVector.empty())
  {
    canvasVector[canvasVector.size() - 1]->Print(outPdfName + "]");
  }

  outFile->Write();
  outTextFile.flush();
  outTextFile.close();
  outFile->Close();

  std::cout << "Press Enter to finish.\n";
  cin.get();

  for (size_t canvasIndex = 0; canvasIndex < functionVector.size(); ++canvasIndex)
  {
    delete functionVector[canvasIndex];
  }

  for (size_t histogramIndex = 0; histogramIndex < histogramVector.size(); ++histogramIndex)
  {
    delete histogramVector[histogramIndex];
  }

  for (size_t canvasIndex = 0; canvasIndex < canvasVector.size(); ++canvasIndex)
  {
    delete canvasVector[canvasIndex];
  }

  std::cout<<"FINISHED";
  delete outFile;
  delete AD;
}