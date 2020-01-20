#include "const.h"
#include <complex>
#include <vector>
#include <iostream>
#include <fstream>

using namespace std;

class UE
{
  public:
  UE();
  ~UE();

  void    Configure( int );

  int     sector_in_control ;
  int     * adj_sector ;
  LOCATION coordinate;

  vector  <double> _multi_path_fading_mag ;

  double  *  static_gain ;
  double  *  LS_gain ;
  double  *  PathLoss ;
  double  *  Distance ;
  

private:
  
  int     _self_idx ;
};

