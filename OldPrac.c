/*
        VERSIO 2  - Crear i destruir threads per a cada part
*/


#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <strings.h>
#include <assert.h>
#include <pthread.h>

#define N (4000L) 
#define ND (N*N/100)
#define MAX_THREADS 128    


int num_threads;

int A[N][N],B[N][N],C[N][N],CD[N][N];
struct {
    int i,j,v;
}AD[ND];

struct{
    long long SumaElems;
    int nElems;
}Suma[MAX_THREADS];

//funcio que executara cada thread 
void *funcio_thread(void* Tnum_thread){
    int i,k, block;
    int num_thread = (int) Tnum_thread;

    //Calculem el block de dades a calcular per la matriu
    block = N/num_threads; 

    //Calculem la posicio en la que ha de començar cada thread
    int block_init = block * num_thread; 
    //Calculem fins on ha de calcular el thread
    int block_end = block * (num_thread + 1);

    //Si es l'ultim thread ajustem el block a calcular
    if(num_thread == num_threads-1)
        block_end = N;        

    for(i=block_init;i<block_end;i++)
        for (k=0;k<ND;k++)
            CD[AD[k].i][i] += AD[k].v * B[AD[k].j][i];
        
    
    pthread_exit(NULL);
}

void *funcio_thread2(void* Tnum_thread){
    // Comprovacio MD x M -> M i MD x MD -> M
    // PARAL.LELITZACIO OPCIONAL
    int i,j, block;
    int num_thread = (int) Tnum_thread;

    //Calculem el block de dades a calcular per la matriu
    block = N/num_threads; 

    //Calculem la posicio en la que ha de començar cada thread
    int block_init = block * num_thread; 
    //
    int block_end = block * (num_thread + 1);
    //Si es l'ultim thread ajustem el block     a calcular
    if(num_thread == num_threads-1)
        block_end = N; 


    for (i=block_init;i<block_end;i++)
        for(j=0;j<N;j++)
         {
             Suma[num_thread].SumaElems += CD[j][i];
             if (CD[j][i])  Suma[num_thread].nElems++;
         }
    pthread_exit(NULL);
}


int main(int n_args, const char *ll_args[])
{
    // Ajustem els paràmetres d'entrada
    if (n_args != 2)
    {
        fprintf(stderr, "Arguments invàlids. Utilització: ./MDxM_ac num_threads\n");
        fprintf(stderr, "On num_threads es el num de threads a utilitzar entre 2 i 128.\n");
    }

    num_threads = atoi(ll_args[1]);
    if(num_threads < 2) num_threads = 2;
    if(num_threads > 128) num_threads = 128;

    int i,j,k;
    time_t t1, t2;
    pthread_t threads[num_threads];
    long long sumaTotal = 0;
    int elemsTotal = 0;

     for(i=0; i<MAX_THREADS; i++){
        Suma[i].SumaElems = 0;
        Suma[i].nElems = 0;
    }



    bzero(A,sizeof(int)*(N*N));
    bzero(C,sizeof(int)*(N*N));
    bzero(CD,sizeof(int)*(N*N));

    for (i=0;i<N;i++)
        for (j=0;j<N;j++)
            B[i][j] = rand()%1000;

    for(k=0;k<ND;k++)
    {
        AD[k].i=rand()%(N-1);
        AD[k].j=rand()%(N-1);
        AD[k].v=rand()%100;
        while (A[AD[k].i][AD[k].j]) {
            if(AD[k].i < AD[k].j)
                AD[k].i = (AD[k].i + 1)%N;
            else
                AD[k].j = (AD[k].j + 1)%N;
        }
        A[AD[k].i][AD[k].j] = AD[k].v;
    }
    clock_t init = clock();
    //Li pasem el block_init que ha de calcular
    for(i=0; i<num_threads; i++){
        printf("Creant thread %d\n", i+1);
        pthread_create(&threads[i],NULL,funcio_thread,(void *)i);  
    }

     //Esperem a que acabin els threads
    for(i=0; i<num_threads; i++){
        printf("Ending thread: %d \n", i+1);
        pthread_join(threads[i],NULL);
    }

    printf("PARALELITZACIO SUMA:\n");
    for(i=0; i<num_threads; i++){
        printf("Creant thread %d\n", i+1);
        pthread_create(&threads[i],NULL,funcio_thread2,(void *)i);  
    }

     //Esperem a que acabin els threads
    for(i=0; i<num_threads; i++){
        printf("Ending thread: %d \n", i+1);
        pthread_join(threads[i],NULL);
    }

    for(i=0; i<num_threads; i++){
        sumaTotal += Suma[i].SumaElems;
        elemsTotal += Suma[i].nElems;
    }


    ////Matriu x matriu original (recorregut de C per columnes)
    //for (i=0;i<N;i++)
    //    for (j=0;j<N;j++)
    //        for (k=0;k<N;k++)
    //            C[j][i] += A[j][k] * B[k][i];
    
    clock_t finish = clock();
    double  total = (finish - init) / CLOCKS_PER_SEC;

    printf ("\nNumero elements de la matriu dispersa C %d\n",elemsTotal);
    printf("Suma dels elements de C %lld \n",sumaTotal);
    printf("Temps d'execucio: %f\n",total);
    printf("N: \n", N);
    
}

