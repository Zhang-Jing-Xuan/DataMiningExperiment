#ifndef FPGrowth_algorithm
#define FPGrowth_algorithm

#include <vector>
#include <unordered_map>
#include <string>
#include <time.h>

using namespace std;

struct FPTreeNode
{
    string name;
    int count;
    unordered_map<string, FPTreeNode *> children;
    FPTreeNode *parent;
    FPTreeNode *link;
    FPTreeNode(string _name = "", int _count = 1, FPTreeNode *_parent = NULL)
    {
        name = _name;
        count = _count;
        children.clear();
        parent = _parent;
        link = NULL;
    }
    ~FPTreeNode()
    {
        for (auto t : children)
        {
            if (t.second != NULL)
            {
                delete t.second;
            }
        }
    }
};

struct FPHeaderTable
{
    string name;
    int count;
    FPTreeNode *head;
    FPTreeNode *end;
    FPHeaderTable(string _name = "", int _count = 1, FPTreeNode *_head = NULL)
    {
        name = _name;
        count = _count;
        head = _head;
    }
};

struct FPTransItem
{
    string name;
    int count;
    FPTransItem(string _name = "", int _count = 1)
    {
        name = _name;
        count = _count;
    }
};

struct FPTrans
{
    vector<FPTransItem> trans;
    FPTrans(vector<FPTransItem> _trans)
    {
        trans = _trans;
    }
};

struct FreqResult
{
    vector<FPTransItem> trans;
    int count;
};

class FPGrowth
{
private:
    clock_t start;
    float minSupportCount;
    float minSupport;
    int ItemsCount;

    unordered_map<string, int> nameIndex;
    FPTreeNode *root;
    vector<FPHeaderTable *> headerTable;
    vector<FPTransItem *> *prefix;

    void buildHeaderTable(string fileName);
    void buildHeaderTable(vector<FPTrans> &trans);
    void sortHeaderTable();

    void buildFPTree(string fileName);
    void buildFPTree(vector<FPTrans> &trans);

    void insertTrans(vector<string> &trans);
    void insertTrans(vector<FPTransItem *> &trans);

    void saveSingleResult(vector<FreqResult> &reault);
    void miningFPTree(vector<FreqResult> &result);

public:
    FPGrowth(float minSupport);
    ~FPGrowth();

    void initFromFile(string fileName);
    void initFromTrans(vector<FPTrans> &trans);

    void outputToFile(string fileName);
};
#endif