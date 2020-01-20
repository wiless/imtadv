#include "ue.h"
#include "rv_generate.h"
#include <complex>

using namespace std;

int t ;
int drop_idx;
int run_times ;
int num_drops ;
int num_user_cell ;
int num_ues ;
int num_received_antenna ;
int num_transmit_antenna ;
int scenario ;
int los_type ;
int ue_distribution;
int PathLoss;
int DelaySpread;
int AngleSpread;
int ChannelCoef;
int num_compute_coef;
int PDF;
int sample_ue;

complex <double> **** Coef ;
double ** Block_PL ;
double ***** Block_SS_1 ;
double ***** Block_SS_2 ;
double ***** Block_SS_3 ;
double ***** Block_SS_4 ;
double ***** Block_SS_5 ;
double ***** Block_SS_6 ;
double ***** Block_SS_7 ;
double ***** Block_SS_8 ;
int ** num_full_path ;

unsigned long _seed ;
int NUM_SECTORS ; 

Rand Rand((unsigned long long int)_seed);

UE                  * ue             ;





