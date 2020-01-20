/*********************************************************************

This random number generator is from

William H. Press, Saul A. Teukolsky, William T. Vetterling and Brian P. Flannery, 
Numerical recipes: the art of scientific computing, Third Edition, 
Cambridge University Press, 2007.

***********************************************************************/

#include <cmath>
#include <complex>

struct Rand 
{
    unsigned long long int p,q,r;
    Rand(unsigned long long int seed) : q(4101842887655102017LL), r(1) 
    {
      p = seed ^ q; int64();
      q = p; int64();
      r = q; int64();
    }
    
    inline unsigned long long int int64() 
    {
      p = p * 2862933555777941757LL + 7046029254386353087LL;
      q ^= q >> 17; q ^= q << 31; q ^= q >> 8;
      r = 4294957665U*(r & 0xffffffff) + (r >> 32);
      unsigned long long int x = p ^ (p << 21); x ^= x >> 35; x ^= x << 4;
      return (x + q) ^ r;
    }
  
    inline double u() { return 5.42101086242752217E-20 * int64(); }
    inline unsigned int i() { return (unsigned int) int64(); }
    inline double n() 
    {
      double nu,nv,nx,ny,nz;
      do
      {
        nu = u();
        nv = 1.7156*(u()-0.5);
        nx = nu - 0.449871;
        ny = fabs(nv) + 0.386595;
        nz = nx*nx + ny*(0.19600*ny-0.25472*nx);
      }
      while (nz > 0.27597 && (nz > 0.27846 || nv*nv > -4.*log(nu)*nu*nu));
      return nv/nu;
    }
};

