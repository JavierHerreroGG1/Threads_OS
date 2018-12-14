#include <semaphore.h>
#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdbool.h>

#define tamano 5
#define nconsumidores 2
#define nproductores 5

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
	int j;

	pthread_t Productores[nproductores],Consumidores[nconsumidores];
	int status;

	for(j=0; j<nproductores; j++){
		pthread_create(&Productores[j], NULL, producir, NULL);
	}

	for(j=0; j<nconsumidores; j++){
		pthread_create(&Consumidores[j], NULL, consumir, NULL);
	}


  for (int i = 0; i <(nproductores) ; i++) {
		pthread_join(Productores[i], NULL);
    }

      for (int i = 0; i <(nconsumidores) ; i++) {
		pthread_join(Consumidores[i], NULL);
    }
     
    printf("Suma del productor %i\n", sumaproductor);
    printf("Suma del consumidor %i\n", sumaconsumidor);
    return 0;
}

void *producir(void *p){
	extern int pos_productor;

	for( unsigned long i = 0 ; i <100*nconsumidores ; i++ ){
	
		int dato=rand()%1001;

		sem_wait(&productor);
		sem_wait(&SC);

		buffer[pos_productor]=dato;
		sumaproductor+=dato;
		pos_productor=(pos_productor+1)%tamano;

		/*printf("BUFFER P\n");
		for(int j=0; j<5; j++){
			printf("[%d]", buffer[j]);
		}
		printf("\n");*/

		sem_post(&SC);
		sem_post(&consumidor);

	}

 	 pthread_exit(NULL);	
}


void *consumir(void *p){
	extern int pos_consumidor;
	int aux=0;

	for( unsigned long i = 0 ; i <100*nproductores ; i++ ){

		sem_wait(&consumidor);
		sem_wait(&SC);

		aux=buffer[pos_consumidor];
		buffer[pos_consumidor]=0;

		/*printf("BUFFER C\n");
		for(int j=0; j<5; j++){
			printf("[%d]", buffer[j]);
		}
		printf("\n");*/

		sumaconsumidor+=aux;
		pos_consumidor=(pos_consumidor+1)%tamano;
		sem_post(&SC);
		sem_post(&productor);


	}

	pthread_exit(NULL);	
}

