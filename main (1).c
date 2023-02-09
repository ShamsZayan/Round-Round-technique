#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
/*
 * Process: Each process has a name and has an execution time.
 *          We keep track of how long the process still needs
 *          to run on the processor. "remaining_time" is the
 *          remaining execution time of the process.
 */
typedef struct
{
    char * name;
    int starting_time;
    int remaining_time;
} Process;
/*
 *
 */
typedef struct Node
{
    Process data;
    struct Node* next;
} Node;
/*
 *
 */
typedef struct
{
    Node * front, * rear;
} Queue;
/*
 *
 */
Queue* init()
{
    Queue * q = malloc(sizeof(Queue));
    q -> front = NULL;
    q -> rear = NULL;
    return q;
}
/*
 *
 */
int isEmpty(Queue *q)
{
    if(q -> front == NULL)
    {
        return 1;
    }
    return 0;
}
/*
 *
 */
Process dequeue(Queue *q)
{
    if(q -> front == NULL)
    {
        q -> rear = NULL;
        return;
    }
    else
    {
        Process process;
        Node * temp = q -> front;
        process = q -> front -> data;
        q -> front = q -> front -> next;
        free(temp);
        return process;
    }
}
/*
 *
 */
void enqueue(Queue *q, Process x)
{
    Node * node = malloc(sizeof(Node));
    node -> data = x;
    node -> next = NULL;
    if(isEmpty(q))
    {
        q -> front = node;
        q -> rear = node;
    }
    else
    {
        q -> rear -> next = node;
        q -> rear = node;
    }
}
/*
 *
 */
void destroy(Queue *q)
{
    free(q);
}
/*
 * RoundRobin Scheduling
 */
void RoundRobin(char* filename)
{
    int size = 0;
    char c;
    FILE * myfile = fopen(filename, "r");
    if(myfile != NULL)
    {
        for (c = getc(myfile); c != EOF; c = getc(myfile))// checking number of lines in file
        {
            if (c == '\n')
            {
                size ++;
            }
        }
        fclose(myfile);
        Process p[size];
        // first line (getting time slot)
        FILE * myFile = fopen(filename, "r");
        int time_slot = 0;
        char temp [100];
        fgets(temp, 100, myfile);
        char * token1 = strtok(temp, " ");
        while(token1 != NULL)
        {
            if(atoi(token1) != 0)
            {
                time_slot = atoi(token1);
            }
            token1 = strtok(NULL, " ");
        }
        // rest of file (getting the names, starting, and remaining times of each process)
        int i = 0;
        while(!feof(myfile))
        {
            fgets(temp, 100, myfile);
            char * token2 = strtok(temp, " ");
            p[i].name = malloc(50 * sizeof(char));
            strcpy(p[i].name, token2);
            token2 = strtok(NULL, " ");
            p[i].starting_time = atoi(token2);
            token2 = strtok(NULL, " ");
            p[i].remaining_time = atoi(token2);
            i++;
        }

        Queue * q = init();
        int k, j;
        int start = p[0].starting_time;
        for(k = 0; k < time_slot; k++)
        {
            for(j = 0; j < size; j++)
            {
                if(p[j].starting_time == start && start == k)
                {
                    p[j].remaining_time --;
                    enqueue(q,p[j]);
                }
                else if(p[j].starting_time == k  && p[j].starting_time != start)
                {
                    p[j].remaining_time --;
                    enqueue(q, p[j]);
                }
            }
            if(!isEmpty(q))
            {
                Process deq = dequeue(q);
                if(deq.remaining_time > 0)
                {
                    printf("%s\t(%d --> %d)\n", deq.name, k, k + 1);
                    deq.remaining_time --;
                    enqueue(q, deq);
                }
                else if(deq.remaining_time == 0)
                {
                    printf("%s\t(%d --> %d) %s aborts\n", deq.name, k, k + 1, deq.name);
                }
            }
            else
            {
                printf("Idle\t(%d --> %d)\n", k, k + 1);
            }
        }
        printf("Stop\n");
        destroy(q);
        fclose(myFile);
    }
}
/*
 *
 */
int main()
{
    char filename[261];
    puts("Enter file name or Ctrl+Z to exit:");
    puts("----------------------------------");
    while(fgets(filename, 260, stdin) != NULL)
    {
        filename[strlen(filename)-1]='\0';
        if(fopen(filename,"r"))
            RoundRobin(filename);
        else
        {
            puts("File Not Found!");
            puts("----------------------------------");
        }
        puts("Enter file name or Ctrl+Z to exit:");
        puts("----------------------------------");
    }

    return 0;
}
