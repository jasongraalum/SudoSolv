// Copyright (c) 2019 Jason Graalum,
//
// CS 531 Performance Analysis
// Portland State University
// Winter 2019
//
// Jason Graalum
// March 10, 2019
//

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <mpi.h>

int main(int argc, char ** argv)
{

    int numProcs, procId;
    char hostname[MPI_MAX_PROCESSOR_NAME];
    int hostname_len;
    int destId, srcId;

    double start_recv, end_recv, start_send, end_send;

    int payload_size = atoi(argv[1]);
    int msg_count = atoi(argv[2]);
    int block;
    if(argc > 2)
        block = atoi(argv[3]);
    else
        block = 1;

    int *payload = (int *) malloc(sizeof(int)*payload_size);

    int loop_cnt = 0;

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &numProcs);
    MPI_Comm_rank(MPI_COMM_WORLD, &procId);

    MPI_Get_processor_name(hostname, &hostname_len);

    destId = (procId == (numProcs-1)) ? 0 : procId+1;
    srcId = (procId == 0) ? numProcs - 1 : procId-1;
    printf("procId:%d\thostname: %s\tsrcId:%d\tdestId:%d\n",procId, hostname, destId, srcId);
    
    payload[0] = msg_count;

    MPI_Request req = MPI_REQUEST_NULL;
    if(procId == 0) {
        // First to send
        // Send to 1
        if(block == 1)
            MPI_Send(payload, payload_size, MPI_INT, destId, 0, MPI_COMM_WORLD);
        else
            MPI_Isend(payload, payload_size, MPI_INT, destId, 0, MPI_COMM_WORLD, &req);
    }

    do {
        // MPI Receive

        start_recv = MPI_Wtime();
        MPI_Recv(payload, payload_size, MPI_INT, srcId, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        end_recv = MPI_Wtime();
        printf("R, %d, %d, %lf\n", procId, srcId, end_recv-start_recv);

        //printf("%d\n", payload[0]);
        payload[0]--;
        loop_cnt++;

        // MPI Send to
        if(payload[0] > 0)
        {
            start_send = MPI_Wtime();
            if(block == 1)
                MPI_Send(payload, payload_size, MPI_INT, destId, 0, MPI_COMM_WORLD);
            else
                MPI_Isend(payload, payload_size, MPI_INT, destId, 0, MPI_COMM_WORLD, &req);
            end_send = MPI_Wtime();
            printf("S, %d, %d, %lf\n", procId, destId, end_send-start_send);
        }
    } while(payload[0] >= numProcs);

    MPI_Barrier(MPI_COMM_WORLD);
    //printf("%d\n", procId);
    
    MPI_Finalize();
    return(0);
}
