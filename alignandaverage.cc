#include "T576Event.hh"
//#include "TUtil.hh"
#include "TAttLine.h"
double getPeak(TGraph *gr)
{
  double x,y;
  gr->GetPoint(0,x,y);
  double peakVal=y*y;
  //  int peakBin=0;
  for(int i=1;i<gr->GetN();i++) {
    gr->GetPoint(i,x,y);
    if( peakVal<(y*y) ) {
      peakVal=(y*y);
      //    peakBin=i;
    }
  }
  //return peakBin;                                                                                      
  return sqrt(peakVal);
}


int main(int argc, char** argv)
{
  TGraph *avg_graph[12];
  vector<vector<TGraph*>> graphs_ch;
  for(int i=0;i<12;i++){    //loop over channels
    T576Event * ev = new T576Event();
    int major  = 3;//atof(argv[1]);
    int minor = 21;//atof(argv[2]);
    int evNum;// = atof(argv[3]);
    T576Event * surfEvent = new T576Event();
    vector<TGraph*> graphs;
    for(evNum=0; evNum<80; evNum++){//loop over events
      ev->loadScopeEvent(major, minor, evNum);
      ev->loadSurfEvent(major, minor, evNum);
      // ev->setInterpGSs(80);
      surfEvent->loadSurfEvent(major, minor, evNum);
      //surfEvent->setInterpGSs(10);
      //    if(i==0){
      double peak = getPeak(surfEvent->surf->ch[i]);
      //  cout << peak << endl;
      if(peak<0.012){
	goto endloop;
      }
      // }
      graphs.push_back((TGraph*)surfEvent->surf->ch[i]->Clone()); 
    endloop: ;
      
    }//end loop over events
    graphs_ch.push_back(graphs);
    auto alignedGraphs=TUtil::alignMultiple(graphs_ch[i], 2.);
    avg_graph[i] = avgGraph(alignedGraphs);
  }//loop over channels


  
  TGraph * envelope[16];
  TCanvas *c0 = new TCanvas("","",450*4,450*3);
  c0->Divide(4,3);
  for(int i=0; i<12; i++){//canvas loop
    c0->cd(i+1);
    char ch_surf[50];
    sprintf(ch_surf, "Ch%d", i);
    avg_graph[i]->SetTitle(ch_surf);
    /* if(i!=0){
       avg_graph[i]->GetXaxis()->SetRangeUser(0, 150);
       }*/
    envelope[i] = TUtil::FFT::hilbertEnvelope(avg_graph[i]);
    envelope[i]->Draw("al");
  }//canvas loop
  // char hname[50];
  //  sprintf(hname,"./wforms/SURF/wf_M%d_m%d_n%d.png",major, minor, evNum);
  c0->SaveAs("avg_graph.png");
  delete c0;
  
  TCanvas *delay = new TCanvas("","",850,850);
  TUtil::setCoolPalette();
  //surfEvent->surf->ch[0]->GetXaxis()->SetRangeUser(0, 90);
  envelope[11]->Draw("al PLC");
  envelope[11]->SetName("Ch11");
  for(int i=0; i<2; i++){//canvas loop
    envelope[i]->Draw("l same PLC");
    char ch_surf[50];
    sprintf(ch_surf, "Ch%d", i);
    envelope[i]->SetName(ch_surf);
  }//canvas loop
  delay->SetGrid();
  delay->cd(1)->BuildLegend(.6, .7, .88, .88, "number of patterns", "l");
  delay->SaveAs("delay.png");
  delete delay;

  TCanvas *correlation = new TCanvas("","",1850,1850);
  TGraph *Corr[12];
  TGraph *Corr_cropped[12];
  Corr[0]= TUtil::crossCorrelate(envelope[5], envelope[0]);
  Corr_cropped[0] = TUtil::getChunkOfGraph(Corr[0], -100, 100);
  Corr_cropped[0]->SetName("Correlation graph of Ch 5 and Ch0");
  Corr_cropped[0]->GetYaxis()->SetRangeUser(0.5, 1);
  Corr_cropped[0]->SetLineWidth(2);
  Corr_cropped[0]->Draw("al PLC");
  gStyle->SetLineStyleString(11,"80 20");
  for(int i=6; i<12; i++){//canvas loop
    char ch_surf[50];
    sprintf(ch_surf, "Correlation graph of Ch 5 and Ch%d", i);
    Corr[i]= TUtil::crossCorrelate(envelope[5], envelope[i]);
    Corr_cropped[i] = TUtil::getChunkOfGraph(Corr[i], -100, 100);
    Corr_cropped[i]->SetLineStyle(i);
    Corr_cropped[i]->SetLineWidth(1);
    Corr_cropped[i]->SetName(ch_surf);
    Corr_cropped[i]->Draw("l same PLC");
  }//canvas loop
  correlation->SetGrid();
  correlation->cd(1)->BuildLegend(.6, .7, .88, .88, "", "l");
  correlation->SaveAs("correlated.png");
  delete correlation;

  

  return 1;
}
