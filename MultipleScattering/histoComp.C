#include <TFile.h>
#include <TH1D.h>
#include <TCanvas.h>
#include <TLegend.h>

TH1D * getHisto(const char * filename, const char* histoname)
{
  TFile * file = (TFile*)gROOT->GetListOfFiles()->FindObject(filename);
  if (!file) {
    file = TFile::Open(filename);
  }

  TH1D* histo = (TH1D*) file->FindObjectAny(histoname);
  return histo;
}

void histoComp()
{
  int lineWidth = 2;

  TH1D* h1 = getHisto("MSC_mu_0001GeV_Graphite_histo.root", "Exit X");
  h1->SetName("Graphite");
  h1->SetTitle("Graphite");
  h1->SetLineColor(1);
  h1->SetLineWidth(lineWidth);

  TH1D* h2 = getHisto("MSC_mu_0001GeV_Si_histo.root", "Exit X");
  h2->SetName("Silicon");
  h2->SetTitle("Silicon");
  h2->SetLineColor(2);
  h2->SetLineWidth(lineWidth);

  TH1D* h3 = getHisto("MSC_mu_0001GeV_Al_histo.root", "Exit X");
  h3->SetName("Aluminium");
  h3->SetTitle("Aluminium");
  h3->SetLineColor(8);
  h3->SetLineWidth(lineWidth);

  TH1D* h4 = getHisto("MSC_mu_0001GeV_Brass_histo.root", "Exit X");
  h4->SetName("Brass");
  h4->SetTitle("Brass");
  h4->SetLineColor(4);
  h4->SetLineWidth(lineWidth);

  TH1D* h5 = getHisto("MSC_mu_0001GeV_PbWO4_histo.root", "Exit X");
  h5->SetName("PbWO4");
  h5->SetTitle("PbWO4");
  h5->SetLineColor(6);
  h5->SetLineWidth(lineWidth);

  TH1D* h6 = getHisto("MSC_mu_0001GeV_Pb_histo.root", "Exit X");
  h6->SetName("Lead");
  h6->SetTitle("Lead");
  h6->SetLineColor(6);
  h6->SetLineWidth(lineWidth);

  TH1D* h7 = getHisto("MSC_mu_0001GeV_Uranium_histo.root", "Exit X");
  h7->SetName("Uranium");
  h7->SetTitle("Uranium");
  h7->SetLineColor(8);
  h7->SetLineWidth(lineWidth);

  TCanvas * c = new TCanvas("c", "c", 700, 500);
  TH1F* frame = c->DrawFrame(-25, 0.1, 25, 3000);
  frame->GetXaxis()->SetTitle("Position [mm]");
  frame->GetYaxis()->SetTitle("Entries");
  c->SetLogy();

  h1->Draw("HIST same");
  h2->Draw("HIST same");
  h4->Draw("HIST same");
  h5->Draw("HIST same");
  h7->Draw("HIST same");

  TLegend * legend = new TLegend(0.75, 0.55, 0.9, 0.9);
  legend->AddEntry(h1);
  legend->AddEntry(h2);
  legend->AddEntry(h4);
  legend->AddEntry(h5);
  legend->AddEntry(h7);
  legend->Draw();

  c->Print("MSC_mu_1GeV.pdf");
}
