#include <iostream>

#include <TFile.h>
#include <TTree.h>
#include <TCanvas.h>
#include <TBox.h>

TTree * steps;
Int_t           ParentID;
Int_t           TrackID;
Int_t           StepID;
Double_t        x;
Double_t        y;
Double_t        z;
Double_t        p;
Double_t        dE;
Double_t        l;
Int_t           state;

void traceing2()
{
  TFile * file = (TFile*)gROOT->GetListOfFiles()->FindObject("MSC_mu_0001GeV_Uranium.root");
  if (!file) {
    file = TFile::Open("MSC_mu_0001GeV_Uranium.root");
  }

  steps = (TTree*) file->FindObjectAny("steps");

  steps->Print();

  steps->SetBranchAddress("ParentID",&ParentID);
  steps->SetBranchAddress("TrackID",&TrackID);
  steps->SetBranchAddress("StepID",&StepID);
  steps->SetBranchAddress("x",&x);
  steps->SetBranchAddress("y",&y);
  steps->SetBranchAddress("z",&z);
  steps->SetBranchAddress("p",&p);
  steps->SetBranchAddress("dE",&dE);
  steps->SetBranchAddress("l",&l);
  steps->SetBranchAddress("state",&state);
  int eventID = -1;

  double nextZ = 275;
  int imageCount = 0;

  TGraph* grOld[100];
  for (int i=0;i<100;i++) grOld[i] = 0;
  TMarker* m = 0;
  TH1F* frame;

  Long64_t nentries = steps->GetEntries();

  double x_[100000];
  double y_[100000];

  Long64_t nbytes = 0;
  for (Long64_t i=0; i<nentries;i++) {
    steps->GetEntry(i);

    if (StepID==1) {
      std::cout << "new track" << std::endl;

      eventID++;
    }

    x_[StepID-1] = z;
    y_[StepID-1] = x;

    if (z>=nextZ || x>+18 || x<-18) {

      if (!grOld[eventID-1]) {
        grOld[eventID-1] = new TGraph(StepID, x_, y_);
        std::cout << "finish" << std::endl;
      }

    }
  }

  TCanvas * c = new TCanvas("c", "c", 1280+4, 720+28);
  c->SetFrameFillColor(kAzure-9);

  frame = c->DrawFrame(0, -20, 300, 20);

  frame->GetXaxis()->SetTitle("z [mm]");
  frame->GetYaxis()->SetTitle("x [mm]");

  TBox * box = new TBox(100, -18, 200, 18);
  box->SetFillColor(8);
  box->Draw();

  for (int i=0;i<eventID;i++) grOld[i]->Draw("L");

  c->Print("tracing.pdf");
  c->Print("tracing.png");
}
