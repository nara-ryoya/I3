#include <assert.h>
#include <complex.h>
#include <math.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

typedef short sample_t;

void die(char * s) {
  perror(s);
  exit(1);
}

/* fd から 必ず n バイト読み, bufへ書く.
   n バイト未満でEOFに達したら, 残りは0で埋める.
   fd から読み出されたバイト数を返す */
ssize_t read_n(int fd, ssize_t n, void * buf) {
  ssize_t re = 0;
  while (re < n) {
    ssize_t r = read(fd, buf + re, n - re);
    if (r == -1) die("read");
    if (r == 0) break;
    re += r;
  }
  memset(buf + re, 0, n - re);
  return re;
}

/* fdへ, bufからnバイト書く */
ssize_t write_n(int fd, ssize_t n, void * buf) {
  ssize_t wr = 0;
  while (wr < n) {
    ssize_t w = write(fd, buf + wr, n - wr);
    if (w == -1) die("write");
    wr += w;
  }
  return wr;
}

/* 標本(整数)を複素数へ変換 */
void sample_to_complex(sample_t * s, 
		       complex double * X, 
		       long n) {
  long i;
  for (i = 0; i < n; i++) X[i] = s[i];
}

/* 複素数を標本(整数)へ変換. 虚数部分は無視 */
void complex_to_sample(complex double * X, 
		       sample_t * s, 
		       long n) {
  long i;
  for (i = 0; i < n; i++) {
    s[i] = creal(X[i]);
  }
}

/* 高速(逆)フーリエ変換;
   w は1のn乗根.
   フーリエ変換の場合   偏角 -2 pi / n
   逆フーリエ変換の場合 偏角  2 pi / n
   xが入力でyが出力.
   xも破壊される
 */
void fft_r(complex double * x, 
	   complex double * y, 
	   long n, 
	   complex double w) {
  if (n == 1) { y[0] = x[0]; }
  else {
    complex double W = 1.0; 
    long i;
    for (i = 0; i < n/2; i++) {
      y[i]     =     (x[i] + x[i+n/2]); /* 偶数行 */
      y[i+n/2] = W * (x[i] - x[i+n/2]); /* 奇数行 */
      W *= w;
    }
    fft_r(y,     x,     n/2, w * w);
    fft_r(y+n/2, x+n/2, n/2, w * w);
    for (i = 0; i < n/2; i++) {
      y[2*i]   = x[i];
      y[2*i+1] = x[i+n/2];
    }
  }
}

void fft(complex double * x, 
	 complex double * y, 
	 long n) {
  long i;
  double arg = 2.0 * M_PI / n;
  complex double w = cos(arg) - 1.0j * sin(arg);
  fft_r(x, y, n, w);
  for (i = 0; i < n; i++) y[i] /= n;
}

void ifft(complex double * y, 
	  complex double * x, 
	  long n) {
  double arg = 2.0 * M_PI / n;
  complex double w = cos(arg) + 1.0j * sin(arg);
  fft_r(y, x, n, w);
}

int pow2check(long N) {
  long n = N;
  while (n > 1) {
    if (n % 2) return 0;
    n = n / 2;
  }
  return 1;
}

void print_complex(FILE * wp, 
		   complex double * Y, long n) {
  long i;
  for (i = 0; i < n; i++) {
    fprintf(wp, "%ld %f %f %f %f\n", 
	    i, 
	    creal(Y[i]), cimag(Y[i]),
	    cabs(Y[i]), atan2(cimag(Y[i]), creal(Y[i])));
  }
}

void one_tone(complex double* y, int n) {
    double fs = 44100;
    double T = n / fs;
    int tmp_max_idx = 0;
    for (int i = 0; i < n ; ++i) {
        if (cabs(y[i]) > cabs(y[tmp_max_idx])) {
            tmp_max_idx = i;
        }
    }
    for (int i = 0; i < n; ++i) {
        if (i != tmp_max_idx) {
            y[i] = 0;
        }

        else {
            y[i] *= 10;
        }
    
    }
}

void bandpass(complex double* y, int n, double low_f, double high_f) {
    double fs = 44100;
    double T = n / fs;
    double l_idx = low_f * T;
    double h_idx = high_f * T;
    for (int i = 0;  i < n; ++i) {
        if (i < l_idx || i > h_idx) {
            y[i] = 0;
        }
    }
}

void shift_higher(complex double* y, int n, double shift_width){
  complex double *new_y = calloc(sizeof(complex double), n);
  for (int i = 0; i < n/shift_width && i < n; ++i)
  {
    new_y[(int)(shift_width*i)] = y[i];
  }
  for (int i = 0; i < n; ++i)
  {
    y[i] = new_y[i];
  }
  free(new_y);

}

void shift_higher_2(complex double* y, int n, int shift_width){
  complex double *new_y = calloc(sizeof(complex double), n);
  for (int i = 0; i < n/2; ++i)
  {
    new_y[i + n/2] = y[i] * 10;
  }
  for (int i = 0; i < n/2; ++i)
  {
    y[i] = 0;
  }
  for (int i = n/2; i < n; ++i)
  {
    y[i] = new_y[i];
  }
  free(new_y);

}


int main(int argc, char ** argv) {
  if (argc != 5) {
      perror("line 160");
      return 1;
  }
  double low_f = atof(argv[2]);
  double high_f = atof(argv[3]);
  long n = atoi(argv[1]);
  double shift_width = atof(argv[4]);
  if (!pow2check(n)) {
    fprintf(stderr, "error : n (%ld) not a power of two\n", n);
    exit(1);
  }
  FILE * wp = fopen("fft.dat", "wb");
  if (wp == NULL) die("fopen");
  sample_t * buf = calloc(sizeof(sample_t), n);
  complex double * X = calloc(sizeof(complex double), n);
  complex double * Y = calloc(sizeof(complex double), n);
  while (1) {
    /* 標準入力からn個標本を読む */
    ssize_t m = read_n(0, n * sizeof(sample_t), buf);
    if (m == 0) break;
    /* 複素数の配列に変換 */
    sample_to_complex(buf, X, n);
    /* FFT -> Y */
    fft(X, Y, n);
    
    print_complex(wp, Y, n);
    fprintf(wp, "----------------\n");

    shift_higher(Y, n, shift_width);
    bandpass(Y, n, low_f, high_f);
    // one_tone(Y, n);

    // shift_higher_2(Y, n, 2);

    /* IFFT -> Z */
    ifft(Y, X, n);
    /* 標本の配列に変換 */
    complex_to_sample(X, buf, n);
    /* 標準出力へ出力 */
    write_n(1, m, buf);
  }
  fclose(wp);
  return 0;
}
