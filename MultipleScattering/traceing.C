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

void traceing()
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

  double nextZ = 5.;
  int imageCount = 0;

  TCanvas * c = new TCanvas("c", "c", 1280+4, 720+28);

  TGraph* gr=0;
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

      nextZ = 5.;

      // if (eventID==1) break;

      c->Clear();
      c->SetFrameFillColor(kAzure-9);

      if (gr) grOld[eventID-1] = new TGraph(*gr);
      if (m) delete m;

      frame = c->DrawFrame(0, -20, 300, 20);

      frame->GetXaxis()->SetTitle("z [mm]");
      frame->GetYaxis()->SetTitle("x [mm]");
    }

    x_[StepID-1] = z;
    y_[StepID-1] = x;

    if (StepID>99) {
      if (StepID==100) {
        m = new TMarker(z, x, 3);
        m->SetMarkerColor(2);
      }

      if (z>=nextZ && z<=280 && x<=+18 && x>=-18) {

        TBox * box = new TBox(100, -18, 200, 18);
        box->SetFillColor(8);
        box->Draw();

        for (int i=0;i<eventID;i++) grOld[i]->Draw("L");

        if (gr) delete gr;
        gr = new TGraph(StepID, x_, y_);
        gr->Draw("L");

        m->SetX(z);
        m->SetY(x);
        m->Draw();

        c->Update();

        c->Print(Form("images/img%05d.png", imageCount++));

        nextZ += 10.0;
      }
    }
  }

  // ffmpeg -framerate 50 -i images/img%05d.png -s:v 1280x720 -c:v libx264 -profile:v high -crf 20 -pix_fmt yuv420p -r 25 Tracing.m4v
}
