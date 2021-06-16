#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
unsigned long long int *fibSequence;
void* fib_runner(void* arg)
{
    int *limit_ptr = (int*) arg;
    int limit = *limit_ptr;
    //long long int *fibSequence=malloc(sizeof(int) *limit);


    fibSequence[0] = 0;
    fibSequence[1] = 1;

    for (int i = 2; i < limit; i++)
    {
        fibSequence[i] = fibSequence[i-1] + fibSequence[i-2];
    }


    pthread_exit(0);
}

int main(int argc, char **argv)
{
     int limit;
    printf("Enter Number: ");
    scanf("%d", &limit);
    fibSequence=calloc(limit, sizeof(int));

    pthread_t tid;
    pthread_attr_t attr;
    pthread_attr_init(&attr);

    pthread_create(&tid, &attr, fib_runner, &limit);
    //long long int *fibSequence;
    pthread_join(tid, (void **)&fibSequence);

  for (int i = 0; i < limit; i++)
  {
    printf("%llu ", fibSequence[i]);
  }
printf("\n\n");
free(fibSequence);
}
