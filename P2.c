#include<stdio.h>
#include<stdlib.h>
#include<semaphore.h>
#include<pthread.h>
int ny,nl,n;
int flagy[10000]={0},flagl[10000]={0};
sem_t buf_limit,seml,semy;
sem_t sl;
sem_t sy;
int nwaity=0,nwaitl=0;
void *Yenter(void *ii);
void *Lenter(void *ii);
pthread_mutex_t mutex;
int main()
{
	//initialsem(sl,0);
	//initialsem(sy,0);
	printf("Number of Yorks:");
	scanf("%d",&ny);
	printf("\nNumber of Lanncasters:");
	scanf("%d",&nl);
	printf("\nInn size:");
	scanf("%d",&n);
	printf("\n");
	pthread_t y[ny],l[nl];
	sem_init(&buf_limit,0,n);
	sem_init(&seml,0,0);
	sem_init(&semy,0,0);
	//sem_init(&sl,0,0);
	//sem_init(&sy,0,0);
	int i,a[ny],b[nl];
	for(i=0;i<ny;i++)
	{
		a[i]=i;
	}
	for(i=0;i<nl;i++)
	{
		b[i]=i;
	}
	for(i=0;i<ny;i++)
	{
	//	pthread_create(&l[i],NULL,&Lenter,&b[i]);
		pthread_create(&y[i],NULL,&Yenter,&a[i]);
	}
	for(i=0;i<nl;i++)
	{
		pthread_create(&l[i],NULL,&Lenter,&b[i]);
	}
	printf("threads created\n");
	for(i=0;i<ny;i++)
	{
		pthread_join(y[i],NULL);
	}
	for(i=0;i<nl;i++)
	{
		pthread_join(l[i],NULL);
	}
	return 0;
}
void *Yenter(void *ii)
{	
	int i = *((int *)ii);
	pthread_mutex_lock(&mutex);
	int f;
	for(f=0;f<nl;f++)
	{
		if(flagl[f]==1)
			break;
	}
	if(f!=nl)
	{
		printf("York %d waiting\n",i+1);
		nwaity++;
		//pthread_mutex_unlock(&mutex);
		
		//printf("yo1\n");
		sem_wait(&seml);
		pthread_mutex_unlock(&mutex);
		//wait(sl);
		//printf("lo1\n");
		
		//wait(sl);
	}
	else
	{
		flagy[i]=1;
		pthread_mutex_unlock(&mutex);
	}	
	sem_wait(&buf_limit);
	pthread_mutex_lock(&mutex);
	printf("York %d entered the Inn\n",i+1);
	flagy[i]=1;
	pthread_mutex_unlock(&mutex);
	sleep(2);
	pthread_mutex_lock(&mutex);
	printf("York %d left the Inn\n",i+1);
	flagy[i]=0;
	//pthread_mutex_unlock(&mutex);
	
	sem_post(&buf_limit);
	//flagy[i]=0;
	
	for(f=0;f<ny;f++)
	{
		if(flagy[f]==1)
			break;
	}
	if(f==ny)
	{
		//pthread_mutex_lock(&mutex);
		//printf("freed l\n");
		int o;
		for(o=0;o<nwaitl;o++)
		{
		sem_post(&semy);
		}
		nwaitl=0;
		
		//signal(sy);
		//pthread_mutex_unlock(&mutex);
	}
	pthread_mutex_unlock(&mutex);
	
}
void *Lenter(void *ii)
{
	int i = *((int *)ii);
	pthread_mutex_lock(&mutex);
	int f;
	for(f=0;f<ny;f++)
	{
		if(flagy[f]==1)
			break;
	}
	if(f!=ny)
	{
		printf("Lanncaster %d waiting\n",i+1);
		nwaitl++;
		pthread_mutex_unlock(&mutex);
		//wait(sy);
		//printf("yo\n");
		sem_wait(&semy);
		pthread_mutex_unlock(&mutex);
		//wait(sy);
		//printf("lo\n");
	}
	else
	{
		flagl[i]=1;
		pthread_mutex_unlock(&mutex);
	}
	sem_wait(&buf_limit);
	pthread_mutex_lock(&mutex);
	printf("Lanncaster %d entered the Inn\n",i+1);
	flagl[i]=1;
	pthread_mutex_unlock(&mutex);
	sleep(2);
	pthread_mutex_lock(&mutex);
	printf("Lanncaster %d left the Inn\n",i+1);
	flagl[i]=0;
	sem_post(&buf_limit);
	for(f=0;f<nl;f++)
	{
		if(flagl[f]==1)
			break;
	}
	if(f==nl)
	{	
		
		//pthread_mutex_lock(&mutex);
		//signal(sl);
		sem_post(&seml);
		//printf("freed y\n");
		int o;
		for(o=0;o<nwaity;o++)
		{
		sem_post(&seml);
		}
		nwaity=0;
		//pthread_mutex_unlock(&mutex);
	}
	pthread_mutex_unlock(&mutex);
}

