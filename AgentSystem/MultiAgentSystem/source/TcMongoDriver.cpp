



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
#include "../include/TcMongoDriver.h"




TcMongoDriver::TcMongoDriver(string pMongoDriverName, string pMongoDriverRemoteConnectionType, string pMongoDriverRemoteConnectionHost, uint16_t pMongoDriverRemoteConnectionPort) {
	this->rmMongoDriverName = pMongoDriverName;
	this->rmMongoDriverRemoteConnectionType = pMongoDriverRemoteConnectionType;
	this->rmMongoDriverRemoteConnectionHost = pMongoDriverRemoteConnectionHost;
	this->rmMongoDriverRemoteConnectionPort = pMongoDriverRemoteConnectionPort;
}

TcMongoDriver::TcMongoDriver() {
	this->rmMongoDriverName = string(MONGO_DRIVER_NAME);
	this->rmMongoDriverRemoteConnectionType = string(MONGO_DEFAULT_CONNTYPE);
	this->rmMongoDriverRemoteConnectionHost = string(MONGO_DEFAULT_HOST);
	this->rmMongoDriverRemoteConnectionPort = (uint16_t) MONGO_DEFAULT_PORT;
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
			return(kErr_Connect_MemAlloc);
		}
		return(kConnect_Ok);
	}
	catch (mongocxx::exception e) {
		fprintf(stdout, "[fConnect] Catched mongocxx::exception - Message %s Value %d Category %s\n", e.code().message().c_str(), e.code().value(), e.code().category().name());
		fflush(stdout);
		return(kErr_Connect_Operation);
	}
}
int TcMongoDriver::fClientAcquire(core::v1::optional<mongocxx::pool::entry>* pOptionalMongoEntry) {
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
			return(kErr_ClientAcquire);
		} else {
			return(kClientAcquire_Ok);
		}
	}
	catch (mongocxx::exception e) {
		fprintf(stdout, "[fClientAcquire] Catched mongocxx::exception - Message %s Value %d Category %s\n", e.code().message().c_str(), e.code().value(), e.code().category().name());
		fflush(stdout);
		return(kErr_ClientAcquire);
	}
}
int TcMongoDriver::fDatabaseExist(string pDatabase, mongocxx::client& pMongoClient) {
	
	try {

		bsoncxx::document::view_or_value cDatabaseExistFilter = bsoncxx::builder::stream::document{} << "filter" << bsoncxx::builder::stream::open_document << "name" << pDatabase << bsoncxx::builder::stream::close_document << bsoncxx::builder::stream::finalize;		
		mongocxx::cursor cMongoCursor = pMongoClient.list_databases(cDatabaseExistFilter.view());
		
		if (cMongoCursor.begin() == cMongoCursor.end()){
			try {
				pMongoClient.database(pDatabase);
				return(kDatabaseExist_Create_Ok);
			}
			catch (mongocxx::operation_exception e) {
				fprintf(stdout, "[fDatabaseExist] Catched mongocxx::operation_exception - Message %s Value %d Category %s\n", e.code().message().c_str(), e.code().value(), e.code().category().name());
				fflush(stdout);
				return(kErr_DatabaseExist_Create);
			}
		} else {
			return(kDatabaseExist_Create_Ok);
		}
	}
	catch (mongocxx::query_exception qe) {
		fprintf(stdout, "[fDatabaseExist] Catched mongocxx::query_exception - Message %s Value %d Category %s\n", qe.code().message().c_str(), qe.code().value(), qe.code().category().name());
		fflush(stdout);
		return(kErr_DatabaseExist);
	}
	catch (mongocxx::operation_exception e) {
		fprintf(stdout, "[fDatabaseExist] Catched mongocxx::operation_exception - Message %s Value %d Category %s\n", e.code().message().c_str(), e.code().value(), e.code().category().name());
		fflush(stdout);
		return(kErr_DatabaseExist);
	}


}
int TcMongoDriver::fCollectionExist(string pCollection, mongocxx::database pMongoDatabase) {
	try {
		bsoncxx::string::view_or_value cCollectionName = bsoncxx::string::view_or_value(pCollection);
		if (!pMongoDatabase.has_collection(cCollectionName)) {
			try {
				pMongoDatabase.create_collection(cCollectionName);
				return(kCollectionExist_Ok);
			} catch (mongocxx::operation_exception e) {
				fprintf(stdout, "[fCollectionExist] Catched mongocxx::operation_exception - Message %s Value %d Category %s\n", e.code().message().c_str(), e.code().value(), e.code().category().name());
				fflush(stdout);
				return(kErr_CollectionExist);
			}
		} else {
			return(kCollectionExist_Ok);
		}
	}
	catch (mongocxx::operation_exception e) {
		fprintf(stdout, "[fCollectionExist] Catched mongocxx::operation_exception - Message %s Value %d Category %s\n", e.code().message().c_str(), e.code().value(), e.code().category().name());
		fflush(stdout);
		return(kErr_CollectionExist);
	}
}
int TcMongoDriver::fInserDocument(string pDatabase, string pCollection, string pDocument) {
	core::v1::optional<mongocxx::pool::entry> cOptionalMongoEntry;
	mongocxx::database cMongoDatabase;
	mongocxx::collection cMongoCollection;
	bsoncxx::document::view_or_value cBsonDocument;
	int rResult = 0;

	rResult = fClientAcquire(&cOptionalMongoEntry);
	if (rResult == kErr_ClientAcquire) {
		fprintf(stdout, "[fInsertDocument] Client acquire fails\n");
		fflush(stdout);
		return(kErr_Insert_ClientAcquire);
	}

	mongocxx::client& cMongoClient = **cOptionalMongoEntry;
	
	rResult = fDatabaseExist(pDatabase, cMongoClient);
	if (rResult == kErr_DatabaseExist_Create) {
		fprintf(stdout, "[fInsertDocument] Database %s not exist, creation fails\n", pDatabase.c_str());
		fflush(stdout);
		return(kErr_Insert_DatabaseExist);
	} else if (rResult == kErr_DatabaseExist) {
		fprintf(stdout, "[fInsertDocument] Database %s not exist, creation fails\n", pDatabase.c_str());
		fflush(stdout);
		return(kErr_Insert_DatabaseExist);
	}
	
	cMongoDatabase = cMongoClient[pDatabase];
	rResult = fCollectionExist(pCollection, cMongoDatabase);
	if (rResult == kErr_CollectionExist_Create) {
		fprintf(stdout, "[fInsertDocument] Collection %s not exist, creation fails\n", pCollection.c_str());
		fflush(stdout);
		return(kErr_Insert_CollectionExist);
	} else if (rResult == kErr_CollectionExist) {
		fprintf(stdout, "[fInsertDocument] Collection %s not exist, creation fails\n", pCollection.c_str());
		fflush(stdout);
		return(kErr_Insert_CollectionExist);
	}
	
	cMongoCollection = cMongoDatabase[pCollection];
	
	try {
		cBsonDocument = bsoncxx::from_json(pDocument);
	} catch (bsoncxx::exception e) {
		cBsonDocument = bsoncxx::builder::stream::document{} << "Value" << pDocument << bsoncxx::builder::stream::finalize;
	}
	
	try {
		core::v1::optional<mongocxx::result::insert_one> cResult = cMongoCollection.insert_one(cBsonDocument.view());
		if (!cResult) {
			return(kErr_Insert);
		} else {
			return(kInsert_Ok);
		}
	} catch (mongocxx::bulk_write_exception e) {
		printf("[fInsertDocument] Catched mongocxx::bulk_write_exception - Message %s Value %d Category %s\n", e.code().message().c_str(), e.code().value(), e.code().category().name());
		fflush(stdout);
		return(kErr_Insert);
	} catch (mongocxx::exception e) {
		fprintf(stdout, "[fInsertDocument] Catched mongocxx::exception - Message %s Value %d Category %s\n", e.code().message().c_str(), e.code().value(), e.code().category().name());
		fflush(stdout);
		return(kErr_Insert);
	}
}


int TcMongoDriver::fInserDocument(string pDatabase, string pCollection, bsoncxx::document::view_or_value pBsonDocument) {
	core::v1::optional<mongocxx::pool::entry> cOptionalMongoEntry;
	mongocxx::database cMongoDatabase;
	mongocxx::collection cMongoCollection;
	int rResult = 0;

	rResult = fClientAcquire(&cOptionalMongoEntry);
	if (rResult == kErr_ClientAcquire) {
		fprintf(stdout, "[fInsertDocument] Client acquire fails\n");
		fflush(stdout);
		return(kErr_Insert_ClientAcquire);
	}

	mongocxx::client& cMongoClient = **cOptionalMongoEntry;

	rResult = fDatabaseExist(pDatabase, cMongoClient);
	if (rResult == kErr_DatabaseExist_Create) {
		fprintf(stdout, "[fInsertDocument] Database %s not exist, creation fails\n", pDatabase.c_str());
		fflush(stdout);
		return(kErr_Insert_DatabaseExist);
	}
	else if (rResult == kErr_DatabaseExist) {
		fprintf(stdout, "[fInsertDocument] Database %s not exist, creation fails\n", pDatabase.c_str());
		fflush(stdout);
		return(kErr_Insert_DatabaseExist);
	}

	cMongoDatabase = cMongoClient[pDatabase];
	rResult = fCollectionExist(pCollection, cMongoDatabase);
	if (rResult == kErr_CollectionExist_Create) {
		fprintf(stdout, "[fInsertDocument] Collection %s not exist, creation fails\n", pCollection.c_str());
		fflush(stdout);
		return(kErr_Insert_CollectionExist);
	}
	else if (rResult == kErr_CollectionExist) {
		fprintf(stdout, "[fInsertDocument] Collection %s not exist, creation fails\n", pCollection.c_str());
		fflush(stdout);
		return(kErr_Insert_CollectionExist);
	}

	cMongoCollection = cMongoDatabase[pCollection];

	try {
		core::v1::optional<mongocxx::result::insert_one> cResult = cMongoCollection.insert_one(pBsonDocument.view());
		if (!cResult) {
			return(kErr_Insert);
		}
		else {
			return(kInsert_Ok);
		}
	}
	catch (mongocxx::bulk_write_exception e) {
		printf("[fInsertDocument] Catched mongocxx::bulk_write_exception - Message %s Value %d Category %s\n", e.code().message().c_str(), e.code().value(), e.code().category().name());
		fflush(stdout);
		return(kErr_Insert);
	}
	catch (mongocxx::exception e) {
		fprintf(stdout, "[fInsertDocument] Catched mongocxx::exception - Message %s Value %d Category %s\n", e.code().message().c_str(), e.code().value(), e.code().category().name());
		fflush(stdout);
		return(kErr_Insert);
	}
}


int TcMongoDriver::fInsertDocumentList(string pDatabase, string pCollection, list<string> pDocuments) {
	core::v1::optional<mongocxx::pool::entry> cOptionalMongoEntry;
	mongocxx::database cMongoDatabase;
	mongocxx::collection cMongoCollection;
	bsoncxx::document::view_or_value cBsonDocument;
	list<bsoncxx::document::view_or_value> cBsonDocuments;
	int rResult = 0;

	rResult = fClientAcquire(&cOptionalMongoEntry);
	if (rResult == kErr_ClientAcquire) {
		fprintf(stdout, "[fInsertDocumentList] Client acquire fails\n");
		fflush(stdout);
		return(kErr_Insert_ClientAcquire);
	}

	mongocxx::client& cMongoClient = **cOptionalMongoEntry;

	rResult = fDatabaseExist(pDatabase, cMongoClient);
	if (rResult == kErr_DatabaseExist_Create) {
		fprintf(stdout, "[fInsertDocumentList] Database %s not exist, creation fails\n", pDatabase.c_str());
		fflush(stdout);
		return(kErr_Insert_DatabaseExist);
	} else if (rResult == kErr_DatabaseExist) {
		fprintf(stdout, "[fInsertDocumentList] Database %s not exist, creation fails\n", pDatabase.c_str());
		fflush(stdout);
		return(kErr_Insert_DatabaseExist);
	}

	cMongoDatabase = cMongoClient[pDatabase];
	rResult = fCollectionExist(pCollection, cMongoDatabase);
	if (rResult == kErr_CollectionExist_Create) {
		fprintf(stdout, "[fInsertDocumentList] Collection %s not exist, creation fails\n", pCollection.c_str());
		fflush(stdout);
		return(kErr_Insert_CollectionExist);
	} else if (rResult == kErr_CollectionExist) {
		fprintf(stdout, "[fInsertDocumentList] Collection %s not exist, creation fails\n", pCollection.c_str());
		fflush(stdout);
		return(kErr_Insert_CollectionExist);
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
		core::v1::optional<mongocxx::result::insert_many> cResult = cMongoCollection.insert_many(cBsonDocuments);
		if (!cResult) {
			return(kErr_Insert);
		}
		else {
			return(kInsert_Ok);
		}
	}
	catch (mongocxx::bulk_write_exception e) {
		printf("[fInsertDocumentList] Catched mongocxx::bulk_write_exception - Message %s Value %d Category %s\n", e.code().message().c_str(), e.code().value(), e.code().category().name());
		fflush(stdout);
		return(kErr_Insert);
	}
	catch (mongocxx::exception e) {
		fprintf(stdout, "[fInsertDocumentList] Catched mongocxx::exception - Message %s Value %d Category %s\n", e.code().message().c_str(), e.code().value(), e.code().category().name());
		fflush(stdout);
		return(kErr_Insert);
	}
}


int TcMongoDriver::fRunQuery(list<string>* pOutputList, string pDatabase, string pCollection, string pFilter, string pProjection, string pSortcriteria, int pSkip, int pLimit, string pGroupcriteria, string pAddfieldscriteria) {
	core::v1::optional<mongocxx::pool::entry> cOptionalMongoEntry;
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
	if (rResult == kErr_ClientAcquire) {
		fprintf(stdout, "[fInsertDocumentList] Client acquire fails\n");
		fflush(stdout);
		return(kErr_Insert_ClientAcquire);
	}

	mongocxx::client& cMongoClient = **cOptionalMongoEntry;

	rResult = fDatabaseExist(pDatabase, cMongoClient);
	if (rResult == kErr_DatabaseExist_Create) {
		fprintf(stdout, "[fInsertDocumentList] Database %s not exist, creation fails\n", pDatabase.c_str());
		fflush(stdout);
		return(-1);
	}
	else if (rResult == kErr_DatabaseExist) {
		fprintf(stdout, "[fInsertDocumentList] Database %s not exist, creation fails\n", pDatabase.c_str());
		fflush(stdout);
		return(-1);
	}

	cMongoDatabase = cMongoClient[pDatabase];
	rResult = fCollectionExist(pCollection, cMongoDatabase);
	if (rResult == kErr_CollectionExist_Create) {
		fprintf(stdout, "[fInsertDocumentList] Collection %s not exist, creation fails\n", pCollection.c_str());
		fflush(stdout);
		return(kErr_Insert_CollectionExist);
	}
	else if (rResult == kErr_CollectionExist) {
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
		return(kErr_Insert);
	}
	
}





int TcMongoDriver::fRunQuery(list<string>* pOutputList, string pDatabase, string pCollection, bsoncxx::document::view_or_value pFilter, bsoncxx::document::view_or_value pProjection, bsoncxx::document::view_or_value pSortcriteria, int pSkip, int pLimit, bsoncxx::document::view_or_value pGroupcriteria){
	core::v1::optional<mongocxx::pool::entry> cOptionalMongoEntry;
	mongocxx::database cMongoDatabase;
	mongocxx::collection cMongoCollection;
	mongocxx::pipeline cQueryPipeline;
	list<string> cOutputList;
	int rResult = 0;

	rResult = fClientAcquire(&cOptionalMongoEntry);
	if (rResult == kErr_ClientAcquire) {
		fprintf(stdout, "[fInsertDocumentList] Client acquire fails\n");
		fflush(stdout);
		return(kErr_Insert_ClientAcquire);
	}

	mongocxx::client& cMongoClient = **cOptionalMongoEntry;

	rResult = fDatabaseExist(pDatabase, cMongoClient);
	if (rResult == kErr_DatabaseExist_Create) {
		fprintf(stdout, "[fInsertDocumentList] Database %s not exist, creation fails\n", pDatabase.c_str());
		fflush(stdout);
		return(-1);
	}
	else if (rResult == kErr_DatabaseExist) {
		fprintf(stdout, "[fInsertDocumentList] Database %s not exist, creation fails\n", pDatabase.c_str());
		fflush(stdout);
		return(-1);
	}

	cMongoDatabase = cMongoClient[pDatabase];
	rResult = fCollectionExist(pCollection, cMongoDatabase);
	if (rResult == kErr_CollectionExist_Create) {
		fprintf(stdout, "[fInsertDocumentList] Collection %s not exist, creation fails\n", pCollection.c_str());
		fflush(stdout);
		return(kErr_Insert_CollectionExist);
	}
	else if (rResult == kErr_CollectionExist) {
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





















