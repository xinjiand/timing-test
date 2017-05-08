void fancy(const char* filein, const char* fileout)
{
  ofstream fout(fileout);
  TTreeBinary *temp = new TTreeBinary(filein,"time/l:data[2000]/S");
  TLeaf *data = temp->GetLeaf("data");
  TLeaf *time = temp->GetLeaf("time");
  double x[2000], y[2000], s[2000],p[2000],base,d, energy;
  int up, down, t0;
  for(int event=0; event<temp->GetEntries(); event++)
    {
      temp->GetEntry(event);
      base=0.;
      for(int samp=0; samp<2000; samp++)
	{
	  x[samp]=samp;
	  y[samp]=data->GetValue(samp);
	  if(samp<500)
	    base+=y[samp];
	}
      base=base/500.;
      for(int samp=0; samp<2000; samp++)
	y[samp]-=base;
      p[0]=s[0]=0.;
      for(int i=1; i<2000; i++)
	{
	  if(i<250)
	    d=y[i];
	  else if(i<280)
	    d=y[i]-y[i-250];
	  else if(i<530)
	    d=y[i]-y[i-250]-y[i-280];
	  else
	    d=d=y[i]-y[i-250]-y[i-280]+y[i-530];
	  p[i]=p[i-1]+d;
	  s[i]=s[i-1]+p[i]+250*d;
	}
      up=down=0;
      for(int i=1; i<2000; i++)
	{
	  if(s[i]>1000000 && s[i-1]<1000000)
	    min=i;
	  if(s[i]<1000000 && s[i-1]>1000000)
	    max=i;
	}
      energy=s[(max+min)/2]/(250.*250.);
      t0=(max+min)/2-268;
      TString function("(x>");
      function+=t0;
      function+=")*[0]*(exp(-(x-";
      function+=t0;
      function+=")/[1])-exp(-(x-";
      function+=t0;
      function+=")/[2]))";
      TF1 *F = new TF1("F",function,0,2000);
      F->SetParameter(1,250);
      F->SetParameter(2,5);
      TGraph *g = new TGraph(2000,x,y);
      g->Fit("F","R && q");
      fout<<event<<" "<<time->GetValue()<<" "<<t0<<" "<<energy<<" "<<F->GetChisquare()<<" "<<F->GetParameter(0)<<" "<<F->GetParameter(1)<<" "<<F->GetParameter(2)<<endl;
    }
  return;
}
