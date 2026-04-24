#include <TChain.h>
#include <TCanvas.h>
#include <TH2F.h>
#include <TStyle.h>
#include <TDirectory.h>
#include <TROOT.h>
#include <TSystem.h>
#include <TString.h>

#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

void dsssd_dE_vs_TOF()
{
    const char* meanFile    = "/home/olivia/Desktop/scripts/DSSSD/dsssd_calib_Str_r0990_r0991_r0992.txt";
    const char* filePattern = "/media/olivia/Partition1/CATS/Remerged/r099*.root";
    const char* cutCondition = "1";

    // ToF axis (X) -- Id_11
    const int    tofBins = 2000;
    const double tofMin  = 210;
    const double tofMax  = 245;   // <-- adjust to your Id_11 range

    // Delta E axis (Y) -- aligned strip
    const int    dEBins = 6000;
    const double dEMin  = -6000;
    const double dEMax  = 30000.0;

    // -------- load strip means --------
    std::vector<double> meanValues(64, 0.0);

    std::ifstream input(meanFile);
    if (!input.is_open())
    {
        std::cout << "Could not open mean file: " << meanFile << std::endl;
        return;
    }

    int    stripIndex      = -1;
    double meanValue       = 0.0;
    double sigmaValue      = 0.0;
    double amplitudeValue  = 0.0;
    double meanErrorValue  = 0.0;

    while (input >> stripIndex >> meanValue >> sigmaValue >> amplitudeValue >> meanErrorValue)
    {
        if (stripIndex >= 0 && stripIndex < 64)
        {
            meanValues[stripIndex] = meanValue;
            std::cout << "Loaded mean for strip " << stripIndex << " : " << meanValue << std::endl;
        }
    }
    input.close();

    // -------- build chain --------
    TString matchedFiles = gSystem->GetFromPipe(Form("ls -1 %s 2>/dev/null", filePattern));
    std::cout << "Matched files:\n" << matchedFiles.Data() << std::endl;

    TChain* AD = new TChain("AD");
    std::stringstream matchedFilesStream(matchedFiles.Data());
    std::string fileName;

    while (std::getline(matchedFilesStream, fileName))
    {
        if (fileName.empty()) continue;
        int addResult = AD->Add(fileName.c_str());
        std::cout << "Adding file: " << fileName << " -> " << addResult << std::endl;
    }

    std::cout << "Entries in chain: "    << AD->GetEntries() << std::endl;

    if (AD->GetEntries() == 0)
    {
        std::cout << "Chain is empty. Fix the path/pattern first." << std::endl;
        return;
    }

    // -------- the single shared 2D histogram --------
    // All strips are filled into this same hist, each with its own alignment.
    TH2F* hDEvsToF = new TH2F("h_dE_ToF",
                              "#DeltaE (aligned) vs ToF;Id_11 (ToF);S1_StrRaw - mean(strip) + 10000",
                              tofBins, tofMin, tofMax,
                              dEBins,  dEMin,  dEMax);
    //hDEvsToF->SetDirectory(0);

    // -------- fill loop: one Draw per strip, accumulating into h_dE_ToF --------
    for (int stripNumber = 0; stripNumber < 3; stripNumber++)
    {
        // Y : aligned strip , X : Id_11     (TTree::Draw syntax is "Y:X")
        TString drawExpression = Form("(S1_StrRaw-(%.10f)+10000):Id_11>>+h_dE_ToF",
                                      meanValues[stripNumber]);
        TString selectionExpression = Form("S1_StrRawNr==%d && %s",
                                           stripNumber, cutCondition);

        std::cout << "Strip " << stripNumber
                  << "  mean = " << meanValues[stripNumber] << std::endl;
        std::cout << "  Draw: " << drawExpression.Data() << std::endl;
        std::cout << "  Sel : " << selectionExpression.Data() << std::endl;

        Long64_t selectedRows = AD->Draw(drawExpression.Data(),
                                         selectionExpression.Data(),
                                         "goff");
        std::cout << "  rows filled: " << selectedRows << std::endl;
    }

    std::cout << "Total entries in 2D hist: " << hDEvsToF->GetEntries() << std::endl;

    // -------- draw --------
    TCanvas* canvas = new TCanvas("canvas_dE_vs_tof", "DeltaE vs ToF", 1200, 900);
   // gStyle->SetOptStat(0);
  //  gStyle->SetPalette(kBird);
   // canvas->SetLogz();                 // comment out if you don't want log Z
    hDEvsToF->Draw("COLZ");
    canvas->Update();
}