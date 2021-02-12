#include <stddef.h>
#ifdef __cplusplus
#include <complex>
#define __GFORTRAN_FLOAT_COMPLEX std::complex<float>
#define __GFORTRAN_DOUBLE_COMPLEX std::complex<double>
#define __GFORTRAN_LONG_DOUBLE_COMPLEX std::complex<long double>
extern "C" {
#else
#define __GFORTRAN_FLOAT_COMPLEX float _Complex
#define __GFORTRAN_DOUBLE_COMPLEX double _Complex
#define __GFORTRAN_LONG_DOUBLE_COMPLEX long double _Complex
#endif

void cubicroots (const double *a, __GFORTRAN_DOUBLE_COMPLEX *z);
void quadraticroots (const double *a, __GFORTRAN_DOUBLE_COMPLEX *z);
void quarticroots (const double *a, __GFORTRAN_DOUBLE_COMPLEX *z);
void solvepolynomial (const double *quarticcoeff, const double *cubiccoeff, const double *quadraticcoeff, const double *linearcoeff, const double *constantcoeff, int *code, __GFORTRAN_DOUBLE_COMPLEX *root1, __GFORTRAN_DOUBLE_COMPLEX *root2, __GFORTRAN_DOUBLE_COMPLEX *root3, __GFORTRAN_DOUBLE_COMPLEX *root4);

#ifdef __cplusplus
}
#endif
