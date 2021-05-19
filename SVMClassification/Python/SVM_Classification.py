from libsvm.svmutil import *
import os
os.chdir("C:\Program_Cplusplus\SVMClassification")

y, x = svm_read_problem(r'libSVM_train.txt')
prob  = svm_problem(y, x)
# param = svm_parameter('-t 2 -b 1') # ACC=98.75%
# param = svm_parameter('-t 0 -b 1') # ACC=97.5%
param = svm_parameter('-t 0 -c 4 -b 1') # ACC=100%
model = svm_train(prob, param)

yt,xt=svm_read_problem(r'libSVM_test.txt')

p_label, p_acc, p_val = svm_predict(yt, xt, model,'-b 1')
print(p_label)
# ACC, MSE, SCC = evaluations(yt, p_label)
# print(ACC,MSE,SCC)

# 将预测结果输出到文件
res=[]
for prob in p_label:
    res.append(str(prob)+'\n')
file_=open('C:\\Program_Cplusplus\\SVMClassification\\Python\\PredictByPython.txt',mode='w')
file_.writelines(res)
file_.close()



'''
`svm-train' Usage
=================

Usage: svm-train [options] training_set_file [model_file]
options:
-s svm_type : set type of SVM (default 0)
	0 -- C-SVC		(multi-class classification)
	1 -- nu-SVC		(multi-class classification)
	2 -- one-class SVM
	3 -- epsilon-SVR	(regression)
	4 -- nu-SVR		(regression)
-t kernel_type : set type of kernel function (default 2)
	0 -- linear: u'*v
	1 -- polynomial: (gamma*u'*v + coef0)^degree
	2 -- radial basis function: exp(-gamma*|u-v|^2)
	3 -- sigmoid: tanh(gamma*u'*v + coef0)
	4 -- precomputed kernel (kernel values in training_set_file)
-d degree : set degree in kernel function (default 3)
-g gamma : set gamma in kernel function (default 1/num_features)
-r coef0 : set coef0 in kernel function (default 0)
-c cost : set the parameter C of C-SVC, epsilon-SVR, and nu-SVR (default 1)
-n nu : set the parameter nu of nu-SVC, one-class SVM, and nu-SVR (default 0.5)
-p epsilon : set the epsilon in loss function of epsilon-SVR (default 0.1)
-m cachesize : set cache memory size in MB (default 100)
-e epsilon : set tolerance of termination criterion (default 0.001)
-h shrinking : whether to use the shrinking heuristics, 0 or 1 (default 1)
-b probability_estimates : whether to train a SVC or SVR model for probability estimates, 0 or 1 (default 0)
-wi weight : set the parameter C of class i to weight*C, for C-SVC (default 1)
-v n: n-fold cross validation mode
-q : quiet mode (no outputs)


option -v randomly splits the data into n parts and calculates cross
validation accuracy/mean squared error on them.

See libsvm FAQ for the meaning of outputs.
'''