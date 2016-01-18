
#include <stdio.h>
#include <math.h>


#define READ_BLOCK_SIZE 4096


//frequency of each byte value
size_t f[256] = { 0 };
size_t total = 0;





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
    for(size_t i = 0; i < 256; i++)
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
    if(argc < 2)
    {
        printf("Usage: entropy [FILE]...\n\n\
Reports entropy in bits per byte\n\
    8.0 = perfectly random\n\
    0.0 = perfectly uniform\n");
        return 0;
    }

    for(int f = 1; f < argc; f++)
    {
        FILE* file = fopen(argv[f], "r");

        if(file == NULL)
        {
            perror(argv[f]);
            return 1;
        }

        process_file(file);
        fclose(file);
    }

    //print the result
    if(total > 0)
    {
        printf("%f bits per byte\n", calc_entropy());
    }

    return 0;
}

