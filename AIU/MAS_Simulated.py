import pymongo as pymongo
import numpy as np
import matplotlib.pyplot as plt
import datetime

MongoClient = pymongo.MongoClient('mongodb://localhost:27017/')

#RawDB = MongoClient['RawDB']
InfoDB = MongoClient['Real_Case_20W']

TestResult = InfoDB['RAW_DB_Retrain']

feature = 135
average = 10
minthreshold = 20
maxthreshold = 40

time,MAE,unix = [],[],[]
mean1,mean2,time1,time2 = [],[],[],[]
meanf,timef = [],[]
plt.figure(figsize=[7,7])
flag=0
mmax=0
for i,d in enumerate(TestResult.find({'Predicted_Feature': feature}).sort('Timestamp', pymongo.ASCENDING)):
      time.append(d['Timestamp'])
      unix.append(d['TimestampUnix'])
      MAE.append(d['Errors']['MAE'])
      if d['Errors']['MAE'] > minthreshold and i >= average:
        mean1 = MAE[i - average : i - int((average/2))]
        mean2= MAE[i - int((average/2)) + 1 : ]
        time1 = unix[i - average : i - int((average/2))]
        time2 = unix[i - int((average/2)) + 1 : ]
        av1 = np.mean(np.array(mean1))
        av2 = np.mean(np.array(mean2))
        avt1 = np.mean(np.array(time1))
        avt2 = np.mean(np.array(time2))
        m=(av2-av1)/(avt2-avt1)
        q=av1-m*avt1
        if m > 0:# and m > mmax:
          mmax=m
          if flag == 0:
            plt.plot([datetime.datetime.fromtimestamp(avt1),datetime.datetime.fromtimestamp((maxthreshold-q)/m)],[m*avt1+q,maxthreshold],color='r',label="Predictions")
            flag=1
          else:
            plt.plot([datetime.datetime.fromtimestamp(avt1),datetime.datetime.fromtimestamp((maxthreshold-q)/m)],[m*avt1+q,maxthreshold],color='r')

plt.plot(time, MAE,linestyle='dashed',color='b',label="AIU Error")
#plt.scatter(time, MAE)
plt.ylabel('Error')
#plt.ylim(-50,100)
plt.xticks(rotation = 45)
plt.legend()
plt.title('Real Case Study [Acc_y] with retrain')
plt.show()