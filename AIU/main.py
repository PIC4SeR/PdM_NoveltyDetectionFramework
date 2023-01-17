# This is a sample Python script.

# Press Shift+F10 to execute it or replace it with your code.
# Press Double Shift to search everywhere for classes, files, tool windows, actions, and settings.
from msilib.schema import File
from datetime import datetime
import numpy as np
import pymongo as pymongo
from typing import List
from typing import Dict
from sklearn.ensemble import RandomForestRegressor
from sklearn.linear_model import LinearRegression, LogisticRegression
from sklearn.tree import DecisionTreeRegressor
import pandas as pd
import joblib
import matplotlib.pyplot as plt
import os
import threading
import msvcrt
import time
import sys



class Predictor:
    def __init__(self, predicted_feature: int, testresult_collection: pymongo.collection.Collection,
                 random_state, n_estimators, max_features):
        self.__rf__ = RandomForestRegressor(random_state=random_state, n_estimators=n_estimators, max_features=max_features)
        #self.__rf__=LinearRegression()
        #self.__rf__ = DecisionTreeRegressor()
        self.__test_result_collection__ = testresult_collection
        self.__predicted_feature__ = predicted_feature
        self.__directory__ = 'Low_Retrain'
        self.__filename__ = 'Predictor_' + str(self.__predicted_feature__) + '_Model.sav'
        

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


    def SaveModel(self, model):
        try:
            path = os.path.join(self.__directory__ + '/' + self.__filename__)
            joblib.dump(model, open(path,'wb'))
            print('Predictor_' + str(self.__predicted_feature__) + ' saved!')
        except Exception as e:
            print(e)
            raise e
    

    def LoadModel(self,filename):
        try:
            model = joblib.load(filename)
            return model
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


    def Trainer(self, trainset: List[List[float]], update = False):
        try:
            if not os.path.isdir(self.__directory__):
                os.mkdir(self.__directory__)
            models=os.listdir(self.__directory__)
            predictor_found = False
            for model in models:
                if 'Predictor_' + str(self.__predicted_feature__) + '_Model' in model and update == False:
                    predictor_found = True
                    trainer = threading.Thread(target=self.Recover, args=[model])
                    trainer.setName('PredictorRecoverer_' + str(self.__predicted_feature__))
                    print('Start recovery on predictor ' + str(self.__predicted_feature__))
                    trainer.start()
            if not(predictor_found) or update == True:
                trainer = threading.Thread(target=self.Train, args=[trainset])
                trainer.setName('PredictorTrainer_' + str(self.__predicted_feature__))
                print('Start train on predictor ' + str(self.__predicted_feature__))
                trainer.start()
            return trainer
        except Exception as e:
            print(e)
            raise e
    

    def Recover(self, model):
        self.__rf__=self.LoadModel(self.__directory__ + '/' + model)
        print('Predictor ' + str(self.__predicted_feature__) + ' recovered')


    def Train(self, trainset: List[List[float]]):
        
        try:
            train_labels = self.SelectLabel(dataset=trainset)
            train_features = self.SelectFeatures(dataset=trainset)
            tstart = datetime.now()
            self.__rf__.fit(X=train_features, y=train_labels)
            print('Train for Predictor ' + str(self.__predicted_feature__) + ' completed in ' + str((datetime.now() - tstart).seconds) + 'sec')
            self.SaveModel(self.__rf__)                        
        except Exception as e:
            print(e)
            raise e


    def Tester(self, testset: List[List[float]], last_testset_time: datetime):
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

        test_result['Timestamp'] = datetime.fromtimestamp(last_testset_time)
        test_result['TimestampUnix'] = datetime.timestamp(test_result['Timestamp'])
        test_result['Predicted_Feature'] = int(self.__predicted_feature__)
        return test_result


class AI_Unit:
    def __init__(self, 
                column_names, 
                nmin_datasets_for_train: int,
                nmin_dataset_for_test: int,
                nmax_dataset_for_test: int,
                rawdataset_collection: pymongo.collection.Collection,
                trainset_collection: pymongo.collection.Collection,
                testresult_collection: pymongo.collection.Collection,
                performance: pymongo.collection.Collection,
                predicted_features: List[int],
                windows: int = 20, 
                random_state: int = 42, n_estimators: int = 10, max_features: int = 10):

        self.__trained__ = False
        self.__tested__ = False
        self.__nmin_datasets_for_train__ = nmin_datasets_for_train
        self.__nmin_datasets_for_test__ = nmin_dataset_for_test
        self.__nmax_datasets_for_test__ = nmax_dataset_for_test
        #self.__last__ = datetime.now()
        self.__rawdataset_collection__ = rawdataset_collection
        self.__trainset_collection__ = trainset_collection
        self.__testresult_collection__ = testresult_collection
        self.__perf_collection__ = performance
        self.__predictors__ = []
        self.__column_names__=column_names
        self.__windows__=windows
        self.__starttime__=datetime.now()
        global startpreprocesstime
        global startpredicttime
        global starttraintime
        global endpredicttime
        global endtraintime
        global endpreprocesstime
        for pf in predicted_features:
            self.__predictors__.append(Predictor(predicted_feature=pf,
                                                 testresult_collection=self.__testresult_collection__, random_state=random_state, n_estimators=n_estimators, max_features=max_features))

    def LoadPredictor(self, predictor: Predictor):
        try:
            self.__predictors__.append(predictor)
        except Exception as e:
            print(e)
            raise e

    def Train(self, trainsets: List[Dict], update = False):
        #print('Preprocess trainsets')
        startpreprocesstime = (datetime.now() - self.__starttime__).total_seconds()
        #print('Start Preprocess Time ' + str(startpreprocesstime))
        complete_trainset = self.Preprocess(datasets=trainsets)
        endpreprocesstime = (datetime.now() - self.__starttime__).total_seconds()
        try:
            self.__perf_collection__.insert_one({'PreprocessTrain_Time': endpreprocesstime - startpreprocesstime})
        except:
            print('---')
        #print('Launch predictors trainers')
        trainers = []
        starttraintime = (datetime.now() - self.__starttime__).total_seconds()
        if update == False:
            for predictor in self.__predictors__:
                trainer = predictor.Trainer(trainset=complete_trainset)
                trainers.append(trainer)
        elif update == True:
            for predictor in self.__predictors__:
                trainer = predictor.Trainer(trainset=complete_trainset, update = True)
                trainers.append(trainer)
        
        #print('Wait for predictors trainers')
        for trainer in trainers:
            trainer.join()
        endtraintime = (datetime.now() - self.__starttime__).total_seconds()
        try:
            self.__perf_collection__.insert_one({'Train_Time': endtraintime - starttraintime})
        except:
            print('---')
            #print(trainer.getName() + ' ends train')

        #print('Predictors trainers end')
        self.__trained__ = True
        return trainsets[len(trainsets) - 1]['TimestampUnix']

    def Test(self, testsets: List[Dict]):
        startpreprocesstime = (datetime.now() - self.__starttime__).total_seconds()
        complete_testset = self.Preprocess(datasets=testsets)
        endpreprocesstime = (datetime.now() - self.__starttime__).total_seconds()
        try:
            self.__perf_collection__.insert_one({'PreprocessTest_Time': endpreprocesstime - startpreprocesstime})
        except:
            print('---')
        last_testset_time = testsets[len(testsets) - 1]['TimestampUnix']
        testers = []
        startpredicttime = (datetime.now() - self.__starttime__).total_seconds()
        for predictor in self.__predictors__:
            tester = predictor.Tester(testset=complete_testset, last_testset_time=last_testset_time)
            testers.append(tester)
        endpredicttime = (datetime.now() - self.__starttime__).total_seconds()
        try:
            self.__perf_collection__.insert_one({'Test_Time': endpredicttime - startpredicttime})
        except:
            print('----')
        
        return last_testset_time


    def Preprocess(self, datasets):
        complete_dataset = []
        for dataset in datasets:
            ds = dataset.copy()
            del ds['Timestamp']
            del ds['TimestampUnix']
            data=pd.DataFrame(ds, index=[0])
            names=[]
            for string in self.__column_names__:
                for i in range(1,self.__windows__+1):
                    names.append(str(string+str(i)))
            column_list=data.columns.values
            dato=[]
            for string in self.__column_names__:
                count=0
                for element in column_list:
                    if string in element:
                        count=count+1
                dato.append(data.columns.get_loc(string + '1'))
                dato.append(data.columns.get_loc(string + str(count)))

            New_Dataframe=pd.DataFrame()
            for j in range(0,len(data),1):
                sum_abs=[]
                for i in range(0,len(dato),2):
                    Data_Filtered=data.iloc[j,dato[i]:dato[i+1]] #Data isolation between indexes chosen before
                    array_windowed = np.array_split(np.array(Data_Filtered.values),self.__windows__) #Windowing 
                    for element in array_windowed:
                        sum_abs.append(np.sum(np.absolute(element))) #Absolute sum for each window 
                All_Data=pd.DataFrame(np.array(sum_abs).reshape(1,-1),columns=names)
                New_Dataframe=pd.concat([New_Dataframe,All_Data],axis=0)
            New_Dataframe=New_Dataframe.reset_index(drop=True)
            #New_Dataframe['Timestamp']=data['Timestamp']
            complete_dataset.append(list(New_Dataframe.values[0]))
        return complete_dataset


    def Run(self):
        print('Wait at least for ' + str(self.__nmin_datasets_for_train__) + ' datasets before training')
        # Initial Train
        try:
            # Initial Train
            already_trained = False
            while not self.__trained__:
                trainsets = []
                if self.__trainset_collection__.count_documents({}) >0:
                    already_trained = True
                    for dataset in self.__trainset_collection__.find({}, {'_id': False}): #Mettere un controllo in caso di nuove label da predirre
                        trainsets.append(dataset)
                    self.__last__ = self.Train(trainsets=trainsets)
                    self.__trained__ = True
                if self.__rawdataset_collection__.count_documents({}) >= self.__nmin_datasets_for_train__ and not(already_trained):
                    for dataset in self.__rawdataset_collection__.find({}, {'_id': False}).sort('TimestampUnix', pymongo.ASCENDING).limit(self.__nmin_datasets_for_train__):
                        self.__trainset_collection__.insert_one(dataset)
                        trainsets.append(dataset)
                    self.__last__ = self.Train(trainsets=trainsets)
                    
            # Testing Loop
            while 1:
                testsets = []
                if len(list(self.__testresult_collection__.find())) > 0:
                    self.__last__= self.__testresult_collection__.find_one(sort=[('TimestampUnix', pymongo.DESCENDING)])['TimestampUnix']
                if self.__rawdataset_collection__.count_documents({'TimestampUnix': {'$gt': self.__last__}}) > self.__nmin_datasets_for_test__:
                    
                    for dataset in self.__rawdataset_collection__.find({'TimestampUnix': {'$gt': self.__last__}}, {'_id': False}).sort('TimestampUnix', pymongo.ASCENDING).limit(self.__nmax_datasets_for_test__):
                        testsets.append(dataset)
                    self.__last__ = self.Test(testsets=testsets)
                else: #Solo per prove
                    print('Dataset Finito')
                    sys.exit("Dataset Finito")
                if msvcrt.kbhit():
                    key = input()
                    if key == 'U':
                        if self.__rawdataset_collection__.count_documents({'TimestampUnix': {'$gt': self.__last__}}) >= self.__nmin_datasets_for_train__:
                            for dataset in self.__rawdataset_collection__.find({'TimestampUnix': {'$gt': self.__last__}}, {'_id': False}).sort('TimestampUnix', pymongo.ASCENDING).limit(self.__nmin_datasets_for_train__):
                                self.__trainset_collection__.insert_one(dataset)
                                trainsets.append(dataset)
                            self.__last__ = self.Train(trainsets=trainsets, update = True)
                    elif key == 'R':
                        self.__trainset_collection__.drop()
                        self.__testresult_collection__.drop()
                        trainsets=[]
                        if self.__rawdataset_collection__.count_documents({'TimestampUnix': {'$gt': self.__last__}}) >= self.__nmin_datasets_for_train__:
                            for dataset in self.__rawdataset_collection__.find({'TimestampUnix': {'$gt': self.__last__}}, {'_id': False}).sort('TimestampUnix', pymongo.ASCENDING).limit(self.__nmin_datasets_for_train__):
                                self.__trainset_collection__.insert_one(dataset)
                                trainsets.append(dataset)
                            self.__last__ = self.Train(trainsets=trainsets, update = True)
        except Exception as e:
            print(e)


# Load a dataset file into defined MongoDB collection
'''def LoadDataset(mongodb_collection: pymongo.collection.Collection, dataset_filepath: str, separator=',', header=None):
    import pandas as pd
    try:
        df = pd.read_csv(dataset_filepath, sep=separator, header=header)
        df['Timestamp'] = df['Timestamp'].apply(lambda strdate: datetime.strptime(strdate, '%d-%b-%Y'))
        df.apply(lambda row: mongodb_collection.insert_one(document=row.to_dict()), axis=1)
    except Exception as e:
        print(e)'''



MongoClient = pymongo.MongoClient('mongodb://localhost:27017')
#MongoClient['SpeaDB']['Perf'].drop()
#MongoClient['SpeaDB']['Res_Tr'].drop()
#MongoClient['SpeaDB']['TestRes'].drop()
#MongoClient['PerformanceDB']['LOW'].drop()
#MongoClient['PerformanceDB']['z'].drop()
#RawDB = MongoClient['RawDB']
InfoDB = MongoClient['PerformanceDB']

InfoDB['Perf'].drop()
InfoDB['Result'].drop()
InfoDB['TR_LOW_Retrain'].drop()
#del InfoDB['TR_LOW_Retrain']
#del InfoDB['Result']
Dataset = InfoDB['DLOW_Retrain']
Trainset = InfoDB['TR_LOW_Retrain']
TestResult = InfoDB['Result']
Performance = InfoDB['Perf']


# LoadDataset(mongodb_collection=Dataset, dataset_filepath='./Dataset.txt', separator=',', header=0)

#Label definition set by user
#label_strings=['ax_a_','ay_a_','ax_b_','ay_b_','ax_c_','ay_c_','ax_d_','ay_d_']
label_strings=['axBearDx_','axBearSx_','ayBearDx_','ayBearSx_','axShaft_','ayShaft_']
#label_strings=['Res_']

windows = 20
#Prediction wanted set by user
features_desired=[]
for i in range(1,len(label_strings)*windows,1):
    features_desired.append(i)

AIU = AI_Unit(
            column_names=label_strings,
            predicted_features=features_desired,
            nmin_datasets_for_train=5,
            nmin_dataset_for_test=2,
            nmax_dataset_for_test=2,
            rawdataset_collection=Dataset,
            trainset_collection=Trainset,
            testresult_collection=TestResult,
            performance = Performance,
            windows = windows, 
            random_state=0, n_estimators=6, max_features=2)
AIU.Run()











