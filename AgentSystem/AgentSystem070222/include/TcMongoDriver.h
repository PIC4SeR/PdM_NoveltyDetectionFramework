/**
	@file TcMongoDriver.h
	@author Giuseppe Pedone
	@version 2.0 04/05/2021
*/
#pragma once
#ifndef TCMONGODRIVER_H
#define TCMONGODRIVER_H

	#include <inttypes.h>
	#include <string>
	#include <list>
	#include <mongocxx/instance.hpp>
	#include <mongocxx/pool.hpp>
	#include <mongocxx/exception/exception.hpp>
	#include <mongocxx/exception/bulk_write_exception.hpp>
	#include <mongocxx/pipeline.hpp>
	#include <boost/optional.hpp>
	#include <json/json.h>
	#include <bsoncxx/document/value.hpp>
	#include <bsoncxx/exception/exception.hpp>	
	#include <bsoncxx/document/view_or_value.hpp>
	#include <bsoncxx/builder/stream/array.hpp>
	#include <bsoncxx/builder/stream/document.hpp>

	#include <mongocxx/client.hpp>
	#include <mongocxx/database.hpp>
	#include <mongocxx/cursor.hpp>
	#include <mongocxx/collection.hpp>
	#include <mongocxx/exception/operation_exception.hpp>
	#include <mongocxx/exception/query_exception.hpp>
	#include <bsoncxx/builder/stream/document.hpp>
	#include <bsoncxx/exception/exception.hpp>
	#include <bsoncxx/json.hpp>
	#include <mongocxx/result/insert_one.hpp>
	#include <mongocxx/exception/bulk_write_exception.hpp>
	
	#pragma region Internal Error Codes
	constexpr int8_t kCLIENTACQUIRE_FAILS = -1;
	constexpr int8_t kCLIENTACQUIRE_SUCCESS = 0;
	constexpr int8_t kDATABASEEXIST_CREATE_FAILS = -2;
	constexpr int8_t kDATABASEEXIST_CREATE_SUCCESS = 1;
	constexpr int8_t kDATABASEEXIST_FAILS = -1;
	constexpr int8_t kDATABASEEXIST_SUCCESS = 0;
	constexpr int8_t kCOLLECTIONEXIST_CREATE_FAILS = -2;
	constexpr int8_t kCOLLECTIONEXIST_CREATE_SUCCESS = 1;
	constexpr int8_t kCOLLECTIONEXIST_FAILS = -1;
	constexpr int8_t kCOLLECTIONEXIST_SUCCESS = 0;
	#pragma endregion

	#pragma region External Error Codes
	constexpr int8_t kCONNECT_MEMALLOC_FAILS = -1;
	constexpr int8_t kCONNECT_OPERATION_FAILS = -2;
	constexpr int8_t kCONNECT_OPERATION_SUCCESS = 0;
	constexpr int8_t kINSERT_CLIENTACQUIRE_FAILS = -1;
	constexpr int8_t kINSERT_DATABASEEXIST_FAILS = -2;
	constexpr int8_t kINSERT_COLLECTIONEXIST_FAILS = -3;
	constexpr int8_t kINSERT_OPERATION_FAILS = -4;
	constexpr int8_t kINSERT_OPERATION_SUCCESS = 0;
	#pragma endregion

	using namespace std;
	
	class TcMongoDriver
	{
	private:


		#pragma region Internal class Attributes
				inline static mongocxx::instance* cmMongoDriverInstance = new mongocxx::instance();
		#pragma endregion

		#pragma region Internal Attributes
				string rmMongoDriverName;
				string rmMongoDriverRemoteConnectionType;
				string rmMongoDriverRemoteConnectionHost;
				uint16_t rmMongoDriverRemoteConnectionPort;
				mongocxx::pool* cmMongoDriverConnectionPool;
		#pragma endregion


		#pragma region Internal Functions

				/**
				* Acquire a Mongo Entry from Mongo Connection Pool.
				*
				* This Function try to acquire (N times) a mongocxx::pool::entry object pointer.
				* @param[out] pOptionalMongoEntry the boost::optional<mongocxx::pool::entry> object pointer that will be returned.
				* @returns Integer Error Code:
				*      - kCLIENTACQUIRE_FAILS if cannot acquire a Mongo Entry.
				*      - kCLIENTACQUIRE_SUCCESS if can acquire a Mongo Entry.
				*/
				int fClientAcquire(boost::optional<mongocxx::pool::entry>* pOptionalMongoEntry);

				/**
				* Verify if a the given Database exists. If it does not exists, it will be created.
				*
				* @param[in] pDatabase the string object that represents the Databasename to verify.
				* @param[in] pMongoClient the mongocxx::client object that has been acquired.
				* @returns Integer Error Code:
				*      - kDATABASEEXIST_CREATE_FAILS if the Database does not exists and its creation fails.
				*      - kDATABASEEXIST_CREATE_SUCCESS if the Database does not exists and its creation success.
				*      - kDATABASEEXIST_FAILS if an exception has been thrown.
				*      - kDATABASEEXIST_SUCCESS if the Database exists.
				*/
				int fDatabaseExist(string pDatabase, mongocxx::client& pMongoClient);


				/**
				* Verify if a the given Database exists. If it does not exists, it will be created.
				*
				* @param[in] pCollection the string object that represents the Databasename to verify.
				* @param[in] pMongoDatabase the mongocxx::database object that has been acquired.
				* @returns Integer Error Code:
				*      - kCOLLECTIONEXIST_CREATE_FAILS if the Collection does not exists and its creation fails.
				*      - kCOLLECTIONEXIST_CREATE_SUCCESS if the Collection does not exists and its creation success.
				*      - kCOLLECTIONEXIST_FAILS if a mongo exception has been thrown.
				*      - kCOLLECTIONEXIST_SUCCESS if the Collection exists.
				*/
				int fCollectionExist(string pCollection, mongocxx::database pMongoDatabase);
	#pragma endregion

	public:





		#pragma region External Functions

				TcMongoDriver(string pMongoDriverName, string pMongoDriverRemoteConnectionType, string pMongoDriverRemoteConnectionHost, uint16_t pMongoDriverRemoteConnectionPort);
				~TcMongoDriver();

				/**
				* Creates a Mongo Instance and a Mongo Connection Pool.
				*
				* @returns Integer Error Code:
				*      - kCONNECT_MEMALLOC_FAILS if an memory allocation fails.
				*      - kCONNECT_OPERATION_FAILS if a a mongo excpetion has been thrown.
				*      - kCONNECT_OPERATION_SUCCESS if all operations success.
				*/
				int fConnect();


				/**
				* Creates a Mongo Instance and a Mongo Connection Pool.
				*
				* @returns Integer Error Code:
				*      - kCONNECT_MEMALLOC_FAILS if an memory allocation fails.
				*      - kCONNECT_OPERATION_FAILS if a a mongo excpetion has been thrown.
				*      - kCONNECT_OPERATION_SUCCESS if all operations success.
				*/
				void fDisconnect();



				/**
				* Insert a Document into a Mongo Collection that is contained in a Mongo Database.
				*
				* @returns Integer Error Code:
				*      - kINSERT_CLIENTACQUIRE_FAILS if the acquisition of a Mongo Entry fails.
				*      - kINSERT_DATABASEEXIST_FAILS if the Database verification fails.
				*      - kINSERT_COLLECTIONEXIST_FAILS if the Collection verification fails.
				*      - kINSERT_OPERATION_FAILS if a a mongo excpetion has been thrown.
				*      - kINSERT_OPERATION_SUCCESS if all operations success.
				*/
				int fInserDocument(string pDatabase, string pCollection, string pDocument);

			    /**
				* Insert a Document List into a Mongo Collection that is contained in a Mongo Database.
				*
				* @returns Integer Error Code:
				*      - kINSERT_CLIENTACQUIRE_FAILS if the acquisition of a Mongo Entry fails.
				*      - kINSERT_DATABASEEXIST_FAILS if the Database verification fails.
				*      - kINSERT_COLLECTIONEXIST_FAILS if the Collection verification fails.
				*      - kINSERT_OPERATION_FAILS if a a mongo excpetion has been thrown.
				*      - kINSERT_OPERATION_SUCCESS if all operations success.
				*/
				int fInsertDocumentList(string pDatabase, string pCollection, list<string> pDocuments);

				
			
				//Int InsertData(String database, String collection, T data);
				//Int InsertDataList(String database, String collection, List<T> data);
				//int ReplaceDocument(string pDatabase, string pCollection, string filter, string pDocument);
				//int ReplaceDocumentList(string pDatabase, string pCollection, string filter, list<string> pDocuments);
				//Int ReplaceData(String database, String collection, String filter, T data);
				//Int ReplaceDataList(String database, String collection, String filter, List<T> data);
				int fRunQuery(list<string>* pOutputList, string pDatabase, string pCollection, string pFilter = "", string pProjection = "", string pSortcriteria = "", int pSkip = 0, int pLimit = 0, string pGroupcriteria = "", string pAddfieldscriteria = "");
		

				int fRunQuery(list<string>* pOutputList, string pDatabase, string pCollection, bsoncxx::document::view_or_value pFilter = bsoncxx::document::view_or_value(), bsoncxx::document::view_or_value pProjection = bsoncxx::document::view_or_value(), bsoncxx::document::view_or_value pSortcriteria = bsoncxx::document::view_or_value(), int pSkip = 0, int pLimit = 0, bsoncxx::document::view_or_value pGroupcriteria = bsoncxx::document::view_or_value());



	#pragma endregion


	/**
				* Insert a Document into a Mongo Collection that is contained in a Mongo Database.
				*
				* @returns Integer Error Code:
				*      - kINSERT_CLIENTACQUIRE_FAILS if the acquisition of a Mongo Entry fails.
				*      - kINSERT_DATABASEEXIST_FAILS if the Database verification fails.
				*      - kINSERT_COLLECTIONEXIST_FAILS if the Collection verification fails.
				*      - kINSERT_OPERATION_FAILS if a a mongo excpetion has been thrown.
				*      - kINSERT_OPERATION_SUCCESS if all operations success.
				*/
				template<class T>
				int fInserDocument(string pDatabase, string pCollection, T pDocument){
					boost::optional<mongocxx::pool::entry> cOptionalMongoEntry;
					mongocxx::database cMongoDatabase;
					mongocxx::collection cMongoCollection;
					bsoncxx::document::view_or_value cBsonDocument;
					int rResult = 0;

					rResult = fClientAcquire(&cOptionalMongoEntry);
					if (rResult == kCLIENTACQUIRE_FAILS) {
						fprintf(stdout, "[fInsertDocument] Client acquire fails\n");
						fflush(stdout);
						return(kINSERT_CLIENTACQUIRE_FAILS);
					}

					mongocxx::client& cMongoClient = **cOptionalMongoEntry;
	
					rResult = fDatabaseExist(pDatabase, cMongoClient);
					if (rResult == kDATABASEEXIST_CREATE_FAILS) {
						fprintf(stdout, "[fInsertDocument] Database %s not exist, creation fails\n", pDatabase.c_str());
						fflush(stdout);
						return(kINSERT_DATABASEEXIST_FAILS);
					} else if (rResult == kDATABASEEXIST_FAILS) {
						fprintf(stdout, "[fInsertDocument] Database %s not exist, creation fails\n", pDatabase.c_str());
						fflush(stdout);
						return(kINSERT_DATABASEEXIST_FAILS);
					}
	
					cMongoDatabase = cMongoClient[pDatabase];
					rResult = fCollectionExist(pCollection, cMongoDatabase);
					if (rResult == kCOLLECTIONEXIST_CREATE_FAILS) {
						fprintf(stdout, "[fInsertDocument] Collection %s not exist, creation fails\n", pCollection.c_str());
						fflush(stdout);
						return(kINSERT_COLLECTIONEXIST_FAILS);
					} else if (rResult == kCOLLECTIONEXIST_FAILS) {
						fprintf(stdout, "[fInsertDocument] Collection %s not exist, creation fails\n", pCollection.c_str());
						fflush(stdout);
						return(kINSERT_COLLECTIONEXIST_FAILS);
					}
	
					cMongoCollection = cMongoDatabase[pCollection];
	
					try {
						cBsonDocument << pDocument;
					} catch (bsoncxx::exception e) {
						cBsonDocument = bsoncxx::builder::stream::document{} << "Value" << pDocument << bsoncxx::builder::stream::finalize;
					}
	
					try {
						boost::optional<mongocxx::result::insert_one> cResult = cMongoCollection.insert_one(cBsonDocument.view());
						if (!cResult) {
							return(kINSERT_OPERATION_FAILS);
						} else {
							return(kINSERT_OPERATION_SUCCESS);
						}
					} catch (mongocxx::bulk_write_exception e) {
						printf("[fInsertDocument] Catched mongocxx::bulk_write_exception - Message %s Value %d Category %s\n", e.code().message().c_str(), e.code().value(), e.code().category().name());
						fflush(stdout);
						return(kINSERT_OPERATION_FAILS);
					} catch (mongocxx::exception e) {
						fprintf(stdout, "[fInsertDocument] Catched mongocxx::exception - Message %s Value %d Category %s\n", e.code().message().c_str(), e.code().value(), e.code().category().name());
						fflush(stdout);
						return(kINSERT_OPERATION_FAILS);
					}
				}

			    /**
				* Insert a Document List into a Mongo Collection that is contained in a Mongo Database.
				*
				* @returns Integer Error Code:
				*      - kINSERT_CLIENTACQUIRE_FAILS if the acquisition of a Mongo Entry fails.
				*      - kINSERT_DATABASEEXIST_FAILS if the Database verification fails.
				*      - kINSERT_COLLECTIONEXIST_FAILS if the Collection verification fails.
				*      - kINSERT_OPERATION_FAILS if a a mongo excpetion has been thrown.
				*      - kINSERT_OPERATION_SUCCESS if all operations success.
				*/
				template<class T>
				int fInsertDocumentList(string pDatabase, string pCollection, list<T> pDocuments){
					boost::optional<mongocxx::pool::entry> cOptionalMongoEntry;
					mongocxx::database cMongoDatabase;
					mongocxx::collection cMongoCollection;
					bsoncxx::document::view_or_value cBsonDocument;
					list<bsoncxx::document::view_or_value> cBsonDocuments;
	
					int rResult = 0;

					rResult = fClientAcquire(&cOptionalMongoEntry);
					if (rResult == kCLIENTACQUIRE_FAILS) {
						fprintf(stdout, "[fInsertDocument] Client acquire fails\n");
						fflush(stdout);
						return(kINSERT_CLIENTACQUIRE_FAILS);
					}

					mongocxx::client& cMongoClient = **cOptionalMongoEntry;
	
					rResult = fDatabaseExist(pDatabase, cMongoClient);
					if (rResult == kDATABASEEXIST_CREATE_FAILS) {
						fprintf(stdout, "[fInsertDocument] Database %s not exist, creation fails\n", pDatabase.c_str());
						fflush(stdout);
						return(kINSERT_DATABASEEXIST_FAILS);
					} else if (rResult == kDATABASEEXIST_FAILS) {
						fprintf(stdout, "[fInsertDocument] Database %s not exist, creation fails\n", pDatabase.c_str());
						fflush(stdout);
						return(kINSERT_DATABASEEXIST_FAILS);
					}
	
					cMongoDatabase = cMongoClient[pDatabase];
					rResult = fCollectionExist(pCollection, cMongoDatabase);
					if (rResult == kCOLLECTIONEXIST_CREATE_FAILS) {
						fprintf(stdout, "[fInsertDocument] Collection %s not exist, creation fails\n", pCollection.c_str());
						fflush(stdout);
						return(kINSERT_COLLECTIONEXIST_FAILS);
					} else if (rResult == kCOLLECTIONEXIST_FAILS) {
						fprintf(stdout, "[fInsertDocument] Collection %s not exist, creation fails\n", pCollection.c_str());
						fflush(stdout);
						return(kINSERT_COLLECTIONEXIST_FAILS);
					}
	
					cMongoCollection = cMongoDatabase[pCollection];

	
					for (T cDocument : pDocuments) {
						try {
							cBsonDocument << cDocument;
						} catch (bsoncxx::exception e) {
							cBsonDocument = bsoncxx::builder::stream::document{} << "Value" << cDocument << bsoncxx::builder::stream::finalize;
						}
						cBsonDocuments.push_back(cBsonDocument);
					}

					try {
						boost::optional<mongocxx::result::insert_many> cResult = cMongoCollection.insert_many(cBsonDocuments);
						if (!cResult) {
							return(kINSERT_OPERATION_FAILS);
						}
						else {
							return(kINSERT_OPERATION_SUCCESS);
						}
					}
					catch (mongocxx::bulk_write_exception e) {
						printf("[fInsertDocumentList] Catched mongocxx::bulk_write_exception - Message %s Value %d Category %s\n", e.code().message().c_str(), e.code().value(), e.code().category().name());
						fflush(stdout);
						return(kINSERT_OPERATION_FAILS);
					}
					catch (mongocxx::exception e) {
						fprintf(stdout, "[fInsertDocumentList] Catched mongocxx::exception - Message %s Value %d Category %s\n", e.code().message().c_str(), e.code().value(), e.code().category().name());
						fflush(stdout);
						return(kINSERT_OPERATION_FAILS);
					}
				}



				template<class T>
				int fRunQuery(list<T>* pOutputList, string pDatabase, string pCollection, string pFilter, string pProjection, string pSortcriteria, int pSkip, int pLimit, string pGroupcriteria, string pAddfieldscriteria = "");
	};




				template<class T>
				int TcMongoDriver::fRunQuery(list<T>* pOutputList, string pDatabase, string pCollection, string pFilter, string pProjection, string pSortcriteria, int pSkip, int pLimit, string pGroupcriteria, string pAddfieldscriteria) {
					boost::optional<mongocxx::pool::entry> cOptionalMongoEntry;
					mongocxx::database cMongoDatabase;
					mongocxx::collection cMongoCollection;
					bsoncxx::document::view_or_value cQueryFilter;
					bsoncxx::document::view_or_value cQueryProjection;
					bsoncxx::document::view_or_value cQuerySortcriteria;
					bsoncxx::document::view_or_value cQueryGroupcriteria;
					bsoncxx::document::view_or_value cQueryAddFields;
					mongocxx::pipeline cQueryPipeline;
					list<T> cOutputList;
					int rResult = 0;

					

					rResult = fClientAcquire(&cOptionalMongoEntry);
					if (rResult == kCLIENTACQUIRE_FAILS) {
						fprintf(stdout, "[fInsertDocumentList] Client acquire fails\n");
						fflush(stdout);
						return(kINSERT_CLIENTACQUIRE_FAILS);
					}

					mongocxx::client& cMongoClient = **cOptionalMongoEntry;

					rResult = fDatabaseExist(pDatabase, cMongoClient);
					if (rResult == kDATABASEEXIST_CREATE_FAILS) {
						fprintf(stdout, "[fInsertDocumentList] Database %s not exist, creation fails\n", pDatabase.c_str());
						fflush(stdout);
						return(-1);
					}
					else if (rResult == kDATABASEEXIST_FAILS) {
						fprintf(stdout, "[fInsertDocumentList] Database %s not exist, creation fails\n", pDatabase.c_str());
						fflush(stdout);
						return(-1);
					}

					cMongoDatabase = cMongoClient[pDatabase];
					rResult = fCollectionExist(pCollection, cMongoDatabase);
					if (rResult == kCOLLECTIONEXIST_CREATE_FAILS) {
						fprintf(stdout, "[fInsertDocumentList] Collection %s not exist, creation fails\n", pCollection.c_str());
						fflush(stdout);
						return(kINSERT_COLLECTIONEXIST_FAILS);
					}
					else if (rResult == kCOLLECTIONEXIST_FAILS) {
						fprintf(stdout, "[fInsertDocumentList] Collection %s not exist, creation fails\n", pCollection.c_str());
						fflush(stdout);
						return(-1);
					}

					cMongoCollection = cMongoDatabase[pCollection];
					if (pFilter != "") {
						cQueryFilter = bsoncxx::from_json(pFilter);
						cQueryPipeline.match(cQueryFilter);
					}
					if (pSortcriteria != "") {
						cQuerySortcriteria = bsoncxx::from_json(pSortcriteria);
						cQueryPipeline.sort(cQuerySortcriteria);
					}
					if (pLimit != 0) {
						cQueryPipeline.limit(pLimit);
					}
					if (pSkip != 0) {
						cQueryPipeline.skip(pSkip);
					}
					if (pGroupcriteria != "") {
						cQueryGroupcriteria = bsoncxx::from_json(pGroupcriteria);
						cQueryPipeline.group(cQueryGroupcriteria);
					}
					if (pProjection != "") {
						cQueryProjection = bsoncxx::from_json(pProjection);
						cQueryPipeline.project(cQueryProjection);
					}
					if (pAddfieldscriteria != "") {
						cQueryAddFields = bsoncxx::from_json(pAddfieldscriteria);
						cQueryPipeline.add_fields(cQueryAddFields);
					}

					try {
						mongocxx::options::aggregate cAggregateOptions;
						cAggregateOptions.allow_disk_use(true);

						mongocxx::cursor cMongoCursor = cMongoCollection.aggregate(cQueryPipeline, mongocxx::options::aggregate());

						for (bsoncxx::document::view cBsonDocument : cMongoCursor) {
							try {
								T cDocument(cBsonDocument);
								cOutputList.push_back(cDocument);
							}
							catch (bsoncxx::exception be) {
								fprintf(stdout, "[fRunQuery] Catched bsoncxx::exception - Message %s Value %d Category %s\n", be.code().message().c_str(), be.code().value(), be.code().category().name());
								fflush(stdout);
								return(-1);
							}
						}

						*pOutputList = cOutputList;
						return(0);	
					} catch (mongocxx::query_exception e) {
						printf("[fRunQuery] Catched mongocxx::query_exception - Message %s Value %d Category %s\n", e.code().message().c_str(), e.code().value(), e.code().category().name());
						fflush(stdout);
						return(-1);
					}
				}







#endif //TCMONGODRIVER_H



