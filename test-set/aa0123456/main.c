/*
Vassilenko George  ID: 0484740
Math 230
Project II
Artificial Neural Network Node
Last Revision: 10/03/2014
*/
#include <stdio.h>
#include <stdlib.h>
struct sset
{
    char* afile;
    double lRate;
    double threshold;
    int iterations;
    int error;
    int size;
    int seed;
};
typedef struct sset settings;
//----------------------------------(prototypes)---------------------
settings processArgs(int args , char *arg[]);
int g(int j,int i);
int procfile(char afile[] ,int data[]);
void perceptron(settings x,int data[]);
int getlen(char afile[]);
//-----------------------------------(main)--------------------------
int main(int args, char *arg[])
{
    settings x = processArgs(args, arg); //get settings from args
    x.size = getlen(x.afile);
    if (x.size ==-1) return(0);
    int *data = malloc( x.size * sizeof(int));
    procfile(x.afile,data); //get input from file
    perceptron(x,data); //main perceprton code
    free(data);
    return(0);
}
//---------------------------------(get # of records)----------------
int getlen(char afile[])
{
    int sz;
    FILE *fp;
    fp = fopen(afile , "r");
    if(fp == NULL)
    {
        perror("Error opening file");
        return(-1);
    }
    fseek(fp, 0L, SEEK_END);
    sz =(int)((ftell(fp)+1)/19);
    fseek(fp, 0L, SEEK_SET);

    fclose(fp);
    return (sz);
}
//---------------------------------(process file)-------------------
int procfile(char afile[] ,int data[])
{
    int cnt = 0;
    int i;
    int j;
    FILE *fp;
    char str[20];

    fp = fopen(afile , "r");
    if(fp == NULL)
    {
        perror("Error opening file");
        return(-1);
    }
    while( fgets (str, 20, fp)!=NULL )
    {
        j=0;
        for (i=0; i<16; i++)
        {
            j = j|((str[i]=='1')<<i); // encodes bits into an integer
        }
        j = j|((str[17]=='1')<<16); // encodes desired output into bit 16
        data[cnt]= j;
        cnt++;
    }
    fclose(fp);
    return(0);
}
//--------------------------(binary extraction)-----------------------
int g(int j,int i)
{
    return (j&(1<<i))>0;//int and 1 shifted i times
}
//----------------------------(argument processing)-------------------
settings processArgs(int args ,char *arg[])
{
    int i;
    settings x;
    static char* z;
    static char y[] = "C:\\Users\\EVIL\\Desktop\\in.csv";  //at home
    //static char y[] = "C:\\Users\\SWC-Student\\Desktop\\in.csv"; //at school
    x.afile = y;
    x.lRate = .001;
    x.threshold = .01;
    x.iterations = 1000; //default settings
    x.error = 2;
    x.seed=18;
    if (args>1)
    {
        for (i=1; i<args; i++)
        {
            switch (i)
            {
                printf("\n");
            case 1:
                z = arg[i]; //school
                x.afile = z;
                break;//file
            case 2:
                x.lRate=(double)atoi(arg[2])/1000;
                break;//learning rate
            case 3:
                x.error=atoi(arg[3]);
                break;//error
            case 4:
                x.iterations = atoi(arg[4])*1000;
                break;//iterations
            case 5:
                x.threshold = (double)atoi(arg[5])/100;
                break;//threshold
            case 6:
                x.seed= atoi(arg[6]);
                break;//randomization seed
            }
        }
    }
    return x;
}
//------------------------------(perceptron)--------------------------
void perceptron(settings x,int data[])
{
    int i; //record
    int j; //bit
    int k; //iterations
    int min=x.size;
    double sum;//per sensor sum
    int errors;//errors per iteration
    double weights[16];
    double optimal[16];//optimal weights
    srand(x.seed);//randomized starting weights
    for (i=0; i<16; i++) //with help from student
        weights[i] =(double)rand()/(double)RAND_MAX;
//---------------------(main loop)-------------------------------
    for (k=0; k<x.iterations; k++)
    {
        errors=0;
        for (i=0; i< x.size; i++) //loop through records
        {
            sum = 0;
            for (j=0; j<16; j++) //get sum
            {
                sum += weights[j]*g(data[i],j);
            }
            if ((g(data[i],16)-(sum>x.threshold)) !=0) errors++;
            for (j=0; j<16; j++) //update weights
            {
                weights[j] += (g(data[i],16)-(sum>x.threshold)) * x.lRate*g(data[i],j);
            }
        }
        if (errors < min)
        {
            min = errors;
            for (i=0; i<16; i++)
                optimal[i] = weights[i];
        }
        if (errors <= x.error)
            break;
    }
    printf("\n---------------------------------------");
    for (j=0; j<16; j++) //output optimal weights
    {
        if (j%4==0)
            printf("\n");
        printf("%9f ",optimal[j] );
    }
    printf("\n---------------------------------------\n");
    printf("%i/%i %f%% \n\n" ,x.size-min,x.size,100-(100*min/(double)x.size));
}
