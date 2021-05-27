import numpy as np
import time
Hash=dict()
HashCenters=dict()
flag=False

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

def updateCenters(classes,dist):
    centers=[]
    trueCenters=[]
    falseCenters=[]
    global flag
    flag=False
    for h in HashCenters:
        if(HashCenters[h]==True):
            trueCenters.append(list(h))
        else:
            falseCenters.append(list(h))
    tmpi,tmpj,minDist=-1,-1,dist
    # print(len(falseCenters),len(trueCenters))
    for i in range(len(falseCenters)):
        for j in range(len(trueCenters)):
            tmp=trueCenters.copy()
            trueCenters[j]=falseCenters[i]
            classes,newSumDist=classify(data,np.array(trueCenters))
            if(newSumDist<minDist):
                tmpi,tmpj,minDist=i,j,newSumDist
                # print("!")
            trueCenters=tmp.copy()
    if(tmpi!=-1):
        flag=True
        trueCenters[tmpj]=falseCenters[tmpi]
        centers=trueCenters.copy()
        for h in HashCenters:
            HashCenters[h]=False
        for c in centers:
            HashCenters[tuple(c)]=True
    return np.array(centers),flag

iterations=1
def kmediods(data,centers,sumDis):
    start=time.time()
    classes,newSumDist=classify(data,centers)
    newCenters,flag=updateCenters(classes,newSumDist)
    end=time.time()
    dur=end-start
    global iterations
    print("iterations=",iterations,end="\t")
    iterations+=1
    print("Cost time: ",format(dur,'.2f'),"s\t","origin distance=",format(sumDis,'.2f'),"\t","new distance=",format(newSumDist,'.2f'))
    if(sumDis==newSumDist or flag==False):
        res=[i for i in range(len(data))]
        for i in range(len(classes)):
            for j in range(len(classes[i])):
                res[Hash[tuple(classes[i][j])]]=i+1
                pass
        print(res)
        return

    kmediods(data,newCenters,newSumDist)

def getData():
    data=[]
    File=open('C:\\Program_Cplusplus\\Cluster\\data.txt','r')
    index=0
    for lines in File.readlines():
        line=lines.strip().split(" ")
        line=list(map(float,line))
        Hash[tuple(line)]=index
        HashCenters[tuple(line)]=False
        index=index+1
        data.append(line)
    # print(len(data))
    File.close()

    return np.array(data)

def getRandRow(data,k): 
    total = data.shape[0]
    sequence = np.random.permutation(total)
    return data[sequence[0:k],:]

if __name__=="__main__":
    data=getData()
    k=int(input("请输入k: "))
    np.random.seed(0)
    centers=getRandRow(data,k)
    for c in centers:
        HashCenters[tuple(c)]=True
    kmediods(data, centers, 0)


# k=5 238.72s=3.98min
# k=10 688.31s=11.47min
# k=20 1h15.83min
