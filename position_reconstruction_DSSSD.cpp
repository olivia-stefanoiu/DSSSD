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

#include <DSSSDGeometry.h>

using namespace std;

void dsssdPositionReconstruction_M1(UShort_t stripNumber, UShort_t sectorNumber, double& r, double& phi_rad)
{
    //TODO gotta get the proper M1 
    //what falls in the dead layer stays in the dead layer

    
    r = DSSSDGeometry::R0 + DSSSDGeometry::H * stripNumber + DSSSDGeometry::Hactif * gRandom->Uniform(0.0, 1.0);
    phi_rad = DSSSDGeometry::SectorWidthRad * sectorNumber + DSSSDGeometry::SectorWidthRad * gRandom->Uniform(0.0, 1.0);
}

//interstrip
void dsssdPositionReconstruction_M2(UShort_t stripNumber, UShort_t sectorNumber, double& r, double& phi_rad){


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
   

    Int_t   S1_StrRawM; //Multiplicity of the strip, corresponds to a quadrant
    UShort_t S1_StrRawNr[num_dsssdStrips]; //Strip number 
    UShort_t S1_StrRaw[num_dsssdStrips]; //Strip energy
    Long64_t S1_StrRawTS[num_dsssdStrips]; //Strip timing?

    Int_t   S1_SecRawM; // for angular coordinate calculation
    UShort_t S1_SecRawNr[num_dsssdSectors];
    UShort_t S1_SecRaw[num_dsssdSectors];
    Long64_t S1_SecRawTS[num_dsssdSectors];

    Float_t S1_X;
    Float_t S1_Y;
    Float_t S1_Z;
    Float_t S1_Theta; 
    Float_t S1_Phi;
    Float_t S1_Erings;
    Float_t S1_Esectors;

    TTree *dsssdTree = (TTree*)inputRootFile->Get("AD");

    //in mm
    const double R0 = 24.0; //inner radius of the dsssd
    const double R0_inter = 25.425; //the center of the first interstrip 
    const double H = 1.505; //radial pitch of the concentric strips
    const double Hactif = 1.425; //active radial width, the part of the strip that actually collects charge
    const double Hinter = H - Hactif; //the intergap width

    const double pi = 3.14159265358979323846;
    const double sectorWidth_rad = 2*pi / 16.0; //16 sectors spread out in a circle (2pi)
    const double quadrantWidth_rad = 2*pi / 4.0; //4 quadrants in the circle

    double r = 0.0; //radial coordinates to be extracted
    double phi_rad = 0.0;
 

    Long64_t entries = dsssdTree->GetEntries();
    for (Long64_t entry = 0; entry < entries; ++entry) {

        dsssdTree->GetEntry(entry);
        //Isotope selection using Delta E TOF ? i think
        if(!(Id_6>xCenter-0.75 && Id_6<xCenter+0.75 && Id_11>yCenter-1.75 && Id_11<yCenter+1.75))continue;
        if (S1_StrRawM == 0) continue; //can't reconstruct without delta E deposition
        //I want to check the thresholds in main to avoid building the function on stack 
        if(S1_StrRawM == 1 && S1_StrRaw[0]>6000 || (S1_StrRawM == 2 && !(S1_StrRaw[0]>6000 && S1_StrRaw[0]>6000) ){
            
            dsssdPositionReconstruction_M1(S1_StrRawNr[0], S1_SecRawNr[0], r, phi_rad);
        }
        //some M2 events can turn into  m1 event if one of the events is noise 
        //interstrip that might revert to an M1
        else if (S1_StrRawM == 2 && !(S1_StrRaw[0]>1000 && S1_StrRaw[0]>1000 )){ //interstrip calculation


        }

}

}