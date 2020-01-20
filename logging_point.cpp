#include "common.h"
#include "const.h"

//void Logging_point( void ) ;
void Get_Ue_Distribution( void ) ;
void Get_PathLoss( void ) ;
void Get_DelaySpread( void ) ;
void Get_AngleSpread( void ) ;
void Get_ChannelCoeff( void ) ;
void Get_PDF( void ) ;
void PDF_path_loss( void ) ;
void PDF_block_SS_1( void ) ;
void PDF_block_SS_2( void ) ;
void PDF_block_SS_3( void ) ;
void PDF_block_SS_4( void ) ;
void PDF_block_SS_5( void ) ;
void PDF_block_SS_6( void ) ;
void PDF_block_SS_7( void ) ;
//void Size_Logging_point_Parameters( void ) ;


/*===================================================================
FUNCTION: Logging_point()

AUTHOR: Kim, Kyung-Won

DESCRIPTION: ITU Channel Model Simulator

NOTES: ...
   
===================================================================*/
void Logging_Point()
{
  if ( ue_distribution == 1 && t == 0 )
  {
    Get_Ue_Distribution();
  }
  if ( PathLoss == 1 && t == 0 )
  {
    Get_PathLoss() ;
  }
  if( DelaySpread == 1 && t == 0 )
  {
    Get_DelaySpread() ;
  }
  if( AngleSpread == 1 && t == 0 )
  {
    Get_AngleSpread() ;
  }
  if( ChannelCoef == 1 && sample_ue > 0 && sample_ue-1 < num_ues )
  {
    Get_ChannelCoeff() ;
  }
  if( PDF == 1 && t == run_times-1 )
  {
    Get_PDF() ;
  }
}


/*===================================================================
FUNCTION: Get_Ue_Distribution()

AUTHOR: Kim, Kyung-Won

DESCRIPTION: Initialize a Drop

NOTES: 
   
===================================================================*/
void Get_Ue_Distribution()
{
  ofstream  Cell_location;
  ofstream  UE_location;

  if( drop_idx == 0 )
  {
    Cell_location.open( "./Output/Cell_location.dat" ,ios::out); 
    UE_location.open( "./Output/UE_location.dat" ,ios::out);
  }
  else
  {
    Cell_location.open( "./Output/Cell_location.dat" ,ios::app); 
    UE_location.open( "./Output/UE_location.dat" ,ios::app);
  }
  Cell_location << "# UE Distribution of " << drop_idx + 1 << " drop " << endl;
  Cell_location << "# The number of ue is " << num_ues << endl << endl;
  UE_location << "# UE Distribution of " << drop_idx  + 1<< " drop " << endl;
  UE_location << "# The number of ue is " << num_ues << endl << endl;

  LOCATION * ue_location;
  ue_location =  new LOCATION[num_ues];
  
  if( scenario == INDOOR_HOTSPOT )
  {
    Cell_location << -30. << "\t" << 0 << endl;
    Cell_location <<  30. << "\t" << 0 << endl;
    Cell_location << endl << endl << endl;

    for (int ue_idx = 0 ; ue_idx < num_ues ; ue_idx++ )
    {
      ue_location[ue_idx].x = ue[ue_idx].coordinate.x;
      ue_location[ue_idx].y = ue[ue_idx].coordinate.y;
    }
  }
  else
  {
    double d;
    if ( scenario == 1 )
      d = 500. ;
    else if ( scenario == 2 )
      d = 200. ;
    else if ( scenario == 3 )
      d = 1732. ;
    else if ( scenario == 4 )
      d = 1299. ;

    LOCATION cell_location[19];

    cell_location[16].x = (3./2.)*d;
    cell_location[16].y = d/sqrt((double)3);

    cell_location[17].x = (5./2.)*d;
    cell_location[17].y = d/sqrt((double)3);

    cell_location[18].x = (7./2.)*d;
    cell_location[18].y = d/sqrt((double)3);

    cell_location[15].x = d;
    cell_location[15].y = d/sqrt((double)3) + (sqrt((double)3)/2.)*d;

    cell_location[ 5].x = 2*d;
    cell_location[ 5].y = d/sqrt((double)3) + (sqrt((double)3)/2.)*d;

    cell_location[ 6].x = 3*d;
    cell_location[ 6].y = d/sqrt((double)3) + (sqrt((double)3)/2.)*d;

    cell_location[ 7].x = 4*d;
    cell_location[ 7].y = d/sqrt((double)3) + (sqrt((double)3)/2.)*d;

    cell_location[14].x = d/2.;
    cell_location[14].y = d/sqrt((double)3) + sqrt((double)3)*d;

    cell_location[ 4].x = (3./2.)*d;
    cell_location[ 4].y = d/sqrt((double)3) + sqrt((double)3)*d;

    cell_location[ 0].x = (5./2.)*d;
    cell_location[ 0].y = d/sqrt((double)3) + sqrt((double)3)*d;

    cell_location[ 1].x = (7./2.)*d;
    cell_location[ 1].y = d/sqrt((double)3) + sqrt((double)3)*d;

    cell_location[ 8].x = (9./2.)*d;
    cell_location[ 8].y = d/sqrt((double)3) + sqrt((double)3)*d;
 
    cell_location[13].x = d;
    cell_location[13].y = d/sqrt((double)3) + (3*sqrt((double)3)/2.)*d;

    cell_location[ 3].x = 2*d;
    cell_location[ 3].y = d/sqrt((double)3) + (3*sqrt((double)3)/2.)*d;

    cell_location[ 2].x = 3*d;
    cell_location[ 2].y = d/sqrt((double)3) + (3*sqrt((double)3)/2.)*d;

    cell_location[ 9].x = 4*d;
    cell_location[ 9].y = d/sqrt((double)3) + (3*sqrt((double)3)/2.)*d;

    cell_location[12].x = (3./2.)*d;
    cell_location[12].y = d/sqrt((double)3) + (4*sqrt((double)3)/2.)*d;

    cell_location[11].x = (5./2.)*d;
    cell_location[11].y = d/sqrt((double)3) + (4*sqrt((double)3)/2.)*d;

    cell_location[10].x = (7./2.)*d;
    cell_location[10].y = d/sqrt((double)3) + (4*sqrt((double)3)/2.)*d;

    for (int cell_idx = 0 ; cell_idx < 19  ; cell_idx++ )
    {
      Cell_location << cell_location[cell_idx].x << "\t" << cell_location[cell_idx].y << endl;
    }
    Cell_location << endl << endl << endl;

    for (int ue_idx = 0 ; ue_idx < num_ues ; ue_idx++ )
    {
      int cell_num = (int)(ue[ue_idx].sector_in_control/3);
      ue_location[ue_idx].x = cell_location[cell_num].x + ue[ue_idx].coordinate.x;
      ue_location[ue_idx].y = cell_location[cell_num].y + ue[ue_idx].coordinate.y;
    }
  }

  for (int ue_idx = 0 ; ue_idx < num_ues ; ue_idx++ )
  {
    UE_location << ue_location[ue_idx].x << "\t" << ue_location[ue_idx].y << "\t" << ue[ue_idx].static_gain[ue[ue_idx].sector_in_control] << endl;
  }
  UE_location << endl << endl << endl;

  if( drop_idx == num_drops )
  {
    Cell_location.close();
    UE_location.close();
  }
}


/*===================================================================
FUNCTION: Get_PathLoss()

AUTHOR: Kim, Kyung-Won

DESCRIPTION: ITU Channel Model Simulator

NOTES: ...
   
===================================================================*/
void Get_PathLoss( void )
{
  ofstream PL ;
  if( drop_idx == 0 )
  {
    PL.open("./Output/PathLoss.dat",ios::out) ;
  }
  else
  {
    PL.open("./Output/PathLoss.dat",ios::app) ;
  }  
  if( drop_idx == 0 )
  {
    PL << "# Output is ( distance[m], PathLoss[dB] )" << endl << endl << endl << endl ;
  }

  PL << "(drop = " << drop_idx << ")" << endl << endl ;
  for(int ue_idx = 0; ue_idx < num_ues / 3; ue_idx++)
  {
    for(int cell_idx = 0; cell_idx < NUM_SECTORS / 3; cell_idx++)
    {
      PL << ue[ ue_idx ].Distance[ 3 * cell_idx ] << "\t" ;
      PL << ue[ ue_idx ].PathLoss[ 3 * cell_idx ] << endl ;
    }
  }
  PL << endl << endl ;

  if( drop_idx == num_drops )
  {
    PL.close();
  }
}

/*===================================================================
FUNCTION: Get_DelaySpread( void )

AUTHOR: Kim, Kyung-Won

DESCRIPTION: ITU Channel Model Simulator

NOTES: ...
   
===================================================================*/
void Get_DelaySpread( void )
{
  ofstream DS ;
  if( drop_idx == 0 )
  {
    DS.open("./Output/DelaySpread.dat",ios::out) ;
  }
  else
  {
    DS.open("./Output/DelaySpread.dat",ios::app) ;
  } 
  DS << "# Output is ( time[second], power[dB] )" << endl
     << "# u is received antenna index." << endl
     << "# s is transmitt antenna index." << endl << endl << endl << endl;

  for( int ue_idx = 0; ue_idx < num_ues; ue_idx++ )
  {
    for( int u = 0; u < num_received_antenna; u++ )
    {
      for( int s = 0; s < num_transmit_antenna; s++ )
      {
        DS << "(ue = " << ue_idx << ", drop = " << drop_idx << ", u = " << u << ", s = " << s << ")" << endl << endl ;
        for(int cluster_idx = 0; cluster_idx < num_path[ue_idx][0]; cluster_idx++)
        {
          DS << delay[ ue_idx ][0][ cluster_idx ][ u ][ s ] << "\t" ;
          DS << cluster_power[ ue_idx ][0][ cluster_idx ][ u ][ s ] << endl ;
        }
        DS << endl << endl ;
      }
    }
  }

  if( drop_idx == num_drops )
  {
    DS.close();
  }
}


/*===================================================================
FUNCTION: Get_AngleSpread( void )

AUTHOR: Kim, Kyung-Won

DESCRIPTION: ITU Channel Model Simulator

NOTES: ...
   
===================================================================*/
void Get_AngleSpread( void )
{
  ofstream AoA ;
  ofstream AoD ;
  if( drop_idx == 0 )
  {
    AoA.open("./Output/AoASpread.dat",ios::out) ;
  }
  else
  {
    AoA.open("./Output/AoASpread.dat",ios::app) ;
  }
  if( drop_idx == 0 )
  {
    AoD.open("./Output/AoDSpread.dat",ios::out) ;
  }
  else
  {
    AoD.open("./Output/AoDSpread.dat",ios::app) ;
  }
  if( drop_idx == 0 )
  {
    AoA << "# Output is ( angle[dgree], power[dB] )" << endl
        << "# u is received antenna index." << endl
        << "# s is transmitt antenna index." << endl << endl << endl << endl;
    AoD << "# Output is ( angle[dgree], power[dB] )" << endl
        << "# u is received antenna index." << endl
        << "# s is transmitt antenna index." << endl << endl << endl << endl;
  }

  for( int ue_idx = 0; ue_idx < num_ues; ue_idx++ )
  {
    for( int u = 0; u < num_received_antenna; u++ )
    {
      for( int s = 0; s < num_transmit_antenna; s++ )
      {
        AoA << "(ue = " << ue_idx << ", drop = " << drop_idx << ", u = " << u << ", s = " << s << ")" << endl
            << "travel angle of is " << travel_angle[ue_idx] << endl<< endl ;
        AoD << "(ue = " << ue_idx << ", drop = " << drop_idx << ", u = " << u << ", s = " << s << ")" << endl << endl ;
        for(int cluster_idx = 0; cluster_idx < num_path[ue_idx][0]; cluster_idx++)
        {
          AoA << arrival_angle[ue_idx][0][cluster_idx][ u ][ s ] << "\t" ;
          AoA << cluster_power[ue_idx][0][cluster_idx][ u ][ s ] << endl ;
          AoD << depart_angle[ue_idx][0][cluster_idx][ u ][ s ] << "\t" ;
          AoD << cluster_power[ue_idx][0][cluster_idx][ u ][ s ] << endl ;
        }
        AoA << endl << endl ;
        AoD << endl << endl ;
      }
    }
  }

  if( drop_idx == num_drops )
  {
    AoA.close();
    AoD.close();
  }
}


/*===================================================================
FUNCTION: Get_ChannelCoeff( void )

AUTHOR: Kim, Kyung-Won

DESCRIPTION: ITU Channel Model Simulator

NOTES: ...
   
===================================================================*/
void Get_ChannelCoeff( void )
{
  for(int path_idx = 0; path_idx < num_path[ sample_ue-1 ][0]; path_idx++)
  {
    for(int u = 0; u < num_received_antenna; u++)
    {
      for(int s = 0; s < num_transmit_antenna; s++)
      {
        Coef[ path_idx ][ u ][ s ][ t ] = Channel_coef[ sample_ue-1 ][0][ path_idx ][ u ][ s ] ;
      }
    }
  }

  if( t == run_times-1 )
  {
    ofstream Channel ;
    if( drop_idx == 0 )
    {
      Channel.open("./Output/Channel_Coef.dat",ios::out) ;
    }
    else
    {
      Channel.open("./Output/Channel_Coef.dat",ios::app) ;
    }
    if( drop_idx == 0 )
    {
      Channel << "# Output is ( time[s], |Channel_Coef| )" << endl
              << "# Channel_Coef is normalized." << endl
              << "# path is multipath index." << endl
              << "# u is received antenna index." << endl
              << "# s is transmitt antenna index." << endl << endl << endl << endl;
    }

    for(int u = 0; u < num_received_antenna; u++)
    {
      for(int s = 0; s < num_transmit_antenna; s++)
      {
        for(int path_idx = 0; path_idx < num_path[ sample_ue-1 ][0]; path_idx++)
        {
          Channel << "(drop = " << drop_idx << ", path = " << path_idx << ", u = " << u << ", s = " << s << ")" ;
          if( num_rays[sample_ue-1][0][path_idx][u][s] != 20 )
          {
            Channel << " # sub-cluster" ;
          }
          Channel << endl << endl ;
          for(int t_idx = 0; t_idx < run_times; t_idx++)
          {
            Channel << 0.001*t_idx << "\t" << abs( Coef[ path_idx ][ u ][ s ][ t_idx ] ) << endl ;
          }
          Channel << endl << endl ;
        }
      }
    }

    if( drop_idx == num_drops )
    {
      Channel.close();
    }
  }
}


/*===================================================================
FUNCTION: Get_PDF( void )

AUTHOR: Kim, Kyung-Won

DESCRIPTION: ITU Channel Model Simulator

NOTES: ...
   
===================================================================*/
void Get_PDF()
{
  for( int ue_idx = 0; ue_idx < num_ues; ue_idx++ )
  {
    num_full_path[drop_idx][ue_idx] = num_path[ue_idx][0]-4 ;
    Block_PL[drop_idx][ue_idx] = ue[ ue_idx ].PathLoss[ (int)( ue[ue_idx].sector_in_control ) ] ;
  }

  if( drop_idx == num_drops-1 )
  {
    PDF_path_loss() ;
    PDF_block_SS_1() ;
    PDF_block_SS_2() ;
    PDF_block_SS_3() ;
    PDF_block_SS_4() ;
    PDF_block_SS_5() ;
    PDF_block_SS_6() ;
    PDF_block_SS_7() ;
  }
}


/*===================================================================
FUNCTION: PDF_path_loss( void )

AUTHOR: Kim, Kyung-Won

DESCRIPTION: ITU Channel Model Simulator

NOTES: ...
   
===================================================================*/
void PDF_path_loss() 
{
  double min_value = 0. ;
  double max_value = 150. ;
  double gab = ( max_value - min_value ) / 199. ;

  ofstream PDF_PL ;

  if ( scenario == INDOOR_HOTSPOT ) 
  {
    if ( los_type == NLOS )
    {
      PDF_PL.open("./Output/PDF_InH_NLOS_PL.dat",ios::out) ;
    }
    else if ( los_type == LOS )
    {
      PDF_PL.open("./Output/PDF_InH_LOS_PL.dat",ios::out) ;
    }
    else
    {
      PDF_PL.open("./Output/PDF_InH_PL.dat",ios::out) ;
    }
  }
  else if ( scenario == URBAN_MACRO )
  {
    if ( los_type == NLOS )
    {
      PDF_PL.open("./Output/PDF_UMa_NLOS_PL.dat",ios::out) ;
    }
    else if ( los_type == LOS )
    {
      PDF_PL.open("./Output/PDF_UMa_LOS_PL.dat",ios::out) ;
    }
    else
    {
      PDF_PL.open("./Output/PDF_UMa_PL.dat",ios::out) ;
    }
  }
  else if ( scenario == URBAN_MICRO ) 
  { 
    if ( los_type == NLOS )
    {
      PDF_PL.open("./Output/PDF_UMi_NLOS_PL.dat",ios::out) ;
    }
    else if ( los_type == LOS )
    {
      PDF_PL.open("./Output/PDF_UMi_LOS_PL.dat",ios::out) ;
    }
    else
    {
      PDF_PL.open("./Output/PDF_UMi_PL.dat",ios::out) ;
    }
  }
  else if ( scenario == RURAL_MACRO ) 
  { 
    if ( los_type == NLOS )
    {
      PDF_PL.open("./Output/PDF_RMa_NLOS_PL.dat",ios::out) ;
    }
    else if ( los_type == LOS )
    {
      PDF_PL.open("./Output/PDF_RMa_LOS_PL.dat",ios::out) ;
    }
    else
    {
      PDF_PL.open("./Output/PDF_RMa_PL.dat",ios::out) ;
    }
  }
  else if ( scenario == SUBURBAN_MACRO ) 
  { 
    if ( los_type == NLOS )
    {
      PDF_PL.open("./Output/PDF_SMa_NLOS_PL.dat",ios::out) ;
    }
    else if ( los_type == LOS )
    {
      PDF_PL.open("./Output/PDF_SMa_LOS_PL.dat",ios::out) ;
    }
    else
    {
      PDF_PL.open("./Output/PDF_SMa_PL.dat",ios::out) ;
    }
  }
  else                         
  { 
    if ( los_type == NLOS )
    {
      PDF_PL.open("./Output/PDF_OtoI_NLOS_PL.dat",ios::out) ;
    }
    else if ( los_type == LOS )
    {
      PDF_PL.open("./Output/PDF_OtoI_LOS_PL.dat",ios::out) ;
    }
    else
    {
      PDF_PL.open("./Output/PDF_OtoI_PL.dat",ios::out) ;
    }
  }

  double out_PL[200] ;
  double PD_PL[200] ;
  for(int PD_idx = 0; PD_idx < 200; PD_idx++)
  {
  	out_PL[ PD_idx ] = 0 ;
  	PD_PL[ PD_idx ] = 0 ;
  }
  for(int PD_idx = 0; PD_idx < 200; PD_idx++)
  {
    for(int drop = 0; drop < num_drops; drop++)
    {
      for(int ue_idx = 0; ue_idx < num_ues; ue_idx++)
      {
        out_PL[ PD_idx ] = min_value + gab * PD_idx ;
        if( Block_PL[drop][ue_idx] >= out_PL[ PD_idx ] 
            && Block_PL[drop][ue_idx] < out_PL[ PD_idx ] + gab )
        {
          PD_PL[ PD_idx ] = PD_PL[ PD_idx ] + 1. / (double)( num_drops * num_ues * gab ) ;
        }
      }
    }
    PDF_PL << out_PL[ PD_idx ] << "\t" ;
    PDF_PL << PD_PL[ PD_idx ] << endl ;
  }
  PDF_PL.close();
}


/*===================================================================
FUNCTION: PDF_block_SS_1( void )

AUTHOR: Kim, Kyung-Won

DESCRIPTION: ITU Channel Model Simulator

NOTES: ...
   
===================================================================*/
void PDF_block_SS_1()
{
  double min_value = 0. ;
  double max_value = pow(0.1, 6) ;
  double gab = ( max_value - min_value ) / 199. ;
  
  ofstream PDF_SS_1 ;
  
  if ( scenario == INDOOR_HOTSPOT ) 
  {
    if ( los_type == NLOS )
    {
      PDF_SS_1.open("./Output/PDF_InH_NLOS_SS_1.dat",ios::out) ;
    }
    else if ( los_type == LOS )
    {
      PDF_SS_1.open("./Output/PDF_InH_LOS_SS_1.dat",ios::out) ;
    }
    else
    {
      PDF_SS_1.open("./Output/PDF_InH_SS_1.dat",ios::out) ;
    }
  }
  else if ( scenario == URBAN_MACRO )
  {
    if ( los_type == NLOS )
    {
      PDF_SS_1.open("./Output/PDF_UMa_NLOS_SS_1.dat",ios::out) ;
    }
    else if ( los_type == LOS )
    {
      PDF_SS_1.open("./Output/PDF_UMa_LOS_SS_1.dat",ios::out) ;
    }
    else
    {
      PDF_SS_1.open("./Output/PDF_UMa_SS_1.dat",ios::out) ;
    }
  }
  else if ( scenario == URBAN_MICRO ) 
  { 
    if ( los_type == NLOS )
    {
      PDF_SS_1.open("./Output/PDF_UMi_NLOS_SS_1.dat",ios::out) ;
    }
    else if ( los_type == LOS )
    {
      PDF_SS_1.open("./Output/PDF_UMi_LOS_SS_1.dat",ios::out) ;
    }
    else
    {
      PDF_SS_1.open("./Output/PDF_UMi_SS_1.dat",ios::out) ;
    }
  }
  else if ( scenario == RURAL_MACRO ) 
  { 
    if ( los_type == NLOS )
    {
      PDF_SS_1.open("./Output/PDF_RMa_NLOS_SS_1.dat",ios::out) ;
    }
    else if ( los_type == LOS )
    {
      PDF_SS_1.open("./Output/PDF_RMa_LOS_SS_1.dat",ios::out) ;
    }
    else
    {
      PDF_SS_1.open("./Output/PDF_RMa_SS_1.dat",ios::out) ;
    }
  }
  else if ( scenario == SUBURBAN_MACRO ) 
  { 
    if ( los_type == NLOS )
    {
      PDF_SS_1.open("./Output/PDF_SMa_NLOS_SS_1.dat",ios::out) ;
    }
    else if ( los_type == LOS )
    {
      PDF_SS_1.open("./Output/PDF_SMa_LOS_SS_1.dat",ios::out) ;
    }
    else
    {
      PDF_SS_1.open("./Output/PDF_SMa_SS_1.dat",ios::out) ;
    }
  }
  else                         
  { 
    if ( los_type == NLOS )
    {
      PDF_SS_1.open("./Output/PDF_OtoI_NLOS_SS_1.dat",ios::out) ;
    }
    else if ( los_type == LOS )
    {
      PDF_SS_1.open("./Output/PDF_OtoI_LOS_SS_1.dat",ios::out) ;
    }
    else
    {
      PDF_SS_1.open("./Output/PDF_OtoI_SS_1.dat",ios::out) ;
    }
  }

//  PDF_SS_1 << "# Output is ( output of Block_SS_1, probability density )" << endl << endl << endl << endl;
  double out_SS_1[200] ;
  double PD_SS_1[200] ;
  for(int PD_idx = 0; PD_idx < 200; PD_idx++)
  {
  	out_SS_1[ PD_idx ] = 0 ;
  	PD_SS_1[ PD_idx ] = 0 ;
  }
  for(int PD_idx = 0; PD_idx < 200; PD_idx++)
	{
    for(int drop = 0; drop < num_drops; drop++)
    {
      for(int ue_idx = 0; ue_idx < num_ues; ue_idx++)
      {
        for(int path_idx = 0; path_idx < num_full_path[drop][ue_idx]; path_idx++)
        {
          for(int u = 0; u < num_received_antenna; u++)
          {
            for(int s = 0; s < num_transmit_antenna; s++)
            {
          		out_SS_1[ PD_idx ] = min_value + gab * PD_idx ;
            	if( Block_SS_1[ drop ][ ue_idx ][ path_idx ][u][s] >= out_SS_1[ PD_idx ] && Block_SS_1[ drop ][ ue_idx ][ path_idx ][u][s] < out_SS_1[ PD_idx ] + gab )
              {
              	PD_SS_1[ PD_idx ] = PD_SS_1[ PD_idx ] + 1. / (double)( num_drops * num_ues * num_full_path[drop][ue_idx] * num_received_antenna * num_transmit_antenna * gab ) ;
              }
            }
        	}
        }
      }
    }
    PDF_SS_1 << out_SS_1[ PD_idx ] << "\t" ;
    PDF_SS_1 << PD_SS_1[ PD_idx ] << endl ;
  }
  PDF_SS_1.close();
}

/*===================================================================
FUNCTION: PDF_block_SS_2( void )

AUTHOR: Kim, Kyung-Won

DESCRIPTION: ITU Channel Model Simulator

NOTES: ...
   
===================================================================*/
void PDF_block_SS_2()
{
  double min_value = 0. ;
  double max_value = pow(0.1, 6) ;
  double gab = ( max_value - min_value ) / 199. ;
  
  ofstream PDF_SS_2 ;
  
  if ( scenario == INDOOR_HOTSPOT ) 
  {
    if ( los_type == NLOS )
    {
      PDF_SS_2.open("./Output/PDF_InH_NLOS_SS_2.dat",ios::out) ;
    }
    else if ( los_type == LOS )
    {
      PDF_SS_2.open("./Output/PDF_InH_LOS_SS_2.dat",ios::out) ;
    }
    else
    {
      PDF_SS_2.open("./Output/PDF_InH_SS_2.dat",ios::out) ;
    }
  }
  else if ( scenario == URBAN_MACRO )
  {
    if ( los_type == NLOS )
    {
      PDF_SS_2.open("./Output/PDF_UMa_NLOS_SS_2.dat",ios::out) ;
    }
    else if ( los_type == LOS )
    {
      PDF_SS_2.open("./Output/PDF_UMa_LOS_SS_2.dat",ios::out) ;
    }
    else
    {
      PDF_SS_2.open("./Output/PDF_UMa_SS_2.dat",ios::out) ;
    }
  }
  else if ( scenario == URBAN_MICRO ) 
  { 
    if ( los_type == NLOS )
    {
      PDF_SS_2.open("./Output/PDF_UMi_NLOS_SS_2.dat",ios::out) ;
    }
    else if ( los_type == LOS )
    {
      PDF_SS_2.open("./Output/PDF_UMi_LOS_SS_2.dat",ios::out) ;
    }
    else
    {
      PDF_SS_2.open("./Output/PDF_UMi_SS_2.dat",ios::out) ;
    }
  }
  else if ( scenario == RURAL_MACRO ) 
  { 
    if ( los_type == NLOS )
    {
      PDF_SS_2.open("./Output/PDF_RMa_NLOS_SS_2.dat",ios::out) ;
    }
    else if ( los_type == LOS )
    {
      PDF_SS_2.open("./Output/PDF_RMa_LOS_SS_2.dat",ios::out) ;
    }
    else
    {
      PDF_SS_2.open("./Output/PDF_RMa_SS_2.dat",ios::out) ;
    }
  }
  else if ( scenario == SUBURBAN_MACRO ) 
  { 
    if ( los_type == NLOS )
    {
      PDF_SS_2.open("./Output/PDF_SMa_NLOS_SS_2.dat",ios::out) ;
    }
    else if ( los_type == LOS )
    {
      PDF_SS_2.open("./Output/PDF_SMa_LOS_SS_2.dat",ios::out) ;
    }
    else
    {
      PDF_SS_2.open("./Output/PDF_SMa_SS_2.dat",ios::out) ;
    }
  }
  else                         
  { 
    if ( los_type == NLOS )
    {
      PDF_SS_2.open("./Output/PDF_OtoI_NLOS_SS_2.dat",ios::out) ;
    }
    else if ( los_type == LOS )
    {
      PDF_SS_2.open("./Output/PDF_OtoI_LOS_SS_2.dat",ios::out) ;
    }
    else
    {
      PDF_SS_2.open("./Output/PDF_OtoI_SS_2.dat",ios::out) ;
    }
  }

//  PDF_SS_2 << "# Output is ( output of Block_SS_2, probability density )" << endl << endl << endl << endl;
  double out_SS_2[200] ;
  double PD_SS_2[200] ;
 	for(int PD_idx = 0; PD_idx < 200; PD_idx++)
  {
  	out_SS_2[ PD_idx ] = 0 ;
  	PD_SS_2[ PD_idx ] = 0 ;
  }
  for(int PD_idx = 0; PD_idx < 200; PD_idx++)
	{
    for(int drop = 0; drop < num_drops; drop++)
    {
      for(int ue_idx = 0; ue_idx < num_ues; ue_idx++)
      {
        for(int path_idx = 0; path_idx < num_full_path[drop][ue_idx]; path_idx++)
        {
          for(int u = 0; u < num_received_antenna; u++)
          {
            for(int s = 0; s < num_transmit_antenna; s++)
            {
          		out_SS_2[ PD_idx ] = min_value + gab * PD_idx ;
            	if( Block_SS_2[ drop ][ ue_idx ][ path_idx ][u][s] >= out_SS_2[ PD_idx ] && Block_SS_2[ drop ][ ue_idx ][ path_idx ][u][s] < out_SS_2[ PD_idx ] + gab )
              {
              	PD_SS_2[ PD_idx ] = PD_SS_2[ PD_idx ] + 1. / (double)( num_drops * num_ues * num_full_path[drop][ue_idx] * num_received_antenna * num_transmit_antenna * gab ) ;
              }
            }
        	}
        }
      }
    }
    PDF_SS_2 << out_SS_2[ PD_idx ] << "\t" ;
    PDF_SS_2 << PD_SS_2[ PD_idx ] << endl ;
  }
  PDF_SS_2.close();
}

/*===================================================================
FUNCTION: PDF_block_SS_3( void )

AUTHOR: Kim, Kyung-Won

DESCRIPTION: ITU Channel Model Simulator

NOTES: ...
   
===================================================================*/
void PDF_block_SS_3()
{
  double min_value = 0. ;
  double max_value = 1. ;
  double gab = ( max_value - min_value ) / 199. ;
  
  ofstream PDF_SS_3 ;
  
  if ( scenario == INDOOR_HOTSPOT ) 
  {
    if ( los_type == NLOS )
    {
      PDF_SS_3.open("./Output/PDF_InH_NLOS_SS_3.dat",ios::out) ;
    }
    else if ( los_type == LOS )
    {
      PDF_SS_3.open("./Output/PDF_InH_LOS_SS_3.dat",ios::out) ;
    }
    else
    {
      PDF_SS_3.open("./Output/PDF_InH_SS_3.dat",ios::out) ;
    }
  }
  else if ( scenario == URBAN_MACRO )
  {
    if ( los_type == NLOS )
    {
      PDF_SS_3.open("./Output/PDF_UMa_NLOS_SS_3.dat",ios::out) ;
    }
    else if ( los_type == LOS )
    {
      PDF_SS_3.open("./Output/PDF_UMa_LOS_SS_3.dat",ios::out) ;
    }
    else
    {
      PDF_SS_3.open("./Output/PDF_UMa_SS_3.dat",ios::out) ;
    }
  }
  else if ( scenario == URBAN_MICRO ) 
  { 
    if ( los_type == NLOS )
    {
      PDF_SS_3.open("./Output/PDF_UMi_NLOS_SS_3.dat",ios::out) ;
    }
    else if ( los_type == LOS )
    {
      PDF_SS_3.open("./Output/PDF_UMi_LOS_SS_3.dat",ios::out) ;
    }
    else
    {
      PDF_SS_3.open("./Output/PDF_UMi_SS_3.dat",ios::out) ;
    }
  }
  else if ( scenario == RURAL_MACRO ) 
  { 
    if ( los_type == NLOS )
    {
      PDF_SS_3.open("./Output/PDF_RMa_NLOS_SS_3.dat",ios::out) ;
    }
    else if ( los_type == LOS )
    {
      PDF_SS_3.open("./Output/PDF_RMa_LOS_SS_3.dat",ios::out) ;
    }
    else
    {
      PDF_SS_3.open("./Output/PDF_RMa_SS_3.dat",ios::out) ;
    }
  }
  else if ( scenario == SUBURBAN_MACRO ) 
  { 
    if ( los_type == NLOS )
    {
      PDF_SS_3.open("./Output/PDF_SMa_NLOS_SS_3.dat",ios::out) ;
    }
    else if ( los_type == LOS )
    {
      PDF_SS_3.open("./Output/PDF_SMa_LOS_SS_3.dat",ios::out) ;
    }
    else
    {
      PDF_SS_3.open("./Output/PDF_SMa_SS_3.dat",ios::out) ;
    }
  }
  else                         
  { 
    if ( los_type == NLOS )
    {
      PDF_SS_3.open("./Output/PDF_OtoI_NLOS_SS_3.dat",ios::out) ;
    }
    else if ( los_type == LOS )
    {
      PDF_SS_3.open("./Output/PDF_OtoI_LOS_SS_3.dat",ios::out) ;
    }
    else
    {
      PDF_SS_3.open("./Output/PDF_OtoI_SS_3.dat",ios::out) ;
    }
  }

//  PDF_SS_3 << "# Output is ( output of Block_SS_3, probability density )" << endl << endl << endl << endl;
  double out_SS_3[200] ;
  double PD_SS_3[200] ;
  for(int PD_idx = 0; PD_idx < 200; PD_idx++)
  {
  	out_SS_3[ PD_idx ] = 0 ;
  	PD_SS_3[ PD_idx ] = 0 ;
  }
  for(int PD_idx = 0; PD_idx < 200; PD_idx++)
	{
    for(int drop = 0; drop < num_drops; drop++)
    {
      for(int ue_idx = 0; ue_idx < num_ues; ue_idx++)
      {
        for(int path_idx = 0; path_idx < num_full_path[drop][ue_idx]; path_idx++)
        {
          for(int u = 0; u < num_received_antenna; u++)
          {
            for(int s = 0; s < num_transmit_antenna; s++)
            {
          		out_SS_3[ PD_idx ] = min_value + gab * PD_idx ;
            	if( Block_SS_3[ drop ][ ue_idx ][ path_idx ][u][s] >= out_SS_3[ PD_idx ] && Block_SS_3[ drop ][ ue_idx ][ path_idx ][u][s] < out_SS_3[ PD_idx ] + gab )
              {
              	PD_SS_3[ PD_idx ] = PD_SS_3[ PD_idx ] + 1. / (double)( num_drops * num_ues * num_full_path[drop][ue_idx] * num_received_antenna * num_transmit_antenna * gab ) ;
              }
            }
        	}
        }
      }
    }
    PDF_SS_3 << out_SS_3[ PD_idx ] << "\t" ;
    PDF_SS_3 << PD_SS_3[ PD_idx ] << endl ;
  }
  PDF_SS_3.close();
}

/*===================================================================
FUNCTION: PDF_block_SS_4( void )

AUTHOR: Kim, Kyung-Won

DESCRIPTION: ITU Channel Model Simulator

NOTES: ...
   
===================================================================*/
void PDF_block_SS_4()
{
  double min_value = 0. ;
  double max_value = 1000. ;
  double gab = ( max_value - min_value ) / 199. ;
  
  ofstream PDF_SS_4 ;
  
  if ( scenario == INDOOR_HOTSPOT ) 
  {
    if ( los_type == NLOS )
    {
      PDF_SS_4.open("./Output/PDF_InH_NLOS_SS_4.dat",ios::out) ;
    }
    else if ( los_type == LOS )
    {
      PDF_SS_4.open("./Output/PDF_InH_LOS_SS_4.dat",ios::out) ;
    }
    else
    {
      PDF_SS_4.open("./Output/PDF_InH_SS_4.dat",ios::out) ;
    }
  }
  else if ( scenario == URBAN_MACRO )
  {
    if ( los_type == NLOS )
    {
      PDF_SS_4.open("./Output/PDF_UMa_NLOS_SS_4.dat",ios::out) ;
    }
    else if ( los_type == LOS )
    {
      PDF_SS_4.open("./Output/PDF_UMa_LOS_SS_4.dat",ios::out) ;
    }
    else
    {
      PDF_SS_4.open("./Output/PDF_UMa_SS_4.dat",ios::out) ;
    }
  }
  else if ( scenario == URBAN_MICRO ) 
  { 
    if ( los_type == NLOS )
    {
      PDF_SS_4.open("./Output/PDF_UMi_NLOS_SS_4.dat",ios::out) ;
    }
    else if ( los_type == LOS )
    {
      PDF_SS_4.open("./Output/PDF_UMi_LOS_SS_4.dat",ios::out) ;
    }
    else
    {
      PDF_SS_4.open("./Output/PDF_UMi_SS_4.dat",ios::out) ;
    }
  }
  else if ( scenario == RURAL_MACRO ) 
  { 
    if ( los_type == NLOS )
    {
      PDF_SS_4.open("./Output/PDF_RMa_NLOS_SS_4.dat",ios::out) ;
    }
    else if ( los_type == LOS )
    {
      PDF_SS_4.open("./Output/PDF_RMa_LOS_SS_4.dat",ios::out) ;
    }
    else
    {
      PDF_SS_4.open("./Output/PDF_RMa_SS_4.dat",ios::out) ;
    }
  }
  else if ( scenario == SUBURBAN_MACRO ) 
  { 
    if ( los_type == NLOS )
    {
      PDF_SS_4.open("./Output/PDF_SMa_NLOS_SS_4.dat",ios::out) ;
    }
    else if ( los_type == LOS )
    {
      PDF_SS_4.open("./Output/PDF_SMa_LOS_SS_4.dat",ios::out) ;
    }
    else
    {
      PDF_SS_4.open("./Output/PDF_SMa_SS_4.dat",ios::out) ;
    }
  }
  else                         
  { 
    if ( los_type == NLOS )
    {
      PDF_SS_4.open("./Output/PDF_OtoI_NLOS_SS_4.dat",ios::out) ;
    }
    else if ( los_type == LOS )
    {
      PDF_SS_4.open("./Output/PDF_OtoI_LOS_SS_4.dat",ios::out) ;
    }
    else
    {
      PDF_SS_4.open("./Output/PDF_OtoI_SS_4.dat",ios::out) ;
    }
  }

//  PDF_SS_4 << "# Output is ( output of Block_SS_4, probability density )" << endl << endl << endl << endl;
  double out_SS_4[200] ;
  double PD_SS_4[200] ;
  for(int PD_idx = 0; PD_idx < 200; PD_idx++)
  {
  	out_SS_4[ PD_idx ] = 0 ;
  	PD_SS_4[ PD_idx ] = 0 ;
  }
  for(int PD_idx = 0; PD_idx < 200; PD_idx++)
	{
    for(int drop = 0; drop < num_drops; drop++)
    {
      for(int ue_idx = 0; ue_idx < num_ues; ue_idx++)
      {
        for(int path_idx = 0; path_idx < num_full_path[drop][ue_idx]; path_idx++)
        {
          for(int u = 0; u < num_received_antenna; u++)
          {
            for(int s = 0; s < num_transmit_antenna; s++)
            {
          		out_SS_4[ PD_idx ] = min_value + gab * PD_idx ;
            	if( Block_SS_4[ drop ][ ue_idx ][ path_idx ][u][s] >= out_SS_4[ PD_idx ] && Block_SS_4[ drop ][ ue_idx ][ path_idx ][u][s] < out_SS_4[ PD_idx ] + gab )
              {
              	PD_SS_4[ PD_idx ] = PD_SS_4[ PD_idx ] + 1. / (double)( num_drops * num_ues * num_full_path[drop][ue_idx] * num_received_antenna * num_transmit_antenna * gab ) ;
              }
            }
        	}
        }
      }
    }
    PDF_SS_4 << out_SS_4[ PD_idx ] << "\t" ;
    PDF_SS_4 << PD_SS_4[ PD_idx ] << endl ;
  }
  PDF_SS_4.close();
}

/*===================================================================
FUNCTION: PDF_block_SS_5( void )

AUTHOR: Kim, Kyung-Won

DESCRIPTION: ITU Channel Model Simulator

NOTES: ...
   
===================================================================*/
void PDF_block_SS_5()
{
  double min_value = 0. ;
  double max_value = 1000. ;
  double gab = ( max_value - min_value ) / 199. ;
  
  ofstream PDF_SS_5 ;
  
  if ( scenario == INDOOR_HOTSPOT ) 
  {
    if ( los_type == NLOS )
    {
      PDF_SS_5.open("./Output/PDF_InH_NLOS_SS_5.dat",ios::out) ;
    }
    else if ( los_type == LOS )
    {
      PDF_SS_5.open("./Output/PDF_InH_LOS_SS_5.dat",ios::out) ;
    }
    else
    {
      PDF_SS_5.open("./Output/PDF_InH_SS_5.dat",ios::out) ;
    }
  }
  else if ( scenario == URBAN_MACRO )
  {
    if ( los_type == NLOS )
    {
      PDF_SS_5.open("./Output/PDF_UMa_NLOS_SS_5.dat",ios::out) ;
    }
    else if ( los_type == LOS )
    {
      PDF_SS_5.open("./Output/PDF_UMa_LOS_SS_5.dat",ios::out) ;
    }
    else
    {
      PDF_SS_5.open("./Output/PDF_UMa_SS_5.dat",ios::out) ;
    }
  }
  else if ( scenario == URBAN_MICRO ) 
  { 
    if ( los_type == NLOS )
    {
      PDF_SS_5.open("./Output/PDF_UMi_NLOS_SS_5.dat",ios::out) ;
    }
    else if ( los_type == LOS )
    {
      PDF_SS_5.open("./Output/PDF_UMi_LOS_SS_5.dat",ios::out) ;
    }
    else
    {
      PDF_SS_5.open("./Output/PDF_UMi_SS_5.dat",ios::out) ;
    }
  }
  else if ( scenario == RURAL_MACRO ) 
  { 
    if ( los_type == NLOS )
    {
      PDF_SS_5.open("./Output/PDF_RMa_NLOS_SS_5.dat",ios::out) ;
    }
    else if ( los_type == LOS )
    {
      PDF_SS_5.open("./Output/PDF_RMa_LOS_SS_5.dat",ios::out) ;
    }
    else
    {
      PDF_SS_5.open("./Output/PDF_RMa_SS_5.dat",ios::out) ;
    }
  }
  else if ( scenario == SUBURBAN_MACRO ) 
  { 
    if ( los_type == NLOS )
    {
      PDF_SS_5.open("./Output/PDF_SMa_NLOS_SS_5.dat",ios::out) ;
    }
    else if ( los_type == LOS )
    {
      PDF_SS_5.open("./Output/PDF_SMa_LOS_SS_5.dat",ios::out) ;
    }
    else
    {
      PDF_SS_5.open("./Output/PDF_SMa_SS_5.dat",ios::out) ;
    }
  }
  else                         
  { 
    if ( los_type == NLOS )
    {
      PDF_SS_5.open("./Output/PDF_OtoI_NLOS_SS_5.dat",ios::out) ;
    }
    else if ( los_type == LOS )
    {
      PDF_SS_5.open("./Output/PDF_OtoI_LOS_SS_5.dat",ios::out) ;
    }
    else
    {
      PDF_SS_5.open("./Output/PDF_OtoI_SS_5.dat",ios::out) ;
    }
  }

//  PDF_SS_5 << "# Output is ( output of Block_SS_5, probability density )" << endl << endl << endl << endl;
  double out_SS_5[200] ;
  double PD_SS_5[200] ;
  for(int PD_idx = 0; PD_idx < 200; PD_idx++)
  {
  	out_SS_5[ PD_idx ] = 0 ;
  	PD_SS_5[ PD_idx ] = 0 ;
  }
  for(int PD_idx = 0; PD_idx < 200; PD_idx++)
	{
    for(int drop = 0; drop < num_drops; drop++)
    {
      for(int ue_idx = 0; ue_idx < num_ues; ue_idx++)
      {
        for(int path_idx = 0; path_idx < num_full_path[drop][ue_idx]; path_idx++)
        {
          for(int u = 0; u < num_received_antenna; u++)
          {
            for(int s = 0; s < num_transmit_antenna; s++)
            {
          		out_SS_5[ PD_idx ] = min_value + gab * PD_idx ;
            	if( Block_SS_5[ drop ][ ue_idx ][ path_idx ][u][s] >= out_SS_5[ PD_idx ] && Block_SS_5[ drop ][ ue_idx ][ path_idx ][u][s] < out_SS_5[ PD_idx ] + gab )
              {
              	PD_SS_5[ PD_idx ] = PD_SS_5[ PD_idx ] + 1. / (double)( num_drops * num_ues * num_full_path[drop][ue_idx] * num_received_antenna * num_transmit_antenna * gab ) ;
              }
            }
        	}
        }
      }
    }
    PDF_SS_5 << out_SS_5[ PD_idx ] << "\t" ;
    PDF_SS_5 << PD_SS_5[ PD_idx ] << endl ;
  }
  PDF_SS_5.close();
}

/*===================================================================
FUNCTION: PDF_block_SS_6( void )

AUTHOR: Kim, Kyung-Won

DESCRIPTION: ITU Channel Model Simulator

NOTES: ...
   
===================================================================*/
void PDF_block_SS_6()
{
  double min_value = -180. ;
  double max_value = 180. ;
  double gab = ( max_value - min_value ) / 49. ;
  
  ofstream PDF_SS_6 ;
  
  if ( scenario == INDOOR_HOTSPOT ) 
  {
    if ( los_type == NLOS )
    {
      PDF_SS_6.open("./Output/PDF_InH_NLOS_SS_6.dat",ios::out) ;
    }
    else if ( los_type == LOS )
    {
      PDF_SS_6.open("./Output/PDF_InH_LOS_SS_6.dat",ios::out) ;
    }
    else
    {
      PDF_SS_6.open("./Output/PDF_InH_SS_6.dat",ios::out) ;
    }
  }
  else if ( scenario == URBAN_MACRO )
  {
    if ( los_type == NLOS )
    {
      PDF_SS_6.open("./Output/PDF_UMa_NLOS_SS_6.dat",ios::out) ;
    }
    else if ( los_type == LOS )
    {
      PDF_SS_6.open("./Output/PDF_UMa_LOS_SS_6.dat",ios::out) ;
    }
    else
    {
      PDF_SS_6.open("./Output/PDF_UMa_SS_6.dat",ios::out) ;
    }
  }
  else if ( scenario == URBAN_MICRO ) 
  { 
    if ( los_type == NLOS )
    {
      PDF_SS_6.open("./Output/PDF_UMi_NLOS_SS_6.dat",ios::out) ;
    }
    else if ( los_type == LOS )
    {
      PDF_SS_6.open("./Output/PDF_UMi_LOS_SS_6.dat",ios::out) ;
    }
    else
    {
      PDF_SS_6.open("./Output/PDF_UMi_SS_6.dat",ios::out) ;
    }
  }
  else if ( scenario == RURAL_MACRO ) 
  { 
    if ( los_type == NLOS )
    {
      PDF_SS_6.open("./Output/PDF_RMa_NLOS_SS_6.dat",ios::out) ;
    }
    else if ( los_type == LOS )
    {
      PDF_SS_6.open("./Output/PDF_RMa_LOS_SS_6.dat",ios::out) ;
    }
    else
    {
      PDF_SS_6.open("./Output/PDF_RMa_SS_6.dat",ios::out) ;
    }
  }
  else if ( scenario == SUBURBAN_MACRO ) 
  { 
    if ( los_type == NLOS )
    {
      PDF_SS_6.open("./Output/PDF_SMa_NLOS_SS_6.dat",ios::out) ;
    }
    else if ( los_type == LOS )
    {
      PDF_SS_6.open("./Output/PDF_SMa_LOS_SS_6.dat",ios::out) ;
    }
    else
    {
      PDF_SS_6.open("./Output/PDF_SMa_SS_6.dat",ios::out) ;
    }
  }
  else                         
  { 
    if ( los_type == NLOS )
    {
      PDF_SS_6.open("./Output/PDF_OtoI_NLOS_SS_6.dat",ios::out) ;
    }
    else if ( los_type == LOS )
    {
      PDF_SS_6.open("./Output/PDF_OtoI_LOS_SS_6.dat",ios::out) ;
    }
    else
    {
      PDF_SS_6.open("./Output/PDF_OtoI_SS_6.dat",ios::out) ;
    }
  }

//  PDF_SS_6 << "# Output is ( output of Block_SS_6, probability density )" << endl << endl << endl << endl;
  double out_SS_6[50] ;
  double PD_SS_6[50] ;
  for(int PD_idx = 0; PD_idx < 50; PD_idx++)
  {
  	out_SS_6[ PD_idx ] = 0 ;
  	PD_SS_6[ PD_idx ] = 0 ;
  }
  for(int PD_idx = 0; PD_idx < 50; PD_idx++)
	{
    for(int drop = 0; drop < num_drops; drop++)
    {
      for(int ue_idx = 0; ue_idx < num_ues; ue_idx++)
      {
        for(int path_idx = 0; path_idx < num_full_path[drop][ue_idx]; path_idx++)
        {
          for(int u = 0; u < num_received_antenna; u++)
          {
            for(int s = 0; s < num_transmit_antenna; s++)
            {
          		out_SS_6[ PD_idx ] = min_value + gab * PD_idx ;
            	if( Block_SS_6[ drop ][ ue_idx ][ path_idx ][u][s] >= out_SS_6[ PD_idx ] && Block_SS_6[ drop ][ ue_idx ][ path_idx ][u][s] < out_SS_6[ PD_idx ] + gab )
              {
              	PD_SS_6[ PD_idx ] = PD_SS_6[ PD_idx ] + 1. / (double)( num_drops * num_ues * num_full_path[drop][ue_idx] * num_received_antenna * num_transmit_antenna * gab ) ;
              }
            }
        	}
        }
      }
    }
    PDF_SS_6 << out_SS_6[ PD_idx ] << "\t" ;
    PDF_SS_6 << PD_SS_6[ PD_idx ] << endl ;
  }
  PDF_SS_6.close();
}

/*===================================================================
FUNCTION: PDF_block_SS_7( void )

AUTHOR: Kim, Kyung-Won

DESCRIPTION: ITU Channel Model Simulator

NOTES: ...
   
===================================================================*/
void PDF_block_SS_7()
{
  double min_value = -180. ;
  double max_value = 180. ;
  double gab = ( max_value - min_value ) / 49. ;
  
  ofstream PDF_SS_7 ;
  
  if ( scenario == INDOOR_HOTSPOT ) 
  {
    if ( los_type == NLOS )
    {
      PDF_SS_7.open("./Output/PDF_InH_NLOS_SS_7.dat",ios::out) ;
    }
    else if ( los_type == LOS )
    {
      PDF_SS_7.open("./Output/PDF_InH_LOS_SS_7.dat",ios::out) ;
    }
    else
    {
      PDF_SS_7.open("./Output/PDF_InH_SS_7.dat",ios::out) ;
    }
  }
  else if ( scenario == URBAN_MACRO )
  {
    if ( los_type == NLOS )
    {
      PDF_SS_7.open("./Output/PDF_UMa_NLOS_SS_7.dat",ios::out) ;
    }
    else if ( los_type == LOS )
    {
      PDF_SS_7.open("./Output/PDF_UMa_LOS_SS_7.dat",ios::out) ;
    }
    else
    {
      PDF_SS_7.open("./Output/PDF_UMa_SS_7.dat",ios::out) ;
    }
  }
  else if ( scenario == URBAN_MICRO ) 
  { 
    if ( los_type == NLOS )
    {
      PDF_SS_7.open("./Output/PDF_UMi_NLOS_SS_7.dat",ios::out) ;
    }
    else if ( los_type == LOS )
    {
      PDF_SS_7.open("./Output/PDF_UMi_LOS_SS_7.dat",ios::out) ;
    }
    else
    {
      PDF_SS_7.open("./Output/PDF_UMi_SS_7.dat",ios::out) ;
    }
  }
  else if ( scenario == RURAL_MACRO ) 
  { 
    if ( los_type == NLOS )
    {
      PDF_SS_7.open("./Output/PDF_RMa_NLOS_SS_7.dat",ios::out) ;
    }
    else if ( los_type == LOS )
    {
      PDF_SS_7.open("./Output/PDF_RMa_LOS_SS_7.dat",ios::out) ;
    }
    else
    {
      PDF_SS_7.open("./Output/PDF_RMa_SS_7.dat",ios::out) ;
    }
  }
  else if ( scenario == SUBURBAN_MACRO ) 
  { 
    if ( los_type == NLOS )
    {
      PDF_SS_7.open("./Output/PDF_SMa_NLOS_SS_7.dat",ios::out) ;
    }
    else if ( los_type == LOS )
    {
      PDF_SS_7.open("./Output/PDF_SMa_LOS_SS_7.dat",ios::out) ;
    }
    else
    {
      PDF_SS_7.open("./Output/PDF_SMa_SS_7.dat",ios::out) ;
    }
  }
  else                         
  { 
    if ( los_type == NLOS )
    {
      PDF_SS_7.open("./Output/PDF_OtoI_NLOS_SS_7.dat",ios::out) ;
    }
    else if ( los_type == LOS )
    {
      PDF_SS_7.open("./Output/PDF_OtoI_LOS_SS_7.dat",ios::out) ;
    }
    else
    {
      PDF_SS_7.open("./Output/PDF_OtoI_SS_7.dat",ios::out) ;
    }
  }

//  PDF_SS_7 << "# Output is ( output of Block_SS_7, probability density )" << endl << endl << endl << endl;
  double out_SS_7[50] ;
  double PD_SS_7[50] ;
  for(int PD_idx = 0; PD_idx < 50; PD_idx++)
  {
  	out_SS_7[ PD_idx ] = 0 ;
  	PD_SS_7[ PD_idx ] = 0 ;
  }
  for(int PD_idx = 0; PD_idx < 50; PD_idx++)
	{
    for(int drop = 0; drop < num_drops; drop++)
    {
      for(int ue_idx = 0; ue_idx < num_ues; ue_idx++)
      {
        for(int path_idx = 0; path_idx < num_full_path[drop][ue_idx]; path_idx++)
        {
          for(int u = 0; u < num_received_antenna; u++)
          {
            for(int s = 0; s < num_transmit_antenna; s++)
            {
          		out_SS_7[ PD_idx ] = min_value + gab * PD_idx ;
            	if( Block_SS_7[ drop ][ ue_idx ][ path_idx ][u][s] >= out_SS_7[ PD_idx ] && Block_SS_7[ drop ][ ue_idx ][ path_idx ][u][s] < out_SS_7[ PD_idx ] + gab )
              {
              	PD_SS_7[ PD_idx ] = PD_SS_7[ PD_idx ] + 1. / (double)( num_drops * num_ues * num_full_path[drop][ue_idx] * num_received_antenna * num_transmit_antenna * gab ) ;
              }
            }
        	}
        }
      }
    }
    PDF_SS_7 << out_SS_7[ PD_idx ] << "\t" ;
    PDF_SS_7 << PD_SS_7[ PD_idx ] << endl ;
  }
  PDF_SS_7.close();
}

/*===================================================================
FUNCTION: Size_Logging_point_Parameters( void )

AUTHOR: Kim, Kyung-Won

DESCRIPTION: ITU Channel Model Simulator

NOTES: ...
   
===================================================================*/
void Size_Logging_point_Parameters()
{
  Coef = new complex <double> *** [ MAX_NUM_CLUSTERS ] ;
  for( int path_idx = 0; path_idx < MAX_NUM_CLUSTERS; path_idx++ )
  {
    Coef[ path_idx ] = new complex <double> ** [ num_received_antenna ] ;
    for( int u = 0; u < num_received_antenna; u++ )
    {
      Coef[ path_idx ][ u ] = new complex <double> * [ num_transmit_antenna ] ;
      for( int s = 0; s < num_transmit_antenna; s++ )
      {
        Coef[ path_idx ][ u ][ s ] = new complex <double> [ run_times ] ;
      }
    }
  }

  num_full_path = new int * [ num_drops ] ;
  for( drop_idx = 0; drop_idx < num_drops; drop_idx ++ )
  {
   num_full_path[ drop_idx ] = new int [ num_ues ] ;
  }

  Block_PL = new double * [ num_drops ] ;
  Block_SS_1 = new double **** [ num_drops ] ;
  Block_SS_2 = new double **** [ num_drops ] ;
  Block_SS_3 = new double **** [ num_drops ] ;
  Block_SS_4 = new double **** [ num_drops ] ;
  Block_SS_5 = new double **** [ num_drops ] ;
  Block_SS_6 = new double **** [ num_drops ] ;
  Block_SS_7 = new double **** [ num_drops ] ;
  for( drop_idx = 0; drop_idx < num_drops; drop_idx ++ )
  {
    Block_PL[ drop_idx ] = new double [ num_ues ] ;
    Block_SS_1[ drop_idx ] = new double *** [ num_ues ] ;
    Block_SS_2[ drop_idx ] = new double *** [ num_ues ] ;
    Block_SS_3[ drop_idx ] = new double *** [ num_ues ] ;
    Block_SS_4[ drop_idx ] = new double *** [ num_ues ] ;
    Block_SS_5[ drop_idx ] = new double *** [ num_ues ] ;
    Block_SS_6[ drop_idx ] = new double *** [ num_ues ] ;
    Block_SS_7[ drop_idx ] = new double *** [ num_ues ] ;
    for( int ue_idx = 0; ue_idx < num_ues; ue_idx++ )
    {
      Block_SS_1[ drop_idx ][ ue_idx ] = new double ** [MAX_NUM_CLUSTERS];
      Block_SS_2[ drop_idx ][ ue_idx ] = new double ** [MAX_NUM_CLUSTERS];
      Block_SS_3[ drop_idx ][ ue_idx ] = new double ** [MAX_NUM_CLUSTERS];
      Block_SS_4[ drop_idx ][ ue_idx ] = new double ** [MAX_NUM_CLUSTERS];
      Block_SS_5[ drop_idx ][ ue_idx ] = new double ** [MAX_NUM_CLUSTERS];
      Block_SS_6[ drop_idx ][ ue_idx ] = new double ** [MAX_NUM_CLUSTERS];
      Block_SS_7[ drop_idx ][ ue_idx ] = new double ** [MAX_NUM_CLUSTERS];
      for( int path_idx = 0; path_idx < MAX_NUM_CLUSTERS; path_idx++ )
      {
        Block_SS_1[ drop_idx ][ ue_idx ][ path_idx ] = new double * [ num_received_antenna ] ;
        Block_SS_2[ drop_idx ][ ue_idx ][ path_idx ] = new double * [ num_received_antenna ] ;
        Block_SS_3[ drop_idx ][ ue_idx ][ path_idx ] = new double * [ num_received_antenna ] ;
        Block_SS_4[ drop_idx ][ ue_idx ][ path_idx ] = new double * [ num_received_antenna ] ;
        Block_SS_5[ drop_idx ][ ue_idx ][ path_idx ] = new double * [ num_received_antenna ] ;
        Block_SS_6[ drop_idx ][ ue_idx ][ path_idx ] = new double * [ num_received_antenna ] ;
        Block_SS_7[ drop_idx ][ ue_idx ][ path_idx ] = new double * [ num_received_antenna ] ;
        for( int u = 0; u < num_received_antenna; u++ )
        {
          Block_SS_1[ drop_idx ][ ue_idx ][ path_idx ][ u ] = new double [ num_transmit_antenna ] ;
          Block_SS_2[ drop_idx ][ ue_idx ][ path_idx ][ u ] = new double [ num_transmit_antenna ] ;
          Block_SS_3[ drop_idx ][ ue_idx ][ path_idx ][ u ] = new double [ num_transmit_antenna ] ;
          Block_SS_4[ drop_idx ][ ue_idx ][ path_idx ][ u ] = new double [ num_transmit_antenna ] ;
          Block_SS_5[ drop_idx ][ ue_idx ][ path_idx ][ u ] = new double [ num_transmit_antenna ] ;
          Block_SS_6[ drop_idx ][ ue_idx ][ path_idx ][ u ] = new double [ num_transmit_antenna ] ;
          Block_SS_7[ drop_idx ][ ue_idx ][ path_idx ][ u ] = new double [ num_transmit_antenna ] ;
        }
      }
    }
  }
}
