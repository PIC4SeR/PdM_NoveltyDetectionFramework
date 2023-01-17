import fnmatch
from AIU.Predictor import Predictor
from AIU.Logger import Logger
from datetime import datetime
import numpy as np
import pymongo
from typing import List
from typing import Dict
import pandas as pd
import os
import sys

try:
    from msvcrt import kbhit
except ImportError:
    from select import select
    def kbhit():
        dr, dw, de = select([sys.stdin], [], [], 0)
        return dr != []


class AI_Unit:
    def __init__(self,
                 mongo_string: str = 'mongodb://localhost:27017',
                 remote_configure: bool = False,
                 predictor_recover_directory: str = '', dataloge: bool = False, trained:bool = False,
                 tested:bool = False, column_names: List[str] = [],
                 nmin_datasets_for_train: int = 0, nmin_dataset_for_test: int = 0, nmax_dataset_for_test: int = 0,
                 raw_db: str = '',  info_db: str = 'InfoDB', rawdataset_collection: str = '',
                 trainset_collection: str = '', testresult_collection: str = '', performance_collection: str = '', configuration_collection: str = 'Configuration',
                 predicted_features: List[int] = [], windows: int = 0,
                 random_state: int = 0, n_estimators: int = 0, max_features: int = 0):

        if remote_configure:
            self.__mongo_client__ = pymongo.MongoClient(mongo_string)
            self.__infodb__ = self.__mongo_client__[info_db]
            self.__configuration_collection__ = self.__infodb__[configuration_collection]
            self.RemoteConfig()
        else:
            self.LocalConfig(mongo_string=mongo_string,
              predictor_recover_directory=predictor_recover_directory,
              dataloge=dataloge, trained=trained, tested=tested,
              column_names=column_names, predicted_features=predicted_features,
              nmin_datasets_for_train=nmin_datasets_for_train, nmin_dataset_for_test=nmin_dataset_for_test,
              nmax_dataset_for_test=nmax_dataset_for_test, raw_db=raw_db, info_db=info_db,
              rawdataset_collection=rawdataset_collection, trainset_collection=trainset_collection,
              testresult_collection=testresult_collection, performance_collection=performance_collection, configuration_collection=configuration_collection,
              windows=windows, random_state=random_state, n_estimators=n_estimators, max_features=max_features)
            self.SaveConfigurationRemotely()


    def SaveConfigurationRemotely(self):
            conf = {}
            conf['predictor_recover_directory'] = self.__predictor_recover_directory__
            conf['column_names'] = self.__column_names__
            conf['predicted_features'] = self.__predicted_features__
            conf['nmin_datasets_for_train'] = self.__nmin_datasets_for_train__
            conf['nmin_dataset_for_test'] = self.__nmin_datasets_for_test__
            conf['nmax_dataset_for_test'] = self.__nmax_datasets_for_test__
            conf['mongo_string'] = self.__mongo_string__
            conf['raw_db'] = self.__rawdb__.name
            conf['info_db'] = self.__infodb__.name
            conf['rawdataset_collection'] = self.__rawdataset_collection__.name
            conf['trainset_collection'] = self.__trainset_collection__.name
            conf['testresult_collection'] = self.__testresult_collection__.name
            conf['performance_collection'] = self.__performance_collection__.name
            conf['configuration_collection'] = self.__configuration_collection__.name
            conf['windows'] = self.__windows__
            conf['random_state'] = self.__random_state__
            conf['n_estimators'] = self.__n_estimators__
            conf['max_features'] = self.__max_features__
            conf['trained'] = self.__trained__
            conf['tested'] = self.__tested__
            conf['dataloge'] = self.__datalog__
            conf['timestamp'] = datetime.now()
            self.__configuration_collection__.insert_one(conf)

    def LocalConfig(self,
                 mongo_string: str,
                 predictor_recover_directory: str, dataloge: bool, trained:bool,
                 tested:bool, column_names: [],
                 nmin_datasets_for_train: int, nmin_dataset_for_test: int, nmax_dataset_for_test: int,
                 raw_db: str, info_db: str, rawdataset_collection: str,
                 trainset_collection: str, testresult_collection: str, performance_collection: str,
                 configuration_collection: str, predicted_features: List[int], windows: int,
                 random_state: int, n_estimators: int, max_features: int):
        self.__predictor_recover_directory__ = predictor_recover_directory
        self.__trained__ = trained
        self.__tested__ = tested
        self.__datalog__ = dataloge
        self.__nmin_datasets_for_train__ = nmin_datasets_for_train
        self.__nmin_datasets_for_test__ = nmin_dataset_for_test
        self.__nmax_datasets_for_test__ = nmax_dataset_for_test
        # self.__last__ = datetime.now()
        self.__mongo_string__ = mongo_string
        self.__mongo_client__ = pymongo.MongoClient(mongo_string)
        self.__rawdb__ = self.__mongo_client__[raw_db]
        self.__infodb__ = self.__mongo_client__[info_db]
        self.__rawdataset_collection__ = self.__rawdb__[rawdataset_collection]
        self.__trainset_collection__ = self.__infodb__[trainset_collection]
        self.__testresult_collection__ = self.__infodb__[testresult_collection]
        self.__performance_collection__ = self.__infodb__[performance_collection]
        self.__configuration_collection__ = self.__infodb__[configuration_collection]
        self.__predictors__ = []
        self.__column_names__ = column_names
        self.__windows__ = windows
        self.__starttime__ = datetime.now()
        self.__predicted_features__ = predicted_features
        self.__random_state__ = random_state
        self.__n_estimators__ = n_estimators
        self.__max_features__ = max_features

        Logger.datalog = self.__datalog__
        if not os.path.isdir(self.__predictor_recover_directory__):
            os.mkdir(self.__predictor_recover_directory__)

        for pf in self.__predicted_features__:
            self.__predictors__.append(Predictor(model_recover_directory=predictor_recover_directory,
                                                 model_recover_filename='Predictor_' + str(pf) + '_Model.sav',
                                                 predicted_feature=pf,
                                                 testresult_collection=self.__testresult_collection__,
                                                 random_state=random_state, n_estimators=n_estimators,
                                                 max_features=max_features))

    def RemoteConfig(self):
        configuration = self.__configuration_collection__.find({}).sort('timestamp', pymongo.DESCENDING).limit(1)[0]
        self.__predictor_recover_directory__ = configuration['predictor_recover_directory']
        self.__trained__ = configuration['trained']
        self.__tested__ = configuration['tested']
        self.__datalog__ = configuration['dataloge']
        self.__nmin_datasets_for_train__ = configuration['nmin_datasets_for_train']
        self.__nmin_datasets_for_test__ = configuration['nmin_dataset_for_test']
        self.__nmax_datasets_for_test__ = configuration['nmax_dataset_for_test']
        self.__mongo_string__ = configuration['mongo_string']
        self.__mongo_client__ = pymongo.MongoClient(self.__mongo_string__)
        self.__rawdb__ = self.__mongo_client__[configuration['raw_db']]
        self.__infodb__ = self.__mongo_client__[configuration['info_db']]
        self.__rawdataset_collection__ = self.__rawdb__[configuration['rawdataset_collection']]
        self.__trainset_collection__ = self.__infodb__[configuration['trainset_collection']]
        self.__testresult_collection__ = self.__infodb__[configuration['testresult_collection']]
        self.__performance_collection__ = self.__infodb__[configuration['performance_collection']]
        self.__configuration_collection__ = self.__infodb__[configuration['configuration_collection']]
        self.__predictors__ = []
        self.__column_names__ = configuration['column_names']
        self.__windows__ = configuration['windows']
        self.__starttime__ = datetime.now()
        self.__random_state__ = configuration['random_state']
        self.__n_estimators__ = configuration['n_estimators']
        self.__max_features__ = configuration['max_features']
        self.__predicted_features__ = configuration['predicted_features']

        Logger.datalog = self.__datalog__
        if not os.path.isdir(self.__predictor_recover_directory__):
            os.mkdir(self.__predictor_recover_directory__)

        for pf in self.__predicted_features__:
            self.__predictors__.append(Predictor(model_recover_directory=self.__predictor_recover_directory__,
                                                 model_recover_filename='Predictor_' + str(pf) + '_Model.sav',
                                                 predicted_feature=pf,
                                                 testresult_collection=self.__testresult_collection__,
                                                 random_state=self.__random_state__, n_estimators=self.__n_estimators__,
                                                 max_features=self.__max_features__))




    def LoadPredictor(self, predictor: Predictor):
        try:
            self.__predictors__.append(predictor)
        except Exception as e:
            print(e)
            raise e

    def Update(self):
        trainsets = []
        for dataset in self.__trainset_collection__.find().sort('TimestampUnix', pymongo.ASCENDING):
            trainsets.append(dataset)
        for dataset in self.__rawdataset_collection__.find(
                {'TimestampUnix': {'$gt': self.__last__}}, {'_id': False}).sort('TimestampUnix',
                                pymongo.ASCENDING).limit(self.__nmin_datasets_for_train__):
                self.__trainset_collection__.insert_one(dataset)
                trainsets.append(dataset)
        self.Train(trainsets=trainsets)

    def Retrain(self):
        self.__trainset_collection__.drop()
        self.__testresult_collection__.drop()
        trainsets = []
        for dataset in self.__rawdataset_collection__.find({'TimestampUnix': {'$gt': self.__last__}},
                                {'_id': False}).sort('TimestampUnix',
                                        pymongo.ASCENDING).limit(self.__nmin_datasets_for_train__):
            self.__trainset_collection__.insert_one(dataset)
            trainsets.append(dataset)
        self.Train(trainsets=trainsets)

    def Recover(self):
        if self.__datalog__:
            recoverystarttime = (datetime.now() - self.__starttime__).total_seconds()


        if os.path.isdir(self.__predictor_recover_directory__) and len(os.listdir(self.__predictor_recover_directory__)) == len(self.__predicted_features__):
            recoverers = []
            for predictor in self.__predictors__:
                recoverer = predictor.Recoverer()
                recoverers.append(recoverer)
            for recoverer in recoverers:
                recoverer.join()
            trained = True
            for predictor in self.__predictors__:
                if not predictor.__trained__:
                    trained = False
            self.__trained__ = trained

        last_train = 0
        if not self.__trained__:
            trainsets = []
            if self.__trainset_collection__.count_documents({}) > 0:
                for dataset in self.__trainset_collection__.find({},{'_id': False}).sort('TimestampUnix', pymongo.ASCENDING):
                    trainsets.append(dataset)
                last_train = self.Train(trainsets=trainsets)
            elif self.__rawdataset_collection__.count_documents({}) > 0:
                for dataset in self.__rawdataset_collection__.find({},{'_id': False}).sort('TimestampUnix', pymongo.ASCENDING).limit(self.__nmin_datasets_for_train__):
                    trainsets.append(dataset)
                    self.__trainset_collection__.insert_one(dataset)
                last_train = self.Train(trainsets=trainsets)


        if self.__testresult_collection__.count_documents({}) > 0:
            last = self.__testresult_collection__.find_one(sort=[('TimestampUnix', pymongo.DESCENDING)])['TimestampUnix']
        elif self.__trainset_collection__.count_documents({}) > 0:
            last = self.__trainset_collection__.find_one(sort=[('TimestampUnix', pymongo.DESCENDING)])['TimestampUnix']
        elif  self.__rawdataset_collection__.count_documents({}) > 0:
            last = self.__rawdataset_collection__.find({},{'_id': False}).sort('TimestampUnix', pymongo.ASCENDING).limit(self.__nmin_datasets_for_train__)[self.__nmin_datasets_for_train__ - 1]['TimestampUnix']
        else:
            last = last_train

        if self.__datalog__:
            recoveryendtime = (datetime.now() - self.__starttime__).total_seconds()
            Logger.MongoLogger(self.__performance_collection__, {'Recovery_Time': recoveryendtime - recoverystarttime})

        return last

    def Train(self, trainsets: List[List[float]]):
        complete_trainset = self.Preprocess(datasets=trainsets)
        trainers = []
        if self.__datalog__:
            starttraintime = (datetime.now() - self.__starttime__).total_seconds()

        for predictor in self.__predictors__:
            trainer = predictor.Trainer(trainset=complete_trainset)
            trainers.append(trainer)

        for trainer in trainers:
            trainer.join()

        trained = True
        for predictor in self.__predictors__:
            if not predictor.__trained__:
                trained = False

        self.__trained__ = trained

        if self.__datalog__:
            endtraintime = (datetime.now() - self.__starttime__).total_seconds()
            Logger.MongoLogger(self.__performance_collection__, {'Train_Time': endtraintime - starttraintime})

        return trainsets[len(trainsets) - 1]['TimestampUnix']

    def Test(self):
        if self.__rawdataset_collection__.count_documents(
                {'TimestampUnix': {'$gt': self.__last__}}) > self.__nmin_datasets_for_test__:
            testsets = []
            for dataset in self.__rawdataset_collection__.find({'TimestampUnix': {'$gt': self.__last__}},
                                                               {'_id': False}).sort('TimestampUnix',
                                                                                    pymongo.ASCENDING).limit(
                self.__nmax_datasets_for_test__):
                testsets.append(dataset)

            complete_testset = self.Preprocess(datasets=testsets)
            last_testset_time = testsets[len(testsets) - 1]['TimestampUnix']
            testers = []

            if self.__datalog__:
                startpredicttime = (datetime.now() - self.__starttime__).total_seconds()

            for predictor in self.__predictors__:
                tester = predictor.Tester(testset=complete_testset, last_testset_time=last_testset_time)
                testers.append(tester)

            if self.__datalog__:
                endpredicttime = (datetime.now() - self.__starttime__).total_seconds()
                Logger.MongoLogger(self.__performance_collection__, {'Test_Time': endpredicttime - startpredicttime})
            return last_testset_time

    def Preprocess(self, datasets):

        if self.__datalog__:
            startpreprocesstime = (datetime.now() - self.__starttime__).total_seconds()

        complete_dataset = []
        for dataset in datasets:
            ds = dataset.copy()
            del ds['Timestamp']
            del ds['TimestampUnix']
            data = pd.DataFrame(ds, index=[0])
            names = []
            for string in self.__column_names__:
                for i in range(1, self.__windows__ + 1):
                    names.append(str(string + str(i)))
            column_list = data.columns.values
            dato = []
            for string in self.__column_names__:
                count = 0
                for element in column_list:
                    if string in element:
                        count = count + 1
                dato.append(data.columns.get_loc(string + '1'))
                dato.append(data.columns.get_loc(string + str(count)))

            New_Dataframe = pd.DataFrame()
            for j in range(0, len(data), 1):
                sum_abs = []
                for i in range(0, len(dato), 2):
                    Data_Filtered = data.iloc[j, dato[i]:dato[i + 1]]  # Data isolation between indexes chosen before
                    array_windowed = np.array_split(np.array(Data_Filtered.values), self.__windows__)  # Windowing
                    for element in array_windowed:
                        sum_abs.append(np.sum(np.absolute(element)))  # Absolute sum for each window
                All_Data = pd.DataFrame(np.array(sum_abs).reshape(1, -1), columns=names)
                New_Dataframe = pd.concat([New_Dataframe, All_Data], axis=0)
            New_Dataframe = New_Dataframe.reset_index(drop=True)
            # New_Dataframe['Timestamp']=data['Timestamp']
            complete_dataset.append(list(New_Dataframe.values[0]))

        if self.__datalog__:
            endpreprocesstime = (datetime.now() - self.__starttime__).total_seconds()
            Logger.MongoLogger(self.__performance_collection__, {'PreprocessTrain_Time': endpreprocesstime - startpreprocesstime})
        return complete_dataset

    def Run(self):
        try:
            print('Check for existing saved models before training')
            self.__last__ = self.Recover()
            # Initial Train - skipped if Model has been recovered
            while not self.__trained__:
                print('Wait at least for ' + str(self.__nmin_datasets_for_train__) + ' datasets before training')
                trainsets = []
                if self.__rawdataset_collection__.count_documents(
                        {}) >= self.__nmin_datasets_for_train__ and not self.__trained__:
                    for dataset in self.__rawdataset_collection__.find({}, {'_id': False}).sort('TimestampUnix',
                                                        pymongo.ASCENDING).limit(self.__nmin_datasets_for_train__):
                        self.__trainset_collection__.insert_one(dataset)
                        trainsets.append(dataset)
                    self.__last__ = self.Train(trainsets)
            # Testing Loop
            while self.__trained__:
                self.__last__ = self.Test()
                self.ExecuteCommand()
        except Exception as e:
            print(e)

    def ExecuteCommand(self):
        if kbhit():
            key = input()
            if key == 'U':
                print("Update")
                self.Update()
            elif key == 'R':
                print("Retrain")
                self.Retrain()
            elif key == 'C':
                print("Recover")
                self.Recover()
            elif key == 'T':
                print("Remote Configuration")
                self.RemoteConfig()
        return

