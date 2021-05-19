#include <fstream>
#include <sstream>
#include <iostream>
#include <string>
using namespace std;

void process(string DataPath, string LabelPath, string OutputPath)
{
    ifstream fin_train, fin_train_label;
    fin_train.open(DataPath);
    fin_train_label.open(LabelPath);
    ofstream fout;
    fout.open(OutputPath);
    string data, label;
    while (getline(fin_train, data) && getline(fin_train_label, label))
    {
        stringstream ss(data);
        string dataResult;
        if (label == "0")
        {

            fout << "-1"
                 << " ";
        }
        else
        {
            fout << label << " ";
        }
        int cnt = 1;
        while (ss >> dataResult)
        {
            fout << cnt << ":" << dataResult << " ";
            cnt++;
        }
        fout << endl;
    }
    fin_train.close();
    fin_train_label.close();
    fout.close();
}

int main()
{
    string OutputPath = "C:\\Program_Cplusplus\\SVMClassification\\libSVM_train.txt";

    // train
    string trainDataPath = "C:\\Program_Cplusplus\\SVMClassification\\dataset\\train_sample.txt";
    string trainLabelPath = "C:\\Program_Cplusplus\\SVMClassification\\dataset\\train_sample_label.txt";
    process(trainDataPath, trainLabelPath, OutputPath);

    //test
    string testDataPath = "C:\\Program_Cplusplus\\SVMClassification\\dataset\\test_sample.txt";
    string testLabelPath = "C:\\Program_Cplusplus\\SVMClassification\\dataset\\test_sample_label.txt";
    OutputPath = "C:\\Program_Cplusplus\\SVMClassification\\libSVM_test.txt";
    process(testDataPath, testLabelPath, OutputPath);

    return 0;
}