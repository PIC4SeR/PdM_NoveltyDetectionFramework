#include "../include/TcMongoDriver.h"

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




TcMongoDriver::TcMongoDriver(string pMongoDriverName, string pMongoDriverRemoteConnectionType, string pMongoDriverRemoteConnectionHost, uint16_t pMongoDriverRemoteConnectionPort) {
	this->rmMongoDriverName = pMongoDriverName;
	this->rmMongoDriverRemoteConnectionType = pMongoDriverRemoteConnectionType;
	this->rmMongoDriverRemoteConnectionHost = pMongoDriverRemoteConnectionHost;
	this->rmMongoDriverRemoteConnectionPort = pMongoDriverRemoteConnectionPort;
}
TcMongoDriver::~TcMongoDriver()
{
	if (this->cmMongoDriverConnectionPool != nullptr) {
		delete this->cmMongoDriverConnectionPool;
		this->cmMongoDriverConnectionPool = nullptr;
	}
}

void TcMongoDriver::fDisconnect(){
	if (this->cmMongoDriverConnectionPool != nullptr) {
		delete this->cmMongoDriverConnectionPool;
		this->cmMongoDriverConnectionPool = nullptr;
	}
}

int TcMongoDriver::fConnect() {

	try {
		string cMongoDriverRemoteConnectionString = this->rmMongoDriverRemoteConnectionType + "://" + this->rmMongoDriverRemoteConnectionHost + ":" + to_string(this->rmMongoDriverRemoteConnectionPort);
		const mongocxx::uri& cMongoDriverConnectionUri{ bsoncxx::string::view_or_value(cMongoDriverRemoteConnectionString) };
		this->cmMongoDriverConnectionPool = new mongocxx::pool(cMongoDriverConnectionUri);
		if (this->cmMongoDriverConnectionPool == nullptr) {
			return(kCONNECT_MEMALLOC_FAILS);
		}
		return(kCONNECT_OPERATION_SUCCESS);
	}
	catch (mongocxx::exception e) {
		fprintf(stdout, "[fConnect] Catched mongocxx::exception - Message %s Value %d Category %s\n", e.code().message().c_str(), e.code().value(), e.code().category().name());
		fflush(stdout);
		return(kCONNECT_OPERATION_FAILS);
	}
}
int TcMongoDriver::fClientAcquire(boost::optional<mongocxx::pool::entry>* pOptionalMongoEntry) {
	try {
		int rRetryMaxConnection = 10;
		int rRetryConnection = 0;
		
		while (rRetryConnection < rRetryMaxConnection) {
			*pOptionalMongoEntry = this->cmMongoDriverConnectionPool->try_acquire();
			if (*pOptionalMongoEntry) {
				break;
			}
			rRetryConnection++;
		}
		
		if (rRetryConnection >= rRetryMaxConnection) {
			return(kCLIENTACQUIRE_FAILS);
		} else {
			return(kCLIENTACQUIRE_SUCCESS);
		}
	}
	catch (mongocxx::exception e) {
		fprintf(stdout, "[fClientAcquire] Catched mongocxx::exception - Message %s Value %d Category %s\n", e.code().message().c_str(), e.code().value(), e.code().category().name());
		fflush(stdout);
		return(kCLIENTACQUIRE_FAILS);
	}
}
int TcMongoDriver::fDatabaseExist(string pDatabase, mongocxx::client& pMongoClient) {
	
	try {

		bsoncxx::document::view_or_value cDatabaseExistFilter = bsoncxx::builder::stream::document{} << "filter" << bsoncxx::builder::stream::open_document << "name" << pDatabase << bsoncxx::builder::stream::close_document << bsoncxx::builder::stream::finalize;		
		mongocxx::cursor cMongoCursor = pMongoClient.list_databases(cDatabaseExistFilter.view());
		
		if (cMongoCursor.begin() == cMongoCursor.end()){
			try {
				pMongoClient.database(pDatabase);
				return(kDATABASEEXIST_CREATE_SUCCESS);
			}
			catch (mongocxx::operation_exception e) {
				fprintf(stdout, "[fDatabaseExist] Catched mongocxx::operation_exception - Message %s Value %d Category %s\n", e.code().message().c_str(), e.code().value(), e.code().category().name());
				fflush(stdout);
				return(kDATABASEEXIST_CREATE_FAILS);
			}
		} else {
			return(kDATABASEEXIST_SUCCESS);
		}
	}
	catch (mongocxx::query_exception qe) {
		fprintf(stdout, "[fDatabaseExist] Catched mongocxx::query_exception - Message %s Value %d Category %s\n", qe.code().message().c_str(), qe.code().value(), qe.code().category().name());
		fflush(stdout);
		return(kDATABASEEXIST_FAILS);
	}
	catch (mongocxx::operation_exception e) {
		fprintf(stdout, "[fDatabaseExist] Catched mongocxx::operation_exception - Message %s Value %d Category %s\n", e.code().message().c_str(), e.code().value(), e.code().category().name());
		fflush(stdout);
		return(kDATABASEEXIST_FAILS);
	}


}
int TcMongoDriver::fCollectionExist(string pCollection, mongocxx::database pMongoDatabase) {
	try {
		bsoncxx::string::view_or_value cCollectionName = bsoncxx::string::view_or_value(pCollection);
		if (!pMongoDatabase.has_collection(cCollectionName)) {
			try {
				pMongoDatabase.create_collection(cCollectionName);
				return(kCOLLECTIONEXIST_SUCCESS);
			} catch (mongocxx::operation_exception e) {
				fprintf(stdout, "[fCollectionExist] Catched mongocxx::operation_exception - Message %s Value %d Category %s\n", e.code().message().c_str(), e.code().value(), e.code().category().name());
				fflush(stdout);
				return(kCOLLECTIONEXIST_FAILS);
			}
		} else {
			return(kCOLLECTIONEXIST_SUCCESS);
		}
	}
	catch (mongocxx::operation_exception e) {
		fprintf(stdout, "[fCollectionExist] Catched mongocxx::operation_exception - Message %s Value %d Category %s\n", e.code().message().c_str(), e.code().value(), e.code().category().name());
		fflush(stdout);
		return(kCOLLECTIONEXIST_FAILS);
	}
}
int TcMongoDriver::fInserDocument(string pDatabase, string pCollection, string pDocument) {
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
		cBsonDocument = bsoncxx::from_json(pDocument);
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


int TcMongoDriver::fInsertDocumentList(string pDatabase, string pCollection, list<string> pDocuments) {
	boost::optional<mongocxx::pool::entry> cOptionalMongoEntry;
	mongocxx::database cMongoDatabase;
	mongocxx::collection cMongoCollection;
	bsoncxx::document::view_or_value cBsonDocument;
	list<bsoncxx::document::view_or_value> cBsonDocuments;
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
		return(kINSERT_DATABASEEXIST_FAILS);
	} else if (rResult == kDATABASEEXIST_FAILS) {
		fprintf(stdout, "[fInsertDocumentList] Database %s not exist, creation fails\n", pDatabase.c_str());
		fflush(stdout);
		return(kINSERT_DATABASEEXIST_FAILS);
	}

	cMongoDatabase = cMongoClient[pDatabase];
	rResult = fCollectionExist(pCollection, cMongoDatabase);
	if (rResult == kCOLLECTIONEXIST_CREATE_FAILS) {
		fprintf(stdout, "[fInsertDocumentList] Collection %s not exist, creation fails\n", pCollection.c_str());
		fflush(stdout);
		return(kINSERT_COLLECTIONEXIST_FAILS);
	} else if (rResult == kCOLLECTIONEXIST_FAILS) {
		fprintf(stdout, "[fInsertDocumentList] Collection %s not exist, creation fails\n", pCollection.c_str());
		fflush(stdout);
		return(kINSERT_COLLECTIONEXIST_FAILS);
	}

	cMongoCollection = cMongoDatabase[pCollection];

	
	for (string cDocument : pDocuments) {
		try {
			cBsonDocument = bsoncxx::from_json(cDocument);
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


int TcMongoDriver::fRunQuery(list<string>* pOutputList, string pDatabase, string pCollection, string pFilter, string pProjection, string pSortcriteria, int pSkip, int pLimit, string pGroupcriteria, string pAddfieldscriteria) {
	boost::optional<mongocxx::pool::entry> cOptionalMongoEntry;
	mongocxx::database cMongoDatabase;
	mongocxx::collection cMongoCollection;
	bsoncxx::document::view_or_value cQueryFilter;
	bsoncxx::document::view_or_value cQueryProjection;
	bsoncxx::document::view_or_value cQuerySortcriteria;
	bsoncxx::document::view_or_value cQueryGroupcriteria;
	bsoncxx::document::view_or_value cQueryAddFields;
	mongocxx::pipeline cQueryPipeline;
	list<string> cOutputList;
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
		mongocxx::cursor cMongoCursor = cMongoCollection.aggregate(cQueryPipeline, cAggregateOptions);
		for (bsoncxx::document::view cBsonDocument : cMongoCursor) {
			string cDocument = "";
			try {
				cDocument = bsoncxx::to_json(cBsonDocument);
			}
			catch (bsoncxx::exception be) {
				fprintf(stdout, "[fRunQuery] Catched bsoncxx::exception - Message %s Value %d Category %s\n", be.code().message().c_str(), be.code().value(), be.code().category().name());
				fflush(stdout);
				return(-1);
			}
			cOutputList.push_back(cDocument);
		}

		*pOutputList = cOutputList;
		return(0);
	} catch (mongocxx::query_exception e) {
		printf("[fRunQuery] Catched mongocxx::query_exception - Message %s Value %d Category %s\n", e.code().message().c_str(), e.code().value(), e.code().category().name());
		fflush(stdout);
		return(kINSERT_OPERATION_FAILS);
	}
	
}





int TcMongoDriver::fRunQuery(list<string>* pOutputList, string pDatabase, string pCollection, bsoncxx::document::view_or_value pFilter, bsoncxx::document::view_or_value pProjection, bsoncxx::document::view_or_value pSortcriteria, int pSkip, int pLimit, bsoncxx::document::view_or_value pGroupcriteria){
	boost::optional<mongocxx::pool::entry> cOptionalMongoEntry;
	mongocxx::database cMongoDatabase;
	mongocxx::collection cMongoCollection;
	mongocxx::pipeline cQueryPipeline;
	list<string> cOutputList;
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

	if (pFilter != bsoncxx::document::view_or_value()) {
		cQueryPipeline.match(pFilter);
	}
	if (pSortcriteria != bsoncxx::document::view_or_value()) {
		cQueryPipeline.sort(pSortcriteria);
	}
	if (pLimit != 0) {
		cQueryPipeline.limit(pLimit);
	}
	if (pSkip != 0) {
		cQueryPipeline.skip(pSkip);
	}
	if (pGroupcriteria != bsoncxx::document::view_or_value()) {
		cQueryPipeline.group(pGroupcriteria);
	}
	if (pProjection != bsoncxx::document::view_or_value()) {
		cQueryPipeline.project(pProjection);
	}


	mongocxx::cursor cMongoCursor = cMongoCollection.aggregate(cQueryPipeline, mongocxx::options::aggregate());

	for (bsoncxx::document::view cBsonDocument : cMongoCursor) {
		string cDocument = "";
		try {
			cDocument = bsoncxx::to_json(cBsonDocument);
		}
		catch (bsoncxx::exception be) {
			fprintf(stdout, "[fRunQuery] Catched bsoncxx::exception - Message %s Value %d Category %s\n", be.code().message().c_str(), be.code().value(), be.code().category().name());
			fflush(stdout);
			return(-1);
		}
		cOutputList.push_back(cDocument);
	}

	*pOutputList = cOutputList;

	return(0);
}








