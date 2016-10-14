#include <iostream>

#include <TFile.h>
#include <TTree.h>
#include <TCanvas.h>
#include <TBox.h>
#include <TStyle.h>

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
  gStyle->SetOptStat(0);
  gStyle->SetOptTitle(0);

  TFile * file = (TFile*)gROOT->GetListOfFiles()->FindObject("BraggPeak_p_0950MeV_Uranium.root");
  if (!file) {
    file = TFile::Open("BraggPeak_p_0950MeV_Uranium.root");
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

  TProfile * prof1 = new TProfile("p vs. z prof", "p vs. z prof", 300, 0, 300);
  prof1->SetMarkerStyle(2);

  TProfile * prof2 = new TProfile("dE vs. z prof", "dE vs. z prof", 300, 0, 300);
  prof2->SetMarkerStyle(2);

  TH1D * h1 = new TH1D("dE", "dE", 300, 0, 15);
  TH2D * h2 = new TH2D("dE vs. z", "dE vs. z", 300, 0, 300, 500, 0, 50);
  TH2D * h3 = new TH2D("p vs. z", "p vs. z", 300, 0, 300, 100, 0, 1.0);

  TGraph* gr[100];
  for (int i=0;i<100;i++) gr[i] = 0;
  TH1F* frame;

  int eventID = -1;
  int lastStepID;
  double x_[100000];
  double y_[100000];

  Long64_t nentries = steps->GetEntries();

  Long64_t nbytes = 0;
  for (Long64_t i=0; i<nentries;i++) {
    steps->GetEntry(i);

    if (StepID==1) eventID++;

    if (eventID<=100) {

      x_[StepID-1] = z;
      y_[StepID-1] = x;

      if (StepID==1 && eventID>0 && !gr[eventID-1]) {
        gr[eventID-1] = new TGraph(lastStepID, x_, y_);
        //std::cout << "finish " << eventID-1 << std::endl;
      }

      lastStepID = StepID;
    }

    if (z>=100) {
      prof1->Fill(z, p);
      prof2->Fill(z, dE);
      h2->Fill(z, 0.001*dE/l);
      if (p>0) h3->Fill(z, p);
    }
    if (z>110 && z<120) h1->Fill(0.001*dE/l);

    //if (eventID>200) break;
  }

  int scale = 1;
  TCanvas * c = new TCanvas("c", "c", (1280+4)/scale, (720+28)/scale);

  c->SetFrameFillColor(kAzure-9);

  frame = c->DrawFrame(0, -20, 300, 20);

  frame->GetXaxis()->SetTitle("z [mm]");
  frame->GetYaxis()->SetTitle("x [mm]");

  TBox * box = new TBox(100, -18, 200, 18);
  box->SetFillColor(8);
  box->Draw();

  for (int i=0;i<100;i++) gr[i]->Draw("L");

  c->Print("tracing.pdf");
  c->Print("tracing.png");

  delete frame;
  delete c;

  c = new TCanvas("c", "c", (1280+4)/scale, (720+28)/scale);

  c->SetFrameFillColor(kAzure-9);

  center = (1.25 - 0.25)/2.;
  frame = c->DrawFrame(0, -0.25, 300, 1.25);

  frame->GetXaxis()->SetTitle("z [mm]");
  frame->GetYaxis()->SetTitle("p [GeV]");

  box = new TBox(100, center-(18.*1.5/40.), 200, center+(18.*1.5/40.));
  box->SetFillColor(8);
  box->Draw();

  h3->Draw("colz same");

  c->Print("pvsz.pdf");
  c->Print("pvsz.png");

  delete frame;
  delete c;

  c = new TCanvas("c", "c", (1280+4)/scale, (720+28)/scale);

  c->SetFrameFillColor(kAzure-9);

  center = (70 - 20)/2.;
  frame = c->DrawFrame(0, -20, 300, 70);

  frame->GetXaxis()->SetTitle("z [mm]");
  frame->GetYaxis()->SetTitle("dE/dz [MeV/mm]");

  box = new TBox(100, center-(18.*90./40.), 200, center+(18.*90./40.));
  box->SetFillColor(8);
  box->Draw();

  h2->Draw("colz same");

  c->Print("dEdz.pdf");
  c->Print("dEdz.png");

  delete frame;
  delete c;

  c = new TCanvas("c", "c", (1280+4)/scale, (720+28)/scale);

  h1->SetNormFactor(1);
  h1->GetXaxis()->SetTitle("dE/dz [MeV/mm]");
  h1->Draw();

  c->Print("dE.pdf");
}
