#include "const.h"

// scenario 0. Indoor Hotspot
// scenario 1. Urban Macro
// scenario 2. Urban Micro
// scenario 3. Rural Macro
// scenario 4. Suburban Macro


// Large Scale Parameters
double carrier_freq ;
double site2site_distance ;
double speed_UE ;
double K_mu, K_sigma ;
double ** Ricean_K ;
double D ;

double location_x, location_y ; 
double distance_x, distance_y ;
double ** ue2sector_distance ;
double ** ue2sector_angle ;
double * travel_angle ;
bool ** los ;
bool ** indoor ;

double h_BS, h_MS, h, W, break_point_distance ;

double shadowing, shadowing_sigma ;
double * path_loss ;
double * antenna ;



// Small Scale Parameters
int my_sector;
int ** num_path ;
complex <double> ***** Channel_coef ;

double clu_shadowing_std ;
double r, ds_mu, ds_sigma ;
double AoA_mu, AoD_mu, AoA_sigma, AoD_sigma, C ;
int X_A1, X_D1 ;
double Y_A1, Y_D1 ;
double ASA_sigma, ASD_sigma, DS_sigma ;
double cluster_ASA, cluster_ASD ;

double ***** delay ;
double ***** cluster_power ;
double ***** arrival_angle ;
double ***** depart_angle ;

int ***** num_rays ;
double ****** offset_angle ;
double ****** phi_vv , ****** phi_vh, ****** phi_hv, ****** phi_hh ;
double **** phi_vv_LOS, **** phi_vh_LOS, **** phi_hv_LOS, **** phi_hh_LOS ;

double d_tx, d_rx ;
double Lambda ;
double ****** XPR ;

double max_p ;
int max_idx, max_idx_2nd ;



// Sector Number Mapping Matrix
int wrap[19][19] ={ { 12,11,10,13,3,2,9,14,4,0,1,8,15,5,6,7,16,17,18 },
                    { 11,10,15,3,2,9,16,4,0,1,8,12,5,6,7,13,17,18,14 },
                    { 17,18,14,12,11,10,15,13,3,2,9,16,4,0,1,8,5,6,7 },
                    { 16,17,18,8,12,11,10,7,13,3,2,9,14,4,0,1,15,5,6 },
                    { 8,12,11,7,13,3,2,18,14,4,0,1,10,15,5,6,9,16,17 },
                    { 13,3,2,14,4,0,1,10,15,5,6,7,9,16,17,18,8,12,11 },
                    { 3,2,9,4,0,1,8,15,5,6,7,13,16,17,18,14,12,11,10 },
                    { 2,9,16,0,1,8,12,5,6,7,13,3,17,18,14,4,11,10,15 },
                    { 10,15,5,2,9,16,17,0,1,8,12,11,6,7,13,3,18,14,4 },
                    { 18,14,4,11,10,15,5,3,2,9,16,17,0,1,8,12,6,7,13 },
                    { 6,7,13,17,18,14,4,12,11,10,15,5,3,2,9,16,0,1,8 },
                    { 5,6,7,16,17,18,14,8,12,11,10,15,13,3,2,9,4,0,1 },
                    { 15,5,6,9,16,17,18,1,8,12,11,10,7,13,3,2,14,4,0 },
                    { 9,16,17,1,8,12,11,6,7,13,3,2,18,14,4,0,10,15,5 },
                    { 1,8,12,6,7,13,3,17,18,14,4,0,11,10,15,5,2,9,16 },
                    { 7,13,3,18,14,4,0,11,10,15,5,6,2,9,16,17,1,8,12 },
                    { 14,4,0,10,15,5,6,2,9,16,17,18,1,8,12,11,7,13,3 },
                    { 4,0,1,15,5,6,7,9,16,17,18,14,8,12,11,10,13,3,2 },
                    { 0,1,8,5,6,7,13,16,17,18,14,4,12,11,10,15,3,2,9 } };



// Correlation Parameters
double *** random_number ;
double sqrt_corr_matrix[5][5] ;
/*=====================================================================
  Correlation Matrix

  Corr_Matrix = [     1      ASDvsDS   ASAvsDS    DSvsSF    DSvsK
                   ASDvsDS      1      ASDvsASA  ASDvsSF   ASDvsK
                   ASAvsDS   ASDvsASA     1      ASAvsSF   ASAvsK
                    DSvsSF   ASDvsSF   ASAvsSF      1       SFvsK
                    DSvsK    ASDvsK    ASAvsK     SFvsK       1      ]
=======================================================================*/
/*=====================================================================
  Sqrt of Correlation Matrix

  sqrt_corr_matrix * sqrt_corr_matrix = Corr_Matrix
=======================================================================*/

