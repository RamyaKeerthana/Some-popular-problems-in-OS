#include<stdio.h>
#include<stdlib.h>
#include<semaphore.h>
#include<pthread.h>
int N;
pthread_mutex_t mutex;
struct bufitem
{
	int value;
	int readf[5];
	sem_t var_stat;
	int status;
};
struct bufitem buf[5];
int wp=0;
int rp[5]={0,0,0,0,0};
void *write();
void *read(void *);


int main()
{
		N=5;
		int o;
		for(o=0;o<N;o++)
		{
		sem_init(&(buf[o].var_stat),0,0);
		buf[o].status=0;
		int p;
		for(p=0;p<5;p++)
			buf[o].readf[p]=0;
		//sem_init(&(buf[o].var_stat),0,0);
		}
		pthread_t writer;
		pthread_t reader[5];
		pthread_create(&writer,NULL,&write,NULL);
		int i;
		int a[5]={1,2,3,4,5};
		for(i=0;i<N;i++)
		{
			//int i_new=i;
			pthread_create(&reader[i],NULL,&read,&a[i]);
		}
		printf("threads created\n");

		pthread_join(writer,NULL);
		for(i=0;i<N;i++)
		{
			//printf("hi\n");
			pthread_join(reader[i],NULL);
		}
		return 0;
}

void *write()
{
	
	while(1)
	{
		int data;
		data=rand()%100;
		int y=wp%5;
		if(buf[y].status==0)//if empty
		{
			buf[y].value=data;
			printf("wrote %d to the buffer at %d(%d)th position\n",data,y+1,wp+1);
			wp++;
			buf[y].status=1;//not empty
			sleep(2);
		}
		else
		{
			
		//	buf[y].varstat==0;
			sem_wait(&buf[y].var_stat); //enters if full read
			pthread_mutex_lock(&mutex);
			buf[y].value=data;
			printf("overwrote %d to the buffer at %d(%d)th position\n",data,y+1,wp+1);
		//	pthread_mutex_lock(&mutex);
			sleep(7);
			wp=wp++;
			buf[y].readf[0]=0;
			buf[y].readf[1]=0;
			buf[y].readf[2]=0;
			buf[y].readf[3]=0;
			buf[y].readf[4]=0;
			pthread_mutex_unlock(&mutex);
			//sleep(3);
		        //pthread_mutex_unlock(&mutex);
			//sem_post(&buf[y].var_stat);//changes to not full read
			
		}
	}
}
void *read(void *ii)
{
	int i = *((int *)ii)-1,flag[5]={0,0,0,0,0};
	while(1)
	{
		int x=rp[i]%5;
		if(buf[x].status!=0)
		{
			
			//if(i==3)
			//{
			if(buf[x].readf[i]!=1)
			{
				printf("reader%d read %d at %dth position\n",i+1,buf[x].value,x+1,rp[i]+1);
				//buf[x].nofreads++;
				//pthread_mutex_lock(&mutex);
				//rp[i]=rp[i]+1;
				//pthread_mutex_unlock(&mutex);
				sleep(7);
				buf[x].readf[i]=1;
				flag[x]=1;
			}
	//	}
			rp[i]=rp[i]+1;
			int t;
			for(t=0;t<5;t++)
			{
				if(buf[x].readf[t]==0)
				break;
			}
			if(t==5 && i==1 && flag[x])
			{	
				//printf("df %d\n\n",x);
				//buf[x].varstat=1;
				sem_post(&buf[x].var_stat);
				flag[x]=0;
			}
		}
		// pthread_mutex_unlock(&mutex);
		
	}
}
