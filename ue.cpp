#include "common.h"
#include "const.h"


#include <iostream>
#include <fstream>

using namespace std;

#include <cstring>
#include <cstdlib>
/*===================================================================
FUNCTION: UE::UE()

AUTHOR: Seong-Jun Oh

DESCRIPTION: UE Object Constructor

NOTES: Currently, nothing is done when a UE is instantiated  
   
===================================================================*/
UE::UE()
{
}

/*===================================================================
FUNCTION: UE::~UE()

AUTHOR: Seong-Jun Oh

DESCRIPTION: UE Object Destructor

NOTES: Currently, nothing is done  
   
===================================================================*/
UE::~UE()
{
}


/*===================================================================
FUNCTION: UE::Configure( int self_idx )

AUTHOR: Seong-Jun Oh, Kyung-Won Kim

DESCRIPTION: UE object gets to know the index of itself

NOTES: 
   
===================================================================*/
void UE::Configure( int self_idx )
{
  void StaticGainGeneration( int , double * , LOCATION * );
  void FadingChannelGeneration( int, int, int, int ) ;

  _self_idx = self_idx ;
  
  if ( drop_idx == 0 )
  {
    static_gain = new double[ NUM_SECTORS ] ;
    LS_gain = new double[ NUM_SECTORS ] ;
    PathLoss = new double[ NUM_SECTORS ] ;
    Distance = new double[ NUM_SECTORS ] ;
    adj_sector = new int[num_compute_coef] ;
  }

  StaticGainGeneration( self_idx , static_gain , &coordinate );

  for(int sec_idx = 0; sec_idx < NUM_SECTORS; sec_idx++)
  {
    PathLoss[ sec_idx ] = path_loss[ sec_idx ] ;
    Distance[ sec_idx ] = ue2sector_distance[self_idx][ sec_idx ] ;
    LS_gain[ sec_idx ] = static_gain[ sec_idx ] + antenna[ sec_idx ] ;
  }

  for(int i = 1; i < NUM_SECTORS; i++)
  {
    double temp;
    int j ;

    temp = LS_gain[i];
    for(j = i; j > 0; j--)
    {
      if( LS_gain[j-1] < temp )
        LS_gain[j] = LS_gain[j-1];
      else break;
    }
    LS_gain[j] = temp;
  }

  for(int sec_idx = 0; sec_idx < NUM_SECTORS; sec_idx++)
  {
    for(int idx = 0; idx < num_compute_coef; idx++)
    {
      if( (int)(1000000*LS_gain[ idx ]) == (int)(1000000*(static_gain[ sec_idx ] + antenna[ sec_idx ])) )
      {
        adj_sector[idx] = sec_idx ;
      }
    }
  }
  sector_in_control = adj_sector[0] ;

  for(int adj_sec_idx = 0; adj_sec_idx < num_compute_coef; adj_sec_idx++)
  {
    for(int u = 0; u < num_received_antenna; u++)
    {
      for(int s = 0; s < num_transmit_antenna; s++)
      {
        FadingChannelGeneration( self_idx, u, s, adj_sec_idx ) ;
      }
    }
  }
}

