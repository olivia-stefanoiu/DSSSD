#include <TChain.h>
#include <TCanvas.h>
#include <TH1F.h>
#include <TStyle.h>
#include <TColor.h>
#include <TDirectory.h>
#include <TROOT.h>
#include <TSystem.h>
#include <TString.h>

#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

void dsssd_aligned_strips()
{
    const char* meanFile = "/home/olivia/Desktop/scripts/DSSSD/dsssd_calib_Str_r1041_r1042_r1043.txt";
    const char* filePattern = "/media/olivia/Partition1/CATS/Remerged/r104*.root";
    const char* cutCondition = "1";
    const int histogramBins = 7500;
    const double histogramMin = 0;
    const double histogramMax = 15000.0;

    std::vector<double> meanValues(64, 0.0);

    std::ifstream input(meanFile);
    if (!input.is_open())
    {
        std::cout << "Could not open mean file: " << meanFile << std::endl;
        return;
    }

    int stripIndex = -1;
    double meanValue = 0.0;
    double sigmaValue = 0.0;
    double amplitudeValue = 0.0;
    double meanErrorValue = 0.0;
    int loadedMeans = 0;

    while (input >> stripIndex >> meanValue >> sigmaValue >> amplitudeValue >> meanErrorValue)
    {
        if (stripIndex >= 0 && stripIndex < 64)
        {
            meanValues[stripIndex] = meanValue;
            loadedMeans++;
            std::cout << "Loaded mean for strip " << stripIndex << " : " << meanValue << std::endl;
        }
    }
    input.close();

    std::cout << "Total loaded mean values: " << loadedMeans << std::endl;

    std::cout << "Looking for files with pattern: " << filePattern << std::endl;
    TString matchedFiles = gSystem->GetFromPipe(Form("ls -1 %s 2>/dev/null", filePattern));
    std::cout << "Matched files:" << std::endl;
    std::cout << matchedFiles.Data() << std::endl;

    TChain* AD = new TChain("AD");

    std::stringstream matchedFilesStream(matchedFiles.Data());
    std::string fileName;
    int addedFiles = 0;

    while (std::getline(matchedFilesStream, fileName))
    {
        if (fileName.empty())
            continue;

        int addResult = AD->Add(fileName.c_str());
        std::cout << "Adding file: " << fileName << " -> " << addResult << std::endl;

        if (addResult > 0)
            addedFiles += addResult;
    }

    std::cout << "Files added to chain: " << addedFiles << std::endl;
    std::cout << "Entries in chain: " << AD->GetEntries() << std::endl;

    if (addedFiles == 0 || AD->GetEntries() == 0)
    {
        std::cout << "Chain is empty. Fix the path/pattern first." << std::endl;
        return;
    }

    std::vector<TH1F*> histograms(64, nullptr);

    TCanvas* canvas = new TCanvas("canvas_aligned_strips", "DSSSD aligned strips", 1400, 900);
    gStyle->SetOptStat(0);

    int colorList[12] = {
        kRed+1, kBlue+1, kGreen+2, kMagenta+1,
        kOrange+7, kCyan+2, kViolet+1, kPink+7,
        kAzure+2, kSpring+5, kTeal+3, kYellow+2
    };

    double globalMaximum = 0.0;

    for (int stripNumber = 0; stripNumber <63; stripNumber++)
    {
        TString histogramName = Form("hAlignedStrip_%d", stripNumber);
        TString drawExpression = Form("(S1_StrRaw-(%.10f)+10000)>>%s(%d,%g,%g)",
                                      meanValues[stripNumber], histogramName.Data(), histogramBins, histogramMin, histogramMax);
        TString selectionExpression = Form("S1_StrRawNr==%d && %s", stripNumber, cutCondition);

        std::cout << "Drawing strip " << stripNumber << " with mean " << meanValues[stripNumber] << std::endl;
        std::cout << "Draw expr: " << drawExpression.Data() << std::endl;
        std::cout << "Selection: " << selectionExpression.Data() << std::endl;

        Long64_t selectedRows = AD->Draw(drawExpression.Data(), selectionExpression.Data(), "goff");
        std::cout << "Selected rows for strip " << stripNumber << " : " << selectedRows << std::endl;

        histograms[stripNumber] = (TH1F*)gDirectory->Get(histogramName);
        if (!histograms[stripNumber])
        {
            std::cout << "Histogram not found for strip " << stripNumber << std::endl;
            continue;
        }

        histograms[stripNumber]->SetDirectory(0);
        histograms[stripNumber]->SetLineWidth(2);
        histograms[stripNumber]->SetLineColor(colorList[stripNumber % 12]);

        std::cout << "Strip " << stripNumber
                  << " color = " << colorList[stripNumber % 12]
                  << " histogram entries = " << histograms[stripNumber]->GetEntries()
                  << ", integral = " << histograms[stripNumber]->Integral()
                  << ", maximum = " << histograms[stripNumber]->GetMaximum()
                  << std::endl;

        if (histograms[stripNumber]->GetMaximum() > globalMaximum)
            globalMaximum = histograms[stripNumber]->GetMaximum();
    }

    std::cout << "Global maximum = " << globalMaximum << std::endl;

    bool firstHistogram = true;

    for (int stripNumber = 0; stripNumber < 64; stripNumber++)
    {
        if (!histograms[stripNumber])
            continue;

        histograms[stripNumber]->SetTitle("Aligned DSSSD strips;S1_StrRaw - mean(strip) + 10000;Counts");
        histograms[stripNumber]->SetMaximum(globalMaximum * 1.05);

        if (firstHistogram)
        {
            histograms[stripNumber]->Draw("hist");
            firstHistogram = false;
        }
        else
        {
            histograms[stripNumber]->Draw("hist same");
        }
    }

    canvas->Update();
}