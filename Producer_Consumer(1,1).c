#include <semaphore.h>
#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdbool.h>

#define tamano 5

int pos_productor=0;
int pos_consumidor=0;

int buffer[tamano];

int sumaproductor = 0;
int sumaconsumidor = 0;

sem_t SC;
sem_t consumidor;
sem_t productor;

void *consumir(void *p);
void *producir(void *p);

int main(){
	srand(time(NULL));
	sem_init(&SC,0,1);
	sem_init(&consumidor,0,0);
	sem_init(&productor, 0, tamano);

	pthread_t hebras[2];
	int status;

			pthread_create(&hebras[0], NULL, producir, NULL);

			pthread_create(&hebras[1], NULL, consumir, NULL);


 	for (int i = 0; i <2 ; i++) {
		pthread_join(hebras[i], NULL);
    }
     
    printf("Suma del productor %i\n", sumaproductor);
    printf("Suma del consumidor %i\n", sumaconsumidor);
    return 0;
}

void *producir(void *p){
	extern int pos_productor;
	extern int sumaproductor;
	int k, encontrado=0;
	
	for( unsigned long i = 0 ; i <800 ; i++ ){

		int dato=1+rand()%1000;

		sem_wait(&productor);
		sem_wait(&SC);

		buffer[pos_productor%tamano]=dato;
		/*printf("BUFFER P\n");
		for(int j=0; j<5; j++){
			printf("[%d]", buffer[j]);
		}
		printf("\n");*/

		sem_post(&SC);
		sem_post(&consumidor);

		sumaproductor+=dato;
		pos_productor++;


	}

  	pthread_exit(NULL);	
}


void *consumir(void *p){
	extern int pos_consumidor;
	extern int sumaconsumidor;
	int aux, k, encontrado=0;

	for(int i = 0 ; i < 800 ; i++ ){
	
		sem_wait(&consumidor);
		sem_wait(&SC);

		aux=buffer[pos_consumidor%tamano];
		buffer[pos_consumidor%tamano]=0;
	/*
		printf("BUFFER C\n");
		for(int j=0; j<5; j++){
			printf("[%d]", buffer[j]);
		}
		printf("\n");*/

		sem_post(&SC);
		sem_post(&productor);

		sumaconsumidor+=aux;
		pos_consumidor++;
	}

	pthread_exit(NULL);	
}

