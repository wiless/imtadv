#include <complex>
#include <iostream>
#include <fstream>
#include "ue.h"
#include "rv_generate.h"

using namespace std;

extern unsigned long _seed ;
extern int t ;
extern int drop_idx;
extern int run_times ;
extern int num_drops ;
extern int num_user_cell ;
extern int num_ues ;
extern int num_received_antenna ;
extern int num_transmit_antenna ;
extern int scenario ;
extern int los_type ;
extern int ue_distribution;
extern int PathLoss;
extern int DelaySpread;
extern int AngleSpread;
extern int ChannelCoef;
extern int num_compute_coef;
extern int PDF;
extern int sample_ue;

extern int my_sector ;
extern complex <double> ***** Channel_coef ;
extern int ** num_path ;
extern int ***** num_rays ;
extern double ***** delay ;
extern double ***** cluster_power ;
extern double ***** arrival_angle ;
extern double ***** depart_angle ;
extern int NUM_SECTORS ; 
extern double ** ue2sector_distance ;
extern double ** ue2sector_angle ;
extern bool ** indoor ;
extern bool ** los ;
extern double * antenna ;
extern double * path_loss ;
extern double * travel_angle ;

extern double ** Block_PL ;
extern double ***** Block_SS_1 ;
extern double ***** Block_SS_2 ;
extern double ***** Block_SS_3 ;
extern double ***** Block_SS_4 ;
extern double ***** Block_SS_5 ;
extern double ***** Block_SS_6 ;
extern double ***** Block_SS_7 ;
extern int ** num_full_path ;

extern complex <double> **** Coef ;

extern UE                  * ue;

extern Rand Rand;
	

