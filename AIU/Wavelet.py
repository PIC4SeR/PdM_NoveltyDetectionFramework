# %%
import pandas as pd
import numpy as np
import json
import datetime
import matplotlib.pyplot as plt

# %%
Data=pd.read_csv('Dataset_Low_Lubrication_3.txt',delimiter=',')


# %%
array=[]
for i in range(1,667):
    array.append(Data.iloc[1]['ayBearDx_'+ str(i)])
plt.plot(array)


# %%
from scipy.signal import cwt,ricker
wavlet=cwt(array,ricker,np.arange(1,100))
plt.plot(wavlet[10])
print(len(wavlet[1]))

plt.show()


