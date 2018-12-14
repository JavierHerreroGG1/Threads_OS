#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdbool.h>

double counter=0.0;

#define ITER 1000
#define NHILOS 4

bool Eligiendo[NHILOS]={false, false, false, false};
int Numero[NHILOS]={0,0,0,0};


int maximo (int v[]);
void* Proceso(void *i);

int main(){
	pthread_t hilos[NHILOS];
	int status, i, v[NHILOS];
	extern double counter;
	double *r_value;

	for(i=0; i<NHILOS; i++){
		v[i]=i;
		if((status= pthread_create(&hilos[i], NULL, Proceso, (void *) &v[i])))		
			exit(status);
	}

	for(i=0; i<NHILOS; i++){
		pthread_join(hilos[i], (void**) &r_value);										
		printf("Value returned by %lu thread: %lf\n", hilos[i], *r_value);
	}

	fprintf(stdout, "%f\n", counter);

	return 0;
}

void* Proceso(void *i){

	extern bool Eligiendo[NHILOS];
	extern int Numero[NHILOS];

	double l, *to_return;
	extern double counter;
	int *id, j=0;

	id=(int*)i;

	for(int t=0; t<NHILOS; t++){
		Eligiendo[*id]=true;
		Numero[*id]=1+maximo(Numero);
		Eligiendo[*id]=false;

		for(j=0; j<NHILOS; j++){
			while(Eligiendo[j]){}
			while((Numero[j]!=0)&&(Numero[j]<Numero[*id] || Numero[j]==Numero[*id] && j<*id)){}
		}


   for (int p = 0; p < ITER; p++) {
	l = counter;
	fprintf(stdout, "Hilo %d: %f\n", *id, counter);
	l++;
	counter = l;
    }

    to_return = malloc(sizeof(double));			

    *to_return = counter;

	Numero[*id]=0;

    pthread_exit((void *) to_return);
}
}

int maximo (int v[]){
	int max=0, i=4;
	for(int i=0; i<4; i++){
		if(max<v[i])
			max=v[i];
	}
	return max;
}