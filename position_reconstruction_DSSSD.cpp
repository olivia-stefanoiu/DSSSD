#include <TFile.h>
#include <TTree.h>
#include <TH1F.h>
#include <TCanvas.h>
#include <TSpectrum.h>
#include <TDirectory.h>
#include <TF1.h>
#include <TH2F.h>
#include <TGraph.h>

#include <vector>
#include <string>
#include <fstream>
#include <algorithm>
#include <cstdio>
#include <array>
#include <iostream>

#include "DSSSDGeometry.h"

using namespace std;

// returns true if the two strip indices in the vector are adjacent
bool stripsAreAdjacent(const vector<int>& strip_indices, const UShort_t* S1_StrRawNr)
{
    if (strip_indices.size() != 2) return false;
    return abs(S1_StrRawNr[strip_indices[0]] - S1_StrRawNr[strip_indices[1]]) == 1;
}

void dsssdPositionReconstruction(
    const vector<int>& strip_indices,
    const UShort_t* S1_StrRawNr,
    const UShort_t* S1_StrRaw,
    Int_t S1_StrRawM_threshold,
    const vector<int>& sector_indices,
    const UShort_t* S1_SecRawNr,
    const UShort_t* S1_SecRaw,
    Int_t S1_SecRawM_threshold,
    Float_t& r,
    Float_t& phi_rad)
{
    //what falls in the dead layer stays in the dead layer
    //We do not take the Me=0 case, too much uncertainty 
    if (S1_StrRawM_threshold == 1)
    {
        UShort_t strip_number  = S1_StrRawNr[strip_indices[0]];
        UShort_t sector_number = S1_SecRawNr[sector_indices[0]];
        //CAREFUL! the right quadrants are inverted
        if (strip_number >= 32) {
            strip_number = 63 - strip_number;
        }

        r = DSSSDGeometry::R0 + DSSSDGeometry::H * (strip_number%16)
            + DSSSDGeometry::Hactif * gRandom->Uniform(0.0, 1.0);
            
        phi_rad = DSSSDGeometry::SectorWidthRad * sector_number
                  + DSSSDGeometry::SectorWidthRad * gRandom->Uniform(0.0, 1.0);
    }
    //TODO check the nr of event
    // else if (S1_StrRawM_threshold == 2)
    // {
    //     if(!(stripsAreAdjacent(strip_indices, S1_StrRawNr))) return;
    //     // check adjacency before doing charge sharing reconstruction
       

    //     UShort_t strip_a  = S1_StrRawNr[strip_indices[0]];
    //     UShort_t strip_b  = S1_StrRawNr[strip_indices[1]];
        
    //     // energy weighted position between the two adjacent strips
    //     r = DSSSDGeometry::R0_inter + DSSSDGeometry::H * ( strip_a +  strip_b)/2
    //         + DSSSDGeometry::Hinter * gRandom->Uniform(0.0, 1.0);

    //     UShort_t sector_number = S1_SecRawNr[sector_indices[0]];
    //     phi_rad = DSSSDGeometry::SectorWidthRad * sector_number
    //               + DSSSDGeometry::SectorWidthRad * gRandom->Uniform(0.0, 1.0);
    // }
}


void polarToCartesian(Float_t radius, Float_t phi_rad,
                      Float_t& dsssdX, Float_t& dsssdY)
{
    dsssdX = radius * cos(phi_rad);
    dsssdY = radius * sin(phi_rad);
}

void calculateThreshMultiplicity(
    Int_t S1_StrRawM,
    UShort_t* S1_StrRaw,
    vector<int>& S1_Str_idx,
    Int_t& S1_StrRawM_threshold,

    Int_t S1_SecRawM,
    UShort_t* S1_SecRaw,
    vector<int>& S1_Sec_idx,
    Int_t& S1_SecRawM_threshold)
{
    S1_Str_idx.clear();
    S1_Sec_idx.clear();

    for (int i = 0; i < S1_StrRawM; ++i)
    {
        if (S1_StrRaw[i] > 6000)
            S1_Str_idx.push_back(i);
    }

    for (int i = 0; i < S1_SecRawM; ++i)
    {
        if (S1_SecRaw[i] > 5000)
            S1_Sec_idx.push_back(i);
    }

    S1_StrRawM_threshold = S1_Str_idx.size();
    S1_SecRawM_threshold = S1_Sec_idx.size();
}

void position_reconstruction_DSSSD()
{

    TString runTag = "r099";
    std::cout << runTag.Data() << '\n';
    TString filePattern = Form("/media/olivia/Partition1/CATS/Remerged/%s*.root", runTag.Data());
    TChain* dsssdTree = new TChain("AD");
    dsssdTree->Add(filePattern);


    Float_t dsssdStripEnergy[DSSSDGeometry::num_dsssdStrips] = {0};
    Float_t dsssdSectorEnergy[DSSSDGeometry::num_dsssdSectors] = {0};
   

    Int_t   S1_StrRawM; //Multiplicity of the strip, corresponds to a quadrant
    UShort_t S1_StrRawNr[DSSSDGeometry::num_dsssdStrips]; //Strip number 
    UShort_t S1_StrRaw[DSSSDGeometry::num_dsssdStrips]; //Strip energy
    Long64_t S1_StrRawTS[DSSSDGeometry::num_dsssdStrips]; //Strip timing?

    Int_t   S1_SecRawM; // for angular coordinate calculation
    UShort_t S1_SecRawNr[DSSSDGeometry::num_dsssdSectors];
    UShort_t S1_SecRaw[DSSSDGeometry::num_dsssdSectors];
    Long64_t S1_SecRawTS[DSSSDGeometry::num_dsssdSectors];

    Float_t S1_Theta; 
    Float_t S1_Phi;
    // Float_t S1_X;
    // Float_t S1_Y;
    // Float_t S1_Z;
    // Float_t S1_Erings;
    // Float_t S1_Esectors;

    Float_t Id_6;
    Float_t Id_11;

    dsssdTree->SetBranchAddress("S1_StrRawM",  &S1_StrRawM);
    dsssdTree->SetBranchAddress("S1_StrRawNr",  S1_StrRawNr);
    dsssdTree->SetBranchAddress("S1_StrRaw",    S1_StrRaw);

    dsssdTree->SetBranchAddress("S1_SecRawM",  &S1_SecRawM);
    dsssdTree->SetBranchAddress("S1_SecRawNr",  S1_SecRawNr);
    dsssdTree->SetBranchAddress("S1_SecRaw",    S1_SecRaw);

    dsssdTree->SetBranchAddress("Id_6",        &Id_6);
    dsssdTree->SetBranchAddress("Id_11",       &Id_11);

    TH2F* dsssd_position_histogram = new TH2F("dsssd_position", "DSSSD Hit Position;X (mm);Y (mm)", 600, -60,60, 600, -60, 60);

    Long64_t entries = dsssdTree->GetEntries();
    std::cout<<entries<<'\n';
    for (Long64_t entry = 0; entry < entries; ++entry) {

        dsssdTree->GetEntry(entry);
        
        //Isotope selection using Delta E TOF ? i think
        //TODO conditia pe id
       // if(!(Id_6>xCenter-0.75 && Id_6<xCenter+0.75 && Id_11>yCenter-1.75 && Id_11<yCenter+1.75))continue;
        if (S1_StrRawM == 0 || S1_SecRawM == 0) continue;
        //can't reconstruct without delta E deposition, and with E = 0 accuracy goes 
        //I want to check the thresholds in main to avoid building the function on stack 

        vector<int> S1_Str_idx;
        //TODO scapa de asta ca nu imi trebuie
        vector<int> S1_Sec_idx;

        Int_t S1_StrRawM_threshold = 0;
        Int_t S1_SecRawM_threshold = 0;

        Float_t r=-1000, phi_rad=-1000;

        calculateThreshMultiplicity(
            S1_StrRawM, S1_StrRaw, S1_Str_idx, S1_StrRawM_threshold,
            S1_SecRawM, S1_SecRaw, S1_Sec_idx, S1_SecRawM_threshold
        );

        //caz m=1 delta e
        //TODO i need the indeces of the valid m 
        //verificam aici daca mult la E = 1 ca sa nu ocup spatiu pe stack cu functia
        if (S1_SecRawM_threshold == 1 && S1_StrRawM_threshold >= 1 && S1_StrRawM_threshold <= 2)
        {
            dsssdPositionReconstruction(
                S1_Str_idx,  S1_StrRawNr, S1_StrRaw, S1_StrRawM_threshold,
                S1_Sec_idx, S1_SecRawNr, S1_SecRaw, S1_SecRawM_threshold,
                r, phi_rad
            );

            if (r == -1000) continue;

            Float_t dsssd_X, dsssd_Y;

            polarToCartesian(r, phi_rad,
                      dsssd_X,dsssd_Y);

            
            dsssd_position_histogram->Fill(dsssd_X, dsssd_Y);

            
        }
    }

    TCanvas* position_canvas = new TCanvas("position_canvas", "DSSSD Position", 900, 900);
    position_canvas->cd();
    dsssd_position_histogram->Draw("COLZ");
    position_canvas->Update();
}