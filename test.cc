#include "T576Event.hh"
#include "TUtil.hh"

//Run as $./test run major, run minor, and event number.
int main(int argc, char** argv)
{
  T576Event * ev = new T576Event();
  int major  = atof(argv[1]);
  int minor = atof(argv[2]);
  int evNum = atof(argv[3]);
  T576Event * surfEvent = new T576Event();
  ev->loadScopeEvent(major, minor, evNum);
  ev->loadSurfEvent(major, minor, evNum);
  surfEvent->loadSurfEvent(major, minor, evNum);

  
  char h1name[50];
  sprintf(h1name,"Run Major = %d, Run Minor = %d, Evt Num = %d", major, minor, evNum);

  /*
    TCanvas *delay = new TCanvas(h1name,h1name,850,850);
    surfEvent->surf->ch[0]->Draw("al PLC");
    surfEvent->surf->ch[0]->GetYaxis()->SetRangeUser(-400, 400);

    for(int i=1; i<8; i++){//canvas loop
    // surfEvent->surf->ch[i]->SetLineColor(i+1);
    surfEvent->surf->ch[i]->Draw("l same PLC");
    surfEvent->surf->ch[i]->GetYaxis()->SetRangeUser(-400, 600);

    }//canvas loop  
    delay->SaveAs("delay.png");
    delete delay;
  */

  
  TCanvas *c4 = new TCanvas(h1name,h1name,850*4,850*2);
  TUtil::setCoolPalette();
  c4->Divide(4,2);
  c4->SetTitle(h1name);
  for(int i=0; i<8; i++){//canvas loop
    c4->cd(i+1);
    char ch_surf[50];
    //  surfEvent->surf->pos[i];
    double x = surfEvent->surf->pos[i].X();
    double y = surfEvent->surf->pos[i].Y();
    double z = surfEvent->surf->pos[i].Z();
    sprintf(ch_surf, "Ch%d (x =  %1.2f m, y = %1.2f m, z = %1.2f m)", i, x, y, z);
    surfEvent->surf->ch[i]->SetTitle(ch_surf);
    //   surfEvent->surf->ch[i]->SetLineColor(i+1);
    surfEvent->surf->ch[i]->Draw("al PLC");
    surfEvent->surf->ch[i]->GetXaxis()->SetRangeUser(50, 200);

  }//canvas loop
  char hname[50];
  sprintf(hname,"./wforms/SURF/wf_M%d_m%d_n%d.png",major, minor, evNum);
  c4->SaveAs(hname);
  delete c4;
  
  TCanvas *c3 = new TCanvas(h1name,h1name,850*2,850*2);
  c3->Divide(2,2);
  c3->SetTitle(h1name);
  char ch_name[50];//[4][50] = {"Ch0", "Ch1", "Ch2","ICT"};
  for(int i=0; i<4; i++){//canvas loop
    c3->cd(i+1);
    double x = ev->scope->pos[i].X();
    double y = ev->scope->pos[i].Y();
    double z = ev->scope->pos[i].Z();
    sprintf(ch_name, "Ch%d (x =  %1.2f m, y = %1.2f m, z = %1.2f m)", i, x, y, z);
    
    ev->scope->ch[i]->SetTitle(ch_name);
    ev->scope->ch[i]->Draw("al");
    ev->scope->ch[i]->GetXaxis()->SetRangeUser(400, 600);
  }//canvas loop
  char h2name[50];
  sprintf(h2name,"./wforms/scope/wf_M%d_m%d_n%d.png",major, minor, evNum);
  c3->SaveAs(h2name);
  delete c3;
  /*
    for(int i=0; i<ev->scope->ch[0]->GetN(); i++){
    //  cout << "i is " << i  << " Freq is " << ev->scope->ch[0]->GetX()[i] << endl;
    }
  */
  TCanvas *c2 = new TCanvas(h1name,h1name,850*2,850*2);
  c2->Divide(2,2);
  TGraph *TheFFT[4];  

  char ch_namefft[4][50] = {"Ch0", "Ch1", "Ch2","ICT"};
  for(int i=0; i<4; i++){//canvas loop                                                                  
    c2->cd(i+1);
    TheFFT[i]  = TUtil::FFT::psd(ev->scope->ch[i]);
    TheFFT[i]->SetTitle(ch_namefft[i]);
    TheFFT[i]->Draw("al");   
  }
  //delete[] TheFFT;
  //TheFFT = NULL;
  char h3name[50];
  sprintf(h3name,"./psd/psd_M%d_m%d_n%d.png",major, minor, evNum);
  c2->SaveAs(h3name);
  delete c2;
  
  
  TCanvas *c1 = new TCanvas(h1name,h1name,850,850);
  ev->drawGeom();
  char h4name[50];
  sprintf(h4name,"./geom/geom_M%d_m%d_n%d.png",major, minor, evNum);
  c1->SaveAs(h4name);
  delete c1;
  delete ev;
  delete surfEvent;

}
