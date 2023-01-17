from AIU.AI_Unit import AI_Unit
import datetime
import numpy as np
import pymongo
from typing import List
from typing import Dict
import time

# Load a dataset file into defined MongoDB collection
'''import pymongo
def LoadDataset(mongodb_collection: pymongo.collection.Collection, dataset_filepath: str, separator=',', header=None):
    import pandas as pd
    try:
        df = pd.read_csv(dataset_filepath, sep=separator, header=header)
        df['Timestamp'] = df['Timestamp'].apply(lambda strdate: datetime.strptime(strdate, '%d-%b-%Y'))
        df.apply(lambda row: mongodb_collection.insert_one(document=row.to_dict()), axis=1)
    except Exception as e:
        print(e)'''


def Configure_AIU_Remotely(
        predictor_recover_directory: str, dataloge: bool, trained:bool, tested:bool, column_names: [],
        nmin_datasets_for_train: int, nmin_dataset_for_test: int, nmax_dataset_for_test: int, mongo_string: str,
        raw_db: str, info_db: str, rawdataset_collection: str,
        trainset_collection: str, testresult_collection: str, performance_collection: str,
        configuration_collection: str, predicted_features: List[int], windows: int = 20,
        random_state: int = 42, n_estimators: int = 10, max_features: int = 10):
    MongoClient = pymongo.MongoClient('mongodb://localhost:27017')
    InfoDB = MongoClient['InfoDB']
    Configuration_Collection = InfoDB['Configuration']

    conf = {}
    conf['predictor_recover_directory'] = predictor_recover_directory
    conf['column_names'] = column_names
    conf['predicted_features'] = predicted_features
    conf['nmin_datasets_for_train'] = nmin_datasets_for_train
    conf['nmin_dataset_for_test'] = nmin_dataset_for_test
    conf['nmax_dataset_for_test'] = nmax_dataset_for_test
    conf['mongo_string'] = mongo_string
    conf['raw_db'] = raw_db
    conf['info_db'] = info_db
    conf['rawdataset_collection'] = rawdataset_collection
    conf['trainset_collection'] = trainset_collection
    conf['testresult_collection'] = testresult_collection
    conf['performance_collection'] = performance_collection
    conf['configuration_collection'] = configuration_collection
    conf['windows'] = windows
    conf['random_state'] = random_state
    conf['n_estimators'] = n_estimators
    conf['max_features'] = max_features
    conf['trained'] = trained
    conf['tested'] = tested
    conf['dataloge'] = dataloge
    conf['timestamp'] = datetime.datetime.now()

    Configuration_Collection.insert_one(conf)





MongoClient = pymongo.MongoClient('mongodb://localhost:27017')
# Label definition set by user
# label_strings=['ax_a_','ay_a_','ax_b_','ay_b_','ax_c_','ay_c_','ax_d_','ay_d_']
label_strings = ['axBearDx_', 'axBearSx_', 'ayBearDx_', 'ayBearSx_', 'axShaft_', 'ayShaft_']

windows = 20
# Prediction wanted set by user
features_desired = []
for i in range(1, len(label_strings) * windows, 1):
    features_desired.append(i)

Configure_AIU_Remotely(predictor_recover_directory='./Recovery',
              dataloge=True, trained=False, tested=False,
              column_names=label_strings, predicted_features=features_desired,
              nmin_datasets_for_train=5, nmin_dataset_for_test=2,
              nmax_dataset_for_test=2, mongo_string='mongodb://localhost:27017', raw_db='RawDB', info_db='InfoDB',
              rawdataset_collection='Dataset', trainset_collection='Trainset',
              testresult_collection='TestResult', performance_collection='Performance',
              configuration_collection='Configuration',
              windows=windows, random_state=0, n_estimators=6, max_features=2)

AIU = AI_Unit(mongo_string='mongodb://localhost:27017',
              remote_configure=False,
              predictor_recover_directory='./Recovery',
              dataloge=True, trained=False, tested=False,
              column_names=label_strings, predicted_features=features_desired,
              nmin_datasets_for_train=5, nmin_dataset_for_test=2,
              nmax_dataset_for_test=2, raw_db='Raw2DB', info_db='InfoDB',
              rawdataset_collection='DLOW', trainset_collection='Trainset',
              testresult_collection='TestResult', performance_collection='Performance',
              configuration_collection='Configuration',
              windows=windows, random_state=0, n_estimators=6, max_features=2)

#AIU = AI_Unit(mongo_string='mongodb://localhost:27017', info_db='InfoDB', configuration_collection='Configuration', remote_configure=True)

AIU.Run()
