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
#include <TROOT.h>

#include <vector>
#include <iostream>
#include <fstream>
using namespace std;

void dsssd_calibration()
{
 //gROOT->SetBatch(kTRUE);

  //==================== choose SEC run here ====================

  //r0990
  TString runTag = "sec_r0990_r0991_r0992";
  int histogramBins = 1000;
  double histogramMin = 0;
  double histogramMax = 20000;
  TString cutCondition = "Id_6>4.68 && Id_6<5.45 && Id_11>222.2 && Id_11<225";
  TString fitFunctionExpression = "gaus(0)+gaus(3)+pol1(6)";
  double fitLeftOffset = 900;
  double fitRightOffset = 600;
  double initialParameters[] = {60, 6900, 170.0, 250, 7250, 100, 0.0, 0.0};
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

   //==================== choose STR run here ====================

   //r0990
  // TString runTag = "r0990_r0991_r0992";
  // int histogramBins = 800;
  // double histogramMin = 8000;
  // double histogramMax = 12000;
  // TString cutCondition = "Id_6>4.68 && Id_6<5.45 && Id_11>222.2 && Id_11<225";
  // TString fitFunctionExpression = "gaus(0)+gaus(3)+pol1(6)";
  // double fitLeftOffset = 600;
  // double fitRightOffset = 900;
  // double initialParameters[] = {2088.77, 9546.38, 108.846, 828.432, 9697.44, 224.788, 0, 0};
  // int numberOfParameters = 8;

  //r1041
  // TString runTag = "r1041_r1042_r1043_rerun_half_bins";
  // int histogramBins = 200;
  // double histogramMin = 8500;
  // double histogramMax = 11000;
  // TString cutCondition = "Id_6>4.44 && Id_6<5.22 && Id_11>221.33 && Id_11<224.18";
  // TString fitFunctionExpression = "gaus(0)+gaus(3)+pol1(6)";
  // double fitLeftOffset = 600;
  // double fitRightOffset = 800;
  // double initialParameters[] = {2088.77, 9546.38, 108.846, 828.432, 9697.44, 224.788, 0, 0};
  // int numberOfParameters = 8;

  //r0944
//   TString runTag = "r0944_r0945_r0946";
//   int histogramBins = 1000;
//   double histogramMin = 13000;
//   double histogramMax = 18000;
//   TString cutCondition = "Id_6>6.56 && Id_6<7.53 && Id_11>226.87 && Id_11<229.02";
//   TString fitFunctionExpression = "gaus(0)+gaus(3)+pol1(6)";
//   double fitLeftOffset = 750;
//   double fitRightOffset = 1800;
// double initialParameters[] = {1465.25, 14761.1, 202.736, 250.557, 15510.1, 273.844, 0,0};
//   int numberOfParameters = 8;

//r1081
// TString runTag = "r1081_r1082_r1083";
// int histogramBins = 500;
// double histogramMin = 8000;
// double histogramMax = 10500;
// TString cutCondition = "Id_6>4.47 && Id_6<5.2 && Id_11>221.69 && Id_11<224.78";
// TString fitFunctionExpression = "gaus(0)+gaus(3)+expo(6)";
// double fitLeftOffset = 600;
// double fitRightOffset = 750;
// double initialParameters[] = {240.912, 9165.89, 100.779, 145.113, 9292.36, 190.524, 0, 0};
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

  //TODO runuri mai tarzii dupa schimbarea cu numexo 1159

  std::cout << runTag.Data() << '\n';
  TString filePattern = Form("/media/olivia/Partition1/CATS/Remerged/r104*.root", runTag.Data());
  TString outFileName = Form("dsssd_calib_Sec_%s.root", runTag.Data());
  TString outTextName = outFileName;
  //nume standardizate
  outTextName.ReplaceAll(".root", ".txt");
  TString outPdfName = outFileName;
  outPdfName.ReplaceAll(".root", ".pdf");

  std::ofstream outTextFile(outTextName.Data());

  TChain* AD = new TChain("AD");
  AD->Add(filePattern);

  //outfile este pointer la adresa unde este stocat fisierul meu 
  TFile* outFile = new TFile(outFileName, "RECREATE");

  std::vector<TCanvas*> canvasVector;
  std::vector<TH1D*> histogramVector;
  std::vector<TF1*> functionVector;

  canvasVector.reserve(64);
  histogramVector.reserve(64);
  functionVector.reserve(64);

  bool openedPdf = false;

  for (int stripNumber = 0; stripNumber <= 15; ++stripNumber)
  {
    //set the current directory to which we write to the output file
    outFile->cd();

    TString histogramName = Form("h_%s_Sec_%d", runTag.Data(), stripNumber);
    TString functionName = Form("f_%s_Sec_%d", runTag.Data(), stripNumber);

    TObject* existingHistogramObject = gDirectory->Get(histogramName);
    if (existingHistogramObject) { delete existingHistogramObject; }

    AD->Draw(Form("S1_SecRaw>>%s(%d,%g,%g)", histogramName.Data(), histogramBins, histogramMin, histogramMax),
             Form("S1_SecRawNr==%d && %s", stripNumber, cutCondition.Data()),
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

    TF1* fitFunction = new TF1(functionName, fitFunctionExpression.Data(), fitLeft, fitRight);

    for (int parameterIndex = 0; parameterIndex < numberOfParameters; ++parameterIndex)
    {
      fitFunction->SetParameter(parameterIndex, initialParameters[parameterIndex]);
    }

    TCanvas* canvas = new TCanvas(Form("c_%s_Sec_%d", runTag.Data(), stripNumber),
                                   Form("c_%s_Sec_%d", runTag.Data(), stripNumber),
                                   900, 700);
    canvas->cd();

    histogramPointer->Fit(fitFunction, "RQ");
    histogramPointer->Draw();

    canvas->Modified();
    canvas->Update();

    double meanSelected = 0.0;
    double sigmaSelected = 0.0;
    double ampSelected = 0.0;
    double meanErrSelected = 0.0;

    if (numberOfParameters >= 6)
    {
      double amp1 = fitFunction->GetParameter(0);
      double mean1 = fitFunction->GetParameter(1);
      double sigma1 = fitFunction->GetParameter(2);
      double meanErr1 = fitFunction->GetParError(1);

      double amp2 = fitFunction->GetParameter(3);
      double mean2 = fitFunction->GetParameter(4);
      double sigma2 = fitFunction->GetParameter(5);
      double meanErr2 = fitFunction->GetParError(4);

      if (amp1 >= amp2)
      {
        meanSelected = mean1;
        sigmaSelected = sigma1;
        ampSelected = amp1;
        meanErrSelected = meanErr1;
      }
      else
      {
        meanSelected = mean2;
        sigmaSelected = sigma2;
        ampSelected = amp2;
        meanErrSelected = meanErr2;
      }
    }
    else
    {
      meanSelected = fitFunction->GetParameter(1);
      sigmaSelected = fitFunction->GetParameter(2);
      ampSelected = fitFunction->GetParameter(0);
      meanErrSelected = fitFunction->GetParError(1);
    }

    std::cout << stripNumber << " "
              << meanSelected << " "
              << sigmaSelected << " "
              << ampSelected << " "
              << meanErrSelected << "\n";

    outTextFile << stripNumber << " "
                << meanSelected << " "
                << sigmaSelected << " "
                << ampSelected << " "
                << meanErrSelected << "\n";

    outFile->cd();
    // histogramPointer->Write();
    // fitFunction->Write();
    canvas->Write();

    if (!openedPdf)
    {
      canvas->Print(outPdfName + "[");
      openedPdf = true;
    }
    canvas->Print(outPdfName);

    canvasVector.push_back(canvas);
    histogramVector.push_back(histogramPointer);
    functionVector.push_back(fitFunction);

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

  //////////////////////////////////////////////////////////////

  std::cout << "FINISHED\n";
  while(kTRUE) gSystem->ProcessEvents();
}