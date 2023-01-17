'''from turtle import width
import pymongo as pymongo
import numpy as np
import matplotlib.pyplot as plt
import datetime

MongoClient = pymongo.MongoClient('mongodb://localhost:27017/')

RawDB = MongoClient['RawDB']
InfoDB = MongoClient['InfoDB']

Dataset = RawDB['DBPFI']
TestResult = InfoDB['BPFI']
TestResult2 = InfoDB['TRLOW_dr']
TestResult3 = InfoDB['TRLOW_rf']
#PredictResult = InfoDB['PR']


color_map = {0: 'blue', 1: 'red', 2: 'green', 3: 'yellow', 4: 'pink', 5: 'gray', 6: 'black', 7: 'violet',
              8: 'lightblue', 9: 'darkgray', 10: 'orange', 11: 'blue', 12: 'darkred', 13: 'cyan', 14: 'fuchsia',
              15: 'olive', 16: 'deeppink', 17: 'purple', 18: 'royalblue', 19: 'lime', 20: 'orange', 21: 'navy'}

#features_desired=[70]
features_desired=[]
for i in range(30,35,1):
    features_desired.append(i)
    
#plt.figure(figsize=(7, 7))
for feature in features_desired:
   #SEs = []
   #MSEs = []
   #RMSEs = []
   SErf,MSErf,RMSErf,MAErf = [],[],[],[]
   MAE = []
   MAEdr = []
   m=[]
   q=[]
   act=[]
   fut=[]
   err=[]
   x=[]
   y=[]
   time,timedr,timerf=[],[],[]
   threshold=10
   for d in TestResult.find({'Predicted_Feature': feature}).sort('TimestampUnix', pymongo.ASCENDING):
      time.append(d['Timestamp'])
      MAE.append(d['Errors']['MAE'])
   for d in TestResult2.find({'Predicted_Feature': feature}).sort('TimestampUnix', pymongo.ASCENDING):
      timedr.append(d['Timestamp'])
      MAEdr.append(d['Errors']['MAE'])
   for d in TestResult3.find({'Predicted_Feature': feature}).sort('TimestampUnix', pymongo.ASCENDING):
      timerf.append(d['Timestamp'])
   plt.plot(time, MAE)#,linestyle='dashed')#, color=color_map[0])
   plt.ylabel('Error')
   plt.xticks(rotation = 45)
   plt.show()
      #SErf.append(d['Errors']['SE'])
      #MSErf.append(d['Errors']['MSE'])
      #RMSErf.append(d['Errors']['RMSE'])
      #MAErf.append(d['Errors']['MAE'])
   for p in PredictResult.find({'Predictor': feature}).sort('ActualErrorTime', pymongo.ASCENDING):
      if p['ActualError'] < threshold:# and flag==0:# and p['M']>0:
         flag=1
         m.append(p['M'])
         q.append(p['Q'])
         fut.append(p['EstimDegradTime'])
         err.append(p['ActualError'])
         #x.append([p['ActualErrorTime'],datetime.datetime.fromtimestamp((datetime.datetime.timestamp(p['ActualErrorTime'])*1000+9804000000)/1000)])
         #print(datetime.datetime.fromtimestamp(((40-p['Q'])/p['M'])/1000))
         #y.append([datetime.datetime.fromtimestamp(((p['M']*p['ActualError'])+p['Q'])/1000.0) , datetime.datetime.fromtimestamp(((p['M']*threshold)+p['Q'])/1000.0)])
         #print(p['M']*datetime.datetime.timestamp(p['ActualErrorTime'])*1000+p['Q'])
         #print(datetime.datetime.fromtimestamp(x)))
         y.append([p['M']*datetime.datetime.timestamp(p['ActualErrorTime'])*1000+p['Q'], threshold]) #p['M']*(datetime.datetime.timestamp(p['ActualErrorTime'])*1000 + 9804000000)+p['Q']])
         x.append([p['ActualErrorTime'], datetime.datetime.fromtimestamp(((threshold-p['Q'])/p['M'])/1000)]) #,datetime.datetime.fromtimestamp((datetime.datetime.timestamp(p['ActualErrorTime'])*1000+9804000000)/1000)])
         print(x)
         #print(y)
         act.append(p['ActualErrorTime'])
         #print(p['Q'])
         
   #print('------')
   #print('Predictor ' + str(feature) + '=' + str(MAEs))
   for j in range(len(y)):
      plt.plot(x[j],y[j])
   plt.plot(time, MAE)#,linestyle='dashed')#, color=color_map[0])
   plt.ylabel('Error')
   plt.xticks(rotation = 45)
   plt.show()
   #print('------')
   #print('Predictor ' + str(feature) + '=' + str(MAEs))'''
''' fig,ax = plt.subplots(3,sharex=True)
fig.align_ylabels([ax[0],ax[1],ax[2]])
   for j in range(len(y)):
   plt.plot(y[j],x[j])
ax[0].plot(timelr,MAElr)
ax[0].set_ylabel('Error')
ax[0].set_title('Linear Regressor - MAE')
ax[1].plot(timedr,MAEdr)
ax[1].set_ylabel('Error')
ax[1].set_title('Decision Tree Regressor - MAE')
ax[2].plot(timerf,MAErf)
ax[2].set_ylabel('Error')
ax[2].set_title('Random Forest Regressor - MAE')
ax[3].plot(timerf,MAErf)
ax[3].set_ylabel('Error')
ax[3].set_title('Mean Absolute Error - MAE')'''

'''dataset=Dataset.find()
data1,data2=[],[]
for i in range(1,3335,1):
   data1.append(dataset[30]['ayBearDx_' + str(i)])
   #data1.append(dataset[0]['ayBearDx_' + str(i)])
plt.figure()
plt.ylabel('Acc [mm/s^2]')
plt.xlabel('Samples')
plt.title('Normal Bearing')
plt.plot(data1)
#plt.ylim(-0.9,0.9)
#plt.plot(data2)
plt.show()'''
from turtle import width
import pymongo as pymongo
import numpy as np
import matplotlib.pyplot as plt
import datetime

MongoClient = pymongo.MongoClient('mongodb://localhost:27017/')

RawDB = MongoClient['PerformanceDB']
InfoDB = MongoClient['Real_Case_20W']

Dataset = RawDB['DBPFI']
TestResult = InfoDB['RAW_DB_Retrain']
PredictResult = InfoDB['PR_Prova']

features_desired=[]
for i in range(5,6,1):
    features_desired.append(i)
    
plt.figure(figsize=(7, 7))
for feature in features_desired:
   MAEs,m,q,act,fut,err,x,y,time = [],[],[],[],[],[],[],[],[]
   threshold=80
   for d in TestResult.find({'Predicted_Feature': feature}).sort('Timestamp', pymongo.ASCENDING):
      time.append(d['Timestamp'])
      MAEs.append(d['Errors']['MAE'])
   '''for p in PredictResult.find({'Predictor': feature}).sort('ActualErrorTime', pymongo.ASCENDING):
      if p['ActualError'] < threshold:
         #m.append(p['M'])
         #q.append(p['Q'])
         #fut.append(p['EstimDegradTime'])
         #err.append(p['ActualError'])
         if p['M'] != 0:
            x.append([datetime.datetime.fromtimestamp(p['AvTime']/1000-7200), datetime.datetime.fromtimestamp(((threshold-p['Q'])/p['M'])/1000-7200)])
            y.append([p['M']*p['AvTime']+p['Q'], threshold]) 
         elif p['M'] == 0:
            x.append([datetime.datetime.fromtimestamp(p['AvTime']/1000-7200), datetime.datetime.fromtimestamp((p['AvTime']+1000000)/1000-7200)])
            y.append([p['Q'], p['Q']]) 
         #act.append(p['ActualErrorTime'])

   for j in range(len(y)):
      plt.plot(x[j],y[j])'''
   
   '''plt.plot(time, MAEs,linestyle='dashed')
   plt.scatter(time, MAEs)
   plt.ylabel('Error')
   #plt.ylim(-50,100)
   plt.xticks(rotation = 45)
   plt.title('Predictor ' + str(feature))
   plt.show()'''
dataset=Dataset.find()
data1,data2=[],[]
for i in range(1,3335,1):
   data1.append(dataset[40]['ayBearDx_' + str(i)])
   #data2.append(dataset[0]['ayBearDx_' + str(i)])
plt.ylabel(r'Acc $[\frac{mm}{s^2}]$')
plt.xlabel('Samples')
plt.title('Faulty Bearing')
plt.plot(data1)
plt.ylim(-4,4)
#plt.plot(data2)
plt.show()

