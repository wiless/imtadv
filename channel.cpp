#include "common.h"
#include "const.h"
#include "channel.h"

//void StaticGainGeneration(int, double *, LOCATION *) ;
void Set_Scenario_Environment() ;
void Setup_Network_Layout() ;
void Assign_UE_Location(int) ;
void Compute_Indoor_Location(int) ;
void Compute_Outdoor_Location(int) ;
void Set_PathLoss_Parameters(int, int) ;
void Determine_LOS(int, int) ;
void Set_Correlation_Matrix(int, int) ;
void Compute_PathLoss(int, int);
void Set_Shadowing(int, int) ;
//void FadingChannelGeneration( int, int, int, int ) ;
void Set_Delay_Parameters(int) ;
void Set_AoAAoD_Parameters(int) ;
void Set_Num_Path(int, int) ;
void Set_Cluster_Delay(int, int, int, int) ;
void Sort_Delay(int, int, int, int) ;
void Set_Cluster_Power(int, int, int, int) ;
void Scaling_Delay(int, int, int, int) ;
void Generate_AoAAoD(int, int, int, int);
void Set_Subclusters(int, int, int, int) ;
void Find_Max_idx(int, int, int, int) ;
void Sort_clusters(int, int, int, int) ;
void Generate_XPR(int, int, int, int) ;
void SetInitialPhase(int, int, int, int);
//void ChannelSampleGeneration(int, int, int, int) ;
void Compute_Channel_Coef(int, int, int, int, int) ;
//void Size_Channel_Parameters( void ) ;

/*===================================================================
FUNCTION: StaticGainGeneration()

AUTHOR: Kim, Kyung-Won

DESCRIPTION: Generate static gains.

NOTES: ...
   
===================================================================*/
void StaticGainGeneration( int self_idx, double * static_gain , LOCATION * coordinate )
{
  Set_Scenario_Environment() ;
  Setup_Network_Layout() ;
  Assign_UE_Location(self_idx) ;

  for(int sec_idx = 0; sec_idx < NUM_SECTORS; sec_idx++)
  {
    Determine_LOS(self_idx, sec_idx) ;
    Set_PathLoss_Parameters(self_idx, sec_idx) ;
    Compute_PathLoss(self_idx, sec_idx);
    Set_Shadowing(self_idx, sec_idx) ;
    antenna[ sec_idx ] = -MIN( 12. * pow((ue2sector_angle[self_idx][sec_idx]/ANGLE_3dB_h),2) , 30. );
        // This equation comes from 8.5.1.1 (2) in dB
    static_gain[ sec_idx ] = - ( shadowing + path_loss[ sec_idx ] ) ;
  } 
  (*coordinate).x = location_x;
  (*coordinate).y = location_y;
   
  
}


/*===================================================================
FUNCTION: Set_Scenario_Environment()

AUTHOR: Kim, Kyung-Won

DESCRIPTION: Set up the environment of each scenario

NOTES: Output are site2site_distance in [m],
                  carrier_freq in [GHz],
                  speed_UE in [m/s].
   
===================================================================*/
void Set_Scenario_Environment()
{  
  if ( scenario == INDOOR_HOTSPOT ) 
  {                              
    carrier_freq = 3.4 * 1000000000. ; 
    K_mu = 7. ;
    K_sigma = 4. ;
    speed_UE = 3. * 1000. /3600. ;
  }
  else if ( scenario == URBAN_MACRO )
  {
    site2site_distance = 500. ;
    carrier_freq = 2.0 * 1000000000. ;
    K_mu = 9. ;
    K_sigma = 3.5 ;
    speed_UE = 30. * 1000. /3600. ;
  }
  else if ( scenario == URBAN_MICRO || scenario == OUTDOOR_2_INDOOR ) 
  { 
    site2site_distance = 200. ; 
    carrier_freq = 2.5 * 1000000000. ; 
    K_mu = 9. ;
    K_sigma = 5. ;
    speed_UE = 3. * 1000. /3600. ;
  }
  else if ( scenario == RURAL_MACRO ) 
  { 
    site2site_distance = 1732. ; 
    carrier_freq = 0.7 * 1000000000. ; 
    K_mu = 7. ;
    K_sigma = 4. ;
    speed_UE = 120. * 1000. /3600. ;
  }
  else if ( scenario == SUBURBAN_MACRO ) 
  { 
    site2site_distance = 1299. ; 
    carrier_freq = 2.0 * 1000000000. ; 
    K_mu = 9. ;
    K_sigma = 7. ;
    speed_UE = 90. * 1000. /3600. ;
  }
  else                         
  { 
    cerr << "Unspeficied scenario" << endl ; 
  }
}


/*===================================================================
FUNCTION: Setup_Network_Layout()

AUTHOR: Kim, Kyung-Won

DESCRIPTION: Set up the network layout
             
NOTES: 
   
===================================================================*/
void Setup_Network_Layout()
{
  if ( scenario == INDOOR_HOTSPOT )
  {
    do
    {
      location_x = 120. * Rand.u() - 60. ;
      location_y = 50. * Rand.u() - 25. ;
    }while( sqrt( (location_x-30)*(location_x-30) + location_y*location_y ) < 3.
            || sqrt( (location_x+30)*(location_x+30) + location_y*location_y ) < 3. ) ;
  }
  else
  {
    do
    {
      location_x = 0.5 * site2site_distance * Rand.u() ;
      location_y = (site2site_distance / sqrt(3.)) * Rand.u() ;
    }while( location_y > (-location_x + site2site_distance) / sqrt(3.)
            || sqrt(location_x*location_x + location_y*location_y) < 10. ) ;
    
    int section ;
    section = (int) (4 * Rand.u()) ; 
    if ( section % 4 == 1 )
    {
      location_x = -location_x ;
    }
    else if ( section % 4 == 2 )
    {
      location_y = -location_y ;
    }
    else if ( section % 4 == 3 )
    {
      location_x = -location_x ;
      location_y = -location_y ;
    }
  }
}


/*===================================================================
FUNCTION: Assign_UE_Location()

AUTHOR: Kim, Kyung-Won

DESCRIPTION: Compute distance and angle of UE to sectors

NOTES: Output are ue2sector_distance in [m], 
                  ue2sector_angle.
   
===================================================================*/
void Assign_UE_Location(int self_idx)
{
  if ( scenario == INDOOR_HOTSPOT )
  {
    Compute_Indoor_Location(self_idx) ;
  }
  else
  {
    Compute_Outdoor_Location(self_idx) ;
  }
  for(int sec_idx = 0; sec_idx < NUM_SECTORS; sec_idx++ )
  {
    if( ue2sector_angle[ self_idx ][ sec_idx ] > 180. )
    {
      ue2sector_angle[ self_idx ][ sec_idx ] = ue2sector_angle[ self_idx ][ sec_idx ] - 360. ;
    }
  }
  travel_angle[self_idx] = 360. * Rand.u() - 180. ;
}

/*===================================================================
FUNCTION: Compute_Indoor_Location()

AUTHOR: Kim, Kyung-Won

DESCRIPTION: Compute distance and angle of UE to cells in indoor environment

NOTES: 
   
===================================================================*/
void Compute_Indoor_Location(int self_idx)
{
  distance_x = location_x + 30. ;
  distance_y = location_y ;
  ue2sector_distance[ self_idx ][0] = sqrt( distance_x * distance_x + distance_y * distance_y ) ;   
  ue2sector_angle[ self_idx ][0] = atan2( distance_y, distance_x ) * 180. / PI ;

  distance_x = location_x - 30. ;
  distance_y = location_y ;
  ue2sector_distance[ self_idx ][1] = sqrt( distance_x * distance_x + distance_y * distance_y ) ;   
  ue2sector_angle[ self_idx ][1] = atan2( distance_y, distance_x ) * 180. / PI ;
}

/*===================================================================
FUNCTION: Compute_Outdoor_Location()

AUTHOR: Kim, Kyung-Won

DESCRIPTION: Compute distance and angle of UE to cells in Outdoor environment 

NOTES: 
   
===================================================================*/
void Compute_Outdoor_Location(int self_idx)    
{
  int mycell = (int)( 18.9999 * Rand.u() ) ;
  int cell_idx = 0 ;
  int cell_num ;
    
  for(int height = 0; height < 5; height++)
  {
    int vert = 2 - height ;
    int num_horiz = 5 - abs( vert ) ;
    for(int horiz = 0; horiz < num_horiz; horiz++)
    {
      distance_x = location_x - ( horiz - 2. + 0.5 * abs(vert) ) * site2site_distance ;
      distance_y = location_y - vert * sqrt(3.0) * site2site_distance / 2. ;
      cell_num = wrap[ mycell ][ cell_idx ] ;
      ue2sector_distance[ self_idx ][ 3 * cell_num ] = sqrt( distance_x * distance_x + distance_y * distance_y ) ;   
      ue2sector_angle[ self_idx ][ 3* cell_num ] = atan2( distance_y, distance_x ) * 180. / PI ;
      cell_idx++ ;
    }
  }
  for(int cell_num = 0; cell_num < NUM_SECTORS/3; cell_num++)
  {
    ue2sector_distance[ self_idx ][ 3 * cell_num + 1 ] = ue2sector_distance[ self_idx ][ 3 * cell_num ] ;
    ue2sector_distance[ self_idx ][ 3 * cell_num + 2 ] = ue2sector_distance[ self_idx ][ 3 * cell_num ] ;
    ue2sector_angle[ self_idx ][ 3 * cell_num + 1 ] = ue2sector_angle[ self_idx ][ 3 * cell_num ] + 120. ;
    ue2sector_angle[ self_idx ][ 3 * cell_num + 2 ] = ue2sector_angle[ self_idx ][ 3 * cell_num ] + 240. ;
  }
}

/*===================================================================
FUNCTION: Determine_LOS()

AUTHOR: Kim, Kyung-Won

DESCRIPTION: Determine LOS

NOTES: 
   
===================================================================*/
void Determine_LOS(int self_idx, int sec_idx)
{
  indoor[self_idx][sec_idx] = false ;
  if ( los_type == NLOS || scenario == OUTDOOR_2_INDOOR )
  {
  	los[self_idx][sec_idx] = false ;
  }
  else if ( los_type == LOS )
  {
  	los[self_idx][sec_idx] = true ;
  }
  else if ( los_type == RANDOM )
  {
    double LOS_prob ;
    if ( scenario == INDOOR_HOTSPOT )
    {
      if ( ue2sector_distance[self_idx][sec_idx] < 18. )
      {
        LOS_prob = 1. ; 
      }
      else if ( ue2sector_distance[self_idx][sec_idx] < 37. )
      {
        LOS_prob = exp( -( ue2sector_distance[self_idx][sec_idx] - 18. ) / 27. ) ;
      }
      else
      {
        LOS_prob = 0.5 ;
      }
    }
    else if ( scenario == URBAN_MACRO )
    {
      LOS_prob = MIN( 18./ue2sector_distance[self_idx][sec_idx] , 1. ) * ( 1. - exp( -ue2sector_distance[self_idx][sec_idx]/63. ) ) 
                 + exp( -ue2sector_distance[self_idx][sec_idx]/63. );
    }
    else if ( scenario == URBAN_MICRO )
    {
      if( Rand.u() < 0.5 )
      {
        LOS_prob = 0. ;
        indoor[self_idx][sec_idx] = true ;
      }
      else
      {
        LOS_prob = MIN( 18./ue2sector_distance[self_idx][sec_idx] , 1. ) * ( 1. - exp( -ue2sector_distance[self_idx][sec_idx]/36. ) ) 
                   + exp( -ue2sector_distance[self_idx][sec_idx]/36. );
      }
    }
    else if ( scenario == RURAL_MACRO )
    {
      LOS_prob = exp( -( ue2sector_distance[self_idx][sec_idx] - 10. ) / 1000. ) ;
    }
    else if ( scenario == SUBURBAN_MACRO )
    {
      LOS_prob = exp ( -( ue2sector_distance[self_idx][sec_idx] - 10. ) / 200. ) ;
    }
    else
    {
      cerr << "Unspeficied scenario" << endl ;
    } 
         // Thses equations come from Table A1-3

    if ( Rand.u() < LOS_prob )
    {
      los[self_idx][sec_idx] = true ;
    }
    else
    {
      los[self_idx][sec_idx] = false ;
    }
  }
  
  Set_Correlation_Matrix(self_idx, sec_idx) ;

  if( los[self_idx][sec_idx] == true )
  {
    double cross_corr = 0 ;
    for( int idx = 0; idx < 5; idx++ )
    {
      cross_corr = cross_corr + sqrt_corr_matrix[4][idx] * random_number[self_idx][sec_idx][idx] ;
    }
    Ricean_K[ self_idx ][ sec_idx ] = K_mu + K_sigma * cross_corr ;
           // This equation come from 1.3.2.1 (1.7)
  }
  else
  {
    Ricean_K[ self_idx ][ sec_idx ] = 0. ;
  }
}


/*===================================================================
FUNCTION: Set_Correlation_Matrix()

AUTHOR: Kim, Kyung-Won

DESCRIPTION: Set up the parameters of cross-correlation

NOTES: 
   
===================================================================*/
void Set_Correlation_Matrix(int self_idx, int sec_idx)
{
  if (scenario == INDOOR_HOTSPOT)
  {
    if ( los[self_idx][sec_idx] == true )
    {
      sqrt_corr_matrix[0][0] =  0.6212; sqrt_corr_matrix[0][1] =  0.3270; sqrt_corr_matrix[0][2] =  0.4985; sqrt_corr_matrix[0][3] = -0.3974; sqrt_corr_matrix[0][4] = -0.3174; 
      sqrt_corr_matrix[1][0] =  0.3270; sqrt_corr_matrix[1][1] =  0.9236; sqrt_corr_matrix[1][2] =  0.1152; sqrt_corr_matrix[1][3] = -0.1486; sqrt_corr_matrix[1][4] =  0.0682; 
      sqrt_corr_matrix[2][0] =  0.4985; sqrt_corr_matrix[2][1] =  0.1152; sqrt_corr_matrix[2][2] =  0.8321; sqrt_corr_matrix[2][3] = -0.1836; sqrt_corr_matrix[2][4] =  0.1103; 
      sqrt_corr_matrix[3][0] = -0.3974; sqrt_corr_matrix[3][1] = -0.1486; sqrt_corr_matrix[3][2] = -0.1836; sqrt_corr_matrix[3][3] =  0.8568; sqrt_corr_matrix[3][4] =  0.2287; 
      sqrt_corr_matrix[4][0] = -0.3174; sqrt_corr_matrix[4][1] =  0.0682; sqrt_corr_matrix[4][2] =  0.1103; sqrt_corr_matrix[4][3] =  0.2287; sqrt_corr_matrix[4][4] =  0.9111; 
    }
    else
    {
      sqrt_corr_matrix[0][0] =  0.9372; sqrt_corr_matrix[0][1] =  0.2136; sqrt_corr_matrix[0][2] = -0.0313; sqrt_corr_matrix[0][3] = -0.2739; sqrt_corr_matrix[0][4] =  0.0000; 
      sqrt_corr_matrix[1][0] =  0.2136; sqrt_corr_matrix[1][1] =  0.9764; sqrt_corr_matrix[1][2] =  0.0069; sqrt_corr_matrix[1][3] =  0.0313; sqrt_corr_matrix[1][4] =  0.0000; 
      sqrt_corr_matrix[2][0] = -0.0313; sqrt_corr_matrix[2][1] =  0.0069; sqrt_corr_matrix[2][2] =  0.9764; sqrt_corr_matrix[2][3] = -0.2136; sqrt_corr_matrix[2][4] =  0.0000; 
      sqrt_corr_matrix[3][0] = -0.2739; sqrt_corr_matrix[3][1] =  0.0313; sqrt_corr_matrix[3][2] = -0.2136; sqrt_corr_matrix[3][3] =  0.9372; sqrt_corr_matrix[3][4] =  0.0000; 
      sqrt_corr_matrix[4][0] =  0.0000; sqrt_corr_matrix[4][1] =  0.0000; sqrt_corr_matrix[4][2] =  0.0000; sqrt_corr_matrix[4][3] =  0.0000; sqrt_corr_matrix[4][4] =  1.0000; 
    }
  }
  else if ( scenario == URBAN_MACRO )
  {
    if ( los[self_idx][sec_idx] == true )
    {
      sqrt_corr_matrix[0][0] =  0.8063; sqrt_corr_matrix[0][1] =  0.2450; sqrt_corr_matrix[0][2] =  0.4792; sqrt_corr_matrix[0][3] = -0.1204; sqrt_corr_matrix[0][4] = -0.2138; 
      sqrt_corr_matrix[1][0] =  0.2450; sqrt_corr_matrix[1][1] =  0.9241; sqrt_corr_matrix[1][2] = -0.1086; sqrt_corr_matrix[1][3] = -0.2716; sqrt_corr_matrix[1][4] =  0.0218; 
      sqrt_corr_matrix[2][0] =  0.4792; sqrt_corr_matrix[2][1] = -0.1086; sqrt_corr_matrix[2][2] =  0.8257; sqrt_corr_matrix[2][3] = -0.2716; sqrt_corr_matrix[2][4] = -0.0556; 
      sqrt_corr_matrix[3][0] = -0.1204; sqrt_corr_matrix[3][1] = -0.2716; sqrt_corr_matrix[3][2] = -0.2716; sqrt_corr_matrix[3][3] =  0.9152; sqrt_corr_matrix[3][4] = -0.0185; 
      sqrt_corr_matrix[4][0] = -0.2138; sqrt_corr_matrix[4][1] =  0.0218; sqrt_corr_matrix[4][2] =  0.0556; sqrt_corr_matrix[4][3] = -0.0185; sqrt_corr_matrix[4][4] =  0.9749; 
    }
    else
    {
      sqrt_corr_matrix[0][0] =  0.9139; sqrt_corr_matrix[0][1] =  0.1477; sqrt_corr_matrix[0][2] =  0.3180; sqrt_corr_matrix[0][3] = -0.2044; sqrt_corr_matrix[0][4] =  0.0000; 
      sqrt_corr_matrix[1][0] =  0.1477; sqrt_corr_matrix[1][1] =  0.9139; sqrt_corr_matrix[1][2] =  0.2044; sqrt_corr_matrix[1][3] = -0.3180; sqrt_corr_matrix[1][4] =  0.0000; 
      sqrt_corr_matrix[2][0] =  0.3180; sqrt_corr_matrix[2][1] =  0.2044; sqrt_corr_matrix[2][2] =  0.9231; sqrt_corr_matrix[2][3] =  0.0704; sqrt_corr_matrix[2][4] =  0.0000; 
      sqrt_corr_matrix[3][0] = -0.2044; sqrt_corr_matrix[3][1] = -0.3180; sqrt_corr_matrix[3][2] =  0.0704; sqrt_corr_matrix[3][3] =  0.9231; sqrt_corr_matrix[3][4] =  0.0000; 
      sqrt_corr_matrix[4][0] =  0.0000; sqrt_corr_matrix[4][1] =  0.0000; sqrt_corr_matrix[4][2] =  0.0000; sqrt_corr_matrix[4][3] =  0.0000; sqrt_corr_matrix[4][4] =  1.0000; 
    }
  }
  else if ( scenario == URBAN_MICRO && indoor[self_idx][sec_idx] == false )
  {
    if ( los[self_idx][sec_idx] == true )
    {
      sqrt_corr_matrix[0][0] =  0.7531; sqrt_corr_matrix[0][1] =  0.2410; sqrt_corr_matrix[0][2] =  0.4541; sqrt_corr_matrix[0][3] = -0.0972; sqrt_corr_matrix[0][4] = -0.3989; 
      sqrt_corr_matrix[1][0] =  0.2410; sqrt_corr_matrix[1][1] =  0.9294; sqrt_corr_matrix[1][2] =  0.1380; sqrt_corr_matrix[1][3] = -0.2424; sqrt_corr_matrix[1][4] = -0.0208; 
      sqrt_corr_matrix[2][0] =  0.4541; sqrt_corr_matrix[2][1] =  0.1380; sqrt_corr_matrix[2][2] =  0.8615; sqrt_corr_matrix[2][3] = -0.1756; sqrt_corr_matrix[2][4] = -0.0414; 
      sqrt_corr_matrix[3][0] = -0.0972; sqrt_corr_matrix[3][1] = -0.2424; sqrt_corr_matrix[3][2] = -0.1756; sqrt_corr_matrix[3][3] =  0.9157; sqrt_corr_matrix[3][4] =  0.2499; 
      sqrt_corr_matrix[4][0] = -0.3989; sqrt_corr_matrix[4][1] = -0.0208; sqrt_corr_matrix[4][2] = -0.0414; sqrt_corr_matrix[4][3] =  0.2499; sqrt_corr_matrix[4][4] =  0.8811; 
    }
    else
    {
      sqrt_corr_matrix[0][0] =  0.9135; sqrt_corr_matrix[0][1] =  0.0000; sqrt_corr_matrix[0][2] =  0.1780; sqrt_corr_matrix[0][3] = -0.3658; sqrt_corr_matrix[0][4] =  0.0000; 
      sqrt_corr_matrix[1][0] =  0.0000; sqrt_corr_matrix[1][1] =  1.0000; sqrt_corr_matrix[1][2] =  0.0000; sqrt_corr_matrix[1][3] =  0.0000; sqrt_corr_matrix[1][4] =  0.0000; 
      sqrt_corr_matrix[2][0] =  0.1780; sqrt_corr_matrix[2][1] =  0.0000; sqrt_corr_matrix[2][2] =  0.9678; sqrt_corr_matrix[2][3] = -0.1780; sqrt_corr_matrix[2][4] =  0.0000; 
      sqrt_corr_matrix[3][0] = -0.3658; sqrt_corr_matrix[3][1] =  0.0000; sqrt_corr_matrix[3][2] = -0.1780; sqrt_corr_matrix[3][3] =  0.9135; sqrt_corr_matrix[3][4] =  0.0000; 
      sqrt_corr_matrix[4][0] =  0.0000; sqrt_corr_matrix[4][1] =  0.0000; sqrt_corr_matrix[4][2] =  0.0000; sqrt_corr_matrix[4][3] =  0.0000; sqrt_corr_matrix[4][4] =  1.0000; 
    }
  }
  else if (scenario == RURAL_MACRO)
  {
    if ( los[self_idx][sec_idx] == true )
    {
      sqrt_corr_matrix[0][0] =  0.9659; sqrt_corr_matrix[0][1] =  0.0000; sqrt_corr_matrix[0][2] =  0.0000; sqrt_corr_matrix[0][3] = -0.2588; sqrt_corr_matrix[0][4] =  0.0000; 
      sqrt_corr_matrix[1][0] =  0.0000; sqrt_corr_matrix[1][1] =  1.0000; sqrt_corr_matrix[1][2] =  0.0000; sqrt_corr_matrix[1][3] =  0.0000; sqrt_corr_matrix[1][4] =  0.0000; 
      sqrt_corr_matrix[2][0] =  0.0000; sqrt_corr_matrix[2][1] =  0.0000; sqrt_corr_matrix[2][2] =  1.0000; sqrt_corr_matrix[2][3] =  0.0000; sqrt_corr_matrix[2][4] =  0.0000; 
      sqrt_corr_matrix[3][0] = -0.2588; sqrt_corr_matrix[3][1] =  0.0000; sqrt_corr_matrix[3][2] =  0.0000; sqrt_corr_matrix[3][3] =  0.9659; sqrt_corr_matrix[3][4] =  0.0000; 
      sqrt_corr_matrix[4][0] =  0.0000; sqrt_corr_matrix[4][1] =  0.0000; sqrt_corr_matrix[4][2] =  0.0000; sqrt_corr_matrix[4][3] =  0.0000; sqrt_corr_matrix[4][4] =  1.0000; 
    }
    else
    {
      sqrt_corr_matrix[0][0] =  0.9556; sqrt_corr_matrix[0][1] = -0.1735; sqrt_corr_matrix[0][2] =  0.0000; sqrt_corr_matrix[0][3] = -0.2384; sqrt_corr_matrix[0][4] =  0.0000; 
      sqrt_corr_matrix[1][0] = -0.1735; sqrt_corr_matrix[1][1] =  0.9380; sqrt_corr_matrix[1][2] =  0.0000; sqrt_corr_matrix[1][3] =  0.3001; sqrt_corr_matrix[1][4] =  0.0000; 
      sqrt_corr_matrix[2][0] =  0.0000; sqrt_corr_matrix[2][1] =  0.0000; sqrt_corr_matrix[2][2] =  1.0000; sqrt_corr_matrix[2][3] =  0.0000; sqrt_corr_matrix[2][4] =  0.0000; 
      sqrt_corr_matrix[3][0] = -0.2384; sqrt_corr_matrix[3][1] =  0.3001; sqrt_corr_matrix[3][2] =  0.0000; sqrt_corr_matrix[3][3] =  0.9237; sqrt_corr_matrix[3][4] =  0.0000; 
      sqrt_corr_matrix[4][0] =  0.0000; sqrt_corr_matrix[4][1] =  0.0000; sqrt_corr_matrix[4][2] =  0.0000; sqrt_corr_matrix[4][3] =  0.0000; sqrt_corr_matrix[4][4] =  1.0000; 
    }
  }
  else if (scenario == SUBURBAN_MACRO)
  {
    if ( los[self_idx][sec_idx] == true )
    {
      sqrt_corr_matrix[0][0] =  0.8540; sqrt_corr_matrix[0][1] = -0.0407; sqrt_corr_matrix[0][2] =  0.4238; sqrt_corr_matrix[0][3] = -0.2989; sqrt_corr_matrix[0][4] =  0.0000; 
      sqrt_corr_matrix[1][0] = -0.0407; sqrt_corr_matrix[1][1] =  0.9588; sqrt_corr_matrix[1][2] = -0.0234; sqrt_corr_matrix[1][3] = -0.2803; sqrt_corr_matrix[1][4] =  0.0000; 
      sqrt_corr_matrix[2][0] =  0.4238; sqrt_corr_matrix[2][1] = -0.0234; sqrt_corr_matrix[2][2] =  0.8795; sqrt_corr_matrix[2][3] = -0.2151; sqrt_corr_matrix[2][4] =  0.0000; 
      sqrt_corr_matrix[3][0] = -0.2989; sqrt_corr_matrix[3][1] = -0.2803; sqrt_corr_matrix[3][2] = -0.2151; sqrt_corr_matrix[3][3] =  0.8865; sqrt_corr_matrix[3][4] =  0.0000; 
      sqrt_corr_matrix[4][0] =  0.0000; sqrt_corr_matrix[4][1] =  0.0000; sqrt_corr_matrix[4][2] =  0.0000; sqrt_corr_matrix[4][3] =  0.0000; sqrt_corr_matrix[4][4] =  1.0000; 
    }
    else
    {
      sqrt_corr_matrix[0][0] =  0.8889; sqrt_corr_matrix[0][1] = -0.0287; sqrt_corr_matrix[0][2] =  0.3941; sqrt_corr_matrix[0][3] = -0.2318; sqrt_corr_matrix[0][4] =  0.0000; 
      sqrt_corr_matrix[1][0] = -0.0287; sqrt_corr_matrix[1][1] =  0.9769; sqrt_corr_matrix[1][2] =  0.0116; sqrt_corr_matrix[1][3] = -0.2116; sqrt_corr_matrix[1][4] =  0.0000; 
      sqrt_corr_matrix[2][0] =  0.3941; sqrt_corr_matrix[2][1] =  0.0116; sqrt_corr_matrix[2][2] =  0.9176; sqrt_corr_matrix[2][3] =  0.0503; sqrt_corr_matrix[2][4] =  0.0000; 
      sqrt_corr_matrix[3][0] = -0.2318; sqrt_corr_matrix[3][1] = -0.2116; sqrt_corr_matrix[3][2] =  0.0503; sqrt_corr_matrix[3][3] =  0.9481; sqrt_corr_matrix[3][4] =  0.0000; 
      sqrt_corr_matrix[4][0] =  0.0000; sqrt_corr_matrix[4][1] =  0.0000; sqrt_corr_matrix[4][2] =  0.0000; sqrt_corr_matrix[4][3] =  0.0000; sqrt_corr_matrix[4][4] =  1.0000; 
    }
  }
  else if (scenario == OUTDOOR_2_INDOOR || indoor[self_idx][sec_idx] == true )
  {
      sqrt_corr_matrix[0][0] =  0.8970; sqrt_corr_matrix[0][1] =  0.2419; sqrt_corr_matrix[0][2] =  0.2236; sqrt_corr_matrix[0][3] = -0.2949; sqrt_corr_matrix[0][4] =  0.0000; 
      sqrt_corr_matrix[1][0] =  0.2419; sqrt_corr_matrix[1][1] =  0.9592; sqrt_corr_matrix[1][2] = -0.0307; sqrt_corr_matrix[1][3] =  0.1432; sqrt_corr_matrix[1][4] =  0.0000; 
      sqrt_corr_matrix[2][0] =  0.2236; sqrt_corr_matrix[2][1] = -0.0307; sqrt_corr_matrix[2][2] =  0.9735; sqrt_corr_matrix[2][3] =  0.0378; sqrt_corr_matrix[2][4] =  0.0000; 
      sqrt_corr_matrix[3][0] = -0.2949; sqrt_corr_matrix[3][1] =  0.1432; sqrt_corr_matrix[3][2] =  0.0367; sqrt_corr_matrix[3][3] =  0.9440; sqrt_corr_matrix[3][4] =  0.0000; 
      sqrt_corr_matrix[4][0] =  0.0000; sqrt_corr_matrix[4][1] =  0.0000; sqrt_corr_matrix[4][2] =  0.0000; sqrt_corr_matrix[4][3] =  0.2499; sqrt_corr_matrix[4][4] =  1.0000; 
  }
  else
  {
    cerr << "Unspeficied scenario" << endl ;
  } 
}


/*===================================================================
FUNCTION: Set_PathLoss_Parameters()

AUTHOR: Kim, Kyung-Won

DESCRIPTION: Set up parameters of Path Loss

NOTES:
   
===================================================================*/
void Set_PathLoss_Parameters(int self_idx, int sec_idx)
{
  if ( scenario == INDOOR_HOTSPOT )
  {
    h_BS = 6.; h_MS = 1.5;
  }
  else if ( scenario == URBAN_MACRO )
  {
    h_BS = 25.; h_MS = 1.5; h = 20.; W = 20.;
    break_point_distance = 4. * ( h_BS - 1. ) * ( h_MS - 1. ) * carrier_freq / SPEED_LIGHT;
  }
  else if ( scenario == URBAN_MICRO && indoor[self_idx][sec_idx] == false )
  {
    h_BS = 10.; h_MS = 1.5;
    break_point_distance = 4. * ( h_BS - 1. ) * ( h_MS - 1. ) * carrier_freq / SPEED_LIGHT;
  }
  else if ( scenario == RURAL_MACRO )
  {
    h_BS = 35.; h_MS = 1.5; h = 5.; W = 20.;
    break_point_distance = 2. * PI * h_BS * h_MS * carrier_freq / SPEED_LIGHT;
  }
  else if ( scenario == SUBURBAN_MACRO )
  {
    h_BS = 35.; h_MS = 1.5; h = 10.; W = 20.;
    break_point_distance = 2. * PI * h_BS * h_MS * carrier_freq / SPEED_LIGHT;
  }
  else if ( scenario == OUTDOOR_2_INDOOR || indoor[self_idx][sec_idx] == true )
  {
    h_BS = 10.; h_MS = 1.5; W = 20.;
    break_point_distance = 4. * ( h_BS - 1. ) * ( h_MS - 1. ) * carrier_freq / SPEED_LIGHT;
  }
  else
  {
    cerr << "Unspeficied scenario" << endl ;
  } 
}
        // These parameters come from Table A1-2 in m


/*===================================================================
FUNCTION: Compute_PathLoss()

AUTHOR: Kim, Kyung-Won

DESCRIPTION: Compute the Path Loss 

NOTES: Output is a path_loss in [dB]
   
===================================================================*/
void Compute_PathLoss(int self_idx, int sec_idx)
{
  if ( los[self_idx][sec_idx] == true )
  {
    if ( scenario == INDOOR_HOTSPOT )
    {
      path_loss[sec_idx] = 16.9*log10( ue2sector_distance[self_idx][sec_idx] ) + 32.8 + 20.*log10( carrier_freq / 1000000000. ) ;
    }
    else if ( scenario == URBAN_MACRO )
    {
      if ( 10. < ue2sector_distance[self_idx][sec_idx]
             && ue2sector_distance[self_idx][sec_idx] < break_point_distance)
      {
        path_loss[sec_idx] = 22.*log10( ue2sector_distance[self_idx][sec_idx] ) + 28. + 20.*log10( carrier_freq / 1000000000. );
      }
      else if ( break_point_distance < ue2sector_distance[self_idx][sec_idx]
              && ue2sector_distance[self_idx][sec_idx] < 5000.)
      {
        double d_1 = ue2sector_distance[self_idx][sec_idx] ;
                  // the distance from the BS to the centre of the perpendicular street in Manhattan grid
        path_loss[sec_idx] = 40. * log10( d_1 ) + 7.8 - 18.*log10( h_BS - 1. ) - 18.*log10( h_MS - 1. ) + 2.*log10( carrier_freq / 1000000000. );
      }
    }
    else if ( scenario == URBAN_MICRO && indoor[self_idx][sec_idx] == false )
    {
      if ( 10. < ue2sector_distance[self_idx][sec_idx]
             && ue2sector_distance[self_idx][sec_idx] < break_point_distance)
      {
        path_loss[sec_idx] = 22.*log10( ue2sector_distance[self_idx][sec_idx] ) + 28. + 20.*log10( carrier_freq / 1000000000. );
      }
      else if ( break_point_distance < ue2sector_distance[self_idx][sec_idx]
              && ue2sector_distance[self_idx][sec_idx] < 5000.)
      {
        double d_1 = ue2sector_distance[self_idx][sec_idx] ;
                  // the distance from the BS to the centre of the perpendicular street in Manhattan grid
        path_loss[sec_idx] = 40. * log10( d_1 ) + 7.8 - 18.*log10( h_BS - 1. ) - 18.*log10( h_MS - 1. ) + 2.*log10( carrier_freq / 1000000000. );
      }
    }
    else if ( scenario == RURAL_MACRO || scenario == SUBURBAN_MACRO )
    {
      if ( 10. < ue2sector_distance[self_idx][sec_idx] 
             && ue2sector_distance[self_idx][sec_idx]< break_point_distance)
      {
        path_loss[sec_idx] = 20.*log10( 40. * PI * ue2sector_distance[self_idx][sec_idx] * carrier_freq / 1000000000. / 3. )
                             + MIN( 0.03 * pow(h,1.72) , 10. ) * log10( ue2sector_distance[self_idx][sec_idx] )
                             - MIN( 0.044 * pow(h,1.72) , 14.77 ) 
                             + 0.002 * log10(h) * ue2sector_distance[self_idx][sec_idx] ;
      }
      else if ( break_point_distance < ue2sector_distance[self_idx][sec_idx] 
              && ue2sector_distance[self_idx][sec_idx]< 10000.)
      {
        path_loss[sec_idx] = 20.*log10( 40. * PI * break_point_distance * carrier_freq / 1000000000. / 3. )
                             + MIN( 0.03 * pow(h,1.72) , 10. ) * log10( break_point_distance )
                             - MIN( 0.044 * pow(h,1.72) , 14.77 ) 
                             + 0.002 * log10(h) * break_point_distance
                             + 40.*log10( ue2sector_distance[self_idx][sec_idx] / break_point_distance ) ;
      }
    }
    else                         
    { 
      cerr << "Unspeficied scenario" << endl ; 
    } 
  }
  else
  {
    if ( scenario == INDOOR_HOTSPOT )
    {
      path_loss[sec_idx] = 43.3*log10( ue2sector_distance[self_idx][sec_idx] ) + 11.5 + 20.*log10( carrier_freq / 1000000000. ) ;
    }
    else if ( scenario == URBAN_MICRO && indoor[self_idx][sec_idx] == false )
    {
      path_loss[sec_idx] = 36.7*log10( ue2sector_distance[self_idx][sec_idx] ) + 22.7 + 26.*log10( carrier_freq / 1000000000. ) ;
    }
    else if ( scenario == URBAN_MACRO || scenario == RURAL_MACRO || scenario == SUBURBAN_MACRO )
    {
      path_loss[sec_idx] = 161.04 - 7.1*log10(W) + 7.5*log10(h)
                           - ( 24.37 - 3.7 * (h/h_BS) * (h/h_BS) ) * log10(h_BS)
                           + ( 43.42 - 3.1*log10(h_BS) ) * ( log10( ue2sector_distance[self_idx][sec_idx] ) - 3 )
                           + 20.*log10(carrier_freq / 1000000000.)
                           - ( 3.2 * ( log10( 11.75 * h_MS ) ) * ( log10( 11.75 * h_MS ) ) - 4.97 ) ;
    }
    else if (scenario == OUTDOOR_2_INDOOR || indoor[self_idx][sec_idx] == true)
    {
      double d_in = 25. * Rand.u() ;
      double PL_b, PL_tw, PL_in ;
      if ( los_type == NLOS )
      {
        PL_b = 36.7*log10( ue2sector_distance[self_idx][sec_idx] ) + 22.7 + 26.*log10( carrier_freq / 1000000000. ) ;
      }
      else if ( los_type == LOS )
      {
        if ( 10. < ue2sector_distance[self_idx][sec_idx]
               && ue2sector_distance[self_idx][sec_idx] < break_point_distance)
        {
          PL_b = 22.*log10( ue2sector_distance[self_idx][sec_idx] ) + 28. + 20.*log10( carrier_freq / 1000000000. );
        }
        else if ( break_point_distance < ue2sector_distance[self_idx][sec_idx]
                && ue2sector_distance[self_idx][sec_idx] < 5000.)
        {
          double d_1 = ue2sector_distance[self_idx][sec_idx] ;
                    // the distance from the BS to the centre of the perpendicular street in Manhattan grid
          PL_b = 40. * log10( d_1 ) + 7.8 - 18.*log10( h_BS - 1. ) - 18.*log10( h_MS - 1. ) + 2.*log10( carrier_freq / 1000000000. );
        }
      }
      else
      {
        double LOS_prob = MIN( 18./ue2sector_distance[self_idx][sec_idx] , 1. ) * ( 1. - exp( -ue2sector_distance[self_idx][sec_idx]/36. ) ) 
                          + exp( -ue2sector_distance[self_idx][sec_idx]/36. );
        if ( Rand.u() < LOS_prob )
        {
          if ( 10. < ue2sector_distance[self_idx][sec_idx]
                 && ue2sector_distance[self_idx][sec_idx] < break_point_distance)
          {
            PL_b = 22.*log10( ue2sector_distance[self_idx][sec_idx] ) + 28. + 20.*log10( carrier_freq / 1000000000. );
          }
          else if ( break_point_distance < ue2sector_distance[self_idx][sec_idx]
                  && ue2sector_distance[self_idx][sec_idx] < 5000.)
          {
            double d_1 = ue2sector_distance[self_idx][sec_idx] ;
                      // the distance from the BS to the centre of the perpendicular street in Manhattan grid
            PL_b = 40. * log10( d_1 ) + 7.8 - 18.*log10( h_BS - 1. ) - 18.*log10( h_MS - 1. ) + 2.*log10( carrier_freq / 1000000000. );
          }
        }
        else
        {
          PL_b = 36.7*log10( ue2sector_distance[self_idx][sec_idx] ) + 22.7 + 26.*log10( carrier_freq / 1000000000. ) ;
        }
      }
      PL_tw = 20. ;
      PL_in = 0.5 * d_in ;
      path_loss[sec_idx] = PL_b + PL_tw + PL_in ;
    }
    else                         
    { 
      cerr << "Unspeficied scenario" << endl ; 
    }
  }
}
        // These equations come from Table A1-2 in dB


/*===================================================================
FUNCTION: Set_Shadowing()

AUTHOR: Kim, Kyung-Won

DESCRIPTION: Set up the Shadowing 

NOTES: Output is a shadowing [dB]
   
===================================================================*/
void Set_Shadowing(int self_idx, int sec_idx)
{
  if (scenario == INDOOR_HOTSPOT)
  {
    if ( los[self_idx][sec_idx] == true )
    {
      shadowing_sigma = 3.;
    }
    else
    {
      shadowing_sigma = 4.;
    }
  }
  else if ( scenario == URBAN_MACRO )
  {
    if ( los[self_idx][sec_idx] == true )
    {
      shadowing_sigma = 4.;
    }
    else
    {
      shadowing_sigma = 6.;
    }
  }
  else if (scenario == URBAN_MICRO && indoor[self_idx][sec_idx] == false)
  {
    if ( los[self_idx][sec_idx] == true )
    {
      shadowing_sigma = 3.;
    }
    else
    {
      shadowing_sigma = 4.;
    }
  }
  else if (scenario == RURAL_MACRO)
  {
    if ( los[self_idx][sec_idx] == true )
    {
      if ( 10. < ue2sector_distance[self_idx][sec_idx]
             && ue2sector_distance[self_idx][sec_idx] < break_point_distance)
      {
        shadowing_sigma = 4.;
      }
      else if ( break_point_distance < ue2sector_distance[self_idx][sec_idx]
              && ue2sector_distance[self_idx][sec_idx] < 5000.)
      {
        shadowing_sigma = 6.;
      }
    }
    else
    {
      shadowing_sigma = 8.;
    }
  }
  else if (scenario == SUBURBAN_MACRO)
  {
    if ( los[self_idx][sec_idx] == true )
    {
      if ( 10. < ue2sector_distance[self_idx][sec_idx]
             && ue2sector_distance[self_idx][sec_idx] < break_point_distance)
      {
        shadowing_sigma = 4.;
      }
      else if ( break_point_distance < ue2sector_distance[self_idx][sec_idx]
              && ue2sector_distance[self_idx][sec_idx] < 5000.)
      {
        shadowing_sigma = 6.;
      }
    }
    else
    {
      shadowing_sigma = 8.;
    }
  }
  else if (scenario == OUTDOOR_2_INDOOR || indoor[self_idx][sec_idx] == true)
  {
  	shadowing_sigma = 7.;
  }  
  else
  {
    cerr << "Unspeficied scenario" << endl ;
  } 

  double cross_corr = 0 ;
  for( int idx = 0; idx < 5; idx++ )
  {
    cross_corr = cross_corr + sqrt_corr_matrix[3][idx] * random_number[self_idx][sec_idx][idx] ;
  }
  shadowing = shadowing_sigma * cross_corr ;
}
        // This parameter comes from Table A1-7 in dB


/*===================================================================
FUNCTION: FadingChannelGeneration()

AUTHOR: Kim, Kyung-Won

DESCRIPTION: Fading Samples are generated and stored

NOTES: ...
   
===================================================================*/
void FadingChannelGeneration( int self_idx, int u, int s, int adj_sec_idx )
{
  my_sector = ue[self_idx].adj_sector[adj_sec_idx] ;

  if( u == 0 && s == 0 )
  {
    Set_Delay_Parameters(self_idx) ;
    Set_AoAAoD_Parameters(self_idx) ;
  }
  Set_Num_Path(self_idx, adj_sec_idx) ;
  Set_Cluster_Delay(self_idx, u, s, adj_sec_idx) ;
  Set_Cluster_Power(self_idx, u, s, adj_sec_idx) ;
  Scaling_Delay(self_idx, u, s, adj_sec_idx) ;
  Generate_AoAAoD(self_idx, u, s, adj_sec_idx) ;
  Set_Subclusters(self_idx, u, s, adj_sec_idx) ;
  Generate_XPR(self_idx, u, s, adj_sec_idx) ;
  SetInitialPhase(self_idx, u, s, adj_sec_idx) ;
}


/*===================================================================
FUNCTION: Set_Delay_Parameters()

AUTHOR: Kim, Kyung-Won

DESCRIPTION: Set up parameters of Delay Spread

NOTES:
   
===================================================================*/
void Set_Delay_Parameters(int self_idx)
{   
  if (scenario == INDOOR_HOTSPOT)
  {
    if ( los[self_idx][my_sector] == true )
    {
      r = 3.6; ds_mu = -7.7; ds_sigma = 0.18;
    }
    else
    {
      r = 3.0; ds_mu = -7.41; ds_sigma = 0.14;
    }
  }
  else if ( scenario == URBAN_MACRO )
  {
    if ( los[self_idx][my_sector] == true )
    {
      r = 2.5; ds_mu = -7.03; ds_sigma = 0.66;
    }
    else
    {
      r = 2.3; ds_mu = -6.44; ds_sigma = 0.39;
    }
  }
  else if (scenario == URBAN_MICRO && indoor[self_idx][my_sector] == false)
  {
    if ( los[self_idx][my_sector] == true )
    {
      r = 3.2; ds_mu = -7.19; ds_sigma = 0.4;
    }
    else
    {
      r = 3.0; ds_mu = -6.89; ds_sigma = 0.54;
    }
  }
  else if (scenario == RURAL_MACRO)
  {
    if ( los[self_idx][my_sector] == true )
    {
      r = 3.8; ds_mu = -7.49; ds_sigma = 0.55;
    }
    else
    {
      r = 1.7; ds_mu = -7.43; ds_sigma = 0.48;
    }
  }
  else if (scenario == SUBURBAN_MACRO)
  {
    if ( los[self_idx][my_sector] == true )
    {
      r = 2.4; ds_mu = -7.23; ds_sigma = 0.38;
    }
    else
    {
      r = 1.5; ds_mu = -7.12; ds_sigma = 0.33;
    }
  }
  else if (scenario == OUTDOOR_2_INDOOR || indoor[self_idx][my_sector] == true)
  {
      r = 2.2; ds_mu = -6.62; ds_sigma = 0.32;
  }
  else
  {
    cerr << "Unspeficied scenario" << endl ;
  } 
  D = 0.7705 - 0.0433*Ricean_K[ self_idx ][ my_sector ] + 0.0002*pow( Ricean_K[ self_idx ][ my_sector ], 2 ) + 0.000017*pow( Ricean_K[ self_idx ][ my_sector ], 3 ) ;
  
  double cross_corr = 0 ;
  for( int idx = 0; idx < 5; idx++ )
  {
    cross_corr = cross_corr + sqrt_corr_matrix[0][idx] * random_number[self_idx][my_sector][idx] ;
  }
  DS_sigma = pow( 10, ds_mu + ds_sigma * cross_corr ) ;
}

        // These parameters come from Table A1-7

/*===================================================================
FUNCTION: Set_AoAAoD_Parameters()

AUTHOR: Kim, Kyung-Won

DESCRIPTION: Set up parameters of Angle Spread

NOTES: 
   
===================================================================*/
void Set_AoAAoD_Parameters(int self_idx)
{
  if (scenario == INDOOR_HOTSPOT)
  {
    if ( los[self_idx][my_sector] == true )
    {
      AoA_mu = 1.62 ;
      AoD_mu = 1.60 ;
      AoA_sigma = 0.22 ;
      AoD_sigma = 0.18 ;
      cluster_ASA = 8.;
      cluster_ASD = 5.;
      C = 1.434 * ( 0.9275 + 0.0439*Ricean_K[ self_idx ][ my_sector ] - 0.0071*pow( Ricean_K[ self_idx ][ my_sector ], 2 ) + 0.0002*pow( Ricean_K[ self_idx ][ my_sector ], 3 ) ) ;
    }
    else
    {
      AoA_mu = 1.77 ;
      AoD_mu = 1.62 ;
      AoA_sigma = 0.16 ;
      AoD_sigma = 0.25 ;      
      cluster_ASA = 11.;
      cluster_ASD = 5.;
      C = 1.501 ;
    }
  }
  else if (scenario == URBAN_MACRO)
  {
    if ( los[self_idx][my_sector] == true )
    {
      AoA_mu = 1.81 ;
      AoD_mu = 1.15 ;
      AoA_sigma = 0.2 ;
      AoD_sigma = 0.28 ;
      cluster_ASA = 11.;
      cluster_ASD = 5.;
      C = 1.146 * ( 1.1035 - 0.028*Ricean_K[ self_idx ][ my_sector ] - 0.002*pow( Ricean_K[ self_idx ][ my_sector ], 2 ) + 0.0001*pow( Ricean_K[ self_idx ][ my_sector ], 3 ) ) ;
    }
    else
    {
      AoA_mu = 1.87 ;
      AoD_mu = 1.41 ;
      AoA_sigma = 0.11 ;
      AoD_sigma = 0.28 ;      
      cluster_ASA = 15;
      cluster_ASD = 2.;
      C = 1.289 ;
    }
  }
  else if (scenario == URBAN_MICRO && indoor[self_idx][my_sector] == false)
  {
    if ( los[self_idx][my_sector] == true )
    {
      AoA_mu = 1.75 ;
      AoD_mu = 1.20 ;
      AoA_sigma = 0.19 ;
      AoD_sigma = 0.43 ;
      cluster_ASA = 17.;
      cluster_ASD = 3.;
      C = 1.146 * ( 1.1035 - 0.028*Ricean_K[ self_idx ][ my_sector ] - 0.002*pow( Ricean_K[ self_idx ][ my_sector ], 2 ) + 0.0001*pow( Ricean_K[ self_idx ][ my_sector ], 3 ) ) ;
    }
    else
    {
      AoA_mu = 1.84 ;
      AoD_mu = 1.41 ;
      AoA_sigma = 0.15 ;
      AoD_sigma = 0.17 ; 
      cluster_ASA = 22.;
      cluster_ASD = 10.;
      C = 1.273 ;
    }
  }
  else if (scenario == RURAL_MACRO)
  {
    if ( los[self_idx][my_sector] == true )
    {
      AoA_mu = 1.52 ;
      AoD_mu = 0.90 ;
      AoA_sigma = 0.24 ;
      AoD_sigma = 0.38 ;
      cluster_ASA = 3.;
      cluster_ASD = 2.;
      C = 1.123 * ( 1.1035 - 0.028*Ricean_K[ self_idx ][ my_sector ] - 0.002*pow( Ricean_K[ self_idx ][ my_sector ], 2 ) + 0.0001*pow( Ricean_K[ self_idx ][ my_sector ], 3 ) ) ;
    }
    else
    {
      AoA_mu = 1.52 ;
      AoD_mu = 0.95 ;
      AoA_sigma = 0.13 ;
      AoD_sigma = 0.45 ;      
      cluster_ASA = 3.;
      cluster_ASD = 2.;
      C = 1.090 ;
    }
  }
  else if (scenario == SUBURBAN_MACRO)
  {
    if ( los[self_idx][my_sector] == true )
    {
      AoA_mu = 1.48 ;
      AoD_mu = 0.78 ;
      AoA_sigma = 0.2 ;
      AoD_sigma = 0.12 ;
      cluster_ASA = 5.;
      cluster_ASD = 5.;
      C = 1.211 * ( 1.1035 - 0.028*Ricean_K[ self_idx ][ my_sector ] - 0.002*pow( Ricean_K[ self_idx ][ my_sector ], 2 ) + 0.0001*pow( Ricean_K[ self_idx ][ my_sector ], 3 ) ) ;
    }
    else
    {
      AoA_mu = 1.65 ;
      AoD_mu = 0.90 ;
      AoA_sigma = 0.25 ;
      AoD_sigma = 0.36 ;      
      cluster_ASA = 10.;
      cluster_ASD = 2.;
      C = 1.190 ;
    }
  }
  else if (scenario == OUTDOOR_2_INDOOR || indoor[self_idx][my_sector] == true)
  {
    AoA_mu = 1.76 ;
    AoD_mu = 1.25 ;
    AoA_sigma = 0.16 ;
    AoD_sigma = 0.42 ;      
    cluster_ASA = 8.;
    cluster_ASD = 5.;
    C = 1.146 ;
  }
  else                         
  { 
    cerr << "Unspeficied scenario" << endl ; 
  } 

  double cross_corr = 0 ;
  for( int idx = 0; idx < 5; idx++ )
  {
    cross_corr = cross_corr + sqrt_corr_matrix[2][idx] * random_number[self_idx][my_sector][idx] ;
  }
  ASA_sigma = pow( 10, AoA_mu + AoA_sigma * cross_corr ) ;

  cross_corr = 0 ;
  for( int idx = 0; idx < 5; idx++ )
  {
    cross_corr = cross_corr + sqrt_corr_matrix[1][idx] * random_number[self_idx][my_sector][idx] ;
  }
  ASD_sigma = pow( 10, AoD_mu + AoD_sigma * cross_corr ) ;
}
        // These parameters come from
        // Table A1-7 & 1.3.2.1 (1.13a) & 1.3.2.1 (1.13b)

/*===================================================================
FUNCTION: Set_Num_Path()

AUTHOR: Kim, Kyung-Won

DESCRIPTION: Set up the number of clusters

NOTES:
   
===================================================================*/
void Set_Num_Path(int self_idx, int adj_sec_idx)
{
  if ( scenario == INDOOR_HOTSPOT )
  {
    if ( los[self_idx][my_sector] == true )
    {
      num_path[self_idx][adj_sec_idx] = 15 ;
      clu_shadowing_std = 6. ;
    }
    else
    {
      num_path[self_idx][adj_sec_idx] = 19 ;
      clu_shadowing_std = 3. ;
    }
  }
  else if ( scenario == URBAN_MACRO )
  {
    if ( los[self_idx][my_sector] == true )
    {
      num_path[self_idx][adj_sec_idx] = 12 ;
      clu_shadowing_std = 3. ;
    }
    else
    {
      num_path[self_idx][adj_sec_idx] = 20 ;
      clu_shadowing_std = 3. ;
    }
  }
  else if ( scenario == URBAN_MICRO && indoor[self_idx][my_sector] == false )
  {
    if ( los[self_idx][my_sector] == true )
    {
      num_path[self_idx][adj_sec_idx] = 12 ;
      clu_shadowing_std = 3. ;
    }
    else
    {
      num_path[self_idx][adj_sec_idx] = 19 ;
      clu_shadowing_std = 3. ;
    }
  }
  else if ( scenario == RURAL_MACRO )
  {
    if ( los[self_idx][my_sector] == true )
    {
      num_path[self_idx][adj_sec_idx] = 11 ;
      clu_shadowing_std = 3. ;
    }
    else
    {
      num_path[self_idx][adj_sec_idx] = 10 ;
      clu_shadowing_std = 3. ;
    }
  }
  else if ( scenario == SUBURBAN_MACRO )
  {
    if ( los[self_idx][my_sector] == true )
    {
      num_path[self_idx][adj_sec_idx] = 15 ;
      clu_shadowing_std = 3. ;
    }
    else
    {
      num_path[self_idx][adj_sec_idx] = 14 ;
      clu_shadowing_std = 3. ;
    }
  }
  else if (scenario == OUTDOOR_2_INDOOR || indoor[self_idx][my_sector] == true)
  {
    num_path[self_idx][adj_sec_idx] = 12 ;
    clu_shadowing_std = 4. ;
  }  
  else
  {
    cerr << "Unspeficied scenario" << endl ;
  } 
}
        // This parameter comes from Table A1-7

/*===================================================================
FUNCTION: Set_Cluster_Delay()

AUTHOR: Kim, Kyung-Won

DESCRIPTION: Set up delays of clusters

NOTES: Output is a delay in [micro sec].
   
===================================================================*/
void Set_Cluster_Delay(int self_idx, int u, int s, int adj_sec_idx)
{  
  for(int cluster_idx = 0; cluster_idx < num_path[self_idx][adj_sec_idx]; cluster_idx++)
  {
    delay[self_idx][adj_sec_idx][cluster_idx][u][s] = -r * DS_sigma * log( Rand.u() ) ;
  }        // This equation come from 1.3.2.1 (1.5)

  Sort_Delay(self_idx, u, s, adj_sec_idx) ;
  
  for(int cluster_idx = num_path[self_idx][adj_sec_idx] - 1; cluster_idx > -1; cluster_idx--)
  {
    delay[self_idx][adj_sec_idx][cluster_idx][u][s] = delay[self_idx][adj_sec_idx][cluster_idx][u][s] - delay[self_idx][adj_sec_idx][0][u][s] ;
    if( PDF == 1 && adj_sec_idx == 0 )
    {
      Block_SS_1[ drop_idx ][self_idx][cluster_idx][u][s] = delay[self_idx][0][cluster_idx][u][s] ;
    }
  }        // This equation come from 1.3.2.1 (1.6)
}

/*===================================================================
FUNCTION: Sort_Delay()

AUTHOR: Kim, Kyung-Won

DESCRIPTION: Sort delays

NOTES: 
   
===================================================================*/
void Sort_Delay(int self_idx, int u, int s, int adj_sec_idx)
{
  for(int i = 1; i < num_path[self_idx][adj_sec_idx]; i++)
  {
    double temp;
    int j ;

    temp = delay[self_idx][adj_sec_idx][i][u][s];
    for(j = i; j > 0; j--)
    {
      if( delay[self_idx][adj_sec_idx][j-1][u][s] > temp )
        delay[self_idx][adj_sec_idx][j][u][s] = delay[self_idx][adj_sec_idx][j-1][u][s];
      else break;
    }
    delay[self_idx][adj_sec_idx][j][u][s] = temp;
  }
}

/*===================================================================
FUNCTION: Set_Cluster_Power()

AUTHOR: Kim, Kyung-Won

DESCRIPTION: Set up powers of clusters

NOTES: Output is a cluster_power
   
===================================================================*/
void Set_Cluster_Power(int self_idx, int u, int s, int adj_sec_idx)
{
  double sum_p = 0;
  max_p = 0;
  
  for(int cluster_idx = 0; cluster_idx < num_path[self_idx][adj_sec_idx]; cluster_idx++)
  {
    cluster_power[self_idx][adj_sec_idx][cluster_idx][u][s] = exp( -delay[self_idx][adj_sec_idx][cluster_idx][u][s] * (r-1.) / (r*DS_sigma) ) * pow( 10., -clu_shadowing_std * Rand.n() / 10. ) ;
    sum_p = sum_p + cluster_power[self_idx][adj_sec_idx][cluster_idx][u][s] ;
  }        // This equation come from 1.3.2.1 (1.9)
  for(int cluster_idx = 0; cluster_idx < num_path[self_idx][adj_sec_idx]; cluster_idx++)
  {
    cluster_power[self_idx][adj_sec_idx][cluster_idx][u][s] = cluster_power[self_idx][adj_sec_idx][cluster_idx][u][s] / sum_p ;
    max_p = MAX( max_p , cluster_power[self_idx][adj_sec_idx][cluster_idx][u][s] ) ;
  }        // This equation come from 1.3.2.1 (1.10)

  for(int cluster_idx = 0; cluster_idx < num_path[self_idx][adj_sec_idx]; cluster_idx++)
  {
    if(max_p / 316.2278 > cluster_power[self_idx][adj_sec_idx][cluster_idx][u][s])
    {
      cluster_power[self_idx][adj_sec_idx][cluster_idx][u][s] = 0 ;
    }
    if( PDF == 1 && adj_sec_idx == 0 )
    {
      Block_SS_3[ drop_idx ][self_idx][cluster_idx][u][s] = cluster_power[self_idx][0][cluster_idx][u][s] ;
    }
  }
}    

/*===================================================================
FUNCTION: Scaling_Delay()

AUTHOR: Kim, Kyung-Won

DESCRIPTION: Scaling delays

NOTES: 
   
===================================================================*/
void Scaling_Delay(int self_idx, int u, int s, int adj_sec_idx)
{
  if ( los[self_idx][my_sector] == true )
  {
    for(int cluster_idx = 0; cluster_idx < num_path[self_idx][adj_sec_idx]; cluster_idx++)
    {
      delay[self_idx][adj_sec_idx][cluster_idx][u][s] = delay[self_idx][adj_sec_idx][cluster_idx][u][s] / D ;
    }      // This equation come from 1.3.2.1 (1.8)
  }
  for(int cluster_idx = 0; cluster_idx < num_path[self_idx][adj_sec_idx]; cluster_idx++)
  {
    if( PDF == 1 && adj_sec_idx == 0 )
    {
      Block_SS_2[ drop_idx ][self_idx][cluster_idx][u][s] = delay[self_idx][0][cluster_idx][u][s] ;
    }
  }      // This equation come from 1.3.2.1 (1.8)
}

/*===================================================================
FUNCTION: Generate_AoAAoD()

AUTHOR: Kim, Kyung-Won

DESCRIPTION: Generate Angle of Arrival and Angle of Departure

NOTES: 
   
===================================================================*/
void Generate_AoAAoD(int self_idx, int u, int s, int adj_sec_idx)
{
  double first_arrival_angle ;
  double first_depart_angle ;
  for(int cluster_idx = 0; cluster_idx < num_path[self_idx][adj_sec_idx]; cluster_idx++)
  {
    // Generate AoA
    int X = 2 * (int)( 2 * Rand.u() ) - 1 ;
    double Y = ASA_sigma / 7. * Rand.n() ;
    if( cluster_idx == 0 )
    {
      X_A1 = X ;
      Y_A1 = Y ;
    }

    if( scenario == INDOOR_HOTSPOT )
    {
      arrival_angle[self_idx][adj_sec_idx][cluster_idx][u][s] = -1 * ASA_sigma * log( cluster_power[self_idx][adj_sec_idx][cluster_idx][u][s] / max_p ) / C ;
    }
    else
    {
      arrival_angle[self_idx][adj_sec_idx][cluster_idx][u][s] = 2. * ASA_sigma / 1.4 * sqrt( -log( cluster_power[self_idx][adj_sec_idx][cluster_idx][u][s] / max_p ) ) / C ;
    }
    if( cluster_idx == 0 )
    {
      first_arrival_angle = arrival_angle[self_idx][adj_sec_idx][cluster_idx][u][s] ;
    }
    if( PDF == 1 && adj_sec_idx == 0 )
    {
      Block_SS_5[ drop_idx ][self_idx][cluster_idx][u][s] = arrival_angle[self_idx][0][cluster_idx][u][s] ;
    }
    
    if( los[self_idx][my_sector] == true )
    {
      arrival_angle[self_idx][adj_sec_idx][cluster_idx][u][s] = X * arrival_angle[self_idx][adj_sec_idx][cluster_idx][u][s] + Y
                                   - ( X_A1 * first_arrival_angle + Y_A1 - ue2sector_angle[self_idx][my_sector] ) ;
    }
    else
    {
      arrival_angle[self_idx][adj_sec_idx][cluster_idx][u][s] = X * arrival_angle[self_idx][adj_sec_idx][cluster_idx][u][s] + Y + ue2sector_angle[self_idx][my_sector] ;
    }
    if( PDF == 1 && adj_sec_idx == 0 )
    {
      Block_SS_7[ drop_idx ][self_idx][cluster_idx][u][s] = arrival_angle[self_idx][0][cluster_idx][u][s] ;
    }

    // Generate AoD
    X = 2 * (int)( 2 * Rand.u() ) - 1 ;
    Y = ASD_sigma / 7. * Rand.n() ;
    if( cluster_idx == 0 )
    {
      X_D1 = X ;
      Y_D1 = Y ;
    }

    if ( scenario == INDOOR_HOTSPOT )
    {
      depart_angle[self_idx][adj_sec_idx][cluster_idx][u][s] = -1 * ASD_sigma * log( cluster_power[self_idx][adj_sec_idx][cluster_idx][u][s] / max_p ) / C ;
    }
    else
    {
      depart_angle[self_idx][adj_sec_idx][cluster_idx][u][s] = 2. * ASD_sigma / 1.4 * sqrt(-log( cluster_power[self_idx][adj_sec_idx][cluster_idx][u][s] / max_p )) / C ;
    }
    if( cluster_idx == 0 )
    {
      first_depart_angle = depart_angle[self_idx][adj_sec_idx][cluster_idx][u][s] ;
    }
    if( PDF == 1 && adj_sec_idx == 0 )
    {
      Block_SS_4[ drop_idx ][self_idx][cluster_idx][u][s] = depart_angle[self_idx][0][cluster_idx][u][s] ;
    }

    if ( los[self_idx][my_sector] == true )
    {
      depart_angle[self_idx][adj_sec_idx][cluster_idx][u][s] = X * depart_angle[self_idx][adj_sec_idx][cluster_idx][u][s] + Y
                                   - ( X_D1 * first_depart_angle + Y_D1 - ue2sector_angle[self_idx][my_sector] ) ;
    }
    else
    {
      depart_angle[self_idx][adj_sec_idx][cluster_idx][u][s] = X * depart_angle[self_idx][adj_sec_idx][cluster_idx][u][s] + Y + ue2sector_angle[self_idx][my_sector] ;
    }
    if( PDF == 1 && adj_sec_idx == 0 )
    {
      Block_SS_6[ drop_idx ][self_idx][cluster_idx][u][s] = depart_angle[self_idx][0][cluster_idx][u][s] ;
    }
  }
}   
             // This equations come from 1.3.2.1 (1.11) & (1.14) & (1.15)


/*===================================================================
FUNCTION: Set_Subclusters()

AUTHOR: Kim, Kyung-Won

DESCRIPTION: for the two strongest clusters, divide by three sub-clusters

NOTES: 
   
===================================================================*/
void Set_Subclusters(int self_idx, int u, int s, int adj_sec_idx)
{
  Find_Max_idx(self_idx, u, s, adj_sec_idx) ;

  delay[self_idx][adj_sec_idx][ num_path[self_idx][adj_sec_idx]   ][u][s] = delay[self_idx][adj_sec_idx][ max_idx     ][u][s] + 0.000000005 ;
  delay[self_idx][adj_sec_idx][ num_path[self_idx][adj_sec_idx]+1 ][u][s] = delay[self_idx][adj_sec_idx][ max_idx     ][u][s] + 0.00000001  ;
  delay[self_idx][adj_sec_idx][ num_path[self_idx][adj_sec_idx]+2 ][u][s] = delay[self_idx][adj_sec_idx][ max_idx_2nd ][u][s] + 0.000000005 ;
  delay[self_idx][adj_sec_idx][ num_path[self_idx][adj_sec_idx]+3 ][u][s] = delay[self_idx][adj_sec_idx][ max_idx_2nd ][u][s] + 0.00000001  ;
  
  cluster_power[self_idx][adj_sec_idx][ max_idx        ][u][s] = cluster_power[self_idx][adj_sec_idx][ max_idx     ][u][s] * 10./20. ;
  cluster_power[self_idx][adj_sec_idx][ max_idx_2nd    ][u][s] = cluster_power[self_idx][adj_sec_idx][ max_idx_2nd ][u][s] * 10./20. ;
  cluster_power[self_idx][adj_sec_idx][ num_path[self_idx][adj_sec_idx]   ][u][s] = cluster_power[self_idx][adj_sec_idx][ max_idx     ][u][s] *  6./20. ;
  cluster_power[self_idx][adj_sec_idx][ num_path[self_idx][adj_sec_idx]+1 ][u][s] = cluster_power[self_idx][adj_sec_idx][ max_idx     ][u][s] *  4./20. ;
  cluster_power[self_idx][adj_sec_idx][ num_path[self_idx][adj_sec_idx]+2 ][u][s] = cluster_power[self_idx][adj_sec_idx][ max_idx_2nd ][u][s] *  6./20. ;
  cluster_power[self_idx][adj_sec_idx][ num_path[self_idx][adj_sec_idx]+3 ][u][s] = cluster_power[self_idx][adj_sec_idx][ max_idx_2nd ][u][s] *  4./20. ;

  arrival_angle[self_idx][adj_sec_idx][ num_path[self_idx][adj_sec_idx]   ][u][s] = arrival_angle[self_idx][adj_sec_idx][ max_idx     ][u][s] ;
  arrival_angle[self_idx][adj_sec_idx][ num_path[self_idx][adj_sec_idx]+1 ][u][s] = arrival_angle[self_idx][adj_sec_idx][ max_idx     ][u][s] ;
  arrival_angle[self_idx][adj_sec_idx][ num_path[self_idx][adj_sec_idx]+2 ][u][s] = arrival_angle[self_idx][adj_sec_idx][ max_idx_2nd ][u][s] ;
  arrival_angle[self_idx][adj_sec_idx][ num_path[self_idx][adj_sec_idx]+3 ][u][s] = arrival_angle[self_idx][adj_sec_idx][ max_idx_2nd ][u][s] ;

  depart_angle[self_idx][adj_sec_idx][ num_path[self_idx][adj_sec_idx]   ][u][s] = depart_angle[self_idx][adj_sec_idx][ max_idx     ][u][s] ;
  depart_angle[self_idx][adj_sec_idx][ num_path[self_idx][adj_sec_idx]+1 ][u][s] = depart_angle[self_idx][adj_sec_idx][ max_idx     ][u][s] ;
  depart_angle[self_idx][adj_sec_idx][ num_path[self_idx][adj_sec_idx]+2 ][u][s] = depart_angle[self_idx][adj_sec_idx][ max_idx_2nd ][u][s] ;
  depart_angle[self_idx][adj_sec_idx][ num_path[self_idx][adj_sec_idx]+3 ][u][s] = depart_angle[self_idx][adj_sec_idx][ max_idx_2nd ][u][s] ;

  for(int cluster_idx = 0; cluster_idx < num_path[self_idx][adj_sec_idx]+4; cluster_idx++)
  {
    if( cluster_idx == max_idx || cluster_idx == max_idx_2nd )
    {
      num_rays[self_idx][adj_sec_idx][cluster_idx][u][s] = 10 ;
      offset_angle[self_idx][adj_sec_idx][cluster_idx][0][u][s] =  0.0447 ;
      offset_angle[self_idx][adj_sec_idx][cluster_idx][1][u][s] = -0.0447 ;
      offset_angle[self_idx][adj_sec_idx][cluster_idx][2][u][s] =  0.1413 ;
      offset_angle[self_idx][adj_sec_idx][cluster_idx][3][u][s] = -0.1413 ;
      offset_angle[self_idx][adj_sec_idx][cluster_idx][4][u][s] =  0.2492 ;
      offset_angle[self_idx][adj_sec_idx][cluster_idx][5][u][s] = -0.2492 ;
      offset_angle[self_idx][adj_sec_idx][cluster_idx][6][u][s] =  0.3715 ;
      offset_angle[self_idx][adj_sec_idx][cluster_idx][7][u][s] = -0.3715 ;
      offset_angle[self_idx][adj_sec_idx][cluster_idx][8][u][s] =  2.1551 ;
      offset_angle[self_idx][adj_sec_idx][cluster_idx][9][u][s] = -2.1551 ;
    }
    else if( cluster_idx == num_path[self_idx][adj_sec_idx] || cluster_idx == num_path[self_idx][adj_sec_idx]+2 )
    {
      num_rays[self_idx][adj_sec_idx][cluster_idx][u][s] = 6 ;  
      offset_angle[self_idx][adj_sec_idx][cluster_idx][0][u][s]  =  0.5129 ;
      offset_angle[self_idx][adj_sec_idx][cluster_idx][1][u][s]  = -0.5129 ;
      offset_angle[self_idx][adj_sec_idx][cluster_idx][2][u][s] =  0.6797 ;
      offset_angle[self_idx][adj_sec_idx][cluster_idx][3][u][s] = -0.6797 ;
      offset_angle[self_idx][adj_sec_idx][cluster_idx][4][u][s] =  1.5195 ;
      offset_angle[self_idx][adj_sec_idx][cluster_idx][5][u][s] = -1.5195 ;
    }
    else if( cluster_idx == num_path[self_idx][adj_sec_idx]+1 || cluster_idx == num_path[self_idx][adj_sec_idx]+3 )
    {
      num_rays[self_idx][adj_sec_idx][cluster_idx][u][s] = 4 ;
      offset_angle[self_idx][adj_sec_idx][cluster_idx][0][u][s] =  0.8844 ;
      offset_angle[self_idx][adj_sec_idx][cluster_idx][1][u][s] = -0.8844 ;
      offset_angle[self_idx][adj_sec_idx][cluster_idx][2][u][s] =  1.1481 ;
      offset_angle[self_idx][adj_sec_idx][cluster_idx][3][u][s] = -1.1481 ;
    }
    else
    {
      num_rays[self_idx][adj_sec_idx][cluster_idx][u][s] = 20 ;
      offset_angle[self_idx][adj_sec_idx][cluster_idx][0][u][s]  =  0.0447 ;
      offset_angle[self_idx][adj_sec_idx][cluster_idx][1][u][s]  = -0.0447 ;
      offset_angle[self_idx][adj_sec_idx][cluster_idx][2][u][s]  =  0.1413 ;
      offset_angle[self_idx][adj_sec_idx][cluster_idx][3][u][s]  = -0.1413 ;
      offset_angle[self_idx][adj_sec_idx][cluster_idx][4][u][s]  =  0.2492 ;
      offset_angle[self_idx][adj_sec_idx][cluster_idx][5][u][s]  = -0.2492 ;
      offset_angle[self_idx][adj_sec_idx][cluster_idx][6][u][s]  =  0.3715 ;
      offset_angle[self_idx][adj_sec_idx][cluster_idx][7][u][s]  = -0.3715 ;
      offset_angle[self_idx][adj_sec_idx][cluster_idx][8][u][s]  =  0.5129 ;
      offset_angle[self_idx][adj_sec_idx][cluster_idx][9][u][s]  = -0.5129 ;
      offset_angle[self_idx][adj_sec_idx][cluster_idx][10][u][s] =  0.6797 ;
      offset_angle[self_idx][adj_sec_idx][cluster_idx][11][u][s] = -0.6797 ;
      offset_angle[self_idx][adj_sec_idx][cluster_idx][12][u][s] =  0.8844 ;
      offset_angle[self_idx][adj_sec_idx][cluster_idx][13][u][s] = -0.8844 ;
      offset_angle[self_idx][adj_sec_idx][cluster_idx][14][u][s] =  1.1481 ;
      offset_angle[self_idx][adj_sec_idx][cluster_idx][15][u][s] = -1.1481 ;
      offset_angle[self_idx][adj_sec_idx][cluster_idx][16][u][s] =  1.5195 ;
      offset_angle[self_idx][adj_sec_idx][cluster_idx][17][u][s] = -1.5195 ;
      offset_angle[self_idx][adj_sec_idx][cluster_idx][18][u][s] =  2.1551 ;
      offset_angle[self_idx][adj_sec_idx][cluster_idx][19][u][s] = -2.1551 ;
    }
  }
  
  num_path[self_idx][adj_sec_idx] = num_path[self_idx][adj_sec_idx]+4;
  Sort_clusters(self_idx, u, s, adj_sec_idx) ;
}


/*===================================================================
FUNCTION: Find_Max_idx()

AUTHOR: Kim, Kyung-Won

DESCRIPTION: Find the two strongest clusters

NOTES: 
   
===================================================================*/
void Find_Max_idx(int self_idx, int u, int s, int adj_sec_idx)
{
  if( cluster_power[self_idx][adj_sec_idx][0][u][s] > cluster_power[self_idx][adj_sec_idx][1][u][s] )
  {
    max_idx = 0 ;
    max_idx_2nd = 1 ;
  }
  else
  {
    max_idx = 1 ;
    max_idx_2nd = 0 ;
  }
  
  for (int cluster_idx = 2; cluster_idx < num_path[self_idx][adj_sec_idx]; cluster_idx++)
  {
    if( cluster_power[self_idx][adj_sec_idx][cluster_idx][u][s] > cluster_power[self_idx][adj_sec_idx][max_idx_2nd][u][s] )
    {
      max_idx_2nd = cluster_idx ;
      if( cluster_power[self_idx][adj_sec_idx][cluster_idx][u][s] > cluster_power[self_idx][adj_sec_idx][max_idx][u][s] )
      {
        int temp ;
        temp = max_idx ;
        max_idx = cluster_idx ;
        max_idx_2nd = temp ;
      }
    }
  }
}


/*===================================================================
FUNCTION: Sort_clusters()

AUTHOR: Kim, Kyung-Won

DESCRIPTION: Sort clusters

NOTES: 
   
===================================================================*/
void Sort_clusters(int self_idx, int u, int s, int adj_sec_idx)
{
  for( int i = 1; i < num_path[self_idx][adj_sec_idx]; i++ )
  {
    double temp1 ;
    double temp2 ;
    double temp3 ;
    double temp4 ;
    int    temp5 ;
    double temp_ray[MAX_NUM_RAYS] ;
    int j ;

    temp1 = delay[self_idx][adj_sec_idx][i][u][s] ;
    temp2 = cluster_power[self_idx][adj_sec_idx][i][u][s] ;
    temp3 = arrival_angle[self_idx][adj_sec_idx][i][u][s] ;
    temp4 = depart_angle[self_idx][adj_sec_idx][i][u][s] ;
    temp5 = num_rays[self_idx][adj_sec_idx][i][u][s] ;
    for( int ray_idx = 0; ray_idx < temp5; ray_idx++ )
    {
      temp_ray[ray_idx] = offset_angle[self_idx][adj_sec_idx][i][ray_idx][u][s] ;
    }
    for( j = i; j > 0; j-- )
    {
      if( delay[self_idx][adj_sec_idx][j-1][u][s] > temp1 )
      {
        delay[self_idx][adj_sec_idx][j][u][s]         = delay[self_idx][adj_sec_idx][j-1][u][s] ;
        cluster_power[self_idx][adj_sec_idx][j][u][s] = cluster_power[self_idx][adj_sec_idx][j-1][u][s] ;
        arrival_angle[self_idx][adj_sec_idx][j][u][s] = arrival_angle[self_idx][adj_sec_idx][j-1][u][s] ;
        depart_angle[self_idx][adj_sec_idx][j][u][s]  = depart_angle[self_idx][adj_sec_idx][j-1][u][s] ;
        num_rays[self_idx][adj_sec_idx][j][u][s]      = num_rays[self_idx][adj_sec_idx][j-1][u][s] ;
        for( int ray_idx = 0; ray_idx < num_rays[self_idx][adj_sec_idx][j-1][u][s]; ray_idx++ )
        {
          offset_angle[self_idx][adj_sec_idx][j][ray_idx][u][s] = offset_angle[self_idx][adj_sec_idx][j-1][ray_idx][u][s] ;
        }
      }
      else break ;
    }

    delay[self_idx][adj_sec_idx][j][u][s]         = temp1 ;
    cluster_power[self_idx][adj_sec_idx][j][u][s] = temp2 ;
    arrival_angle[self_idx][adj_sec_idx][j][u][s] = temp3 ;
    depart_angle[self_idx][adj_sec_idx][j][u][s]  = temp4 ;
    num_rays[self_idx][adj_sec_idx][j][u][s]      = temp5 ;
    for( int ray_idx = 0; ray_idx < temp5; ray_idx++ )
    {
      offset_angle[self_idx][adj_sec_idx][j][ray_idx][u][s] = temp_ray[ray_idx] ;
    }
  }
}

/*===================================================================
FUNCTION: Generate_XPR()

AUTHOR: Kim, Kyung-Won

DESCRIPTION: Generate the Cross Polarisation Power Ratio

NOTES: 
   
===================================================================*/
void Generate_XPR(int self_idx, int u, int s, int adj_sec_idx)
{
  for(int cluster_idx = 0; cluster_idx < num_path[self_idx][adj_sec_idx]; cluster_idx++)
  {
    for(int ray_idx = 0; ray_idx < num_rays[self_idx][adj_sec_idx][cluster_idx][u][s]; ray_idx++)
    {
      if (scenario == INDOOR_HOTSPOT)
      {
        if ( los[self_idx][my_sector] == true )
        {
          XPR[self_idx][adj_sec_idx][cluster_idx][ray_idx][u][s] = 11. + 0. * Rand.n() ;
        }
        else
        {
          XPR[self_idx][adj_sec_idx][cluster_idx][ray_idx][u][s] = 10. + 0. * Rand.n() ;
        }
      }
      else if (scenario == URBAN_MACRO)
      {
        if ( los[self_idx][my_sector] == true )
        {
          XPR[self_idx][adj_sec_idx][cluster_idx][ray_idx][u][s] = 8. + 0. * Rand.n() ;
        }
        else
        {
          XPR[self_idx][adj_sec_idx][cluster_idx][ray_idx][u][s] = 7. + 0. * Rand.n() ;
        }
      }
      else if (scenario == URBAN_MICRO && indoor[self_idx][my_sector] == false)
      {
        if ( los[self_idx][my_sector] == true )
        {
          XPR[self_idx][adj_sec_idx][cluster_idx][ray_idx][u][s] = 9. + 0. * Rand.n() ;
        }
        else
        {
          XPR[self_idx][adj_sec_idx][cluster_idx][ray_idx][u][s] = 8. + 0. * Rand.n() ;
        }
      }
      else if (scenario == RURAL_MACRO)
      {
        if ( los[self_idx][my_sector] == true )
        {
          XPR[self_idx][adj_sec_idx][cluster_idx][ray_idx][u][s] = 12. + 0. * Rand.n() ;
        }
        else
        {
          XPR[self_idx][adj_sec_idx][cluster_idx][ray_idx][u][s] = 7. + 0. * Rand.n() ;
        }
      }
      else if (scenario == SUBURBAN_MACRO)
      {
        if ( los[self_idx][my_sector] == true )
        {
          XPR[self_idx][adj_sec_idx][cluster_idx][ray_idx][u][s] = 8. + 0. * Rand.n() ;
        }
        else
        {
          XPR[self_idx][adj_sec_idx][cluster_idx][ray_idx][u][s] = 4. + 0. * Rand.n() ;
        }
      }
      else if (scenario == OUTDOOR_2_INDOOR || indoor[self_idx][my_sector] == true)
      {
        XPR[self_idx][adj_sec_idx][cluster_idx][ray_idx][u][s] = 9. + 0. * Rand.n() ;
      }
      else                         
      { 
        cerr << "Unspeficied scenario" << endl ; 
      }
      XPR[self_idx][adj_sec_idx][cluster_idx][ray_idx][u][s] = pow( 10. , XPR[self_idx][adj_sec_idx][cluster_idx][ray_idx][u][s]/10. ) ;
    }
  }  
}
        // These parameters come from Table A1-7

/*===================================================================
FUNCTION: SetInitialPhase()

AUTHOR: Kim, Kyung-Won

DESCRIPTION: Set initial phases.

NOTES: 
   
===================================================================*/
void SetInitialPhase(int self_idx, int u, int s, int adj_sec_idx)
{
  for(int cluster_idx = 0; cluster_idx < num_path[self_idx][adj_sec_idx]; cluster_idx++)
  {
    for(int ray_idx = 0; ray_idx < num_rays[self_idx][adj_sec_idx][cluster_idx][u][s]; ray_idx++)
    {
      phi_vv[self_idx][adj_sec_idx][cluster_idx][ray_idx][u][s] = 360. * Rand.u() - 180. ;
      phi_vh[self_idx][adj_sec_idx][cluster_idx][ray_idx][u][s] = 360. * Rand.u() - 180. ;
      phi_hv[self_idx][adj_sec_idx][cluster_idx][ray_idx][u][s] = 360. * Rand.u() - 180. ;
      phi_hh[self_idx][adj_sec_idx][cluster_idx][ray_idx][u][s] = 360. * Rand.u() - 180. ;
    }
  }
  phi_vv_LOS[self_idx][adj_sec_idx][u][s] = 360. * Rand.u() - 180. ;
  phi_hh_LOS[self_idx][adj_sec_idx][u][s] = 360. * Rand.u() - 180. ;
}

/*===================================================================
FUNCTION: ChannelSampleGeneration()

AUTHOR: Kim, Kyung-Won

DESCRIPTION: compute channel coefficients and stored

NOTES: Output is a Channel_Coef.
   
===================================================================*/
void ChannelSampleGeneration( int self_idx, int u, int s, int adj_sec_idx )
{
  if( t == 0 )
  { 
    Lambda = SPEED_LIGHT / carrier_freq ;
    
    if (num_received_antenna == 1)
    { d_tx = 0 ; }
    else if (num_received_antenna > 0)
    { d_tx = 10 * Lambda ; }
    else
    { cerr << "num_received_antenna is wrong" << endl ; } 
    if (num_transmit_antenna == 1)
    { d_rx = 0 ; }
    else if (num_transmit_antenna > 0)
    { d_rx = 0.5 * Lambda ; }
    else
    { cerr << "num_transmit_antenna is wrong" << endl ; } 
  }
  my_sector = ue[self_idx].adj_sector[adj_sec_idx] ;
  
  for(int cluster_idx = 0; cluster_idx < num_path[self_idx][adj_sec_idx]; cluster_idx++)
  {
    Compute_Channel_Coef(self_idx, cluster_idx, u, s, adj_sec_idx) ;
  }
}


/*===================================================================
FUNCTION: Compute_Channel_Coef()

AUTHOR: Kim, Kyung-Won

DESCRIPTION: Compute channel coefficients.

NOTES: Output is a Channel_Coef.
   
===================================================================*/
void Compute_Channel_Coef(int self_idx, int cluster_idx, int u, int s, int adj_sec_idx)
{
  Channel_coef[ self_idx ][ adj_sec_idx ][ cluster_idx ][ u ][ s ] = ( 0. , 0. ) ;
  for(int ray_idx = 0; ray_idx < num_rays[self_idx][adj_sec_idx][cluster_idx][u][s]; ray_idx++)
  {
    double arr_angle = arrival_angle[self_idx][adj_sec_idx][cluster_idx][u][s] + cluster_ASA * offset_angle[self_idx][adj_sec_idx][cluster_idx][ray_idx][u][s] ;
    double dep_angle = depart_angle[self_idx][adj_sec_idx][cluster_idx][u][s] + cluster_ASD * offset_angle[self_idx][adj_sec_idx][cluster_idx][ray_idx][u][s] ;
    double height = h_BS - h_MS ;
    double angle_v = atan2(height,ue2sector_angle[self_idx][my_sector]) * 180. / PI ;
    double F_tx_h = 12. * pow((dep_angle/ANGLE_3dB_h),2) ;
    double F_rx_h = 0. ;
    double F_tx_v = 12. * pow(((angle_v-ANGLE_tilt)/ANGLE_3dB_v),2) ;
    double F_rx_v = 0. ;
    double F_vv = pow( 10. , -MIN( F_rx_v+F_tx_v, 20. ) / 10. ) ;
    double F_vh = pow( 10. , -MIN( F_rx_v+F_tx_h, 20. ) / 10. ) ;
    double F_hv = pow( 10. , -MIN( F_rx_h+F_tx_v, 20. ) / 10. ) ;
    double F_hh = pow( 10. , -MIN( F_rx_h+F_tx_h, 20. ) / 10. ) ;
    double v = speed_UE * cos( ( arr_angle - travel_angle[self_idx] ) * PI / 180. ) / Lambda ;
             // This equation comes from 1.3.2.1 (1.19)

    complex <double> exp_vv ( 0. , phi_vv[self_idx][adj_sec_idx][cluster_idx][ray_idx][u][s] * PI / 180 ) ;
    complex <double> exp_vh ( 0. , phi_vh[self_idx][adj_sec_idx][cluster_idx][ray_idx][u][s] * PI / 180 ) ;
    complex <double> exp_hv ( 0. , phi_hv[self_idx][adj_sec_idx][cluster_idx][ray_idx][u][s] * PI / 180 ) ;
    complex <double> exp_hh ( 0. , phi_hh[self_idx][adj_sec_idx][cluster_idx][ray_idx][u][s] * PI / 180 ) ;
    complex <double> exp_dtx ( 0. , d_tx*2*PI/Lambda*sin( dep_angle * PI / 180. ) ) ;
    complex <double> exp_drx ( 0. , d_rx*2*PI/Lambda*sin( arr_angle * PI / 180. ) ) ;
    complex <double> j2pivt ( 0. , 2.*PI*v*t/1000. ) ;

    Channel_coef[ self_idx ][ adj_sec_idx ][ cluster_idx ][ u ][ s ] = Channel_coef[ self_idx ][ adj_sec_idx ][ cluster_idx ][ u ][ s ] 
                                                                       + sqrt( cluster_power[self_idx][adj_sec_idx][cluster_idx][u][s] / num_rays[self_idx][adj_sec_idx][cluster_idx][u][s] )
                                                                         * (                                                                        F_vv * exp( exp_vv ) + F_hh * exp( exp_hh )
                                                                             + sqrt( 1/XPR[self_idx][adj_sec_idx][cluster_idx][ray_idx][u][s] ) * ( F_vh * exp( exp_vh ) + F_hv * exp( exp_hv ) )
                                                                           )
                                                                         * exp( exp_dtx + exp_drx + j2pivt ) ;
             // These equations come from 1.3.2.1 (1.18)
  }

  if ( los[self_idx][my_sector] == true && cluster_idx == 0 )
  {
    double height = h_BS - h_MS ;
    double angle_v = atan2(height,ue2sector_angle[self_idx][my_sector]) * 180. / PI ;
    double F_tx_h = 12. * pow((ue2sector_angle[self_idx][my_sector]/ANGLE_3dB_h),2) ;
    double F_rx_h = 0. ;
    double F_tx_v = 12. * pow(((angle_v-ANGLE_tilt)/ANGLE_3dB_v),2) ;
    double F_rx_v = 0. ;
    double F_vv = pow( 10. , -MIN( F_rx_v+F_tx_v, 20. ) / 10. ) ;
    double F_hh = pow( 10. , -MIN( F_rx_h+F_tx_h, 20. ) / 10. ) ;
    double v = speed_UE * cos( ( ue2sector_angle[self_idx][my_sector] - travel_angle[self_idx] ) * PI / 180. ) / Lambda ;
    double k_factor = pow( 10. , Ricean_K[ self_idx ][ my_sector ] / 10. ) ;

    complex <double> exp_vv_LOS ( 0. , phi_vv_LOS[self_idx][adj_sec_idx][u][s] * PI / 180. ) ;
    complex <double> exp_hh_LOS ( 0. , phi_hh_LOS[self_idx][adj_sec_idx][u][s] * PI / 180. ) ;
    complex <double> exp_dtx ( 0. , d_tx*2*PI/Lambda*sin( ue2sector_angle[self_idx][my_sector] * PI / 180. ) ) ;
    complex <double> exp_drx ( 0. , d_rx*2*PI/Lambda*sin( ue2sector_angle[self_idx][my_sector] * PI / 180. ) ) ;
    complex <double> j2pivt ( 0. , 2.*PI*v*t/1000. ) ;

    Channel_coef[ self_idx ][ adj_sec_idx ][ cluster_idx ][ u ][ s ] = sqrt( 1. / (k_factor + 1.) ) * Channel_coef[ self_idx ][ adj_sec_idx ][ cluster_idx ][ u ][ s ]
                                                                       + sqrt( k_factor / (k_factor + 1.) )
                                                                         * ( F_vv * exp( exp_vv_LOS ) + F_hh * exp( exp_hh_LOS ) )
                                                                         * exp( exp_dtx + exp_drx + j2pivt ) ;
  }         // These equations come from 1.3.2.1 (1.21)
}    

/*===================================================================
FUNCTION: Size_Channel_Parameters()

AUTHOR: Kim, Kyung-Won

DESCRIPTION: Determine size of parameters

NOTES: 
   
===================================================================*/
void Size_Channel_Parameters()
{
  // Large Scale Parameters
  ue2sector_distance = new double * [num_ues] ;
  ue2sector_angle = new double * [num_ues] ;
  los = new bool * [num_ues] ;
  indoor = new bool * [num_ues] ;
  Ricean_K = new double * [num_ues] ;
  for( int ue_idx = 0; ue_idx < num_ues; ue_idx++ )
  {
    ue2sector_distance[ ue_idx ] = new double [NUM_SECTORS] ;
    ue2sector_angle[ ue_idx ] = new double [NUM_SECTORS] ;
    los[ ue_idx ] = new bool [NUM_SECTORS] ;
    indoor[ ue_idx ] = new bool [NUM_SECTORS] ;
    Ricean_K[ ue_idx ] = new double [NUM_SECTORS] ;
  }

  travel_angle = new double [num_ues] ;
  antenna = new double [NUM_SECTORS] ;
  path_loss = new double [NUM_SECTORS] ;

  // Small Scale Parameters
  num_path = new int * [ num_ues ] ;
  for( int ue_idx = 0; ue_idx < num_ues; ue_idx++ )
  {
    num_path[ ue_idx ] = new int [ num_compute_coef ] ;
  }

  Channel_coef = new complex <double> **** [ num_ues ] ;
  delay = new double **** [ num_ues ] ;
  cluster_power = new double **** [ num_ues ] ;
  arrival_angle = new double **** [ num_ues ] ;
  depart_angle = new double **** [ num_ues ] ;
  num_rays = new int **** [ num_ues ] ;
  for( int ue_idx = 0; ue_idx < num_ues; ue_idx++ )
  {
    Channel_coef[ ue_idx ] = new complex <double> *** [num_compute_coef];
    delay[ ue_idx ] = new double *** [num_compute_coef];
    cluster_power[ ue_idx ] = new double *** [num_compute_coef];
    arrival_angle[ ue_idx ] = new double *** [num_compute_coef];
    depart_angle[ ue_idx ] = new double *** [num_compute_coef];
    num_rays[ ue_idx ] = new int *** [num_compute_coef];
    for(int adj_sec_idx = 0; adj_sec_idx < num_compute_coef; adj_sec_idx++)
    {
      Channel_coef[ ue_idx ][ adj_sec_idx ] = new complex <double> ** [MAX_NUM_CLUSTERS];
      delay[ ue_idx ][ adj_sec_idx ] = new double ** [MAX_NUM_CLUSTERS];
      cluster_power[ ue_idx ][ adj_sec_idx ] = new double ** [MAX_NUM_CLUSTERS];
      arrival_angle[ ue_idx ][ adj_sec_idx ] = new double ** [MAX_NUM_CLUSTERS];
      depart_angle[ ue_idx ][ adj_sec_idx ] = new double ** [MAX_NUM_CLUSTERS];
      num_rays[ ue_idx ][ adj_sec_idx ] = new int ** [MAX_NUM_CLUSTERS];
      for( int path_idx = 0; path_idx < MAX_NUM_CLUSTERS; path_idx++ )
      {
        Channel_coef[ ue_idx ][ adj_sec_idx ][ path_idx ] = new complex <double> * [ num_received_antenna ] ;
        delay[ ue_idx ][ adj_sec_idx ][ path_idx ] = new double * [ num_received_antenna ] ;
        cluster_power[ ue_idx ][ adj_sec_idx ][ path_idx ] = new double * [ num_received_antenna ] ;
        arrival_angle[ ue_idx ][ adj_sec_idx ][ path_idx ] = new double * [ num_received_antenna ] ;
        depart_angle[ ue_idx ][ adj_sec_idx ][ path_idx ] = new double * [ num_received_antenna ] ;
        num_rays[ ue_idx ][ adj_sec_idx ][ path_idx ] = new int * [ num_received_antenna ] ;
        for( int u = 0; u < num_received_antenna; u++ )
        {
          Channel_coef[ ue_idx ][ adj_sec_idx ][ path_idx ][ u ] = new complex <double> [ num_transmit_antenna ] ;
          delay[ ue_idx ][ adj_sec_idx ][ path_idx ][ u ] = new double [ num_transmit_antenna ] ;
          cluster_power[ ue_idx ][ adj_sec_idx ][ path_idx ][ u ] = new double [ num_transmit_antenna ] ;
          arrival_angle[ ue_idx ][ adj_sec_idx ][ path_idx ][ u ] = new double [ num_transmit_antenna ] ;
          depart_angle[ ue_idx ][ adj_sec_idx ][ path_idx ][ u ] = new double [ num_transmit_antenna ] ;
          num_rays[ ue_idx ][ adj_sec_idx ][ path_idx ][ u ] = new int [ num_transmit_antenna ] ;
        }
      }
    }
  }
  
  XPR = new double *****[ num_ues ] ;
  offset_angle = new double ***** [ num_ues ] ;
  phi_vv = new double ***** [ num_ues ] ;
  phi_vh = new double ***** [ num_ues ] ;
  phi_hv = new double ***** [ num_ues ] ;
  phi_hh = new double ***** [ num_ues ] ;
  for( int ue_idx = 0; ue_idx < num_ues; ue_idx++ )
  {
    XPR[ ue_idx ] = new double **** [num_compute_coef] ;
    offset_angle[ ue_idx ] = new double **** [num_compute_coef] ;
    phi_vv[ ue_idx ] = new double **** [num_compute_coef] ;
    phi_vh[ ue_idx ] = new double **** [num_compute_coef] ;
    phi_hv[ ue_idx ] = new double **** [num_compute_coef] ;
    phi_hh[ ue_idx ] = new double **** [num_compute_coef] ;
    for(int adj_sec_idx = 0; adj_sec_idx < num_compute_coef; adj_sec_idx++)
    {
      XPR[ ue_idx ][ adj_sec_idx ] = new double *** [MAX_NUM_CLUSTERS] ;
      offset_angle[ ue_idx ][ adj_sec_idx ] = new double *** [MAX_NUM_CLUSTERS] ;
      phi_vv[ ue_idx ][ adj_sec_idx ] = new double *** [MAX_NUM_CLUSTERS] ;
      phi_vh[ ue_idx ][ adj_sec_idx ] = new double *** [MAX_NUM_CLUSTERS] ;
      phi_hv[ ue_idx ][ adj_sec_idx ] = new double *** [MAX_NUM_CLUSTERS] ;
      phi_hh[ ue_idx ][ adj_sec_idx ] = new double *** [MAX_NUM_CLUSTERS] ;
      for( int path_idx = 0; path_idx < MAX_NUM_CLUSTERS; path_idx++ )
      {
        XPR[ ue_idx][ adj_sec_idx ][ path_idx ] = new double ** [ MAX_NUM_RAYS ] ;
        offset_angle[ ue_idx ][ adj_sec_idx ][ path_idx ] = new double ** [ MAX_NUM_RAYS ] ;
        phi_vv[ ue_idx ][ adj_sec_idx ][ path_idx ] = new double ** [ MAX_NUM_RAYS ] ;
        phi_vh[ ue_idx ][ adj_sec_idx ][ path_idx ] = new double ** [ MAX_NUM_RAYS ] ;
        phi_hv[ ue_idx ][ adj_sec_idx ][ path_idx ] = new double ** [ MAX_NUM_RAYS ] ;
        phi_hh[ ue_idx ][ adj_sec_idx ][ path_idx ] = new double ** [ MAX_NUM_RAYS ] ;
        for( int ray_idx = 0; ray_idx < MAX_NUM_RAYS; ray_idx++ )
        {
          XPR[ ue_idx ][ adj_sec_idx ][ path_idx ][ ray_idx ] = new double * [ num_received_antenna ] ;
          offset_angle[ ue_idx ][ adj_sec_idx ][ path_idx ][ ray_idx ] = new double * [ num_received_antenna ] ;
          phi_vv[ ue_idx ][ adj_sec_idx ][ path_idx ][ ray_idx ] = new double * [ num_received_antenna ] ;
          phi_vh[ ue_idx ][ adj_sec_idx ][ path_idx ][ ray_idx ] = new double * [ num_received_antenna ] ;
          phi_hv[ ue_idx ][ adj_sec_idx ][ path_idx ][ ray_idx ] = new double * [ num_received_antenna ] ;
          phi_hh[ ue_idx ][ adj_sec_idx ][ path_idx ][ ray_idx ] = new double * [ num_received_antenna ] ;
          for( int u = 0; u < num_received_antenna; u++ )
          {
            XPR[ ue_idx ][ adj_sec_idx ][ path_idx ][ ray_idx ][ u ] = new double [ num_transmit_antenna ] ;
            offset_angle[ ue_idx ][ adj_sec_idx ][ path_idx ][ ray_idx ][ u ] = new double [ num_transmit_antenna ] ;
            phi_vv[ ue_idx ][ adj_sec_idx ][ path_idx ][ ray_idx ][ u ] = new double [ num_transmit_antenna ] ;
            phi_vh[ ue_idx ][ adj_sec_idx ][ path_idx ][ ray_idx ][ u ] = new double [ num_transmit_antenna ] ;
            phi_hv[ ue_idx ][ adj_sec_idx ][ path_idx ][ ray_idx ][ u ] = new double [ num_transmit_antenna ] ;
            phi_hh[ ue_idx ][ adj_sec_idx ][ path_idx ][ ray_idx ][ u ] = new double [ num_transmit_antenna ] ;
          }
        }
      }
    }
  }
  phi_vv_LOS = new double *** [ num_ues ] ;
  phi_vh_LOS = new double *** [ num_ues ] ;
  phi_hv_LOS = new double *** [ num_ues ] ;
  phi_hh_LOS = new double *** [ num_ues ] ;
  for( int ue_idx = 0; ue_idx < num_ues; ue_idx++ )
  {
    phi_vv_LOS[ ue_idx ] = new double ** [ num_compute_coef ] ;
    phi_vh_LOS[ ue_idx ] = new double ** [ num_compute_coef ] ;
    phi_hv_LOS[ ue_idx ] = new double ** [ num_compute_coef ] ;
    phi_hh_LOS[ ue_idx ] = new double ** [ num_compute_coef ] ;
    for(int adj_sec_idx = 0; adj_sec_idx < num_compute_coef; adj_sec_idx++)
    {
      phi_vv_LOS[ ue_idx ][ adj_sec_idx ] = new double * [ num_received_antenna ] ;
      phi_vh_LOS[ ue_idx ][ adj_sec_idx ] = new double * [ num_received_antenna ] ;
      phi_hv_LOS[ ue_idx ][ adj_sec_idx ] = new double * [ num_received_antenna ] ;
      phi_hh_LOS[ ue_idx ][ adj_sec_idx ] = new double * [ num_received_antenna ] ;
      for( int u = 0; u < num_received_antenna; u++ )
      {
        phi_vv_LOS[ ue_idx ][ adj_sec_idx ][ u ] = new double [ num_transmit_antenna ] ;
        phi_vh_LOS[ ue_idx ][ adj_sec_idx ][ u ] = new double [ num_transmit_antenna ] ;
        phi_hv_LOS[ ue_idx ][ adj_sec_idx ][ u ] = new double [ num_transmit_antenna ] ;
        phi_hh_LOS[ ue_idx ][ adj_sec_idx ][ u ] = new double [ num_transmit_antenna ] ;
      }
    }
  }
  
  // correlation parameter
  random_number = new double ** [ num_ues ] ;
  for( int ue_idx = 0; ue_idx < num_ues; ue_idx++ )
  {
    random_number[ ue_idx ] = new double * [NUM_SECTORS] ;
    for( int sec_idx = 0; sec_idx < NUM_SECTORS; sec_idx++ )
    {
      random_number[ ue_idx ][ sec_idx ] = new double [5] ;
      for( int idx = 0; idx < 5; idx++ )
      {
        random_number[ ue_idx ][ sec_idx ][ idx ] = Rand.n() ;
      }
    }
  }
}
