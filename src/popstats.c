/*

  ... | popstats nsam [fcol]

  expects stdin in bed format, with the 5th and 6th columns
  representing the number of ref and alt alleles respectively:
  chrom    pos-1    pos    name    nref    nalt

  nsam - total number of alleles in the population
  fcol - column number (1-indexed) of the factor over which 
  the stats should be calculated. The default is to output 
  stats per chromosome, but the fourth name column could be used 
  instead to calculate over some group of features. [default = 0]
  
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <rwk_parse.h>
#include <rwk_popstats.h>

int main(int argc, char **argv) {

char help[] = "  ... | popstats nsam [fcol]";
 
  int nsam, fcol;

  if (argc == 1) {
    printf("%s\n", help);
  } else if (argc == 2) {
    nsam = atoi(argv[1]);
    fcol = 0;
  } else if (argc == 3) {
    nsam = atoi(argv[1]);
    fcol = atoi(argv[2]) - 1;
  } else {
    printf("%s\n", help);
  }
  
  int ncols = 6;
  int lcols = 1024;
  int lwidth = 2048;
  char delim = '\t';
  char newline = '\n';
  char buffer[lwidth];

  char **array;
  array = calloc(ncols, sizeof (char*));
  
  char chr[lcols];
  char factor[lcols];
  long long int startpos, stoppos;
  long long int start_region, stop_region;

  int s, pi;
  int nref, nalt;
  double tw_val, pi_val;
  
  struct rwkThetaW thetaW;
  struct rwkThetaPi thetaPi;
  
  rwkThetaWInit(&thetaW, nsam);
  rwkThetaPiInit(&thetaPi, nsam);
  
  int coln;
  char *tmp;
  int startindex = 0;
  while (fgets(buffer, sizeof(buffer), stdin)) {
    rwkStrtoArray(array, buffer, &delim);
    
    startpos = atoll(array[1]);
    stoppos = atoll(array[2]);
    nref = atoi(array[4]);
    nalt = atoi(array[5]);

    if (startindex == 0) {
      strcpy(chr, array[0]);
      strcpy(factor, array[fcol]);
      start_region = startpos;
      startindex = 1;
    }
    
    if (strcmp(array[fcol], factor) != 0) {
      tw_val = thetaW.eval(&thetaW);
      pi_val = thetaPi.eval(&thetaPi);
      printf("%s\t%lld\t%lld\t%s\t%d\t%d\t%d\t%lf\t%lf\t%lf\n",
	     chr, start_region, stop_region, factor,
	     thetaW.nsam, thetaW.nsites, thetaW.s,
	     tw_val / thetaW.nsites, pi_val / thetaPi.nsites,
	     rwkTajD(thetaW.nsam, thetaW.s, tw_val, pi_val));
      
      thetaW.reset(&thetaW);
      thetaPi.reset(&thetaPi);
      strcpy(factor, array[fcol]);
      start_region = startpos;
    }
    
    if (nref > 0 && nalt > 0) {
      s = 1;
      pi = nref;
    } else {
      s = 0;
      pi = 0;
    }

    if (nref + nalt == nsam) {
      thetaW.add(&thetaW, s);
      thetaPi.add(&thetaPi, pi);
      //printf("%d %lld %lld %lf\n", pi, thetaPi.pisum, thetaPi.nsites, thetaPi.eval(&thetaPi));
      stop_region = stoppos;
    }
    strcpy(chr, array[0]);
  }

  tw_val = thetaW.eval(&thetaW);
  pi_val = thetaPi.eval(&thetaPi);
  printf("%s\t%lld\t%lld\t%s\t%d\t%d\t%d\t%lf\t%lf\t%lf\n",
	 chr, start_region, stop_region, factor,
	 thetaW.nsam, thetaW.nsites, thetaW.s,
	 tw_val / thetaW.nsites, pi_val / thetaPi.nsites,
	 rwkTajD(thetaW.nsam, thetaW.s, tw_val, pi_val));

  free(array);
  
  return 0; }
