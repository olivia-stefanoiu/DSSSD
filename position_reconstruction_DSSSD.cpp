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
using namespace std;

Float_t dsssd_Strip_Calibration(int index, Float_t energy)
{
    Float_t calibratedEnergy=0;
    for(int i=0;i<63;i++)
    {
        calibratedEnergy+=dsssdStripCalibCoeff[index][i]*pow(energy,i);
    }
    
    calibratedEnergy+=gRandom->Uniform(1.0)-.5;
    
    return calibratedEnergy;
}

Float_t dsssd_Sector_Calibration(int index, Float_t energy)
{
    Float_t calibratedEnergy=0;
    for(int i=0;i<16;i++)
    {
        calibratedEnergy+=dsssdSectorCalibCoeff[index][i]*pow(energy,i);
    }
    
    calibratedEnergy+=gRandom->Uniform(1.0)-.5;
    
    return calibratedEnergy;
}

void fillEnergies(Int_t S1_StrRawM, UShort_t* S1_StrRawNr, UShort_t* S1_StrRaw,
                  Int_t S1_SecRawM, UShort_t* S1_SecRawNr, UShort_t* S1_SecRaw,
                  Float_t* dsssdStripEnergy, Float_t* dsssdSectorEnergy)
{
    dsssdStripEnergyTotal.clear();
    dsssdSectorEnergyTotal.clear();

    for(int i = 0; i < S1_StrRawM; i++)
    {
        Float_t e = dsssdStripCalibrate(S1_StrRawNr[i], S1_StrRaw[i]);
        dsssdStripEnergy[S1_StrRawNr[i]] = e;
        dsssdStripEnergyTotal.push_back(e);
    }

    for(int i = 0; i < S1_SecRawM; i++)
    {
        Float_t e = dsssdSectorCalibrate(S1_SecRawNr[i], S1_SecRaw[i]);
        dsssdSectorEnergy[dsssdSectorConfig[S1_SecRawNr[i]][1]] = e;
        dsssdSectorEnergyTotal.push_back(e);
    }
}

Float_t reconstructRadius(Int_t S1_StrRawM, UShort_t* S1_StrRawNr)
{
    if(S1_StrRawM == 1)
    {
        return dsssdInnerActiveRadius +
               (dsssdStripConfig[S1_StrRawNr[0]][1] + gRandom->Uniform(1.0)) * dsssdStripPitch;
    }
    else if(S1_StrRawM == 2)
    {
        if(abs(dsssdStripConfig[S1_StrRawNr[0]][1] - dsssdStripConfig[S1_StrRawNr[1]][1]) == 1)
        {
            return dsssdInnerActiveRadius +
                   ((dsssdStripConfig[S1_StrRawNr[0]][1] * dsssdStripEnergyTotal[0] +
                     dsssdStripConfig[S1_StrRawNr[1]][1] * dsssdStripEnergyTotal[1]) /
                    (dsssdStripEnergyTotal[0] + dsssdStripEnergyTotal[1]) + 0.5) * dsssdStripPitch;
        }
        else
        {
            shitCounter++;
            return 0.0;
        }
    }
    else
    {
        shitCounter++;
        return 0.0;
    }
}

Float_t reconstructPhi(Int_t S1_SecRawM, UShort_t* S1_SecRawNr, Float_t* dsssdSectorEnergy)
{
    if(S1_SecRawM == 1)
    {
        Float_t phi = (dsssdSectorConfig[S1_SecRawNr[0]][1] + gRandom->Uniform(1.0)) *
                      (360.0 / dsssdNoSectors);
        if(phi > 360.0) phi -= 360.0;
        return phi;
    }
    else if(S1_SecRawM == 2)
    {
        int s0 = dsssdSectorConfig[S1_SecRawNr[0]][1];
        int s1 = dsssdSectorConfig[S1_SecRawNr[1]][1];

        if(abs(s0 - s1) == 1)
        {
            return ((s0 * dsssdSectorEnergyTotal[0] + s1 * dsssdSectorEnergyTotal[1]) /
                    (dsssdSectorEnergyTotal[0] + dsssdSectorEnergyTotal[1]) + 0.5) *
                   (360.0 / dsssdNoSectors);
        }
        else if(((s0 == 0) || (s1 == 0)) && ((s0 == 15) || (s1 == 15)))
        {
            Float_t phi = ((15 * dsssdSectorEnergy[15] + 16 * dsssdSectorEnergy[0]) /
                           (dsssdSectorEnergy[0] + dsssdSectorEnergy[15]) + 0.5) *
                          (360.0 / dsssdNoSectors);
            if(phi > 360.0) phi -= 360.0;
            return phi;
        }
        else
        {
            shitCounter++;
            return 0.0;
        }
    }
    else
    {
        shitCounter++;
        return 0.0;
    }
}

void polarToCartesian(Float_t radius, Float_t phi_deg,
                      Float_t& dsssdX, Float_t& dsssdY, bool& dsssdGood)
{
    Float_t phi_rad = phi_deg * 3.141592653 / 180.0;
    dsssdX = radius * cos(phi_rad);
    dsssdY = radius * sin(phi_rad);
    dsssdGood = true;
}

void position_reconstruction_DSSSD()
{
    Float_t dsssdInteractionRadius = 0., dsssdInteractionPhi = 0.;
    bool dsssdRadiusGood = false, dsssdPhiGood = false;

    UShort_t num_dsssdStrips = 64;
    UShort_t num_dsssdSectors = 16;
    Float_t dsssdStripEnergy[num_dsssdStrips] = {0};
    Float_t dsssdSectorEnergy[num_dsssdSectors] = {0};
   

    Int_t   S1_StrRawM; //Multiplicity of the strip
    UShort_t S1_StrRawNr[/*S1_StrRawM*/]; //Strip number 
    UShort_t S1_StrRaw[/*S1_StrRawM*/]; //Strip energy
    Long64_t S1_StrRawTS[/*S1_StrRawM*/]; //Strip timing?

    Int_t   S1_SecRawM;
    UShort_t S1_SecRawNr[/*S1_SecRawM*/];
    UShort_t S1_SecRaw[/*S1_SecRawM*/];
    Long64_t S1_SecRawTS[/*S1_SecRawM*/];

    Float_t S1_X;
    Float_t S1_Y;
    Float_t S1_Z;
    Float_t S1_Theta;
    Float_t S1_Phi;
    Float_t S1_Erings;
    Float_t S1_Esectors;

    TTree *dsssdTree = (TTree*)inputRootFile->Get("AD");

    const double R0 = 24.0; //inner radius of the dsssd
    const double H = 1.505; //radial pitch of the concentric strips
    const double Hactif_mm = 1.425; //active radial width, the part of the strip that actually collects charge
    const double Hinter_mm = H_mm - Hactif_mm; //the intergap width

    const double pi = 3.14159265358979323846;
    const double sectorWidth_rad = 2*pi / 16.0; //16 sectors spread out in a circle (2pi)
    const double quadrantWidth_rad = 2*pi / 4.0; //4 quadrants in the circle

    double r = 0.0; //radial coordinate
    double phi_rad = 0.0;

    if (E_present)
    {
        r_mm = R0_mm + H_mm * static_cast<double>(Piste_i) + Hactif_mm * gRandom->Uniform(0.0, 1.0);
        phi_rad = sectorWidth_rad * static_cast<double>(Secteur_j) + sectorWidth_rad * gRandom->Uniform(0.0, 1.0);
    }
    else
    {
        r_mm = R0_mm + H_mm * static_cast<double>(Piste_i) + Hactif_mm * gRandom->Uniform(0.0, 1.0);
        phi_rad = quadrantWidth_rad * static_cast<double>(Quadrant_j) + quadrantWidth_rad * gRandom->Uniform(0.0, 1.0);
    }

    if(S1_SecRawM > 0 || S1_StrRawM > 0)
    {
        fillEnergies(S1_StrRawM, S1_StrRawNr, S1_StrRaw,
                     S1_SecRawM, S1_SecRawNr, S1_SecRaw,
                     dsssdStripEnergy, dsssdSectorEnergy);

        dsssdInteractionRadius = reconstructRadius(S1_StrRawM, S1_StrRawNr);
        dsssdRadiusGood = (dsssdInteractionRadius != 0.0);

        dsssdInteractionPhi = reconstructPhi(S1_SecRawM, S1_SecRawNr, dsssdSectorEnergy);
        dsssdPhiGood = (dsssdInteractionPhi != 0.0);

        if(dsssdRadiusGood && dsssdPhiGood)
            polarToCartesian(dsssdInteractionRadius, dsssdInteractionPhi, dsssdX, dsssdY, dsssdGood);
    }
}