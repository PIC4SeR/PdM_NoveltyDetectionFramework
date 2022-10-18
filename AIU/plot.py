from turtle import width
import pymongo as pymongo
import numpy as np
import matplotlib.pyplot as plt
import datetime

MongoClient = pymongo.MongoClient('mongodb://localhost:27017/')

RawDB = MongoClient['RawDB']
InfoDB = MongoClient['InfoDB']

Dataset = RawDB['Dataset']
TestResult = InfoDB['TRLOW_lr']
TestResult2 = InfoDB['TRLOW_dr']
TestResult3 = InfoDB['TRLOW_rf']
#PredictResult = InfoDB['PR']


'''color_map = {0: 'blue', 1: 'red', 2: 'green', 3: 'yellow', 4: 'pink', 5: 'gray', 6: 'black', 7: 'violet',
              8: 'lightblue', 9: 'darkgray', 10: 'orange', 11: 'blue', 12: 'darkred', 13: 'cyan', 14: 'fuchsia',
              15: 'olive', 16: 'deeppink', 17: 'purple', 18: 'royalblue', 19: 'lime', 20: 'orange', 21: 'navy'}'''

#features_desired=[70]
features_desired=[]
for i in range(70,71,1):
    features_desired.append(i)
    
#plt.figure(figsize=(7, 7))
for feature in features_desired:
   #SEs = []
   #MSEs = []
   #RMSEs = []
   SErf,MSErf,RMSErf,MAErf = [],[],[],[]
   MAElr = []
   MAEdr = []
   m=[]
   q=[]
   act=[]
   fut=[]
   err=[]
   x=[]
   y=[]
   timelr,timedr,timerf=[],[],[]
   threshold=40
   for d in TestResult.find({'Predicted_Feature': feature}).sort('TimestampUnix', pymongo.ASCENDING):
      timelr.append(d['Timestamp'])
      MAElr.append(d['Errors']['MAE'])
   for d in TestResult2.find({'Predicted_Feature': feature}).sort('TimestampUnix', pymongo.ASCENDING):
      timedr.append(d['Timestamp'])
      MAEdr.append(d['Errors']['MAE'])
   for d in TestResult3.find({'Predicted_Feature': feature}).sort('TimestampUnix', pymongo.ASCENDING):
      timerf.append(d['Timestamp'])
      #SErf.append(d['Errors']['SE'])
      #MSErf.append(d['Errors']['MSE'])
      #RMSErf.append(d['Errors']['RMSE'])
      MAErf.append(d['Errors']['MAE'])
   '''for p in PredictResult.find({'Predictor': feature}).sort('ActualErrorTime', pymongo.ASCENDING):
      if p['ActualError'] < threshold:
         m.append(p['M'])
         q.append(p['Q'])
         x.append([p['ActualError'],40])
         y.append([datetime.datetime.fromtimestamp(((p['M']*p['ActualError'])+p['Q'])/1000.0) , datetime.datetime.fromtimestamp(((p['M']*threshold)+p['Q'])/1000.0)])
         act.append(p['ActualErrorTime'])
         fut.append(p['EstimDegradTime'])
         err.append(p['ActualError'])'''
   #print('------')
   #print('Predictor ' + str(feature) + '=' + str(MAEs))
fig,ax = plt.subplots(3,sharex=True)
fig.align_ylabels([ax[0],ax[1],ax[2]])
'''   for j in range(len(y)):
   plt.plot(y[j],x[j])'''
ax[0].plot(timelr,MAElr)
ax[0].set_ylabel('Error')
ax[0].set_title('Linear Regressor - MAE')
ax[1].plot(timedr,MAEdr)
ax[1].set_ylabel('Error')
ax[1].set_title('Decision Tree Regressor - MAE')
ax[2].plot(timerf,MAErf)
ax[2].set_ylabel('Error')
ax[2].set_title('Random Forest Regressor - MAE')
'''ax[3].plot(timerf,MAErf)
ax[3].set_ylabel('Error')
ax[3].set_title('Mean Absolute Error - MAE')'''
#plt.plot(time, MAEs)#,linestyle='dashed')#, color=color_map[0])
#plt.ylabel('Error')
plt.xticks(rotation = 45)
plt.show()
