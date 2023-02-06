> Project: "Novelty Detection Framework"

> Owner: "Giuseppe Pedone, Umberto Albertin" 

> Date: "2023:01" 

---

# Automatic Row Crop Generator User Guide

## Licence 

All the code and models inside this repository are released under the MIT Licence.

```
MIT License

Copyright (c) 2022 Marco Ambrosio, Brenno Tuberga

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following  conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
```

## Description of the project

The code in this repository provides useful tools to automatically generate models of row-based crops for Blender and Gazebo.

## Installation procedure

###  NDF Installation and Requirements

In order to install the framework, you need to perform the following tasks:
	-   Install MongoDB on your system.
	-   Launch the mongod deamon as superuser.
	-   Install, using 'pip install -r requirements.txt'.
	-   Install the last version of Boost Library on your system.
	-   Install libmongocxx version 3.6.7 or later.
	-   Install libbsoncxx version 3.6.7 or later.
	-   Install libmongoc-1.0.
	-   Install libbson-1.0.
	-   Install the last version of OpenSSL.
  
## User Guide

### NDF Configuration

The NDF configuration is based on local/remote json files, like `configuration.json` file found into the directory tree of of the application.

### Configuration structure for NDF (AIU and Agents):

```
{
	predictor_recover_directory:"./Recovery",     //setting the path of Recovery directory
	column_names:predicted_features,				      //setting the predicted features names
	nmin_datasets_for_train:5,					          //set the minimum number of dataset to read before start the training process
	nmin_dataset_for_test:2,					            //set the minimum number of dataset to read before start the testing process
	nmax_dataset_for_test:2,					            //set the maximum number of dataset to read before start the testing process
	mongo_string:mongodb://localhost:27017				//set the mongodb URI string used to connect to mongod daemon
	raw_db:RawDB,							                    //set the name of raw data database 
	info_db:InfoDB,							                  //set the name of info data database
	rawdataset_collection:Dataset,					      //set the name of raw data collection
	trainset_collection:Trainset,,					      //set the name of trained data collection
	testresult_collection:TestResult,				      //set the name of test result collection
	performance_collection:Performance,,				  //set the name of performance collection
	configuration_collection:Configuration,				//set the name of configuration collection
	windows:20,	,						                      //set the number of time consecutive samples considered into a record 
	random_state:0,		,					                  //set the initial random state of the Random Forest Regressor
	n_estimators:6,							                  //set the number of estimators of the Random Forest Regressor
	max_features:2,							                  //set the max features
	trained:false,							                  //identify the trained phase for AIU
	tested:false,							                    //identify the testing phase of AIU
	dataloge:true,							                  //set log enable
	timestamp:2022-11-19T03:06:35.397+00:00,			//set the time of current configuration
	agents: 							                        //Identify the agents and their configurations. Set an Agent ID in in order to add a new agent
		AG0:{
			num_samples_read:10,				                  //set the number of samples to read before performing an action
			predictor:7,					                        //set the predictor number that is managed by the agent, used to get properly data.
			predicted_error_type:MAE,			                //set the type of error used by the agent for taking decisions, used to get properly data.
			predicted_error_value:100.1,			            //set the error value used by the agent for time predictions, used to predict a time of error.
			min_operative_threshold_error:20.2,		        //set the minimum error value to reach in order to activate the agent computation, if it is not reached, the agents do nothing.
			max_operative_threshold_error:80.2,		        //set the maximum error value to reach in order to activate the agent computation, if it is overcomed, the agents do nothing.
			min_num_of_regr_samples:10,			              //set the minimum number of samples to use for regression, if it is not reached, the prediction is not performed.
			prevention_threshold_time:68000000000,		    //set the duration, in milliseconds, of enduser notification threshold. It sets the minimum time duration within a certain model changes have to occur in order to notify the enduser.  
			test_result_collection:TestResult,		        //set the name of test result collection
			prediction_result_collection:Prediction,	    //set the name of prediction collection
			database_name:InfoDB,				                  //set the name of info data database
			mongo_driver_remote_connection_type:mongodb,	//set the mongodb connection type
			mongodriver_remote_connection_host:localhost,	//set the mongodb connection host
			mongo_driver_remote_connection_port:27017,	  //set the mongodb connection port
			agent_id:AG0,					                        //set the agent id used to attach the right configuration to the right agent
			agent_name:Agent 0,				                    //set the agent name
			step_run_time:5000,				                    //set the agent execution step time. Agent will be executed and ready every t+Step milliseconds 
			priority:4,					//set the execution order priority used to sort the executing agent queue. 
			stopped:false}					//set the agent stop state. If it will be true, you need to activate the agent via code with agent.fActivate() function, called inside the Agent Manager costructor. 
}
```


### Run NDF

After setting the parameters into the local `configuration.json` file, or remotly on mongodb configuration collection, open a root terminal and run the following command:

```
cd <PATH TO ROOT OF NDF>

```


and the On Linux:

```
./Demo.sh

```

On Windows:

```
 .\\Demo.bat

```

## How to interact with AIU

After the NDF is running, you can interact with NDFs AIU using keyboard commands. Type:


```
	- 'R' on the AIU Shell to retrain the NDF.
	- 'U' on the AIU Shell to update the NDF.
	- 'S' on the AIU Shell to save a new NDF Configuration.
	- 'T' on the AIU Shell to load the last NDF Configuration.
	- 'P' on the Plot Shell to plot all NDF predictions.
	- 'Q' on the Plot Shell to close the Plot Shell.
  
```


## Authors

Marco Ambrosio, Brenno Tuberga
