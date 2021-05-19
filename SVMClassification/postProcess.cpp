#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <cmath>
#include <assert.h>

using namespace std;

int TP = 0, TN = 0, P = 0, N = 0;

bool eql(double x, double y)
{
    return fabs(x - y) < 1e-6;
}

template <class Type>
Type stringToNum(const string &str)
{
    stringstream ss(str);
    Type num;
    ss >> num;
    return num;
}

int main()
{
    ifstream finPredict, finGT;
    finPredict.open("C:\\Program_Cplusplus\\SVMClassification\\CPlusPlus\\PredictByCPlusPlus.txt");
    finGT.open("C:\\Program_Cplusplus\\SVMClassification\\dataset\\test_sample_label.txt");
    string predict, gt;
    while (getline(finPredict, predict) && getline(finGT, gt))
    {
        double x = stringToNum<double>(predict);
        double y = stringToNum<double>(gt);
        if (eql(y, 0.0))
            y = -1.0;
        eql(y, 1.0) ? P++ : N++;
        if (eql(x, y))
            eql(y, 1.0) ? TP++ : TN++;
    }
    cout << TP << " " << P << " " << TN << " " << N << endl;
    assert(P + N != 0);
    cout << "准确率为：" << (TP + TN) * 1.0 / (P + N) << endl;
    assert(P != 0);
    cout << "召回率为：" << TP * 1.0 / P << endl;
    finPredict.close();
    finGT.close();
    return 0;
}