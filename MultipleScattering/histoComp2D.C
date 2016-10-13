#include <TFile.h>
#include <TH1D.h>
#include <TCanvas.h>
#include <TLegend.h>
#include <TStyle.h>

TH2D * getHisto(const char * filename, const char* histoname)
{
  TFile * file = (TFile*)gROOT->GetListOfFiles()->FindObject(filename);
  if (!file) {
    file = TFile::Open(filename);
  }

  TH2D* histo = (TH2D*) file->FindObjectAny(histoname);
  return histo;
}

void histoComp2D()
{
  gStyle->SetOptStat(0);
  gStyle->SetOptTitle(0);

  int lineWidth = 2;

  TH2D* h1 = (TH2D*)getHisto("MSC_mu_0001GeV_Graphite_histo.root", "Exit XY")->Rebin2D(5, 5);
  h1->SetName("Graphite");
  h1->SetTitle("Graphite");

  TH2D* h2 = (TH2D*)getHisto("MSC_mu_0001GeV_Si_histo.root", "Exit XY")->Rebin2D(5, 5);
  h2->SetName("Silicon");
  h2->SetTitle("Silicon");

  TH2D* h3 = (TH2D*)getHisto("MSC_mu_0001GeV_Al_histo.root", "Exit XY")->Rebin2D(5, 5);
  h3->SetName("Aluminium");
  h3->SetTitle("Aluminium");

  TH2D* h4 = (TH2D*)getHisto("MSC_mu_0001GeV_Brass_histo.root", "Exit XY")->Rebin2D(5, 5);
  h4->SetName("Brass");
  h4->SetTitle("Brass");
  h4->GetXaxis()->SetTitle("x [mm]");
  h4->GetXaxis()->SetTitleOffset(1.5);
  h4->GetYaxis()->SetTitle("y [mm]");
  h4->GetYaxis()->SetTitleOffset(1.5);
  h4->GetZaxis()->SetTitle("Entries");
  h4->GetZaxis()->SetTitleOffset(1.25);

  TH2D* h5 = (TH2D*)getHisto("MSC_mu_0001GeV_PbWO4_histo.root", "Exit XY")->Rebin2D(5, 5);
  h5->SetName("PbWO4");
  h5->SetTitle("PbWO4");

  TH2D* h6 = (TH2D*)getHisto("MSC_mu_0001GeV_Pb_histo.root", "Exit XY")->Rebin2D(5, 5);
  h6->SetName("Lead");
  h6->SetTitle("Lead");

  TH2D* h7 = (TH2D*)getHisto("MSC_mu_0001GeV_Uranium_histo.root", "Exit XY")->Rebin2D(5, 5);
  h7->SetName("Uranium");
  h7->SetTitle("Uranium");
  h7->SetBarOffset(0.0);
  h7->SetBarWidth(1.0);
  h7->GetXaxis()->SetTitle("x [mm]");
  h7->GetXaxis()->SetTitleOffset(1.75);
  h7->GetYaxis()->SetTitle("y [mm]");
  h7->GetYaxis()->SetTitleOffset(1.75);
  h7->GetZaxis()->SetTitle("Entries");
  h7->GetZaxis()->SetTitleOffset(1.25);
  h7->GetXaxis()->SetRangeUser(-22, 22);
  h7->GetYaxis()->SetRangeUser(-22, 22);

  TCanvas * c = new TCanvas("c", "c", 700, 700);

  h7->Draw("LEGO2");

  c->Print("MSC_mu_1GeV_Uranium_2D.pdf");

  c->Clear();

  TH1F * frame = c->DrawFrame(-20, -20, 20, 20);
  frame->GetXaxis()->SetTitle("x [mm]");
  frame->GetYaxis()->SetTitle("y [mm]");

  TEllipse * e;

  TF2 *f = new TF2("f", "xygaus", -22, 22, -22, 22);

  h7->Fit(f, "NR");
  e = new TEllipse(f->GetParameter(1), f->GetParameter(3), 2.355*f->GetParameter(2), 2.355*f->GetParameter(4));
  e->SetLineColor(8);
  e->SetLineWidth(lineWidth);
  e->Draw();

  h5->Fit(f, "NR");
  e = new TEllipse(f->GetParameter(1), f->GetParameter(3), 2.355*f->GetParameter(2), 2.355*f->GetParameter(4));
  e->SetLineColor(6);
  e->SetLineWidth(lineWidth);
  e->Draw();

  h4->Fit(f, "NR");
  e = new TEllipse(f->GetParameter(1), f->GetParameter(3), 2.355*f->GetParameter(2), 2.355*f->GetParameter(4));
  e->SetLineColor(4);
  e->SetLineWidth(lineWidth);
  e->Draw();

  h2->Fit(f, "NR");
  e = new TEllipse(f->GetParameter(1), f->GetParameter(3), 2.355*f->GetParameter(2), 2.355*f->GetParameter(4));
  e->SetLineColor(2);
  e->SetLineWidth(lineWidth);
  e->Draw();

  h1->Fit(f, "NR");
  e = new TEllipse(f->GetParameter(1), f->GetParameter(3), 2.355*f->GetParameter(2), 2.355*f->GetParameter(4));
  e->SetLineColor(1);
  e->SetLineWidth(lineWidth);
  e->Draw("same");

  c->Print("MSC_mu_1GeV_2D.pdf");
}
