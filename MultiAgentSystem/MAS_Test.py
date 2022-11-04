import pymongo as pymongo
import time
import threading

def Upload(i,Data):
    for dato in Data:
        del dato['_id']
        Collection_up.insert_one(dato)
        print('Feature sent: ' + str(i))
        time.sleep(5)

MongoClient = pymongo.MongoClient('mongodb://localhost:27017/')
Database = MongoClient['InfoDB']

Collection_down = Database['TestResult']
Collection_up = Database['TR']

threads = []
for i in range(1,39,1):
    Data = Collection_down.find({'Predicted_Feature': i}).sort('Last_Testset_Time', pymongo.ASCENDING)
    t=threading.Thread(target=Upload, args=(i,Data))
    t.start()
    threads.append(t)
    

for t in threads:
    t.join()
    print("Joined thread: " + str(t.getName()))




