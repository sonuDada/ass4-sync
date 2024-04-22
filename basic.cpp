#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
sem_t r, w;
int reader_count = 0;
pthread_t writer_threads[100], reader_threads[100];
void *reader(void *t)
{
    sem_wait(&r);
    reader_count++;
    if (reader_count == 1)
        sem_wait(&w);
    sem_post(&r);
    printf("%d reader is inside \n", reader_count);
    usleep(3);
    sem_wait(&r);
    reader_count--;
    if (reader_count == 0)
        sem_post(&w);
    sem_post(&r);
    printf("%d Reader is exiting\n", reader_count + 1);
    return NULL;
}
void *writer(void *t)
{
    printf("Writer is waiting\n");
    sem_wait(&w);
    printf("Writer is entered\n");
    sem_post(&w);
    printf("writer is exiting\n");
    return NULL;
}
int main()
{
    int n, i;
    printf("Enter number of readers : ");
    scanf("%d", &n);
    sem_init(&r, 0, 1);
    sem_init(&w, 0, 1);
    for (i = 0; i < n; i++)
    {
        pthread_create(&writer_threads[i], NULL, reader, NULL);
        pthread_create(&reader_threads[i], NULL, writer, NULL);
    }
    for (i = 0; i < n; i++)
    {
        pthread_join(writer_threads[i], NULL);
        pthread_join(reader_threads[i], NULL);
    }
    return 0;
}
