#include <iostream>
#include <fstream>

using namespace std;

#include <cstring>
#include <cstdlib>
#include "const.h"
#include "common.h"

void SimulationConfiguration(int argc, char *argv[]);
void InitializeSystem(void);
void ChannelSampleGeneration(int, int, int, int);
void InitializeADrop(void);
void Size_Channel_Parameters(void);
void Size_Logging_point_Parameters(void);
void loading();
void Logging_Point(void);

/*===================================================================
FUNCTION: main()

AUTHOR: Kim, Kyung-Won (Kyung-Won@korea.ac.kr)

DESCRIPTION: ITU Channel Model Simulator

NOTES: ...
   
===================================================================*/
int main(int argc, char *argv[])
{
  SimulationConfiguration(argc, argv);
  InitializeSystem();
  Size_Channel_Parameters();
  Size_Logging_point_Parameters();

  float N0 = -174;
  float TxPowerdBm = 46;
  float BW = linear2dB(10e6);
  float NoisedB = +N0 + BW;
  float Noise = dB2linear(NoisedB);
  for (drop_idx = 0; drop_idx < num_drops; drop_idx++)
  {
    InitializeADrop();

    ofstream ofile;
    ofile.open("CL.dat");
    cout << " List of UEs and Associated Sector" << endl;
    for (int ue_idx = 0; ue_idx < num_ues; ue_idx++)
    {
      int k = ue_idx;
      int sector = ue[k].sector_in_control;
      float signal = ue[k].LS_gain[0] + TxPowerdBm;
      cout << k << "\t" << sector << "\t S " << ue[k].LS_gain[0] << " S " << signal << " No " << NoisedB << "   " << Noise << endl;

      float interference = 0;
      for (int isec = 1; isec < num_compute_coef; isec++)
      {
        // printf("%d(%f) ",ue[k].adj_sector[isec],ue[k].LS_gain[ adjsec ]);
        double tmp = ue[k].LS_gain[isec] + TxPowerdBm;
        double interf = dB2linear(tmp);
        interference += interf;
        printf(" \t %d :  %f   %f CS=%e\n ", ue[k].adj_sector[isec], ue[k].LS_gain[isec], tmp, interf);
      }
      float SINR;
      printf("\n Total Interference[linear|dB] %f | %f  ", interference, linear2dB(interference));
      float IplusN;
      IplusN = linear2dBm(interference + Noise);
      SINR = signal - IplusN;
      cout << " I  = " << IplusN << "   N0 " << N0 + BW << " SINR " << SINR << endl;
      ofile << k << "\t" << sector << "\t" << ue[k].LS_gain[0] << "\t" << SINR << endl;
    }

    // ue[k].adj_sector[i]

    break;

    for (t = 0; t < run_times; t++)
    {
      for (int ue_idx = 0; ue_idx < num_ues; ue_idx++)
      {
        for (int adj_sec_idx = 0; adj_sec_idx < num_compute_coef; adj_sec_idx++)
        {
          for (int u = 0; u < num_received_antenna; u++)
          {
            for (int s = 0; s < num_transmit_antenna; s++)
            {
              ChannelSampleGeneration(ue_idx, u, s, adj_sec_idx);
            }
          }
        }
      }

      // Simulator Here

      Logging_Point();
      loading();
    }
  }
}
