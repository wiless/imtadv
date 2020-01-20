#include <iostream>
#include <fstream>

using namespace std;

#include <cstring>
#include <cstdlib>
#include "const.h"
#include "common.h"


void SimulationConfiguration(int argc,char *argv[]);
void InitializeSystem( void ) ;
void ChannelSampleGeneration( int, int, int, int ) ;
void InitializeADrop( void );
void Size_Channel_Parameters( void ) ;
void Size_Logging_point_Parameters( void ) ;
void loading ();
void Logging_Point( void ) ;


/*===================================================================
FUNCTION: main()

AUTHOR: Kim, Kyung-Won (Kyung-Won@korea.ac.kr)

DESCRIPTION: ITU Channel Model Simulator

NOTES: ...
   
===================================================================*/
int main( int argc,char *argv[] )
{
  SimulationConfiguration( argc, argv );
  InitializeSystem();
  Size_Channel_Parameters();
  Size_Logging_point_Parameters() ;
  
  for(drop_idx = 0; drop_idx < num_drops; drop_idx ++) 
  {
    InitializeADrop();
    for(t = 0; t < run_times; t ++)
    {
      for(int ue_idx = 0; ue_idx < num_ues; ue_idx ++)
      {
        for(int adj_sec_idx = 0; adj_sec_idx < num_compute_coef; adj_sec_idx++)
        {
          for(int u = 0; u < num_received_antenna; u++)
          {
            for(int s = 0; s < num_transmit_antenna; s++)
            {
              ChannelSampleGeneration( ue_idx, u, s, adj_sec_idx ) ;
            }
          }
        }
      }

      // Simulator Here

      Logging_Point() ;
      loading () ;
    }
  }
}


