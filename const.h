
#ifndef _CONST_H_
#define _CONST_H_
#include <cmath>
#define MIN(x,y) (((x)<=(y))?(x):(y))
#define MAX(x,y) (((x)>=(y))?(x):(y))
#define dB2linear(x) (pow(10.0,x/10.0))
#define dBm2linear(x) (pow(10.0, (x- 30)/10.0))
#define linear2dB(x) (10*log10(x))
#define linear2dBm (x) (10*log10(x * 1000 )

const double PI = 4 * atan(1.);
const double INITIAL_POWER = 1. ; 

const double SPEED_LIGHT = 299800000. ; // [m/s]
const int MAX_NUM_CLUSTERS = 24 ;
const int MAX_NUM_RAYS = 20 ;
const double ANGLE_3dB_h = 70 ;
const double ANGLE_3dB_v = 15 ;
const double ANGLE_tilt = 0 ;


enum SCENARIO_TYPE
{
  INDOOR_HOTSPOT,
  URBAN_MACRO,
  URBAN_MICRO,
  RURAL_MACRO,
  SUBURBAN_MACRO,
  OUTDOOR_2_INDOOR
};

enum LOS_TYPE
{
  LOS,
  NLOS,
  RANDOM
};

typedef struct 
{
  double x;
  double y;
} LOCATION;

typedef struct
{
  double PL ;
} LOGGING_POINT ;

#endif

