/**
	@file TcMongoDriver.h
	@author Giuseppe Pedone
	@version 2.0 04/05/2021
*/
#pragma once
#ifndef TCMONGODRIVER_H
#define TCMONGODRIVER_H


#define MONGO_DEFAULT_PORT 50085
#define MONGO_DEFAULT_HOST "localhost"
#define MONGO_DEFAULT_CONNTYPE "mongodb"
#define MONGO_DRIVER_NAME "SpeaMongoDriver"

#include <mongocxx/instance.hpp>
#include <mongocxx/pool.hpp>
#include <mongocxx/exception/exception.hpp>
#include <mongocxx/exception/bulk_write_exception.hpp>
#include <mongocxx/pipeline.hpp>
#include <boost/optional.hpp>
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
#include <inttypes.h>
#include <string>
#include <list>
#include <optional>




#pragma region Internal Error Codes
constexpr int8_t kErr_ClientAcquire = -1;
constexpr int8_t kClientAcquire_Ok = 0;
constexpr int8_t kErr_DatabaseExist_Create = -2;
constexpr int8_t kErr_DatabaseExist = -1;
constexpr int8_t kDatabaseExist_Create_Ok = 0;
constexpr int8_t kErr_CollectionExist_Create = -2;
constexpr int8_t kCollectionExist_Create_Ok = 1;
constexpr int8_t kErr_CollectionExist = -1;
constexpr int8_t kCollectionExist_Ok = 0;
#pragma endregion

#pragma region External Error Codes
constexpr int8_t kErr_Connect_MemAlloc = -1;
constexpr int8_t kErr_Connect_Operation = -2;
constexpr int8_t kConnect_Ok = 0;
constexpr int8_t kErr_Insert_ClientAcquire = -1;
constexpr int8_t kErr_Insert_DatabaseExist = -2;
constexpr int8_t kErr_Insert_CollectionExist = -3;
constexpr int8_t kErr_Insert = -4;
constexpr int8_t kInsert_Ok = 0;
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
				TcMongoDriver();
				~TcMongoDriver();


				/**
				* Creates a Mongo Instance and a Mongo Connection Pool.
				*
				* @returns Integer Error Code:
				*/
				void fDisconnect();



				/**
				* Insert a Document into a Mongo Collection that is contained in a Mongo Database.
				*
				*/
				int fInserDocument(string pDatabase, string pCollection, string pDocument);
				int fInserDocument(string pDatabase, string pCollection, bsoncxx::document::view_or_value pBsonDocument);

				/**
				* Insert a Document List into a Mongo Collection that is contained in a Mongo Database.
				*
				*/
				int fInsertDocumentList(string pDatabase, string pCollection, list<string> pDocuments);
				int fInsertDocumentList(string pDatabase, string pCollection, list<bsoncxx::document::view_or_value> pBsonDocuments);

				int fRunQuery(list<string>* pOutputList, string pDatabase, string pCollection, string pFilter = "", string pProjection = "", string pSortcriteria = "", int pSkip = 0, int pLimit = 0, string pGroupcriteria = "", string pAddfieldscriteria = "");
				int fRunQuery(list<string>* pOutputList, string pDatabase, string pCollection, bsoncxx::document::view_or_value pFilter = bsoncxx::document::view_or_value(), bsoncxx::document::view_or_value pProjection = bsoncxx::document::view_or_value(), bsoncxx::document::view_or_value pSortcriteria = bsoncxx::document::view_or_value(), int pSkip = 0, int pLimit = 0, bsoncxx::document::view_or_value pGroupcriteria = bsoncxx::document::view_or_value());
		#pragma endregion
		
		#pragma region External Generic Functions
			template<class T>
			int fInsertDocument(string pDatabase, string pCollection, T pDocument);

			template<class T>
			int fInsertDocumentList(string pDatabase, string pCollection, list<T> pDocuments);

			template<class T>
			int fRunQuery(list<T>* pOutputList, string pDatabase, string pCollection, string pFilter, string pProjection, string pSortcriteria, int pSkip = 0, int pLimit = 0, string pGroupcriteria = "", string pAddfieldscriteria = "");
		#pragma endregion

};






template<class T>
int TcMongoDriver::fInsertDocument(string pDatabase, string pCollection, T pDocument) {
	optional<mongocxx::pool::entry> cOptionalMongoEntry;
	mongocxx::database cMongoDatabase;
	mongocxx::collection cMongoCollection;
	bsoncxx::document::view_or_value cBsonDocument;

	int rResult = 0;

	rResult = TcMongoDriver::fClientAcquire(&cOptionalMongoEntry);
	if (rResult == kErr_ClientAcquire) {
		fprintf(stdout, "Client acquire fails\n");
		fflush(stdout);
		return(kErr_Insert_ClientAcquire);
	}

	mongocxx::client& cMongoClient = **cOptionalMongoEntry;

	rResult = TcMongoDriver::fDatabaseExist(pDatabase, cMongoClient);
	if (rResult == kErr_DatabaseExist_Create) {
		fprintf(stdout, "Database %s not exist, creation fails\n", pDatabase.c_str());
		fflush(stdout);
		return(kErr_Insert_DatabaseExist);
	}
	else if (rResult == kErr_DatabaseExist) {
		fprintf(stdout, "Database %s not exist, creation fails\n", pDatabase.c_str());
		fflush(stdout);
		return(kErr_Insert_DatabaseExist);
	}

	cMongoDatabase = cMongoClient[pDatabase];
	rResult = TcMongoDriver::fCollectionExist(pCollection, cMongoDatabase);
	if (rResult == kErr_CollectionExist_Create) {
		fprintf(stdout, "Collection %s not exist, creation fails\n", pCollection.c_str());
		fflush(stdout);
		return(kErr_Insert_CollectionExist);
	}
	else if (rResult == kErr_CollectionExist) {
		fprintf(stdout, "Collection %s not exist, creation fails\n", pCollection.c_str());
		fflush(stdout);
		return(kErr_Insert_CollectionExist);
	}

	cMongoCollection = cMongoDatabase[pCollection];


	try {
		cBsonDocument = T::fSerializeObjectBsonValue(pDocument);
	}
	catch (mongocxx::query_exception e) {
		fprintf(stdout, "Catched exception - Message %s Value %d Category %s\n", e.code().message().c_str(), e.code().value(), e.code().category().name());
		fflush(stdout);
	}

	try {
		boost::optional<mongocxx::result::insert_one> cResult = cMongoCollection.insert_one(cBsonDocument);
		if (!cResult) {
			return(kErr_Insert);
		}
		else {
			return(kInsert_Ok);
		}
	}
	catch (mongocxx::bulk_write_exception e) {
		printf("Catched exception - Message %s Value %d Category %s\n", e.code().message().c_str(), e.code().value(), e.code().category().name());
		fflush(stdout);
		return(kErr_Insert);
	}
	catch (mongocxx::exception e) {
		fprintf(stdout, "Catched exception - Message %s Value %d Category %s\n", e.code().message().c_str(), e.code().value(), e.code().category().name());
		fflush(stdout);
		return(kErr_Insert);
	}
}

template<class T>
int TcMongoDriver::fInsertDocumentList(string pDatabase, string pCollection, list<T> pDocuments) {
	optional<mongocxx::pool::entry> cOptionalMongoEntry;
	mongocxx::database cMongoDatabase;
	mongocxx::collection cMongoCollection;
	bsoncxx::document::view_or_value cBsonDocument;
	list<bsoncxx::document::view_or_value> cBsonDocuments;

	int rResult = 0;

	rResult = TcMongoDriver::fClientAcquire(&cOptionalMongoEntry);
	if (rResult == kErr_ClientAcquire) {
		fprintf(stdout, "Client acquire fails\n");
		fflush(stdout);
		return(kErr_Insert_ClientAcquire);
	}

	mongocxx::client& cMongoClient = **cOptionalMongoEntry;

	rResult = TcMongoDriver::fDatabaseExist(pDatabase, cMongoClient);
	if (rResult == kErr_DatabaseExist_Create) {
		fprintf(stdout, "Database %s not exist, creation fails\n", pDatabase.c_str());
		fflush(stdout);
		return(kErr_Insert_DatabaseExist);
	}
	else if (rResult == kErr_DatabaseExist) {
		fprintf(stdout, "Database %s not exist, creation fails\n", pDatabase.c_str());
		fflush(stdout);
		return(kErr_Insert_DatabaseExist);
	}

	cMongoDatabase = cMongoClient[pDatabase];
	rResult = TcMongoDriver::fCollectionExist(pCollection, cMongoDatabase);
	if (rResult == kErr_CollectionExist_Create) {
		fprintf(stdout, "Collection %s not exist, creation fails\n", pCollection.c_str());
		fflush(stdout);
		return(kErr_Insert_CollectionExist);
	}
	else if (rResult == kErr_CollectionExist) {
		fprintf(stdout, "Collection %s not exist, creation fails\n", pCollection.c_str());
		fflush(stdout);
		return(kErr_Insert_CollectionExist);
	}

	cMongoCollection = cMongoDatabase[pCollection];


	for (T cDocument : pDocuments) {
		try {
			cBsonDocument = T::fSerializeObjectBsonValue(cDocument);
			cBsonDocuments.push_back(cBsonDocument);
		}
		catch (mongocxx::query_exception e) {
			fprintf(stdout, "Catched exception - Message %s Value %d Category %s\n", e.code().message().c_str(), e.code().value(), e.code().category().name());
			fflush(stdout);
		}
	}

	try {
		boost::optional<mongocxx::result::insert_many> cResult = cMongoCollection.insert_many(cBsonDocuments);
		if (!cResult) {
			return(kErr_Insert);
		}
		else {
			return(kInsert_Ok);
		}
	}
	catch (mongocxx::bulk_write_exception e) {
		printf("Catched exception - Message %s Value %d Category %s\n", e.code().message().c_str(), e.code().value(), e.code().category().name());
		fflush(stdout);
		return(kErr_Insert);
	}
	catch (mongocxx::exception e) {
		fprintf(stdout, "Catched exception - Message %s Value %d Category %s\n", e.code().message().c_str(), e.code().value(), e.code().category().name());
		fflush(stdout);
		return(kErr_Insert);
	}
}

template<class T>
int TcMongoDriver::fRunQuery(list<T>* pOutputList, string pDatabase, string pCollection, string pFilter, string pProjection, string pSortcriteria, int pSkip, int pLimit, string pGroupcriteria, string pAddfieldscriteria) {
	optional<mongocxx::pool::entry> cOptionalMongoEntry;
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



	rResult = TcMongoDriver::fClientAcquire(&cOptionalMongoEntry);
	if (rResult == kErr_ClientAcquire) {
		fprintf(stdout, "Client acquire fails\n");
		fflush(stdout);
		return(kErr_Insert_ClientAcquire);
	}

	mongocxx::client& cMongoClient = **cOptionalMongoEntry;

	rResult = TcMongoDriver::fDatabaseExist(pDatabase, cMongoClient);
	if (rResult == kErr_DatabaseExist_Create) {
		fprintf(stdout, "Database %s not exist, creation fails\n", pDatabase.c_str());
		fflush(stdout);
		return(kErr_DatabaseExist_Create);
	}
	else if (rResult == kErr_DatabaseExist) {
		fprintf(stdout, "Database %s not exist, creation fails\n", pDatabase.c_str());
		fflush(stdout);
		return(kErr_DatabaseExist);
	}

	cMongoDatabase = cMongoClient[pDatabase];
	rResult = TcMongoDriver::fCollectionExist(pCollection, cMongoDatabase);
	if (rResult == kErr_CollectionExist_Create) {
		fprintf(stdout, "Collection %s not exist, creation fails\n", pCollection.c_str());
		fflush(stdout);
		return(kErr_Insert_CollectionExist);
	}
	else if (rResult == kErr_CollectionExist) {
		fprintf(stdout, "Collection %s not exist, creation fails\n", pCollection.c_str());
		fflush(stdout);
		return(kErr_CollectionExist);
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

		mongocxx::cursor cMongoCursor = cMongoCollection.aggregate(cQueryPipeline, cAggregateOptions);


		for (bsoncxx::document::view cBsonDocument : cMongoCursor) {
			try {
				T cDocument(cBsonDocument);

				cOutputList.push_back(cDocument);
			}
			catch (bsoncxx::exception be) {
				fprintf(stdout, "Catched exception - Message %s Value %d Category %s\n", be.code().message().c_str(), be.code().value(), be.code().category().name());
				fflush(stdout);
				return(-1);
			}
		}

		*pOutputList = cOutputList;
		return(0);
	}
	catch (mongocxx::query_exception e) {
		fprintf(stdout, "Catched exception - Message %s Value %d Category %s\n", e.code().message().c_str(), e.code().value(), e.code().category().name());
		fflush(stdout);
		return(-1);
	}
}

#endif //TCMONGODRIVER_H



