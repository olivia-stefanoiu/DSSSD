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
    for(int i=0;i<3;i++)
    {
        calibratedEnergy+=dsssdStripCalibCoeff[index][i]*pow(energy,i);
    }
    
    calibratedEnergy+=gRandom->Uniform(1.0)-.5;
    
    return calibratedEnergy;
}

Float_t dsssd_Sector_Calibration(int index, Float_t energy)
{
    Float_t calibratedEnergy=0;
    for(int i=0;i<3;i++)
    {
        calibratedEnergy+=dsssdSectorCalibCoeff[index][i]*pow(energy,i);
    }
    
    calibratedEnergy+=gRandom->Uniform(1.0)-.5;
    
    return calibratedEnergy;
}



void position_reconstruction_DSSSD(){

    
    Float_t dsssdInteractionRadius=0., dsssdInteractionPhi=0.;
    bool dsssdRadiusGood=0, dsssdPhiGood=0;
    dsssdStripEnergyTotal.clear();
    dsssdSectorEnergyTotal.clear();
    
    Float_t dsssdCalibratedEnergy;

    Float_t dsssdStripEnergy[dsssdNoQuadrants*dsssdNoStrips] = {0};
    Float_t dsssdStripEnergyQuadrants[dsssdNoQuadrants] = {0};
    Float_t dsssdStripEnergyStrips[dsssdNoStrips] = {0};
    Float_t dsssdSectorEnergy[dsssdNoSectors] = {0};

    Int_t   S1_StrRawM;
    UShort_t S1_StrRawNr[/*S1_StrRawM*/];
    UShort_t S1_StrRaw[/*S1_StrRawM*/];
    Long64_t S1_StrRawTS[/*S1_StrRawM*/];

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


    if(S1_SecRawM>0||S1_StrRawM||0)
       
        
        for(int i=0;i<S1_StrRawM;i++)
        {
            dsssdCalibratedEnergy=dsssdStripCalibrate(S1_StrRawNr[i],S1_StrRaw[i]);
            
            dsssdStripEnergy[S1_StrRawNr[i]]=dsssdCalibratedEnergy;
         //   dsssdStripEnergyStrips[dsssdStripConfig[S1_StrRawNr[i]][1]]+=dsssdCalibratedEnergy;
         //   dsssdStripEnergyQuadrants[dsssdStripConfig[S1_StrRawNr[i]][2]]+=dsssdCalibratedEnergy;
            dsssdStripEnergyTotal.push_back(dsssdCalibratedEnergy);
        }
        
        for(int i=0;i<S1_SecRawM;i++)
        {
            dsssdCalibratedEnergy=dsssdSectorCalibrate(S1_SecRawNr[i],S1_SecRaw[i]);
            
            dsssdSectorEnergy[dsssdSectorConfig[S1_SecRawNr[i]][1]]=dsssdCalibratedEnergy;
            dsssdSectorEnergyTotal.push_back(dsssdCalibratedEnergy);
        }
        
        if(S1_StrRawM==1)
        {
            dsssdInteractionRadius=dsssdInnerActiveRadius+(dsssdStripConfig[S1_StrRawNr[0]][1]+gRandom->Uniform(1.0))*dsssdStripPitch;
            dsssdRadiusGood=1;
        }
        else if(S1_StrRawM==2)
        {
            if(abs(dsssdStripConfig[S1_StrRawNr[0]][1]-dsssdStripConfig[S1_StrRawNr[1]][1])==1)
            {
                dsssdInteractionRadius=dsssdInnerActiveRadius+((dsssdStripConfig[S1_StrRawNr[0]][1]*dsssdStripEnergyTotal[0]+dsssdStripConfig[S1_StrRawNr[1]][1]*dsssdStripEnergyTotal[1])/(dsssdStripEnergyTotal[0]+dsssdStripEnergyTotal[1])+0.5)*dsssdStripPitch;
                dsssdRadiusGood=1;
            }
            else 
            {
                dsssdInteractionRadius=0;
                shitCounter++;
            }
                //std::cout<<((dsssdStripConfig[S1_StrRawNr[0]][1]*dsssdStripEnergyTotal[0]+dsssdStripConfig[S1_StrRawNr[1]][1]*dsssdStripEnergyTotal[1])/(dsssdStripEnergyTotal[0]+dsssdStripEnergyTotal[1])+0.5)<<" "<<dsssdStripConfig[S1_StrRawNr[0]][1]<<" "<<dsssdStripConfig[S1_StrRawNr[1]][1]<<std::endl;
        }
        else if(S1_StrRawM>=3)
        {
            dsssdInteractionRadius=0;
            shitCounter++;
        }
        
        if(S1_SecRawM==1)
        {
            dsssdInteractionPhi=(dsssdSectorConfig[S1_SecRawNr[0]][1]+gRandom->Uniform(1.0))*(360./dsssdNoSectors);
            if(dsssdInteractionPhi>360.)
                dsssdInteractionPhi-=360.;
            dsssdPhiGood=1;
            
            //std::cout<<dsssdInteractionPhi<<std::endl;
        }
        else if(S1_SecRawM==2)
        {
            if(abs(dsssdSectorConfig[S1_SecRawNr[0]][1]-dsssdSectorConfig[S1_SecRawNr[1]][1])==1)
            {
                dsssdInteractionPhi=((dsssdSectorConfig[S1_SecRawNr[0]][1]*dsssdSectorEnergyTotal[0]+dsssdSectorConfig[S1_SecRawNr[1]][1]*dsssdSectorEnergyTotal[1])/(dsssdSectorEnergyTotal[0]+dsssdSectorEnergyTotal[1])+0.5)*(360./dsssdNoSectors);
                dsssdPhiGood=1;
            }
            else if(((dsssdSectorConfig[S1_SecRawNr[0]][1]==0)||(dsssdSectorConfig[S1_SecRawNr[1]][1]==0))&&((dsssdSectorConfig[S1_SecRawNr[0]][1]==15)||(dsssdSectorConfig[S1_SecRawNr[1]][1]==15)))
            {
                dsssdInteractionPhi=((15*dsssdSectorEnergy[15]+16*dsssdSectorEnergy[0])/(dsssdSectorEnergy[0]+dsssdSectorEnergy[15])+0.5)*(360./dsssdNoSectors);
                if(dsssdInteractionPhi>360.)
                    dsssdInteractionPhi-=360.;
                
                //std::cout<<((15*dsssdSectorEnergy[15]+16*dsssdSectorEnergy[0])/(dsssdSectorEnergy[0]+dsssdSectorEnergy[15])+0.5)<<" "<<15<<" "<<dsssdSectorEnergy[15]<<" "<<0<<" "<<dsssdSectorEnergy[0]<<std::endl;
                dsssdPhiGood=1;
            }
            else 
            {
                dsssdInteractionPhi=0;
                shitCounter++;
            }
        }
        else if(S1_SecRawM>=3)
        {
            dsssdInteractionPhi=0;
            shitCounter++;
        }
        
        if(dsssdRadiusGood==1&&dsssdPhiGood==1)
        {
            dsssdInteractionPhi=dsssdInteractionPhi*3.141592653/180.;
            dsssdX=dsssdInteractionRadius*cos(dsssdInteractionPhi);
            dsssdY=dsssdInteractionRadius*sin(dsssdInteractionPhi);
            dsssdGood=1;
        }
        
      
    }
        
