/*===================================================================
FUNCTION: Initialization

DESCRIPTION:

SimulationConfiguration()
InitializeSystem()
InitializeADrop()
ClearMAPs2StartADrop()
StandardInitialization()
  
===================================================================*/
#include "initialization.h"
#include "const.h"
#include <iostream>
#include <fstream>
#include <cmath>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
// #include <unistd.h>

using namespace std;

#include <cstring>
#include <cstdlib>


/*===================================================================
FUNCTION: SimulationConfiguration()

AUTHOR: Noh, Junghoon

DESCRIPTION: Configure the parameters from the input file. Otherwise
             simulation is configured by default values

NOTES: ...
   
===================================================================*/
void SimulationConfiguration(int argc,char *argv[]) 
{
  bool OpenFileIn(fstream & , char * );
  double Seek_Argument(fstream & ,const char *, double );
  char* Seek_Char(fstream & ,const char *,const char * );
  void senario_decision( char * Senario_char );
  void los_type_decision( char * LOS_char );
  void make_directory( void );
  
  if (argc == 2 )
  {
    fstream infile;
    
    if(!OpenFileIn(infile,argv[1]))
    {
      cout << "error : Can't find the file "<<"'"<<argv[1]<<"'"<<endl;
      exit(1);
    } 
     
    _seed              = int( Seek_Argument(infile,"_seed", 100000) );
    run_times          = int( Seek_Argument(infile,"run_times", 100) );
    num_drops          = int( Seek_Argument(infile,"num_drops", 2) );
    num_user_cell      = int( Seek_Argument(infile,"num_user_cell", 10) );
    num_received_antenna  = int( Seek_Argument(infile,"num_received_antenna", 2) );
    num_transmit_antenna = int( Seek_Argument(infile,"num_transmit_antenna", 2) );
    ue_distribution    = int( Seek_Argument(infile,"ue_distribution", 0) );
    PathLoss           = int( Seek_Argument(infile,"PathLoss", 0) );
    DelaySpread        = int( Seek_Argument(infile,"DelaySpread", 0) );
    AngleSpread        = int( Seek_Argument(infile,"AngleSpread", 0) );
    ChannelCoef        = int( Seek_Argument(infile,"ChannelCoef", 0) );
    num_compute_coef   = int( Seek_Argument(infile,"num_compute_coef", 1) );
    PDF                = int( Seek_Argument(infile,"PDF", 0) );
    sample_ue          = int( Seek_Argument(infile,"sample_ue", 0) );
    char * Senario_char   = Seek_Char(infile,"scenario", "URBAN_MACRO");
    char * LOS_char       = Seek_Char(infile,"los_type", "RANDOM");
    senario_decision( Senario_char );
    los_type_decision( LOS_char );
  }
  else if (argc == 1)
  {
    _seed = 100000;
    run_times = 100 ;
    num_drops = 2 ;
    num_user_cell = 10 ;
    num_received_antenna = 1 ;
    num_transmit_antenna = 1 ;
    scenario = 1;
    los_type = 2;
    ue_distribution = 0;
    PathLoss = 0;
    DelaySpread = 0;
    AngleSpread = 0;
    ChannelCoef = 0;
    num_compute_coef = 1;
    PDF = 0;
    sample_ue = 0;
  }
  else if (argc > 2)
  {
    cout << " Usage : run.exe filename " << endl; 
    exit(1);
  }
  // Configuration from Input file - TBA
  if ( scenario == INDOOR_HOTSPOT )
  {
    NUM_SECTORS = 2 ;
    num_compute_coef = 2 ;
  }
  else
  {
    NUM_SECTORS = 57 ;
  }
  num_ues = num_user_cell * NUM_SECTORS ;
}



/*===================================================================
FUNCTION: OpenFileIn()

AUTHOR: Noh, Junghoon

DESCRIPTION: used by simulationConfigure() to open the inputfile
   
===================================================================*/
bool OpenFileIn( fstream &file , char *name)
{
  bool status;

  file.open(name, ios::in);
  if (file.fail())
    status = false;
  else
    status = true;
  return status;
}

/*===================================================================
FUNCTION: Seek_Argument

AUTHOR: Noh, Junghoon

DESCRIPTION: find the argument from simulation configure file.
   
===================================================================*/
double Seek_Argument( fstream &file ,const char *name, double _default )
{ 
  void eatline( fstream &file );
  double argument = 0.0;
  bool I_got_it = false;
  char temp[30]; 
  char *_name = new char[30];
  strcpy( _name , name);
  
  while (!file.eof() && !I_got_it )
  {
    char comment_notice;
    file >> comment_notice;
    if ( comment_notice == '#')
      eatline( file ); 
    
    file.seekg(-1,ios::cur);
    file >> temp;
    if (!strcmp( _name,temp))
    {
      I_got_it = true;
      file >> argument;
    }
  }
  file.clear();
  file.seekg(0,ios::beg);
	
  if(I_got_it)
    return(argument);
  else
    return(_default);
}

/*===================================================================
FUNCTION: Seek_Char

AUTHOR: Noh, Junghoon

DESCRIPTION: Find the char variable from simulation configure file.
   
===================================================================*/
char* Seek_Char( fstream &file ,const char *name, const char* _default )
{
  void eatline( fstream &file );
  char* argument = new char[80];; 
  bool I_got_it = false;
  char temp[80];
  char *_name = new char[80];
  strcpy( _name , name);

  while (!file.eof() && !I_got_it )
  { 
    char comment_notice;
    file >> comment_notice;
    if ( comment_notice == '#')
      eatline( file ); 

    file.seekg(-1,ios::cur);
    file >> temp;
    if (!strcmp( _name,temp))
    {
      I_got_it = true;
      file >> argument;
    }
  }
  file.clear();
  file.seekg(0,ios::beg);

  if(I_got_it)
    return(argument);
  else
    return((char*)_default);
}

/*===================================================================
FUNCTION: eatline( fstream &file )

AUTHOR: Noh, Junghoon

DESCRIPTION: 

NOTES: ...
   
===================================================================*/
void eatline( fstream &file )
{
  char null_input[1000];
  file.getline(null_input,1000);
}

/*===================================================================
FUNCTION: senario_decision( char * Senario_char );

AUTHOR: Noh, Junghoon

DESCRIPTION: Read the senario type from configuration file

NOTES: ...
   
===================================================================*/
void senario_decision( char * Senario_char )
{
  char * Senario_char_;
  Senario_char_ = new char[50];
  strcpy( Senario_char_ , Senario_char);
  char * Senario_type_[6];
  
  for (int idx = 0; idx < 6; idx++)
  {
    Senario_type_[idx] = new char [50];
  }
   
  Senario_type_[0] = (char * )"INDOOR_HOTSPOT";
  Senario_type_[1] = (char * )"URBAN_MACRO";
  Senario_type_[2] = (char * )"URBAN_MICRO";
  Senario_type_[3] = (char * )"RURAL_MACRO";
  Senario_type_[4] = (char * )"SUBURBAN_MACRO";
  Senario_type_[5] = (char * )"OUTDOOR_2_INDOOR";

  for (int idx = 0;idx < 6; idx++ )
  {
    if (!strcmp( Senario_char_,Senario_type_[idx]))
    scenario = idx;
  }
}



/*===================================================================
FUNCTION: link_type_decision( char * Link_char );

AUTHOR: Kim, Kyung-Won

DESCRIPTION: Decide if it is Up-link or Down-link

NOTES: ...
   
===================================================================*/
void los_type_decision( char * LOS_char )
{
  char * LOS_char_;
  LOS_char_ = new char[50];
  strcpy( LOS_char_ , LOS_char);
  char * LOS_type_[3];
  
  for (int idx = 0; idx < 3; idx++)
  {
    LOS_type_[idx] = new char [50];
  }
   
  LOS_type_[0] = (char * )"LOS";
  LOS_type_[1] = (char * )"NLOS";
  LOS_type_[2] = (char * )"RANDOM";

  for (int idx = 0;idx < 3; idx++ )
  {
    if (!strcmp( LOS_char_,LOS_type_[idx]))
    los_type = idx;
  }
}


/*===================================================================
FUNCTION: InitializeSystem()

AUTHOR: SJ, DH, DW

DESCRIPTION: UE instantiation

NOTES: ...
   
===================================================================*/
void InitializeSystem()
{
  // Sector & UE instantiation
  ue     = new UE    [ num_ues  ] ;
}

/*===================================================================
FUNCTION: InitializeADrop()

AUTHOR: SJ

DESCRIPTION: Initialize a Drop

NOTES: Configure UE, initialize the maps
   
===================================================================*/
void InitializeADrop()
{
  for(int idx = 0 ; idx < num_ues ; idx ++)
  {
    ue[ idx ].Configure( idx ) ;
  }
}

/*===================================================================
FUNCTION: loading() 

AUTHOR: Noh, Junghoon

DESCRIPTION: Show How much simulation has proceeded

NOTES: Updated 7/10
   
===================================================================*/
void loading ()
{

    system("clear");
    cout << " Pleas Wait Until it get to the 100% " << endl;
    cout << "------------------------" <<"\t" 
         << 100.*(double)(t + 1 + drop_idx*run_times)/(double)(run_times*num_drops) << "%" << "\t"
         << "------------------------" <<endl;
    cout << " Current Drop is " << drop_idx+1 << "/" << num_drops <<endl;
    cout << "------------------------" <<"\t" 
         << 100.*(double)(t + 1)/(double)(run_times) << "%" << "\t"
         << "------------------------" <<endl;
}



void make_directory( void ){
  
}
