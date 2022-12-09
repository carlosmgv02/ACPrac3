#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <math.h>
#include <assert.h>

#define N 50000

int nn;
double *D[N],*apD,*X, *Y, *Z;


int main(int np, char*p[])
{   FILE *fp = fopen("trazaNormal.txt", "w");
    clock_t start_t, end_t;
    double total_t;

    int i,j,rr;
    long long sD;
    
    assert(np==2);

    nn = atoi(p[1]);
    
    assert(nn<=N);
    srand(1);

    printf("Dimensio dades =~ %g Mbytes\n",((double)(nn*(nn+2))*sizeof(double))/(1024*1024));

    //creacio matrius i vectors
    apD = calloc(nn*nn,sizeof(double)); assert (apD);
    D[0] = apD;

    start_t = clock();
    for (i=0;i<nn;i++) {
        D[i+1] = D[i] + nn;
    }
    end_t = clock();   
    total_t = (double)(end_t - start_t) / CLOCKS_PER_SEC;
    printf("TEMPS BUCLE 1: %f\n", total_t  );

    X = calloc(nn,sizeof(double)); assert (X);
    Y = calloc(nn,sizeof(double)); assert (Y);
    Z = calloc(nn,sizeof(double)); assert (Z);

    // Inicialitzacio
    rr = rand();
    start_t = clock();
    for (i=0;i<nn;i++) {
        X[i]=(rr*i)%100 - 49.0;
        Y[i]=(rr*2*i)%100 - 49.0;
        Z[i]=(rr*3*i)%100 - 49.0;
        //printf("%lg, %lg, %lg \n",X[i],Y[i],Z[i]);
    }
    end_t = clock();   
    total_t = (double)(end_t - start_t) / CLOCKS_PER_SEC;
    printf("TEMPS BUCLE 2: %f\n", total_t  );

    // calcul de distancies
    start_t = clock();
    for (i=0;i<nn;i++) {
        for (j=0;j<nn;j++) {
            /*fprintf(fp,"D[%d][%d]\n",D[i][j],i,j);
            fprintf(fp,"X[%d]= %f, X[%d]= %f\n",i,j,X[i],X[j]);
            fprintf(fp,"Y[%d]= %f, Y[%d]= %f\n",i,j,Y[i],Y[j]); 
            fprintf(fp,"Y[%d]= %f, Y[%d]= %f\n",i,j,Y[i],Y[j]);
            */
            D[i][j] = sqrt(pow((X[i] - X[j]),2)
                           + pow((Y[i] - Y[j]),2)
                           + pow((Z[i] - Z[j]),2));
            //fprintf(fp,"D[%d][%d]= %f\n",i,j,D[i][j]);
        }
    }
    end_t = clock();   
    total_t = (double)(end_t - start_t) / CLOCKS_PER_SEC;
    printf("TEMPS BUCLE 3: %f\n", total_t  );

    // comprovacio
    sD = 0;
    start_t = clock();
    for (i=0;i<nn;i++) {
        
        for (j=i+1;j<nn;j++) {
            //printf("%lg ",D[i][j]);
            sD += (long long) (D[i][j]);
            if (D[i][j] != D[j][i]) {
                printf("diff in %d,%d: %g != %g",i,j,D[i][j],D[j][i]);
                abort();
            }
        }
        //printf("\n");
    }
    end_t = clock();   
    total_t = (double)(end_t - start_t) / CLOCKS_PER_SEC;
    printf("TEMPS BUCLE 4: %f\n", total_t  );

    printf("Suma elements de D: %lld \n",sD);
    fclose(fp);
    exit(0);
}


