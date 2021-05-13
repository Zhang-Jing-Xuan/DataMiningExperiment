#include <iostream>
#include <cstdio>
#include <algorithm>
#include <cstring>
#include <vector>
#include <time.h>
#include <string>
#include <set>
#include <fstream>
#include <tuple>
using namespace std;

class Apriori
{
private:
    int step;
    int minSupportCount;
    int SupportCount;
    int TransCount;
    vector<vector<int>> transactions;
    vector<vector<int>> C, L;
    vector<vector<vector<int>>> items;
    //不作要求
    vector<tuple<vector<int>, vector<int>, double, double>> AssociationRules;

public:
    Apriori(vector<vector<int>> _transactions, int _minSupportCount)
    {
        for (auto i : _transactions)
        {
            sort(i.begin(), i.end());
            transactions.push_back(i);
        }
        minSupportCount = _minSupportCount;
        step = 0;
        // AssociationRules.push_back({});
        // items.push_back({{}});
    }

    int getTransCount(){
        return TransCount;
    }

    vector<vector<vector<int>>> getItems()
    {
        return items;
    }

    vector<tuple<vector<int>, vector<int>, double, double>> getAssociationRules()
    {
        return AssociationRules;
    }

    void process()
    {
        while (true)
        {
            C = generateNextC();
            step++;
            if (C.size() == 0)
                break;
            // cout<<step<<endl;
            L = generateNextL();
            items.push_back(L);
        }
        // cout<<items[0].size()<<endl;

        // 不做要求
        // for (auto i : items)
        // {
        //     for (auto j : i)
        //     {
        //         vector<int> X, Y;
        //         GenerateAssociationRules(j, X, Y, 0);
        //     }
        // }
    }

    //本实验不做要求
    /*
    void GenerateAssociationRules(vector<int> items, vector<int> X, vector<int> Y, int index)
    {
        if (index == items.size())
        {
            if (X.size() == 0 || Y.size() == 0)
            {
                return;
            }
            double XYSupport = getSupport(items);
            double XSupport = getSupport(X);
            if (XSupport == 0)
            {
                return;
            }
            double support = XYSupport*1.0;
            double confidence = XYSupport / XSupport*1.0;
            AssociationRules.push_back({X, Y, support, confidence});
            return;
        }
        X.push_back(items[index]);
        GenerateAssociationRules(items, X, Y, index + 1);
        X.pop_back();
        Y.push_back(items[index]);
        GenerateAssociationRules(items, X, Y, index + 1);
    }
    */
    vector<int> getElement(vector<vector<int>> a)
    {
        set<int> S;
        for (auto i : a)
        {
            for (auto j : i)
            {
                S.insert(j);
            }
        }
        vector<int> res;
        for (auto i : S)
        {
            res.push_back(i);
        }
        return res;
    }

    vector<vector<int>> generateNextC()
    {
        if (step == 0)
        {
            vector<vector<int>> res;
            vector<int> tmp = getElement(transactions);
            for (auto i : tmp)
                res.push_back(vector<int>(1, i));
            return res;
        }
        else
        {
            return pruning(joining());
        }
    }

    vector<vector<int>> joining()
    {
        vector<vector<int>> res;
        for (int i = 0; i < L.size(); i++)
        {
            for (int j = i + 1; j < L.size(); j++)
            {
                int k = 0;
                for (k = 0; k < step - 1; k++)
                {
                    if (L[i][k]  != L[j][k])
                    {
                        break;
                    }
                }
                if (k == step - 1)
                {
                    vector<int> tmp;
                    for (k = 0; k < step - 1; k++)
                    {
                        tmp.push_back(L[i][k]);
                    }
                    int a = L[i][step - 1];
                    int b = L[j][step - 1];
                    if (a > b)
                        swap(a, b);
                    tmp.push_back(a);
                    tmp.push_back(b);
                    res.push_back(tmp);
                    // cout<<j<<endl;
                }
            }
            // cout<<i<<endl;
        }
        // cout<<step<<endl;
        return res;
    }

    vector<vector<int>> pruning(vector<vector<int>> joined)
    {
        set<vector<int>> S;
        for (auto i : C)
            S.insert(i);
        vector<vector<int>> res;
        int i = 0;
        // cout<<step<<endl;
        for (auto row : joined)
        {
            for (i = 0; i < row.size(); i++)
            {
                vector<int> tmp = row;
                tmp.erase(tmp.begin() + i);
                if (S.find(tmp) == S.end())
                {
                    break;
                }
            }
            if (i == row.size())
            {
                res.push_back(row);
            }
        }
        return res;
    }

    double getSupport(vector<int> items)
    {
        int res = 0, i = 0, j = 0;
        for (auto row : transactions)
        {
            if (row.size() < items.size())
                continue;
            for (i = 0, j = 0; i < row.size(); i++)
            {
                if (j == items.size())
                    break;
                if (row[i] == items[j])
                    j++;
            }
            if (j == items.size())
                res++;
        }
        SupportCount = res;
        return SupportCount * 1.0;
        // cout<<SupportCount<<endl;
    }

    vector<vector<int>> generateNextL()
    {
        vector<vector<int>> res;
        for (auto i : C)
        {
            getSupport(i);
            if (SupportCount < minSupportCount)
                continue;
            res.push_back(i);
        }
        // for(auto i:res){
        //     for(auto j:i){
        //         cout<<j<<" ";
        //     }
        //     cout<<endl;
        // }
        return res;
    }
};
class InputReader
{
private:
    fstream fin;
    vector<vector<int>> transactions;
    int transCount;
public:
    InputReader(string fileName)
    {
        fin.open(fileName);
        if (!fin)
        {
            printf("Cannot open the input file.\n");
            exit(0);
        }
        transCount=0;
        parse();
    }
    int getTransCount(){
        return transCount;
    }

    void parse()
    {

        string str;
        while (!getline(fin, str).eof())
        {
            transCount++;
            int pre = 0;
            int Size = str.size();
            vector<int> tmp;
            for (int i = 0; i < Size; i++)
            {
                if (str[i] == ' ')
                {
                    int num = atoi(str.substr(pre, i).c_str());
                    pre = i + 1;
                    tmp.push_back(num);
                }
            }
            transactions.push_back(tmp);
            // for(int i=0;i<tmp.size();i++){
            //     cout<<tmp[i]<<" ";
            // }
            // cout<<endl;
        }
    }
    vector<vector<int>> getTransactions()
    {
        return transactions;
    }
};

class OutputPrinter
{
private:
    fstream fout;
    int transCount;
public:
    OutputPrinter(string fileName, vector<vector<vector<int>>> items, vector<tuple<vector<int>, vector<int>, double, double>> rules,int _transCount)
    {
        fout.open(fileName);
        if (!fout)
        {
            printf("Cannot open the output file\n");
            exit(0);
        }
        print(items);
        fout.close();
        /*
        fout.open(fileName.substr(0, 12) + "NotRequired");
        if (!fout)
        {
            printf("Cannot open the output file for association rules.");
            cout<<fileName.substr(0, 12) + "NotRequired"<<endl;
            exit(0);
        }
        transCount=_transCount;
        print(rules,transCount);
        fout.close();
        */
    }
    void print(vector<vector<vector<int>>> items)
    {
        for (auto i : items)
        {
            for (auto j : i)
            {
                for (auto k : j)
                {
                    fout << k << " ";
                }
                fout << endl;
            }
            fout << endl;
        }
    }
    /*
    void print(vector<tuple<vector<int>, vector<int>, double, double>> rules,int transCount)
    {
        for (auto line : rules)
        {
            string x = Vector2String(get<0>(line));
            string y = Vector2String(get<1>(line));
            double support = get<2>(line)/transCount*1.0*100;
            double confidence = get<3>(line)*100;
            // cout<<confidence<<endl;
            fout << x << "\t=>\t" << y << "\t";

            fout << "support=";
            fout << fixed;
            fout.precision(2);
            fout << support<< "%\t";

            fout << "confidence=";
            fout << fixed;
            fout.precision(2);
            fout << confidence<< "%\t";

            fout << endl;
        }
        
    }

    string Vector2String(vector<int> a)
    {
        string res;
        res += "{";
        for (int i = 0; i < a.size(); i++)
        {
            res += (to_string(a[i]));
            if (i != a.size() - 1)
                res += " ,";
        }
        res += "}";
        return res;
    }
    */
};

int main(int argc, char **argv)
{
    clock_t start, end;
    start = clock();
    if (argc != 4)
    {
        printf("The number of arguments should be 4.\n");
        exit(0);
    }
    string minSupportCount(argv[1]);
    string inputFileName(argv[2]);
    string OutputFileName(argv[3]);

    InputReader input(inputFileName);
    vector<vector<int>> transactions = input.getTransactions();

    Apriori apriori(transactions, stoll(minSupportCount));
    apriori.process();

    OutputPrinter output(OutputFileName, apriori.getItems(), apriori.getAssociationRules(),input.getTransCount());

    end = clock();

    printf("Run time is %.2lf(s).\n", (end - start) * 1.0 / CLOCKS_PER_SEC);
    return 0;
}

/*
cd Apriori
g++ -o apriori_1 apriori_1.cpp

./apriori_1 6 C:\Program_Cplusplus\Apriori-master\T1014D1K.dat T1014D1K_6.out //65.12s
./apriori_1 8 C:\Program_Cplusplus\Apriori-master\T1014D1K.dat T1014D1K_8.out //41.27s
./apriori_1 10 C:\Program_Cplusplus\Apriori-master\T1014D1K.dat T1014D1K_10.out //22.36s

.\apriori_1 60 C:\Program_Cplusplus\Apriori-master\T1014D10K.dat T1014D10K_60.out //687.01s=11.45min
.\apriori_1 80 C:\Program_Cplusplus\Apriori-master\T1014D10K.dat T1014D10K_80.out //385.86s=6.431min
.\apriori_1 100 C:\Program_Cplusplus\Apriori-master\T1014D10K.dat T1014D10K_100.out//265.48s


.\apriori_1 300 C:\Program_Cplusplus\Apriori-master\T1014D50K.dat T1014D50K_300.out //3700.36s=61.67min
.\apriori_1 400 C:\Program_Cplusplus\Apriori-master\T1014D50K.dat T1014D50K_400.out //2713.13s
.\apriori_1 500 C:\Program_Cplusplus\Apriori-master\T1014D50K.dat T1014D50K_500.out //1430.62s=23.84min

.\apriori_1 600 C:\Program_Cplusplus\Apriori-master\T10I4D100K.dat T10I4D100K_600.out //5271.97s=87.86min=1.46h
.\apriori_1 800 C:\Program_Cplusplus\Apriori-master\T10I4D100K.dat T10I4D100K_800.out //3947.38s=65.79min
.\apriori_1 1000 C:\Program_Cplusplus\Apriori-master\T10I4D100K.dat T10I4D100K_1000.out //2900.52s=48.342min
*/