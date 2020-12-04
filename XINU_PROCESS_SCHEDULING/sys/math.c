#include <stdio.h>
#include <math.h>
#include <kernel.h>

double pow(double x, int y){
		
    double x_pow_y = 1.0;
	
    if(y==0)
	return 1;
	
    while(y){
	if(y%2){
	    x_pow_y = x_pow_y *x;
	    y = y - 1;
	}
	else{
	    x = x * x;
	    y = y/2;
	}
    }
    return x_pow_y;
	 
}

/* Taylor series for ln(x) centered at a = 1 is given by
 * (x-1) - (x-1)^2/2 + (x-1)^3/3 - (x-1)^4/4 ... = (-1)^(k+1) * (x-1)^k/k 
 * = (-1) * [(1-x)^k/k]
 * */
double log(double x){
    
    if (x > 0){
        int i;
	double logx = 0;

	for(i=1; i<=20; i++)
	   logx = logx + (pow((1-x),i)/i) ;

        logx = logx * (-1);

	return logx;   
    }
    else{
	return (SYSERR);
    }   
    		
}

double expdev(double lambda) {
    double dummy;
    do
        dummy= (double) rand() / RAND_MAX;
    while (dummy == 0.0);
    return -log(dummy) / lambda;
}
