#include "draw.C+"
#include "style.C+"

void drawResvsN(const char* inputfile = "18x275")
{
  style();

  //  TFile *fin = new TFile(Form("pythia_%s_vtx.root",inputfile));
  TFile *fin = new TFile(Form("d0_noBurn.root"));
  TTree *t = (TTree *)fin->Get("vtxTree");
  /*
  TH1F *hN = new TH1F("N","",15,0,15);
  TH1F *hMN = new TH1F("MN","",15,0,15);
  t->Project("N","nTrks","abs(vz)<50");
  t->Project("MN","nTrks","abs(vzm)<50");
  plot(hMN, hN, 0, 15, 0, hMN->GetMaximum()/0.8, "# of Reconstructed Tracks","Events","MC","Rec","VzComp",0);
  return;
  */

  const Int_t N = 3;
  const Char_t *Name[N] = {"x","y","z"};
  TH2F *hDVtxN[N];
  TGraphErrors *gr_DVtxNRes[N];
  for(int i=0;i<N;i++) {
    hDVtxN[i] = new TH2F(Form("DVtxN_%s",Name[i]),"",20,0,40,200,-0.2,0.2);
    t->Project(Form("DVtxN_%s",Name[i]),Form("v%s-v%sm:nTrks",Name[i],Name[i]),"abs(vz)<150");
    hDVtxN[i]->FitSlicesY();
    gr_DVtxNRes[i] = new TGraphErrors((TH1F *)gDirectory->Get(Form("DVtxN_%s_2",Name[i])));
  }

  TCanvas *c1 = new TCanvas("c1","",800,600);
  c1->Draw();

  double x1=0;
  double x2=40;
  double y1=0;
  double y2=0.1;
  TH1D *h0 = new TH1D("d0","",1,x1,x2);
  h0->SetMinimum(y1);
  h0->SetMaximum(y2);
  h0->GetXaxis()->SetLabelOffset(0.007);
  h0->SetXTitle("# of Reconstructed Tracks");
  h0->SetYTitle("Resolution (mm)");
  h0->Draw();

  const Int_t kColor[N] = {1, 2, 4};
  const Int_t kMarkerStyle[N] = {20, 21, 22};
  for(int i=0;i<N;i++) {
    setGraphMarker(gr_DVtxNRes[i], kMarkerStyle[i], kColor[i]);
    setGraphLine(gr_DVtxNRes[i], 1, kColor[i], 2);
    gr_DVtxNRes[i]->Draw("p");
  }

  TLegend *leg = new TLegend(0.22, 0.74, 0.52, 0.94);
  leg->SetLineColor(10);
  leg->SetTextSize(0.035);
  leg->SetFillColorAlpha(kWhite,0);
  for(int i=0;i<N;i++) {
    leg->AddEntry(gr_DVtxNRes[i], Name[i], "pl");
  }
  leg->Draw();
  
  drawHistBox(x1,x2,y1,y2);

  c1->Update();
  c1->SaveAs(Form("fig/d0_HF_%s.pdf",inputfile));
  c1->SaveAs(Form("fig/d0_HF_%s.png",inputfile));
  
  
}
