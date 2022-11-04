from turtle import width
import pymongo as pymongo
import numpy as np
import matplotlib.pyplot as plt
import datetime

MongoClient = pymongo.MongoClient('mongodb://localhost:27017/')

RawDB = MongoClient['RawDB']
InfoDB = MongoClient['InfoDB']

Dataset = RawDB['Dataset']
TestResult = InfoDB['TR']
PredictResult = InfoDB['PR']

features_desired=[38]
#for i in range(20,25,1):
#    features_desired.append(i)

M_MAX = 0
plt.figure(figsize=(7, 7))
for feature in features_desired:
	MAEs,m,q,act,fut,err,x,y,time = [],[],[],[],[],[],[],[],[]
	threshold=40
	
	for d in TestResult.find({'Predicted_Feature': feature}).sort('Last_Testset_Time', pymongo.ASCENDING):
		time.append(d['Last_Testset_Time'])
		MAEs.append(d['Errors']['MAE'])
	for p in PredictResult.find({'Predictor': feature}).sort('ActualErrorTime', pymongo.ASCENDING):
		if p['ActualError'] < threshold and p['M']>0.00000001:# and 1/p['M'] > M_MAX:
			M_MAX = 1/p['M']
			M = 1/p['M']
			Q = - (p['Q']/p['M'])
			m.append(M)
			q.append(Q)
			fut.append(p['EstimDegradTime'])
			err.append(p['ActualError'])
			y.append([M*datetime.datetime.timestamp(p['ActualErrorTime'])*1000+Q, 40]) 
			x.append([p['ActualErrorTime'], datetime.datetime.fromtimestamp(((40-Q)/M)/1000)])
			act.append(p['ActualErrorTime'])

	for j in range(len(y)):
		plt.plot(x[j],y[j])
	plt.plot(time, MAEs,linestyle='dashed')
	plt.scatter(time, MAEs)
	plt.ylabel('Error')
	plt.ylim(-50,100)
	plt.xticks(rotation = 45)
	plt.title('Predictor ' + str(feature))
	plt.show()
