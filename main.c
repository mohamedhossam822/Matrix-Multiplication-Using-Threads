#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>


typedef struct
{
    int** Matrix1;
    int** Matrix2;
    int** FMatrix;
    int R;
    int C;
    int M;

}Args;

void *CalculateElement(Args* Matrices);
void *CalculateRow(Args* Matrices);

int main()
{
    clock_t start, end;      //Time variables
    double cpu_time_used;


    int threadcounter=0;
    int NRows1,NColumns1,NRows2,NColumns2,i,j;
    Args Matrices;       //the 3 matrices


    //Reading File

    FILE* Fptr;
    Fptr=fopen("MatrixFile.txt","r");
    if(Fptr!=NULL)
    {
            fscanf(Fptr,"%d %d",&NRows1,&NColumns1);

            Matrices.Matrix1=(int**)malloc(NRows1*sizeof(int*));
            for (i=0; i<NRows1; i++) Matrices.Matrix1[i] =(int*)malloc(NColumns1*sizeof(int));    //Allocating memory for matrix

            for(i=0;i<NRows1;i++)
                {
                    for(j=0;j<NColumns1;j++)
                    {
                    fscanf(Fptr,"%d",&Matrices.Matrix1[i][j]);
                    if(j<(NColumns1-1)) fscanf(Fptr," ");
                    }
                    fscanf(Fptr,"\n");
                }

           fscanf(Fptr,"%d %d",&NRows2,&NColumns2);

            Matrices.Matrix2=(int**)malloc(NRows2*sizeof(int*));
            for (i=0; i<NRows2; i++) Matrices.Matrix2[i] =(int*)malloc(NColumns2*sizeof(int));  //Allocating memory for matrix

            for(i=0;i<NRows2;i++)
                {
                    for(j=0;j<NColumns2;j++)
                    {
                    fscanf(Fptr,"%d",&Matrices.Matrix2[i][j]);
                    if(j<(NColumns2-1)) fscanf(Fptr," ");
                    }
                    fscanf(Fptr,"\n");
                }
    }
    fclose(Fptr);




    Matrices.FMatrix=(int**)malloc(NRows1*sizeof(int*));
    for (i=0; i<NRows1; i++) Matrices.FMatrix[i] =(int*)malloc(NColumns2*sizeof(int));        //Allocating memory for matrix

    for(i=0;i<NRows1;i++) for(j=0;j<NColumns2;j++)  Matrices.FMatrix[i][j]=0; //Initializing ResultMatrix

       pthread_t thread_id[NRows1*NColumns2] ;
       start = clock();
       for(i=0;i<NRows1;i++)
       {
            for(j=0;j<NColumns2;j++)
            {
                Matrices.R=i;
                Matrices.C=j;
                Matrices.M=NColumns1;

            pthread_create(&thread_id[threadcounter], NULL,&CalculateElement,&Matrices);
            pthread_join(thread_id[threadcounter],NULL);
            threadcounter++;
            }
       }
       end = clock();
       cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;

        FILE* FO;
        FO=fopen("Output.txt","w");
        fprintf(FO,"Element By Element Matrix : \n\n");
        for(i=0;i<NRows1;i++)
                {
                    for(j=0;j<NColumns2;j++)
                    {
                    fprintf(FO,"%d ",Matrices.FMatrix[i][j]);
                    }
                    fprintf(FO,"\n");
                }
        fprintf(FO,"Time Elapsed = %f \n\n",cpu_time_used);
        fclose(FO);

    for(i=0;i<NRows1;i++)
    for(j=0;j<NColumns2;j++) Matrices.FMatrix[i][j]=0;

    pthread_t thread_idR[NRows1] ;
    threadcounter=0;
        start=clock();
        for(i=0;i<NRows1;i++)
        {
            Matrices.R=i;
            Matrices.C=NColumns2;
            Matrices.M=NColumns1;
            pthread_create(&thread_idR[threadcounter], NULL,&CalculateRow,&Matrices);
            pthread_join(thread_idR[threadcounter],NULL);
            threadcounter++;
        }
       end = clock();
       cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;

        FO=fopen("Output.txt","a");
        fprintf(FO,"Row By Row Matrix : \n\n");
        for(i=0;i<NRows1;i++)
                {
                    for(j=0;j<NColumns2;j++)
                    {
                    fprintf(FO,"%d ",Matrices.FMatrix[i][j]);
                    }
                    fprintf(FO,"\n\n");
                }
        fprintf(FO,"Time Elapsed = %f \n",cpu_time_used);
        fclose(FO);

  return 0;
}

void * CalculateElement(Args * Matrices)
{
    int k;
    for(k=0;k<(*Matrices).M;k++)
            {
                (*Matrices).FMatrix[(*Matrices).R][(*Matrices).C]=(*Matrices).FMatrix[(*Matrices).R][(*Matrices).C]+((*Matrices).Matrix1[(*Matrices).R][k]*((*Matrices).Matrix2[k])[(*Matrices).C]);

            }
}

void *CalculateRow(Args * Matrices)
{
    int j;
    int k;
    for(j=0;j<(*Matrices).C;j++)
        {
         for(k=0;k<(*Matrices).M;k++)
            {
                (*Matrices).FMatrix[(*Matrices).R][j]=(*Matrices).FMatrix[(*Matrices).R][j]+((*Matrices).Matrix1[(*Matrices).R][k]*((*Matrices).Matrix2[k])[j]);


            }
        }
}

