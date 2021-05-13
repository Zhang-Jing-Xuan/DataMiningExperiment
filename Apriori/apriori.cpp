#include <iostream>
#include <cstdio>
#include <cstring>
#include <algorithm>
#include <vector>
#include <fstream>
#include <string>
#include <set>
#include <time.h>
#include <cstdlib>

using namespace std;

class Apriori
{
private:
    int step;
    int minSupportCount;
    int SupportCount;
    vector<vector<int>> transactions;
    vector<vector<int>> C, L;
    vector<vector<vector<int>>> items;

public:
    Apriori(vector<vector<int>> _transactions, int _minSupportCount)
    {
        step = 0;
        minSupportCount = _minSupportCount;
        for (auto i : _transactions)
        {
            sort(i.begin(), i.end());
            transactions.push_back(i);
        }
        items.push_back({{}});
    }

    vector<vector<vector<int>>> getItems()
    {
        return items;
    }

    void process()
    {
        while (true)
        {
            C = generateNextC();
            step++;
            if (C.size() == 0)
                break;

            L = generateNextL();
            items.push_back(L);
        }
    }

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
            vector<int> element = getElement(transactions);
            for (auto i : element)
            {
                res.push_back(vector<int>(1, i));
            }
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
                    if (L[i][k] != L[j][k])
                    {
                        break;
                    }
                }
                if (k == step - 1)
                {
                    vector<int> tmp;
                    for (int p = 0; p < step - 1; p++)
                    {
                        tmp.push_back(L[i][p]);
                    }
                    int a = L[i][step - 1];
                    int b = L[j][step - 1];
                    if (a > b)
                        swap(a, b);
                    tmp.push_back(a);
                    tmp.push_back(b);
                    res.push_back(tmp);
                }
            }
        }
        return res;
    }
    vector<vector<int>> pruning(vector<vector<int>> joined)
    {
        set<vector<int>> S;
        vector<vector<int>> res;
        for (auto i : L)
            S.insert(i);
        int j = 0;
        for (auto i : joined)
        {
            for (j = 0; j < i.size(); j++)
            {
                vector<int> tmp = i;
                tmp.erase(tmp.begin() + j);
                if (S.find(tmp) == S.end())
                {
                    break;
                }
            }
            if (j == i.size())
            {
                res.push_back(i);
            }
        }
        return res;
    }

    void getSupport(vector<int> item)
    {
        int i = 0, j = 0, res = 0;
        for (auto row : transactions)
        {
            if (row.size() < item.size())
                continue;
            for (i = 0, j = 0; i < row.size(); i++)
            {
                if (j == item.size())
                    break;
                if (row[i] == item[j])
                    j++;
            }
            if (j == item.size())
                res++;
        }
        SupportCount = res;
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
        return res;
    }
};

class InputReader
{
private:
    fstream fin;
    vector<vector<int>> transactions;

public:
    InputReader(string fileName)
    {
        fin.open(fileName);
        if (!fin)
        {
            printf("Cannot open the InputFile");
            exit(0);
        }
        parse();
    }
    void parse()
    {
        string line;
        while (!getline(fin, line).eof())
        {
            vector<int> tmp;
            int Size = line.size();
            int pre = 0;
            for (int i = 0; i < Size; i++)
            {
                if (line[i] == ' ')
                {
                    int num = atoi(line.substr(pre, i).c_str());
                    tmp.push_back(num);
                    pre = i + 1;
                }
            }
            transactions.push_back(tmp);
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

public:
    OutputPrinter(string fileName, vector<vector<vector<int>>> items)
    {
        fout.open(fileName);
        if (!fout)
        {
            printf("Cannot not open the output file");
            exit(0);
        }
        print(items);
    }
    void print(vector<vector<vector<int>>> items)
    {
        vector<vector<vector<int>>>::iterator i;
        for (i = items.begin(); i != items.end(); i++)
        {
            vector<vector<int>>::iterator j;
            for (j = (*i).begin(); j != (*i).end(); j++)
            {
                vector<int>::iterator k;
                for (k = (*j).begin(); k != (*j).end(); k++)
                {
                    fout << (*k) << " ";
                }
                fout << endl;
            }
            fout << endl;
        }
    }
};

int main(int argc, char **argv)
{
    clock_t start, end;
    start = clock();
    if (argc != 4)
    {
        cout << "The number of arguments should be 4" << endl;
        exit(0);
    }
    string minSupport(argv[1]);
    string InputFileName(argv[2]);
    string OutputFileName(argv[3]);

    InputReader input(InputFileName);
    vector<vector<int>> transactions = input.getTransactions();

    Apriori apriori(transactions, stoll(minSupport));
    apriori.process();

    OutputPrinter output(OutputFileName, apriori.getItems());

    end = clock();

    printf("Run time is %.2lfs\n", (end - start) * 1.0 / CLOCKS_PER_SEC);
    return 0;
}

/*
cd Apriori
g++ -o apriori apriori.cpp
./apriori 10 C:\Program_Cplusplus\Apriori-master\T1014D1K.dat T1014D1K_10.out
*/
