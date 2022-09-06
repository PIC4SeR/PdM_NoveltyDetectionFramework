import pymongo as pymongo
import numpy as np
import matplotlib.pyplot as plt

MongoClient = pymongo.MongoClient('mongodb://localhost:27017/')

RawDB = MongoClient['RawDB']
InfoDB = MongoClient['InfoDB']

Dataset = RawDB['Dataset']
TestResult = InfoDB['TestResult']


color_map = {0: 'blue', 1: 'red', 2: 'green', 3: 'yellow', 4: 'pink', 5: 'gray', 6: 'black', 7: 'violet',
              8: 'lightblue', 9: 'darkgray', 10: 'orange', 11: 'blue', 12: 'darkred', 13: 'cyan', 14: 'fuchsia',
              15: 'olive', 16: 'deeppink', 17: 'purple', 18: 'royalblue', 19: 'lime', 20: 'orange', 21: 'navy'}

features_desired=[1,10,2]
    
plt.figure()
for feature in features_desired:
   SEs = []
   MSEs = []
   RMSEs = []
   MAEs = []
   for d in TestResult.find({'Predicted_Feature': feature}).sort('Last_Testset_Time', pymongo.ASCENDING):
      SEs.append(d['Errors']['SE'])
      MSEs.append(d['Errors']['MSE'])
      RMSEs.append(d['Errors']['RMSE'])
      MAEs.append(d['Errors']['MAE'])
   print('------')
   print('Predictor ' + str(feature) + '=' + str(MAEs))
   #plt.plot(np.arange(MAEs.__len__()), MAEs, color=color_map[0])
   #plt.title('Predictor ' + str(feature))
   #plt.show()


