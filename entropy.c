
#include <stdio.h>
#include <math.h>


#define READ_BLOCK_SIZE 4096


//frequency of each byte value
int f[256];
int total = 0;





double p(unsigned char b)
{
    if(total == 0) return 0.0;
    else           return (double) f[b] / (double) total;
}


void process_file(FILE* file)
{
    unsigned char block[READ_BLOCK_SIZE];

    while(!feof(file))
    {
        size_t bytes = fread(&block, 1, READ_BLOCK_SIZE, file);
        
        total += bytes;
        
        //build the frequency map
        for(size_t i = 0; i < bytes; i++)
        {
            f[block[i]]++;
        }
    }
}


/*
 * uses the frequency and total byte counts to calculate shannon entropy
 * -sum( p(xi) * log2(p(xi)) )
 * where 0 * log2(0) == 0
 */
double calc_entropy()
{
    double entropy = 0.0;

    //test the probability of all byte values
    for(int i = 0; i < 256; i++)
    {
        if(f[i] == 0)
            continue; //these don't contribute any entropy

        double prob = p(i);
        entropy += prob * log2(prob);
    }

    return -entropy;
}


int main(int argc, char* argv[])
{
    if(argc != 2)
    {
        //TODO: help output
        return 1;
    }

    FILE* file = fopen(argv[1], "r");

    if(file == NULL)
    {
        //TODO: output "no file"
        return 1;
    }

    process_file(file);
    fclose(file);

    printf("%f bits per byte\n", calc_entropy());

    return 0;
}

