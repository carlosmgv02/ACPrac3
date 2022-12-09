#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <math.h>
#include <assert.h>
#include <pthread.h>

#define N 50000
#define MAX_THREADS

int nn;
int sD=0;
int nThreads;
FILE *fp;

double *D[N],*apD,*X, *Y, *Z;

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER; 


void * calc_distancies(void* thread_id){  //array[2] = {inicial, final}
    int n_thread=(int)thread_id;
    int i,j,rang;
    rang=N/nThreads;
    int rang_ini=rang * n_thread;
    int rang_fin=rang * (n_thread + 1);
    
    if(n_thread==nThreads)
        rang_fin=N;
        
    
    for (i=rang_ini;i<rang_fin;i++) {
        for (j=0;j<nn;j++) {

            pthread_mutex_lock(&mutex);
            D[i][j] = sqrt(pow((X[i] - X[j]),2)
                           + pow((Y[i] - Y[j]),2)
                           + pow((Z[i] - Z[j]),2));
            pthread_mutex_unlock(&mutex);
        }
    }
    pthread_exit(NULL);
    
}

void *comprovacio(void* thread_id){
    int i,j, rang;
    int n_thread=(int)thread_id;
    rang=N/nThreads;
    int rang_ini=rang * n_thread;
    int rang_fin=rang * (n_thread + 1);

    if(n_thread==nThreads-1){
        rang_fin=N;
    }
    
    sD = 0;
    for (i=rang_ini;i<rang_fin;i++) {
        for (j=i+1;j<nn;j++) {
            //printf("%lg ",D[i][j]);
            pthread_mutex_lock(&mutex);
            sD += (long long) (D[i][j]);
            pthread_mutex_unlock(&mutex);
            if (D[i][j] != D[j][i]) {
                printf("diff in %d,%d: %g != %g",i,j,D[i][j],D[j][i]);
                abort();
            }
        }
        //printf("\n");
    }
    pthread_exit(NULL);
}
int main(int np, char*p[])  
{
    int i,j,rr;
    long long sD;
    
    assert(np==3);
    
    nn = atoi(p[1]);
    nThreads=atoi(p[2]);
    
    
    if(nThreads < 2)
        nThreads = 2;
    
    if(nThreads > 256) 
        nThreads = 256;
    pthread_t tid[nThreads];
    
    assert(nn<=N);
    srand(1);

    printf("Dimensio dades =~ %g Mbytes\n",((double)(nn*(nn+2))*sizeof(double))/(1024*1024));

    //creacio matrius i vectors
    apD = calloc(nn*nn,sizeof(double)); assert (apD);
    D[0] = apD;
    for (i=0;i<nn;i++) {
        D[i+1] = D[i] + nn;
    }
    X = calloc(nn,sizeof(double)); assert (X);
    Y = calloc(nn,sizeof(double)); assert (Y);
    Z = calloc(nn,sizeof(double)); assert (Z);

    // Inicialitzacio
    rr = rand();
    for (i=0;i<nn;i++) {
        X[i]=(rr*i)%100 - 49.0;
        Y[i]=(rr*2*i)%100 - 49.0;
        Z[i]=(rr*3*i)%100 - 49.0;
        //printf("%lg, %lg, %lg \n",X[i],Y[i],Z[i]);
    }
    

    for(i=0; i<nThreads; i++){
        printf("Creant thread %d\n", i+1);
        pthread_create(&tid[i],NULL,calc_distancies,(void *)i);
    }

     //Esperem a que acabin els threads
    for(i=0; i<nThreads; i++){
        printf("Acabant thread: %d \n", i+1);
        pthread_join(tid[i],NULL);
    }

    for(i=0; i<nThreads; i++){
        printf("Creant thread %d\n", i+1);
        pthread_create(&tid[i],NULL,comprovacio,(void *)i);  
    }

     //Esperem a que acabin els threads
    for(i=0; i<nThreads; i++){
        printf("Ending thread: %d \n", i+1);
        pthread_join(tid[i],NULL);
    }


    printf("Suma elements de D: %lld \n",sD);

    exit(0);
}
