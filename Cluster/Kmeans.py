import numpy as np

Hash=dict()

def classify(data,centers):
    length=centers.shape[0]
    classes=[ [] for i in range(length) ]
    sumDist=0
    for i in range(data.shape[0]):
        perData=data[i]
        # print("haha ",np.tile(perData,(length,1)).shape,centers.shape)
        distMat=np.tile(perData,(length,1))-centers
        sqDistMat=(distMat**2).sum(axis=1)
        
        sortedIndex=sqDistMat.argsort()
        classes[sortedIndex[0]].append(list(perData))
        sumDist+=sqDistMat[sortedIndex[0]] 

    return classes,sumDist

def updateCenters(classes):
    centers=[]
    for i in range(len(classes)):
        perClass=classes[i]
        perClass=np.array(perClass)
        center=perClass.sum(axis=0)/len(perClass)
        centers.append(center)
    # print(centers)
    return np.array(centers)


def kmeans(data,centers,sumDis):
    classes,newSumDist=classify(data,centers)
    if(sumDis==newSumDist):
        res=[i for i in range(len(data))]
        for i in range(len(classes)):
            for j in range(len(classes[i])):
                res[Hash[tuple(classes[i][j])]]=i+1
                pass
        print(res)
        return 
    newCenters=updateCenters(classes)
    kmeans(data,newCenters,newSumDist)

def getData():
    data=[]
    File=open('C:\\Program_Cplusplus\\Cluster\\data.txt','r')
    index=0
    for lines in File.readlines():
        line=lines.strip().split(" ")
        line=list(map(float,line))
        Hash[tuple(line)]=index
        index=index+1
        data.append(line)
    # print(len(data))
    File.close()

    return np.array(data)

def getRandRow(data,k): 
    total = data.shape[0]
    sequence = np.random.permutation(total)
    # print(sequence)
    return data[sequence[0:k],:]

if __name__=="__main__":
    data=getData()
    # print(data[0])
    k=int(input("请输入k: "))
    np.random.seed(0)
    centers=getRandRow(data,k)
    kmeans(data, centers, 0)




