#include "FPGrowth.h"
#include <iostream>

using namespace std;

int main()
{
    float minSupport = 600.0/100000.0;
    FPGrowth *fp = new FPGrowth(minSupport);
    fp->initFromFile("C:\\Program_Cplusplus\\Apriori-master\\T10I4D100K.dat");
    fp->outputToFile("C:\\Program_Cplusplus\\FPGrowth_again\\output.txt");
    delete fp;
    return 0;
}