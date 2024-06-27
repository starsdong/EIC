/* this is a ROOT5 macro */
#include "stdio.h"
#include <iomanip>

void drawDist(const char* inputfile = "18x275")
{
  gROOT->Reset();

  gROOT->LoadMacro("~/work/work/C/plot.C");
  gROOT->LoadMacro("~/work/work/C/plot2D.C");
  gROOT->LoadMacro("~/work/work/C/eff_err.C");

  //  TFile *fin = new TFile(Form("pythia_%s_vtx.root",inputfile));
  TFile *fin = new TFile(Form("d0_noBurn.root"));
  TTree *t = (TTree *)fin->Get("vtxTree");

  TH1F *hN = new TH1F("N","",20,0,40);
  TH1F *hMN = new TH1F("MN","",20,0,40);
  t->Project("N","nTrks","abs(vz)<150");
  t->Project("MN","nTrks","abs(vzm)<150");
  plot(hMN, hN, 0, 40, 0, hMN->GetMaximum()/0.8, "# of Reconstructed Tracks","Events","MC","Rec",Form("VzComp_%s",inputfile),0);
  TH1F *hEff = new TH1F("Eff","",20,0,40);
  for(int i=0;i<20;i++) {
    double m = hN->GetBinContent(i+1);
    double n = hMN->GetBinContent(i+1);
    double eff, err;
    eff_err(m, n, &eff, &err);
    cout << eff << " +/- " << err << endl;
    hEff->SetBinContent(i+1, eff);
    hEff->SetBinError(i+1, err);
  }
  plot(hEff, 0, 40, 0., 1.1, "# of Reconstructed Tracks","Efficiency",Form("Eff_%s",inputfile),0);


  TH2F *hVxy = new TH2F("Vxy","",100,-1.,1.,100,-1.,1.);
  TH2F *hVxyMc = new TH2F("VxyMc","",100,-1.,1.,100,-1.,1.);
  TH2F *hVzCorr = new TH2F("VzCorr","",200,-100,100,200,-100,100);
  t->Project("Vxy","vy:vx","abs(vz)<150");
  plot2D(hVxy,-150,150,-150,150,"Vx (mm)", "Vy (mm)", Form("Vxy_%s",inputfile));
  t->Project("VxyMc","vym:vxm","abs(vzm)<150");
  plot2D(hVxyMc,-150,150,-150,150,"VxMc (mm)", "VyMc (mm)", Form("VxyMc_%s",inputfile));
  t->Project("VzCorr","vz:vzm","abs(vz)<150");
  plot2D(hVzCorr,-150,150,-150,150,"VzMc (mm)", "Vz (mm)", Form("VzCorr_%s",inputfile));


  TH2F *hVxyDiff = new TH2F("VxyDiff","", 100,-1.,1.,100,-1.,1.);
  t->Project("VxyDiff","vy-vym:vx-vxm","abs(vz)<150");
  plot2D(hVxyDiff,-1.,1.,-1.,1.,"Vx-VxMc (mm)", "Vy-VyMc (mm)", Form("VxyDiff_%s",inputfile));
  TH1F *hVzDiff = new TH1F("VzDiff","", 100,-1.,1.);
  t->Project("VzDiff","vz-vzm","abs(vz)<150");
  plot(hVzDiff,-1.,1.,0,hVzDiff->GetMaximum()/0.8,"Vz-VzMc (mm)", "Counts", Form("VzDiff_%s",inputfile));


  TH2F *hPtEta = new TH2F("PtEta","",50,0.,5.0,50,-5.,5.);
  t->Project("PtEta","eta:pt");
  plot2D(hPtEta, 0., 5., -5., 5., "p_{T} (GeV/c)", "#eta", Form("PtEta_%s",inputfile));
  TH2F *hEtaPhi = new TH2F("EtaPhi","",50,-3.14,3.14,50,-5.,5.);
  t->Project("EtaPhi","eta:phi");
  plot2D(hEtaPhi, -3.14, 3.14, -5., 5., "#phi", "#eta", Form("EtaPhi_%s",inputfile));

  return;
  
  
}
