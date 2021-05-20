### 一.为正样本(鸟)，0为负样本(其他)

### 二. 数据维数是690维。

### 三. 训练集组成:80(正样本)+240(负样本) ;   测试集组成:20(正样本)+60(负样本)


```
g++ -o preProcess preProcess.cpp
./preProcess

cd CplusPlus
g++ -o svm svm.cpp svm-train.c
./svm -t 0 C:\\Program_Cplusplus\\SVMClassification\\libSVM_train.txt
g++ -o predict svm.cpp svm-predict.c
./predict C:\\Program_Cplusplus\\SVMClassification\\libSVM_test.txt C:\Program_Cplusplus\SVMClassification\libSVM_train.txt.model PredictByCPlusPlus.txt

cd ..
g++ -o postProcess postProcess.cpp
./postProcess
```