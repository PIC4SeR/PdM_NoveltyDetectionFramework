# This is a sample Python script.

# Press Shift+F10 to execute it or replace it with your code.
# Press Double Shift to search everywhere for classes, files, tool windows, actions, and settings.
import this
from datetime import datetime
import numpy as np
import pymongo as pymongo
from typing import List
from typing import Dict
from sklearn.ensemble import RandomForestRegressor

import matplotlib.pyplot as plt




class Predictor:
    def __init__(self, predicted_feature: int, testresult_collection: pymongo.collection.Collection,
                 random_state: int = 100, n_estimators: int = 10):
        self.__rf__ = RandomForestRegressor(random_state=random_state, n_estimators=n_estimators)
        self.__test_result_collection__ = testresult_collection
        self.__predicted_feature__ = predicted_feature

    def SelectLabel(self, dataset: List[List[float]]):
        try:
            data_array = np.array(dataset)
            return list(data_array[:, self.__predicted_feature__])
        except Exception as e:
            print(e)
            raise e

    def SelectFeatures(self, dataset):
        try:
            features = []
            data_array = np.array(dataset)
            features_array = np.concatenate((data_array[:,:self.__predicted_feature__], data_array[:, self.__predicted_feature__ + 1:]), axis=1)
            for fa in features_array:
                features.append(list(fa))
            return list(features)
        except Exception as e:
            print(e)
            raise e

    def Predict(self, test_features: List[List[float]]):
        try:
            return list(self.__rf__.predict(X=test_features))
        except Exception as e:
            print(e)
            raise e

    def Publish_Test_Result(self, test_result: dict):
        try:
            result = self.__test_result_collection__.insert_one(test_result)
            return result
        except Exception as e:
            print(e)
            raise e


    def Trainer(self, trainset: List[List[float]]):
        import threading
        try:
            trainer = threading.Thread(target=self.Train, args=[trainset])
            trainer.setName('PredictorTrainer_' + str(self.__predicted_feature__))
            print('Start train on predictor ' + str(self.__predicted_feature__))
            trainer.start()
            return trainer
        except Exception as e:
            print(e)
            raise e

    def Train(self, trainset: List[List[float]]):
        try:
            train_labels = self.SelectLabel(dataset=trainset)
            train_features = self.SelectFeatures(dataset=trainset)
            tstart = datetime.now()
            self.__rf__.fit(X=train_features, y=train_labels)
            print('Train for Predictor_' + str(self.__predicted_feature__) + ' completed in ' + str((datetime.now() - tstart).seconds) + 'sec')
        except Exception as e:
            print(e)
            raise e


    def Tester(self, testset: List[List[float]], last_testset_time: datetime):
        import threading
        try:
            tester = threading.Thread(target=self.Test, args=[testset, last_testset_time])
            tester.setName('PredictorTester_' + str(self.__predicted_feature__))
            print('Start test on predictor ' + str(self.__predicted_feature__))
            tester.start()
            return tester
        except Exception as e:
            print(e)
            raise e

    def Test(self, testset: List[List[float]], last_testset_time: datetime):
        try:
            test_labels = self.SelectLabel(dataset=testset)
            test_features = self.SelectFeatures(dataset=testset)
            predictions = self.Predict(test_features=test_features)
            test_result = self.Compute_Test_Result(predictions=predictions, labels=test_labels, last_testset_time=last_testset_time)
            result = self.Publish_Test_Result(test_result=test_result)
        except Exception as e:
            print(e)
            raise e


    @staticmethod
    def Scores(predictions: List[float], labels: List[float]):
        from sklearn.metrics import d2_pinball_score
        from sklearn.metrics import d2_tweedie_score
        from sklearn.metrics import r2_score
        from sklearn.metrics import d2_absolute_error_score
        try:
            r2_score = r2_score(y_true=labels, y_pred=predictions, force_finite=True)
            d2_absolute_score = d2_absolute_error_score(y_true=labels, y_pred=predictions)
            d2_pinball_score = d2_pinball_score(y_true=labels, y_pred=predictions)
            d2_tweedie_score = d2_tweedie_score(y_true=labels, y_pred=predictions)
            scores = {'R2': r2_score, 'D2A': d2_absolute_score, 'D2P': d2_pinball_score, 'D2T': d2_tweedie_score}
            score_result = {'Scores': dict(scores)}
            return score_result
        except Exception as e:
            print(e)
            raise e

    @staticmethod
    def Errors(predictions: List[float], labels: List[float]):
        from sklearn.metrics import mean_squared_error
        from sklearn.metrics import mean_absolute_error

        try:
            s_error = sum(pow(np.array(predictions) - np.array(labels), 2))
            ms_error = mean_squared_error(y_true=labels, y_pred=predictions, squared=True)
            rms_error = mean_squared_error(y_true=labels, y_pred=predictions, squared=False)
            ma_error = mean_absolute_error(y_true=labels, y_pred=predictions)
            errors = {'SE': float(s_error), 'MSE': float(ms_error), 'RMSE': float(rms_error), 'MAE': float(ma_error)}
            error_result = {'Errors': dict(errors)}
            return error_result
        except Exception as e:
            print(e)
            raise e

    def Compute_Test_Result(self, predictions: List[float], labels: List[float], last_testset_time: datetime):
        test_result = {}
        import concurrent.futures
        try:
            with concurrent.futures.ThreadPoolExecutor(max_workers=2) as executor:
                futures = []
                try:
                    futures.append(executor.submit(Predictor.Errors, predictions, labels))
                    futures.append(executor.submit(Predictor.Scores, predictions, labels))
                except Exception as e:
                    print(e)
                    raise e

                for future in futures:
                    try:
                        result = future.result(timeout=1)
                        if future.done() and not future.cancelled() and not future.running():
                            test_result = dict(test_result, **result)
                    except Exception as e:
                        print(e)
                        raise e
        except Exception as e:
            print(e)
            raise e

        test_result['Last_Testset_Time'] = last_testset_time
        test_result['Predicted_Feature'] = int(self.__predicted_feature__)
        return test_result


class AI_Unit:
    def __init__(self, nmin_datasets_for_train: int,
                 nmin_dataset_for_test: int,
                 nmax_dataset_for_test: int,
                 rawdataset_collection: pymongo.collection.Collection,
                 testresult_collection: pymongo.collection.Collection,
                 predicted_features: List[int], random_state: int = 10, n_estimators: int = 10):

        self.__trained__ = False
        self.__tested__ = False
        self.__nmin_datasets_for_train__ = nmin_datasets_for_train
        self.__nmin_datasets_for_test__ = nmin_dataset_for_test
        self.__nmax_datasets_for_test__ = nmax_dataset_for_test
        self.__last__ = datetime.now()
        self.__rawdataset_collection__ = rawdataset_collection
        self.__testresult_collection__ = testresult_collection
        self.__predictors__ = []
        for pf in predicted_features:
            self.__predictors__.append(Predictor(predicted_feature=pf,
                                                 testresult_collection=self.__testresult_collection__, random_state=random_state, n_estimators=n_estimators))

    def LoadPredictor(self, predictor: Predictor):
        try:
            self.__predictors__.append(predictor)
        except Exception as e:
            print(e)
            raise e

    def Train(self, trainsets: List[Dict]):
        print('Preprocess trainsets')
        complete_trainset = self.Preprocess(datasets=trainsets)

        print('Launch predictors trainers')
        trainers = []
        for predictor in self.__predictors__:
            trainer = predictor.Trainer(trainset=complete_trainset)
            trainers.append(trainer)

        print('Wait for predictors trainers')
        for trainer in trainers:
            trainer.join()
            print(trainer.getName() + ' ends train')

        print('Predictors trainers end')

        self.__trained__ = True
        return trainsets[len(trainsets) - 1]['Timestamp']

    def Test(self, testsets: List[Dict]):
        complete_testset = self.Preprocess(datasets=testsets)
        last_testset_time = testsets[len(testsets) - 1]['Timestamp']
        testers = []
        for predictor in self.__predictors__:
            tester = predictor.Tester(testset=complete_testset, last_testset_time=last_testset_time)
            testers.append(tester)
        return testsets[len(testsets) - 1]['Timestamp']

    def Preprocess(self, datasets):
        complete_dataset = []
        for dataset in datasets:
            ds = dataset.copy()
            del ds['_id']
            del ds['Timestamp']
            # del ds['Fs']
            complete_dataset.append(list(ds.values()))
        return complete_dataset


    def Run(self):
        print('Wait at least for ' + str(self.__nmin_datasets_for_train__) + ' datasets before training')
        # Initial Train
        try:
            # Initial Train
            while not self.__trained__:
                trainsets = []
                if self.__rawdataset_collection__.count_documents({}) >= self.__nmin_datasets_for_train__:
                    for dataset in self.__rawdataset_collection__.find({}).sort('Timestamp', pymongo.ASCENDING).limit(self.__nmin_datasets_for_train__):
                        trainsets.append(dataset)
                    self.__last__ = self.Train(trainsets=trainsets)

            # Testing Loop
            while 1:
                testsets = []
                if self.__rawdataset_collection__.count_documents({'Timestamp': {'$gt': self.__last__}}) > self.__nmin_datasets_for_test__:
                    for dataset in self.__rawdataset_collection__.find({'Timestamp': {'$gt': self.__last__}}).sort('Timestamp', pymongo.ASCENDING).limit(self.__nmax_datasets_for_test__):
                        testsets.append(dataset)
                    self.__last__ = self.Test(testsets=testsets)
        except Exception as e:
            print(e)





# Load a dataset file into defined MongoDB collection
def LoadDataset(mongodb_collection: pymongo.collection.Collection, dataset_filepath: str, separator=',', header=None):
    import pandas as pd
    try:
        df = pd.read_csv(dataset_filepath, sep=separator, header=header)
        df['Timestamp'] = df['Timestamp'].apply(lambda strdate: datetime.strptime(strdate, '%d-%b-%Y'))
        df.apply(lambda row: mongodb_collection.insert_one(document=row.to_dict()), axis=1)
    except Exception as e:
        print(e)




MongoClient = pymongo.MongoClient('mongodb://localhost:27017/')

RawDB = MongoClient['RawDB']
InfoDB = MongoClient['InfoDB']

Dataset = RawDB['Dataset']
TestResult = InfoDB['TestResult']


color_map = {0: 'blue', 1: 'red', 2: 'green', 3: 'yellow', 4: 'pink', 5: 'gray', 6: 'black', 7: 'violet',
              8: 'lightblue', 9: 'darkgray', 10: 'orange', 11: 'blue', 12: 'darkred', 13: 'cyan', 14: 'fuchsia',
              15: 'olive', 16: 'deeppink', 17: 'purple', 18: 'royalblue', 19: 'lime', 20: 'orange', 21: 'navy'}

SEs = []
MSEs = []
RMSEs = []
MAEs = []
for d in TestResult.find({'Predicted_Feature': 1}).sort('Last_Testset_Time', pymongo.ASCENDING):
   SEs.append(d['Errors']['SE'])
   MSEs.append(d['Errors']['MSE'])
   RMSEs.append(d['Errors']['RMSE'])
   MAEs.append(d['Errors']['MAE'])

i = 0
plt.figure()

plt.plot(np.arange(SEs.__len__()), SEs, color=color_map[i])
i += 1
plt.plot(np.arange(MSEs.__len__()), MSEs, color=color_map[i])
i += 1
plt.plot(np.arange(RMSEs.__len__()), RMSEs, color=color_map[i])
i += 1
plt.plot(np.arange(MAEs.__len__()), MAEs, color=color_map[i])
i += 1
plt.show()




SEs = []
MSEs = []
RMSEs = []
MAEs = []
for d in TestResult.find({'Predicted_Feature': 10}).sort('Last_Testset_Time', pymongo.ASCENDING):
   SEs.append(d['Errors']['SE'])
   MSEs.append(d['Errors']['MSE'])
   RMSEs.append(d['Errors']['RMSE'])
   MAEs.append(d['Errors']['MAE'])

i = 0
plt.figure()

plt.plot(np.arange(SEs.__len__()), SEs, color=color_map[i])
i += 1
plt.plot(np.arange(MSEs.__len__()), MSEs, color=color_map[i])
i += 1
plt.plot(np.arange(RMSEs.__len__()), RMSEs, color=color_map[i])
i += 1
plt.plot(np.arange(MAEs.__len__()), MAEs, color=color_map[i])
i += 1
plt.show()



SEs = []
MSEs = []
RMSEs = []
MAEs = []
for d in TestResult.find({'Predicted_Feature': 50}).sort('Last_Testset_Time', pymongo.ASCENDING):
   SEs.append(d['Errors']['SE'])
   MSEs.append(d['Errors']['MSE'])
   RMSEs.append(d['Errors']['RMSE'])
   MAEs.append(d['Errors']['MAE'])

i = 0
plt.figure()

plt.plot(np.arange(SEs.__len__()), SEs, color=color_map[i])
i += 1
plt.plot(np.arange(MSEs.__len__()), MSEs, color=color_map[i])
i += 1
plt.plot(np.arange(RMSEs.__len__()), RMSEs, color=color_map[i])
i += 1
plt.plot(np.arange(MAEs.__len__()), MAEs, color=color_map[i])
i += 1
plt.show()



SEs = []
MSEs = []
RMSEs = []
MAEs = []
for d in TestResult.find({'Predicted_Feature': 80}).sort('Last_Testset_Time', pymongo.ASCENDING):
   SEs.append(d['Errors']['SE'])
   MSEs.append(d['Errors']['MSE'])
   RMSEs.append(d['Errors']['RMSE'])
   MAEs.append(d['Errors']['MAE'])

i = 0
plt.figure()

plt.plot(np.arange(SEs.__len__()), SEs, color=color_map[i])
i += 1
plt.plot(np.arange(MSEs.__len__()), MSEs, color=color_map[i])
i += 1
plt.plot(np.arange(RMSEs.__len__()), RMSEs, color=color_map[i])
i += 1
plt.plot(np.arange(MAEs.__len__()), MAEs, color=color_map[i])
i += 1
plt.show()



# LoadDataset(mongodb_collection=Dataset, dataset_filepath='./Dataset.txt', separator=',', header=0)

AIU = AI_Unit(predicted_features=[1, 10, 50, 80],
              nmin_datasets_for_train=5,
              nmin_dataset_for_test=2,
              nmax_dataset_for_test=2,
              rawdataset_collection=Dataset,
              testresult_collection=TestResult, random_state=42, n_estimators=1000)
AIU.Run()









