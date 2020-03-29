#include<stdio.h>
#include<pthread.h>
#include<stdlib.h>
#include <unistd.h>
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t  finished  = PTHREAD_COND_INITIALIZER;
int barrier = 0, count, size, counter=0, invoke=0;
void init(int new_threads)
{
    if ( count < size ) { barrier = count; return; }
    barrier = new_threads;
}
		
int decrease()
{
    if (barrier == 0) {
		
        return 0;
    }
		
    if(pthread_mutex_lock(&lock) != 0)
    {
        perror("Lock Failed.");
        return -1;
    }
		
    barrier--;
		
    if(pthread_mutex_unlock(&lock) != 0)
    {
        perror("Unlock failed.");
        return -1;
    }

    return 0;
}	
int wait()
{
    if(decrease() < 0)
    {
        return -1;
    }

    while (barrier)
    {
        if(pthread_mutex_lock(&lock) != 0)
        {
            perror("\n Error locking mutex");
            return -1;
        }

        if(pthread_cond_wait(&finished, &lock) != 0)
        {
            perror("\n Error condition wait.");
            return -1;
        }
    }
    if(0 == barrier)
    {
        if(pthread_mutex_unlock(&lock) != 0)
        {
            perror("\n Error locking mutex");
            return -1;
        }
        if(pthread_cond_signal(&finished) != 0)
        {
            perror("\n Error signaling.");
            return -1;
        }
    }

    return 0;
}

void * point(void *num)
{

     int r = rand() % 5;

     printf("\nThread %d \nPerforming init task of length %d sec\n",++counter,r);
     sleep(r);

     wait();
     if (size!=0) {
       if ((count - (invoke++) ) % size == 0) {
         printf("\nBarrier Released\n");
       }
       printf("\ntask after barrier\n");

     }


     return  NULL;
}
int main()
{

    printf("Enter Barrier Size:\t");
    scanf("%d", &size);

    printf("Enter no. of thread:\t");
    scanf("%d", &count);



    if (size>=0 && count>=0) {
        pthread_t tid[count];

        init(size);

        for(int i =0; i < count; i++)
        {
            pthread_create(&(tid[i]), NULL, &point, &count);
        }


        for(int j = 0; j < count; j++)
        {
            pthread_join(tid[j], NULL);
        }
    }

    else{
      printf("wrong data…\n");
      main();
    }

    return 0;
}


