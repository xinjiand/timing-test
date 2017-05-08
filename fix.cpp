#include <fstream>
#include <iostream>
#include <stdlib.h>
#include <stdio.h>

using namespace std;

int main(int argc, char** argv)
{
  ifstream time(argv[1],ios::in|ios::binary);
  ifstream wave(argv[2],ios::in|ios::binary);
  ofstream fout(argv[3],ios::out|ios::binary);
  int length=atoi(argv[4]), count=0;
  unsigned long int t;
  short *waveform = new short[length];
  while(time.read((char*)&t,8) && wave.read((char*)waveform,2*length))
    {
      fout.write((char*)&t,8);
      fout.write((char*)waveform,2*length);
    }
  time.close();
  wave.close();
  fout.close();
  return 0;
}
