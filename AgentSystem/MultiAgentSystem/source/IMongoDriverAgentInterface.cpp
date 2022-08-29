#include <bsoncxx/types.hpp>
#include <bsoncxx/builder/stream/document.hpp>
#include <bsoncxx/json.hpp>
#include <bsoncxx/builder/basic/document.hpp>
#include <boost/serialization/serialization.hpp>

#include "../include/IMongoDriverAgentInterface.h"
#include "../include/TcMongoDriver.h"
#include "../include/TcMeasure.h"


IMongoDriverAgentInterface::IMongoDriverAgentInterface(string pMongoDriverName, string pMongoDriverRemoteConnectionType, string pMongoDriverRemoteConnectionHost, uint16_t pMongoDriverRemoteConnectionPort){
    this->cmMongoDriver = new TcMongoDriver(pMongoDriverName, pMongoDriverRemoteConnectionType, pMongoDriverRemoteConnectionHost, pMongoDriverRemoteConnectionPort);
}

IMongoDriverAgentInterface::IMongoDriverAgentInterface() {
    this->cmMongoDriver = new TcMongoDriver();
}

IMongoDriverAgentInterface::~IMongoDriverAgentInterface(){
    if (this->cmMongoDriver != nullptr) {
        delete this->cmMongoDriver;
        this->cmMongoDriver = nullptr;
    }
}

TcMongoDriver* IMongoDriverAgentInterface::fGetDriver() {
    return(this->cmMongoDriver);
}

int IMongoDriverAgentInterface::fConnectDriver() {

    if (this->cmMongoDriver == nullptr) {
        return(kConnectionFails);
    }
    int rResult = this->cmMongoDriver->fConnect();
    if (rResult < 0) {
        return(kConnectionFails);
    }
    return(kConnectionSuccess);
}

void IMongoDriverAgentInterface::fDisconnectDriver() {
    if (this->cmMongoDriver == nullptr) {
        return;
    }
    this->cmMongoDriver->fDisconnect();
}


int IMongoDriverAgentInterface::fGetInBackWindow(list<string> *pOutput, string pDatabase, string pCollection, string pSortattribute, int pLimit, string pGroupattribute, string pProjectionattribute){
    list<string> cOutputList;
	int rResult = 0;
    
    string rSortString;
    string rProjectionString;
    string rFilterString;


    if (pSortattribute == "") {
        rSortString = "";
    } else {
        rSortString = "{ \"" + pSortattribute + "\" : 1 }";
    }


    rFilterString = "";

    if (pProjectionattribute == "") {
        rProjectionString = "";
    }
    else {
        rProjectionString = "{ \"_id\" : 0, \"" + pProjectionattribute + "\" : 1 }";
    }
    
    

	rResult = this->cmMongoDriver->fRunQuery(&cOutputList, pDatabase, pCollection,
									    rFilterString,
										rProjectionString,
										rSortString,
                                        0,
										pLimit
	);

    if (rResult < 0) {
        return(IMongoDriverAgentInterface::kQueryFails);
    }

	*pOutput = cOutputList;

    return(IMongoDriverAgentInterface::kGetSuccess);
}

int IMongoDriverAgentInterface::fGetMaxOfInBackWindow(string *pOutput, string pDatabase, string pCollection, string pSortattribute, int pLimit, string pMaxattribute, string pGroupattribute, string pProjectionattribute){
	list<string> cOutputList;
	int rResult = 0;
    
    string rSortString;
    string rGroupString;
    string rProjectionString;
    string rFilterString;

    if (pSortattribute == "") {
        rSortString = "";
    } else {
        rSortString = "{ \"" + pSortattribute + "\" : 1 }";
    }

    rFilterString = "";


    if (pProjectionattribute == "") {
        rProjectionString = "";
    }
    else {
        rProjectionString = "{ \"_id\" : 0, \"max\" : 1 }";
    }


    if (pMaxattribute == "") {
        rGroupString = "";
    }
    else {
        rGroupString = "{\"_id\" : null, \"max\" : { \"$max\" : \"$" + pMaxattribute + "\"  } }";
    }


	rResult = this->cmMongoDriver->fRunQuery(&cOutputList, pDatabase, pCollection,
									    rFilterString,
										rProjectionString,
										rSortString,
										0,
										pLimit,
                                        rGroupString 
	);

    if (rResult < 0) {
        return(IMongoDriverAgentInterface::kQueryFails);
    }


    *pOutput = (cOutputList.size() > 0 ? cOutputList.front() : string(""));
    return(IMongoDriverAgentInterface::kGetSuccess);
}

int IMongoDriverAgentInterface::fGetMinOfInBackWindow(string *pOutput, string pDatabase, string pCollection, string pSortattribute, int pLimit, string pMinattribute, string pGroupattribute, string pProjectionattribute){
	list<string> cOutputList;
	int rResult = 0;
    
    string rSortString;
    string rGroupString;
    string rProjectionString;
    string rFilterString;

    if (pSortattribute == "") {
        rSortString = "";
    } else {
        rSortString = "{ \"" + pSortattribute + "\" : 1 }";
    }

    rFilterString = "";


    if (pProjectionattribute == "") {
        rProjectionString = "";
    }
    else {
        rProjectionString = "{ \"_id\" : 0, \"min\" : 1 }";
    }


    if (pMinattribute == "") {
        rGroupString = "";
    }
    else {
        rGroupString = "{\"_id\" : null, \"min\" : { \"$min\" : \"$" + pMinattribute + "\"  } }";
    }

   
	rResult = this->cmMongoDriver->fRunQuery(&cOutputList, pDatabase, pCollection,
									    rFilterString,
										rProjectionString,
										rSortString,
										0,
										pLimit,
                                        rGroupString 
	);

    if (rResult < 0) {
        return(IMongoDriverAgentInterface::kQueryFails);
    }


    *pOutput = (cOutputList.size() > 0 ? cOutputList.front() : string(""));
    return(IMongoDriverAgentInterface::kGetSuccess);
}



int IMongoDriverAgentInterface::fGetAvgOfInBackWindow(string *pOutput, string pDatabase, string pCollection, string pSortattribute, int pLimit, string pAvgattribute,  string pGroupattribute, string pProjectionattribute){
	list<string> cOutputList;
	int rResult = 0;
    
    string rSortString;
    string rGroupString;
    string rProjectionString;
    string rFilterString;

    if(pSortattribute == ""){
        rSortString = "";
    } else {
        rSortString = "{ \"" + pSortattribute + "\" : 1 }";
    }

    rFilterString = "";

    if(pProjectionattribute == ""){
        rProjectionString = "";
    } else {
        rProjectionString = "{ \"_id\" : 0, \"average\" : 1 }";
    }
    

    if(pAvgattribute == ""){
        rGroupString = "";
    } else {
        rGroupString = "{\"_id\" : null, \"average\" : { \"$avg\" : \"$" + pAvgattribute + "\"  } }";
    }



	rResult = this->cmMongoDriver->fRunQuery(&cOutputList, pDatabase, pCollection,
									    rFilterString,
										rProjectionString,
										rSortString,
										0,
										pLimit,
                                        rGroupString 
	);
	
    if (rResult < 0) {
        return(IMongoDriverAgentInterface::kQueryFails);
    }

    *pOutput = (cOutputList.size() > 0 ? cOutputList.front() : string(""));
    return(IMongoDriverAgentInterface::kGetSuccess);
}



int IMongoDriverAgentInterface::fGetSumOfInBackWindow(string *pOutput, string pDatabase, string pCollection, string pSortattribute, int pLimit, string pSumattribute,  string pGroupattribute, string pProjectionattribute){
	list<string> cOutputList;
	int rResult = 0;
    
    string rSortString;
    string rGroupString;
    string rProjectionString;
    string rFilterString;

    if (pSortattribute == "") {
        rSortString = "";
    } else {
        rSortString = "{ \"" + pSortattribute + "\" : 1 }";
    }

    rFilterString = "";


    if (pProjectionattribute == "") {
        rProjectionString = "";
    } else {
        rProjectionString = "{ \"_id\" : 0, \"sum\" : 1 }";
    }


    if (pSumattribute == "") {
        rGroupString = "";
    }
    else {
        rGroupString = "{\"_id\" : null, \"sum\" : { \"$sum\" : \"$" + pSumattribute + "\"  } }";
    }


	rResult = this->cmMongoDriver->fRunQuery(&cOutputList, pDatabase, pCollection,
									    rFilterString,
										rProjectionString,
										rSortString,
										0,
										pLimit,
                                        rGroupString 
	);

    if (rResult < 0) {
        return(IMongoDriverAgentInterface::kQueryFails);
    }


    
    *pOutput = (cOutputList.size() > 0 ? cOutputList.front() : string(""));
    return(IMongoDriverAgentInterface::kGetSuccess);
}


int IMongoDriverAgentInterface::fGetBetweenValues(list<string> *pOutput, string pDatabase, string pCollection, string pBetweenattribute, string pUpperbound, string pLowerbound,  string pSortattribute, string pProjectionattribute){
    list<string> cOutputList;
	int rResult = 0;
    
    string rProjectionString;
    string rFilterString;
    string rFilterSubString;
    string rSortString;

    if (pSortattribute == "") {
        rSortString = "";
    } else {
        rSortString = "{ \"" + pSortattribute + "\" : 1 }";
    }

    rFilterString = "";

    if(pProjectionattribute == ""){
        rProjectionString = "";
    } else {
        rProjectionString = "{ \"_id\" : 0, \"" + pProjectionattribute + "\" : 1 }";
    }

	rResult = this->cmMongoDriver->fRunQuery(&cOutputList, pDatabase, pCollection,
									    rFilterString,
										rProjectionString, 
                                        rSortString
    );

    if (rResult < 0) {
        pOutput = nullptr;
        return(IMongoDriverAgentInterface::kQueryFails);
    }

	*pOutput = cOutputList;
    return(IMongoDriverAgentInterface::kGetSuccess);
}


int IMongoDriverAgentInterface::fGetGreatherThanValue(list<string> *pOutput, string pDatabase, string pCollection, string pCompareattribute, string pComparevalue,  string pSortattribute, string pProjectionattribute){
    list<string> cOutputList;
	int rResult = 0;
    
    string rProjectionString;
    string rFilterString;
    string rFilterSubString;
    string rSortString;


    if (pSortattribute == "") {
        rSortString = "";
    } else {
        rSortString = "{ \"" + pSortattribute + "\" : 1 }";
    }

    rFilterString = "";


    if (pProjectionattribute == "") {
        rProjectionString = "";
    }
    else {
        rProjectionString = "{ \"_id\" : 0, \"" + pProjectionattribute + "\" : 1 }";
    }


    rResult = this->cmMongoDriver->fRunQuery(&cOutputList, pDatabase, pCollection,
                                        rFilterString,
                                        rProjectionString,
                                        rSortString
    );

    if (rResult < 0) {
        return(IMongoDriverAgentInterface::kQueryFails);
    }

	*pOutput = cOutputList;
    return(IMongoDriverAgentInterface::kGetSuccess);
}


int IMongoDriverAgentInterface::fGetLowerThanValue(list<string> *pOutput, string pDatabase, string pCollection, string pCompareattribute, string pComparevalue, string pSortattribute, string pProjectionattribute){
    list<string> cOutputList;
	int rResult = 0;
   
    string rProjectionString;
    string rFilterString;
    string rFilterSubString;
    string rSortString;

    if (pSortattribute == "") {
        rSortString = "";
    } else {
        rSortString = "{ \"" + pSortattribute + "\" : 1 }";
    }

    rFilterString = "";

    if (pProjectionattribute == "") {
        rProjectionString = "";
    }
    else {
        rProjectionString = "{ \"_id\" : 0, \"" + pProjectionattribute + "\" : 1 }";
    }

	
	rResult = this->cmMongoDriver->fRunQuery(&cOutputList, pDatabase, pCollection,
									    rFilterString,
										rProjectionString,
                                        rSortString
    );

    if (rResult < 0) {
        return(IMongoDriverAgentInterface::kQueryFails);
    }

	*pOutput = cOutputList;
    return(IMongoDriverAgentInterface::kGetSuccess);
}


int IMongoDriverAgentInterface::fGetAvgOfBetweenValues(string* pOutput, string pDatabase, string pCollection, string pBetweenattribute, string pUpperbound, string pLowerbound, string pAvgattribute) {
    list<string> cOutputList;
    int rResult = 0;

   
    string rFilterString;
    string rFilterSubString;
    string rGroupString;
    string rProjectionString;

    rFilterString = "";


    if (pAvgattribute == "") {
        rGroupString = "";
    }
    else {
        rGroupString = "{\"_id\" : null, \"average\" : { \"$avg\" : \"$" + pAvgattribute + "\"  } }";
    }


    rProjectionString = "{ \"_id\" : 0 }";


    rResult = this->cmMongoDriver->fRunQuery(&cOutputList, pDatabase, pCollection,
        rFilterString,
        rProjectionString,
        "",
        0,
        0,
        rGroupString
    );


    if (rResult < 0) {
        return(IMongoDriverAgentInterface::kQueryFails);
    }

    *pOutput = (cOutputList.size() > 0 ? cOutputList.front() : string(""));
    return(IMongoDriverAgentInterface::kGetSuccess);
}


int IMongoDriverAgentInterface::fGetAvgOfGreatherThanValue(string* pOutput, string pDatabase, string pCollection, string pCompareattribute, string pComparevalue, string pAvgattribute) {
    list<string> cOutputList;
    int rResult = 0;


    string rFilterString;
    string rFilterSubString;
    string rGroupString;
    string rProjectionString;


    rFilterString = "";


    if (pAvgattribute == "") {
        rGroupString = "";
    } else {
        rGroupString = "{\"_id\" : null, \"average\" : { \"$avg\" : \"$" + pAvgattribute + "\"  } }";
    }


    rProjectionString = "{ \"_id\" : 0 }";


    rResult = this->cmMongoDriver->fRunQuery(&cOutputList, pDatabase, pCollection,
        rFilterString,
        rProjectionString,
        "",
        0,
        0,
        rGroupString
    );

    if (rResult < 0) {
        return(IMongoDriverAgentInterface::kQueryFails);
    }

    *pOutput = (cOutputList.size() > 0 ? cOutputList.front() : string(""));
    return(IMongoDriverAgentInterface::kGetSuccess);

}

int IMongoDriverAgentInterface::fGetAvgOfLowerThanValue(string* pOutput, string pDatabase, string pCollection, string pCompareattribute, string pComparevalue, string pAvgattribute) {
    list<string> cOutputList;
    int rResult = 0;


    string rFilterString;
    string rFilterSubString;
    string rGroupString;
    string rProjectionString;


    rFilterString = "";


    if (pAvgattribute == "") {
        rGroupString = "";
    }
    else {
        rGroupString = "{\"_id\" : null, \"average\" : { \"$avg\" : \"$" + pAvgattribute + "\"  } }";
    }

    rProjectionString = "{ \"_id\" : 0 }";


    rResult = this->cmMongoDriver->fRunQuery(&cOutputList, pDatabase, pCollection,
                                        rFilterString,
                                        rProjectionString,
                                        "",
                                        0,
                                        0,
                                        rGroupString
    );

    if (rResult < 0) {
        return(IMongoDriverAgentInterface::kQueryFails);
    }

    *pOutput = (cOutputList.size() > 0 ? cOutputList.front() : string(""));
    return(IMongoDriverAgentInterface::kGetSuccess);

}

int IMongoDriverAgentInterface::fGetMaxOfBetweenValues(string* pOutput, string pDatabase, string pCollection, string pBetweenattribute, string pUpperbound, string pLowerbound, string pMaxattribute) {
    list<string> cOutputList;
    int rResult = 0;


    string rFilterString;
    string rFilterSubString;
    string rGroupString;
    string rProjectionString;


    rFilterString = "";


    if (pMaxattribute == "") {
        rGroupString = "";
    }
    else {
        rGroupString = "{\"_id\" : null, \"max\" : { \"$max\" : \"$" + pMaxattribute + "\"  } }";
    }

    rProjectionString = "{ \"_id\" : 0}";


    rResult = this->cmMongoDriver->fRunQuery(&cOutputList, pDatabase, pCollection,
                                        rFilterString,
                                        rProjectionString,
                                        "",
                                        0,
                                        0,
                                        rGroupString
    );

    if (rResult < 0) {
        return(IMongoDriverAgentInterface::kQueryFails);
    }

    *pOutput = (cOutputList.size() > 0 ? cOutputList.front() : string(""));
    return(IMongoDriverAgentInterface::kGetSuccess);

}

int IMongoDriverAgentInterface::fGetMaxOfGreatherThanValue(string* pOutput, string pDatabase, string pCollection, string pCompareattribute, string pComparevalue, string pMaxattribute) {
    list<string> cOutputList;
    int rResult = 0;


    string rFilterString;
    string rFilterSubString;
    string rGroupString;
    string rProjectionString;


    rFilterString = "";


    if (pMaxattribute == "") {
        rGroupString = "";
    }
    else {
        rGroupString = "{\"_id\" : null, \"max\" : { \"$max\" : \"$" + pMaxattribute + "\"  } }";
    }

    rProjectionString = "{ \"_id\" : 0}";


    rResult = this->cmMongoDriver->fRunQuery(&cOutputList, pDatabase, pCollection,
        rFilterString,
        rProjectionString,
        "",
        0,
        0,
        rGroupString
    );

    if (rResult < 0) {
        return(IMongoDriverAgentInterface::kQueryFails);
    }

    *pOutput = (cOutputList.size() > 0 ? cOutputList.front() : string(""));
    return(IMongoDriverAgentInterface::kGetSuccess);

}


int IMongoDriverAgentInterface::fGetMaxOfLowerThanValue(string* pOutput, string pDatabase, string pCollection, string pCompareattribute, string pComparevalue, string pMaxattribute) {
    list<string> cOutputList;
    int rResult = 0;


    string rFilterString;
    string rFilterSubString;
    string rGroupString;
    string rProjectionString;


    rFilterString = "";

    if (pMaxattribute == "") {
        rGroupString = "";
    }
    else {
        rGroupString = "{\"_id\" : null, \"max\" : { \"$max\" : \"$" + pMaxattribute + "\"  } }";
    }

    rProjectionString = "{ \"_id\" : 0}";


    rResult = this->cmMongoDriver->fRunQuery(&cOutputList, pDatabase, pCollection,
                                        rFilterString,
                                        rProjectionString,
                                        "",
                                        0,
                                        0,
                                        rGroupString
    );

    if (rResult < 0) {
        return(IMongoDriverAgentInterface::kQueryFails);
    }

    *pOutput = (cOutputList.size() > 0 ? cOutputList.front() : string(""));
    return(IMongoDriverAgentInterface::kGetSuccess);

}

int IMongoDriverAgentInterface::fGetMinOfBetweenValues(string* pOutput, string pDatabase, string pCollection, string pBetweenattribute, string pUpperbound, string pLowerbound, string pMinattribute) {
    list<string> cOutputList;
    int rResult = 0;


    string rFilterString;
    string rFilterSubString;
    string rGroupString;
    string rProjectionString;


    rFilterString = "";


    if (pMinattribute == "") {
        rGroupString = "";
    }
    else {
        rGroupString = "{\"_id\" : null,  \"min\" : { \"$min\" : \"$" + pMinattribute + "\"  } }";
    }

    rProjectionString = "{ \"_id\" : 0}";


    rResult = this->cmMongoDriver->fRunQuery(&cOutputList, pDatabase, pCollection,
                                        rFilterString,
                                        rProjectionString,
                                        "",
                                        0,
                                        0,
                                        rGroupString
    );

    if (rResult < 0) {
        return(IMongoDriverAgentInterface::kQueryFails);
    }

    *pOutput = (cOutputList.size() > 0 ? cOutputList.front() : string(""));
    return(IMongoDriverAgentInterface::kGetSuccess);

}


int IMongoDriverAgentInterface::fGetMinOfGreatherThanValue(string* pOutput, string pDatabase, string pCollection, string pCompareattribute, string pComparevalue, string pMinattribute) {
    list<string> cOutputList;
    int rResult = 0;


    string rFilterString;
    string rFilterSubString;
    string rGroupString;
    string rProjectionString;


    rFilterString = "";


    if (pMinattribute == "") {
        rGroupString = "";
    }
    else {
        rGroupString = "{\"_id\" : null, \"min\" : { \"$min\" : \"$" + pMinattribute + "\"  } }";
    }

    rProjectionString = "{ \"_id\" : 0}";


    rResult = this->cmMongoDriver->fRunQuery(&cOutputList, pDatabase, pCollection,
                                        rFilterString,
                                        rProjectionString,
                                        "",
                                        0,
                                        0,
                                        rGroupString
    );

    if (rResult < 0) {
        return(IMongoDriverAgentInterface::kQueryFails);
    }

    *pOutput = (cOutputList.size() > 0 ? cOutputList.front() : string(""));
    return(IMongoDriverAgentInterface::kGetSuccess);

}

int IMongoDriverAgentInterface::fGetMinOfLowerThanValue(string* pOutput, string pDatabase, string pCollection, string pCompareattribute, string pComparevalue, string pMinattribute) {
    list<string> cOutputList;
    int rResult = 0;


    string rFilterString;
    string rFilterSubString;
    string rGroupString;
    string rProjectionString;


    rFilterString = "";


    if (pMinattribute == "") {
        rGroupString = "";
    }
    else {
        rGroupString = "{\"_id\" : null, \"min\" : { \"$min\" : \"$" + pMinattribute + "\"  } }";
    }

    rProjectionString = "{ \"_id\" : 0}";


    rResult = this->cmMongoDriver->fRunQuery(&cOutputList, pDatabase, pCollection,
                                        rFilterString,
                                        rProjectionString,
                                        "",
                                        0,
                                        0,
                                        rGroupString
    );

    if (rResult < 0) {
        return(IMongoDriverAgentInterface::kQueryFails);
    }

    *pOutput = (cOutputList.size() > 0 ? cOutputList.front() : string(""));
    return(IMongoDriverAgentInterface::kGetSuccess);

}

int IMongoDriverAgentInterface::fGetSumOfBetweenValues(string* pOutput, string pDatabase, string pCollection, string pBetweenattribute, string pUpperbound, string pLowerbound, string pSumattribute) {
    list<string> cOutputList;
    int rResult = 0;


    string rFilterString;
    string rFilterSubString;
    string rProjectionString;
    string rGroupString;


    rFilterString = "";

    rProjectionString = "{ \"_id\" : 0}";


    if (pSumattribute == "") {
        rGroupString = "";
    }
    else {
        rGroupString = "{\"_id\" : null, \"sum\" : { \"$sum\" : \"$" + pSumattribute + "\"  } }";
    }


    rResult = this->cmMongoDriver->fRunQuery(&cOutputList, pDatabase, pCollection,
                                        rFilterString,
                                        "",
                                        "",
                                        0,
                                        0,
                                        rGroupString
    );

    if (rResult < 0) {
        return(IMongoDriverAgentInterface::kQueryFails);
    }

    *pOutput = (cOutputList.size() > 0 ? cOutputList.front() : string(""));
    return(IMongoDriverAgentInterface::kGetSuccess);
}

int IMongoDriverAgentInterface::fGetSumOfGreatherThanValue(string* pOutput, string pDatabase, string pCollection, string pCompareattribute, string pComparevalue, string pSumattribute) {
    list<string> cOutputList;
    int rResult = 0;


    string rFilterString;
    string rFilterSubString;
    string rGroupString;
    string rProjectionString;

    rFilterString = "";

    rProjectionString = "{ \"_id\" : 0}";

    if (pSumattribute == "") {
        rGroupString = "";
    }
    else {
        rGroupString = "{\"_id\" : null, \"sum\" : { \"$sum\" : \"$" + pSumattribute + "\"  } }";
    }

    rResult = this->cmMongoDriver->fRunQuery(&cOutputList, pDatabase, pCollection,
                                        rFilterString,
                                        rProjectionString,
                                        "",
                                        0,
                                        0,
                                        rGroupString
    );

    if (rResult < 0) {
        return(IMongoDriverAgentInterface::kQueryFails);
    }

    *pOutput = (cOutputList.size() > 0 ? cOutputList.front() : string(""));
    return(IMongoDriverAgentInterface::kGetSuccess);
}

int IMongoDriverAgentInterface::fGetSumOfLowerThanValue(string* pOutput, string pDatabase, string pCollection, string pCompareattribute, string pComparevalue, string pSumattribute) {
    list<string> cOutputList;
    int rResult = 0;


    string rFilterString;
    string rFilterSubString;
    string rProjectionString;
    string rGroupString;


    rFilterString = "";

    rProjectionString = "{ \"_id\" : 0}";

    if (pSumattribute == "") {
        rGroupString = "";
    }
    else {
        rGroupString = "{\"_id\" : null, \"sum\" : { \"$sum\" : \"$" + pSumattribute + "\"  } }";
    }


    rResult = this->cmMongoDriver->fRunQuery(&cOutputList, pDatabase, pCollection,
                                        rFilterString,
                                        rProjectionString,
                                        "",
                                        0,
                                        0,
                                        rGroupString
    );

    if (rResult < 0) {
        return(IMongoDriverAgentInterface::kQueryFails);
    }

    *pOutput = (cOutputList.size() > 0 ? cOutputList.front() : string(""));
    return(IMongoDriverAgentInterface::kGetSuccess);

}

int IMongoDriverAgentInterface::fGetCountOfBetweenValues(string* pOutput, string pDatabase, string pCollection, string pBetweenattribute, string pUpperbound, string pLowerbound) {
    list<string> cOutputList;
    int rResult = 0;


    string rFilterString;
    string rFilterSubString;
    string rProjectionString;
    string rGroupString;
   

    rFilterString = "";

    rProjectionString = "{ \"_id\" : 0}";
    rGroupString = "{\"_id\" : null, \"count\" : { \"$sum\" : 1 } }";


    rResult = this->cmMongoDriver->fRunQuery(&cOutputList, pDatabase, pCollection,
                                        rFilterString,
                                        rProjectionString,
                                        "",
                                        0,
                                        0,
                                        rGroupString
    );

    if (rResult < 0) {
        return(IMongoDriverAgentInterface::kQueryFails);
    }

    *pOutput = (cOutputList.size() > 0 ? cOutputList.front() : string(""));
    return(IMongoDriverAgentInterface::kGetSuccess);



}


int IMongoDriverAgentInterface::fGetCountOfGreatherThanValue(string* pOutput, string pDatabase, string pCollection, string pCompareattribute, string pComparevalue) {
    list<string> cOutputList;
    int rResult = 0;


    string rFilterString;
    string rFilterSubString;
    string rGroupString;
    string rProjectionString;

    rFilterString = "";

    rProjectionString = "{ \"_id\" : 0}";
    rGroupString = "{\"_id\" : null, \"count\" : { \"$sum\" : 1 } }";
    

    rResult = cmMongoDriver->fRunQuery(&cOutputList, pDatabase, pCollection,
        rFilterString,
        rProjectionString,
        "",
        0,
        0,
        rGroupString
    );



   
    if (rResult < 0) {
        return(IMongoDriverAgentInterface::kQueryFails);
    }

    *pOutput = (cOutputList.size() > 0 ? cOutputList.front() : string(""));
    return(IMongoDriverAgentInterface::kGetSuccess);
}


int IMongoDriverAgentInterface::fGetCountOfLowerThanValue(string* pOutput, string pDatabase, string pCollection, string pCompareattribute, string pComparevalue) {
    list<string> cOutputList;
    int rResult = 0;


    string rFilterString;
    string rFilterSubString;
    string rProjectionString;
    string rGroupString;


    rFilterString = "";

    rProjectionString = "{ \"_id\" : 0}";
    rGroupString = "{\"_id\" : null, \"count\" : { \"$sum\" : 1} }";
 

    
    rResult = this->cmMongoDriver->fRunQuery(&cOutputList, pDatabase, pCollection,
                                        rFilterString,
                                        rProjectionString,
                                        "",
                                        0,
                                        0,
                                        rGroupString
    );

    if (rResult < 0) {
        return(IMongoDriverAgentInterface::kQueryFails);
    }

    *pOutput = (cOutputList.size() > 0 ? cOutputList.front() : string(""));
    return(IMongoDriverAgentInterface::kGetSuccess);

}



int IMongoDriverAgentInterface::fGetBetweenTimePoints(list<string>* output, string database, string collection, string betweenattribute, chrono::time_point<chrono::high_resolution_clock> to, chrono::time_point<chrono::high_resolution_clock> from,  string sortattribute, string projectionattribute) {
    list<string> cOutputList;
    int rResult = 0;

    chrono::high_resolution_clock::time_point cEquality_highsystem = chrono::high_resolution_clock::now();
    chrono::system_clock::time_point cEquality_system = chrono::system_clock::now();
    auto cConvertedTimeTo = cEquality_system + (to - cEquality_highsystem);
    auto cConvertedTimeFrom = cEquality_system + (from - cEquality_highsystem);

    
    bsoncxx::document::view_or_value cFilter;

    bsoncxx::document::view_or_value cProjectionDocument;
    bsoncxx::document::view_or_value cSortDocument;
    bsoncxx::document::view_or_value cGroupDocument;
    
    
    cFilter = bsoncxx::builder::stream::document{} << betweenattribute
        << bsoncxx::builder::stream::open_document 
        << "$lt" << bsoncxx::types::b_date(chrono::time_point_cast<chrono::milliseconds>(cConvertedTimeTo)) 
        << "$gt" << bsoncxx::types::b_date(chrono::time_point_cast<chrono::milliseconds>(cConvertedTimeFrom)) 
        << bsoncxx::builder::stream::close_document
        << bsoncxx::builder::stream::finalize;
    
    
    
    

    if (projectionattribute == "") {
        cProjectionDocument = bsoncxx::builder::stream::document{} << "_id" << 0 << bsoncxx::builder::stream::finalize;
    } else {
        cProjectionDocument = bsoncxx::builder::stream::document{} << "_id" << 0 << projectionattribute << 1 << bsoncxx::builder::stream::finalize;
    }

    if (sortattribute == "") {
        cSortDocument = bsoncxx::document::view_or_value();
    } else {
        cSortDocument = bsoncxx::builder::stream::document{} << sortattribute << 1 << "_id" << 1 << bsoncxx::builder::stream::finalize;
    }


    rResult = this->cmMongoDriver->fRunQuery(&cOutputList, database, collection,
        cFilter,
        cProjectionDocument,
        cSortDocument
    );

    if (rResult < 0) {
        return(IMongoDriverAgentInterface::kQueryFails);
    }

    *output = cOutputList;
    return(IMongoDriverAgentInterface::kGetSuccess);
}

int IMongoDriverAgentInterface::fGetAfterTimePoint(list<string>* output, string database, string collection, string compareattribute, chrono::time_point<chrono::high_resolution_clock> after,  string sortattribute, string projectionattribute) {
    list<string> cOutputList;
    int rResult = 0;

    chrono::high_resolution_clock::time_point cEquality_highsystem = chrono::high_resolution_clock::now();
    chrono::system_clock::time_point cEquality_system = chrono::system_clock::now();
    auto cConvertedTimeAfter = cEquality_system + (after - cEquality_highsystem);

    
    bsoncxx::document::view_or_value cFilter;

    bsoncxx::document::view_or_value cProjectionDocument;
    bsoncxx::document::view_or_value cSortDocument;
    bsoncxx::document::view_or_value cGroupDocument;


    cFilter = bsoncxx::builder::stream::document{} << compareattribute
        << bsoncxx::builder::stream::open_document
        << "$gt" << bsoncxx::types::b_date(chrono::time_point_cast<chrono::milliseconds>(cConvertedTimeAfter))
        << bsoncxx::builder::stream::close_document
        << bsoncxx::builder::stream::finalize;

    

    

    if (projectionattribute == "") {
        cProjectionDocument = bsoncxx::builder::stream::document{} << "_id" << 0 << bsoncxx::builder::stream::finalize;
    }
    else {
        cProjectionDocument = bsoncxx::builder::stream::document{} << "_id" << 0 << projectionattribute << 1 << bsoncxx::builder::stream::finalize;
    }

    if (sortattribute == "") {
        cSortDocument = bsoncxx::document::view_or_value();
    }
    else {
        cSortDocument = bsoncxx::builder::stream::document{} << sortattribute << 1 << "_id" << 1 << bsoncxx::builder::stream::finalize;
    }


    rResult = cmMongoDriver->fRunQuery(&cOutputList, database, collection,
        cFilter,
        cProjectionDocument,
        cSortDocument
    );

    if (rResult < 0) {
        return(IMongoDriverAgentInterface::kQueryFails);
    }

    *output = cOutputList;
    return(IMongoDriverAgentInterface::kGetSuccess);
}
int IMongoDriverAgentInterface::fGetBeforeTimePoint(list<string>* output, string database, string collection, string compareattribute, chrono::time_point<chrono::high_resolution_clock> before,  string sortattribute, string projectionattribute) {
    list<string> cOutputList;
    int rResult = 0;

    chrono::high_resolution_clock::time_point cEquality_highsystem = chrono::high_resolution_clock::now();
    chrono::system_clock::time_point cEquality_system = chrono::system_clock::now();
    auto cConvertedTimeBefore = cEquality_system + (before - cEquality_highsystem);

    
    bsoncxx::document::view_or_value cFilter;

    bsoncxx::document::view_or_value cProjectionDocument;
    bsoncxx::document::view_or_value cSortDocument;
    bsoncxx::document::view_or_value cGroupDocument;


    cFilter = bsoncxx::builder::stream::document{} << compareattribute
        << bsoncxx::builder::stream::open_document
        << "$lt" << bsoncxx::types::b_date(chrono::time_point_cast<chrono::milliseconds>(cConvertedTimeBefore))
        << bsoncxx::builder::stream::close_document
        << bsoncxx::builder::stream::finalize;

    

    

    if (projectionattribute == "") {
        cProjectionDocument = bsoncxx::builder::stream::document{} << "_id" << 0 << bsoncxx::builder::stream::finalize;
    }
    else {
        cProjectionDocument = bsoncxx::builder::stream::document{} << "_id" << 0 << projectionattribute << 1 << bsoncxx::builder::stream::finalize;
    }

    if (sortattribute == "") {
        cSortDocument = bsoncxx::document::view_or_value();
    }
    else {
        cSortDocument = bsoncxx::builder::stream::document{} << sortattribute << 1 << "_id" << 1 << bsoncxx::builder::stream::finalize;
    }


    rResult = cmMongoDriver->fRunQuery(&cOutputList, database, collection,
        cFilter,
        cProjectionDocument,
        cSortDocument
    );

    if (rResult < 0) {
        return(IMongoDriverAgentInterface::kQueryFails);
    }

    *output = cOutputList;
    return(IMongoDriverAgentInterface::kGetSuccess);
}


int IMongoDriverAgentInterface::fGetMinBetweenTimePoints(string* output, string database, string collection, string betweenattribute, chrono::time_point<chrono::high_resolution_clock> to, chrono::time_point<chrono::high_resolution_clock> from, string minattribute,  string projectionattribute) {
    list<string> cOutputList;
    int rResult = 0;

    chrono::high_resolution_clock::time_point cEquality_highsystem = chrono::high_resolution_clock::now();
    chrono::system_clock::time_point cEquality_system = chrono::system_clock::now();
    auto cConvertedTimeTo = cEquality_system + (to - cEquality_highsystem);
    auto cConvertedTimeFrom = cEquality_system + (from - cEquality_highsystem);

    
    bsoncxx::document::view_or_value cFilter;

    bsoncxx::document::view_or_value cProjectionDocument;
    bsoncxx::document::view_or_value cGroupDocument;


    cFilter = bsoncxx::builder::stream::document{} << betweenattribute
        << bsoncxx::builder::stream::open_document
        << "$lt" << bsoncxx::types::b_date(chrono::time_point_cast<chrono::milliseconds>(cConvertedTimeTo))
        << "$gt" << bsoncxx::types::b_date(chrono::time_point_cast<chrono::milliseconds>(cConvertedTimeFrom))
        << bsoncxx::builder::stream::close_document
        << bsoncxx::builder::stream::finalize;

    

    

    if (projectionattribute == "") {
        cProjectionDocument = bsoncxx::builder::stream::document{} << "_id" << 0 << bsoncxx::builder::stream::finalize;
    }
    else {
        cProjectionDocument = bsoncxx::builder::stream::document{} << "_id" << 0 << projectionattribute << 1 << bsoncxx::builder::stream::finalize;
    }

    if (minattribute == "") {
        cGroupDocument = bsoncxx::document::view_or_value();
    }
    else {
        cGroupDocument = bsoncxx::builder::stream::document{} << "_id" << "null" << "min" << bsoncxx::builder::stream::open_document << "$min" << "$" + minattribute << bsoncxx::builder::stream::close_document << bsoncxx::builder::stream::finalize;
    }

    rResult = this->cmMongoDriver->fRunQuery(&cOutputList, database, collection,
        cFilter,
        cProjectionDocument,
        bsoncxx::document::view_or_value(),
        0,
        0,
        cGroupDocument
    );

    if (rResult < 0) {
        return(IMongoDriverAgentInterface::kQueryFails);
    }

    *output = (cOutputList.size() > 0 ? cOutputList.front() : string(""));
    return(IMongoDriverAgentInterface::kGetSuccess);
}


int IMongoDriverAgentInterface::fGetMinAfterTimePoint(string* output, string database, string collection, string compareattribute, chrono::time_point<chrono::high_resolution_clock> after, string minattribute,  string projectionattribute) {
    list<string> cOutputList;
    int rResult = 0;

    chrono::high_resolution_clock::time_point cEquality_highsystem = chrono::high_resolution_clock::now();
    chrono::system_clock::time_point cEquality_system = chrono::system_clock::now();
    auto cConvertedTimeAfter = cEquality_system + (after - cEquality_highsystem);

    
    bsoncxx::document::view_or_value cFilter;

    bsoncxx::document::view_or_value cProjectionDocument;
    bsoncxx::document::view_or_value cGroupDocument;

    cFilter = bsoncxx::builder::stream::document{} << compareattribute
        << bsoncxx::builder::stream::open_document
        << "$gt" << bsoncxx::types::b_date(chrono::time_point_cast<chrono::milliseconds>(cConvertedTimeAfter))
        << bsoncxx::builder::stream::close_document
        << bsoncxx::builder::stream::finalize;

    

    

    if (projectionattribute == "") {
        cProjectionDocument = bsoncxx::builder::stream::document{} << "_id" << 0 << bsoncxx::builder::stream::finalize;
    }
    else {
        cProjectionDocument = bsoncxx::builder::stream::document{} << "_id" << 0 << projectionattribute << 1 << bsoncxx::builder::stream::finalize;
    }

    if (minattribute == "") {
        cGroupDocument = bsoncxx::document::view_or_value();
    }
    else {
        cGroupDocument = bsoncxx::builder::stream::document{} << "_id" << "null" << "min" << bsoncxx::builder::stream::open_document << "$min" << "$" + minattribute << bsoncxx::builder::stream::close_document << bsoncxx::builder::stream::finalize;
    }

    rResult = cmMongoDriver->fRunQuery(&cOutputList, database, collection,
        cFilter,
        cProjectionDocument,
        bsoncxx::document::view_or_value(),
        0,
        0,
        cGroupDocument
    );

    if (rResult < 0) {
        return(IMongoDriverAgentInterface::kQueryFails);
    }

    *output = (cOutputList.size() > 0 ? cOutputList.front() : string(""));
    return(IMongoDriverAgentInterface::kGetSuccess);
}

int IMongoDriverAgentInterface::fGetMinBeforeTimePoint(string* output, string database, string collection, string compareattribute, chrono::time_point<chrono::high_resolution_clock> before, string minattribute,  string projectionattribute) {
    list<string> cOutputList;
    int rResult = 0;

    chrono::high_resolution_clock::time_point cEquality_highsystem = chrono::high_resolution_clock::now();
    chrono::system_clock::time_point cEquality_system = chrono::system_clock::now();
    auto cConvertedTimeBefore = cEquality_system + (before - cEquality_highsystem);

    
    
    bsoncxx::document::view_or_value cFilter;

    bsoncxx::document::view_or_value cProjectionDocument;
    bsoncxx::document::view_or_value cGroupDocument;

    cFilter = bsoncxx::builder::stream::document{} << compareattribute
        << bsoncxx::builder::stream::open_document
        << "$lt" << bsoncxx::types::b_date(chrono::time_point_cast<chrono::milliseconds>(cConvertedTimeBefore))
        << bsoncxx::builder::stream::close_document
        << bsoncxx::builder::stream::finalize;

    

    

    if (projectionattribute == "") {
        cProjectionDocument = bsoncxx::builder::stream::document{} << "_id" << 0 << bsoncxx::builder::stream::finalize;
    }
    else {
        cProjectionDocument = bsoncxx::builder::stream::document{} << "_id" << 0 << projectionattribute << 1 << bsoncxx::builder::stream::finalize;
    }

    if (minattribute == "") {
        cGroupDocument = bsoncxx::document::view_or_value();
    }
    else {
        cGroupDocument = bsoncxx::builder::stream::document{} << "_id" << "null" << "min" << bsoncxx::builder::stream::open_document << "$min" << "$" + minattribute << bsoncxx::builder::stream::close_document << bsoncxx::builder::stream::finalize;
    }

    rResult = cmMongoDriver->fRunQuery(&cOutputList, database, collection,
        cFilter,
        cProjectionDocument,
        bsoncxx::document::view_or_value(),
        0,
        0,
        cGroupDocument
    );

    if (rResult < 0) {
        return(IMongoDriverAgentInterface::kQueryFails);
    }

    *output = (cOutputList.size() > 0 ? cOutputList.front() : string(""));
    return(IMongoDriverAgentInterface::kGetSuccess);
}




int IMongoDriverAgentInterface::fGetMaxBetweenTimePoints(string* output, string database, string collection, string betweenattribute, chrono::time_point<chrono::high_resolution_clock> to, chrono::time_point<chrono::high_resolution_clock> from, string maxattribute,  string projectionattribute) {
    list<string> cOutputList;
    int rResult = 0;

    chrono::high_resolution_clock::time_point cEquality_highsystem = chrono::high_resolution_clock::now();
    chrono::system_clock::time_point cEquality_system = chrono::system_clock::now();
    auto cConvertedTimeTo = cEquality_system + (to - cEquality_highsystem);
    auto cConvertedTimeFrom = cEquality_system + (from - cEquality_highsystem);

    
    bsoncxx::document::view_or_value cFilter;

    bsoncxx::document::view_or_value cProjectionDocument;
    bsoncxx::document::view_or_value cGroupDocument;


    cFilter = bsoncxx::builder::stream::document{} << betweenattribute
        << bsoncxx::builder::stream::open_document
        << "$lt" << bsoncxx::types::b_date(chrono::time_point_cast<chrono::milliseconds>(cConvertedTimeTo))
        << "$gt" << bsoncxx::types::b_date(chrono::time_point_cast<chrono::milliseconds>(cConvertedTimeFrom))
        << bsoncxx::builder::stream::close_document
        << bsoncxx::builder::stream::finalize;


    if (projectionattribute == "") {
        cProjectionDocument = bsoncxx::builder::stream::document{} << "_id" << 0 << bsoncxx::builder::stream::finalize;
    }
    else {
        cProjectionDocument = bsoncxx::builder::stream::document{} << "_id" << 0 << projectionattribute << 1 << bsoncxx::builder::stream::finalize;
    }

    if (maxattribute == "") {
        cGroupDocument = bsoncxx::document::view_or_value();
    }
    else {
        cGroupDocument = bsoncxx::builder::stream::document{} << "_id" << "null" << "max" << bsoncxx::builder::stream::open_document << "$max" << "$" + maxattribute << bsoncxx::builder::stream::close_document << bsoncxx::builder::stream::finalize;
    }

    rResult = this->cmMongoDriver->fRunQuery(&cOutputList, database, collection,
        cFilter,
        cProjectionDocument,
        bsoncxx::document::view_or_value(),
        0,
        0,
        cGroupDocument
    );

    if (rResult < 0) {
        return(IMongoDriverAgentInterface::kQueryFails);
    }

    *output = (cOutputList.size() > 0 ? cOutputList.front() : string(""));
    return(IMongoDriverAgentInterface::kGetSuccess);
}

int IMongoDriverAgentInterface::fGetMaxAfterTimePoint(string* output, string database, string collection, string compareattribute, chrono::time_point<chrono::high_resolution_clock> after, string maxattribute,  string projectionattribute) {
    list<string> cOutputList;
    int rResult = 0;

    chrono::high_resolution_clock::time_point cEquality_highsystem = chrono::high_resolution_clock::now();
    chrono::system_clock::time_point cEquality_system = chrono::system_clock::now();
    auto cConvertedTimeBefore = cEquality_system + (after - cEquality_highsystem);


    
    bsoncxx::document::view_or_value cFilter;

    bsoncxx::document::view_or_value cProjectionDocument;
    bsoncxx::document::view_or_value cGroupDocument;

    cFilter = bsoncxx::builder::stream::document{} << compareattribute
        << bsoncxx::builder::stream::open_document
        << "$lt" << bsoncxx::types::b_date(chrono::time_point_cast<chrono::milliseconds>(cConvertedTimeBefore))
        << bsoncxx::builder::stream::close_document
        << bsoncxx::builder::stream::finalize;


    if (projectionattribute == "") {
        cProjectionDocument = bsoncxx::builder::stream::document{} << "_id" << 0 << bsoncxx::builder::stream::finalize;
    }
    else {
        cProjectionDocument = bsoncxx::builder::stream::document{} << "_id" << 0 << projectionattribute << 1 << bsoncxx::builder::stream::finalize;
    }

    if (maxattribute == "") {
        cGroupDocument = bsoncxx::document::view_or_value();
    }
    else {
        cGroupDocument = bsoncxx::builder::stream::document{} << "_id" << "null" << "max" << bsoncxx::builder::stream::open_document << "$max" << "$" + maxattribute << bsoncxx::builder::stream::close_document << bsoncxx::builder::stream::finalize;
    }

    rResult = cmMongoDriver->fRunQuery(&cOutputList, database, collection,
        cFilter,
        cProjectionDocument,
        bsoncxx::document::view_or_value(),
        0,
        0,
        cGroupDocument
    );

    if (rResult < 0) {
        return(IMongoDriverAgentInterface::kQueryFails);
    }

    *output = (cOutputList.size() > 0 ? cOutputList.front() : string(""));
    return(IMongoDriverAgentInterface::kGetSuccess);
}

int IMongoDriverAgentInterface::fGetMaxBeforeTimePoint(string* output, string database, string collection, string compareattribute, chrono::time_point<chrono::high_resolution_clock> before, string maxattribute,  string projectionattribute) {
    list<string> cOutputList;
    int rResult = 0;

    chrono::high_resolution_clock::time_point cEquality_highsystem = chrono::high_resolution_clock::now();
    chrono::system_clock::time_point cEquality_system = chrono::system_clock::now();
    auto cConvertedTimeBefore = cEquality_system + (before - cEquality_highsystem);


    bsoncxx::document::view_or_value cFilter;

    bsoncxx::document::view_or_value cProjectionDocument;
    bsoncxx::document::view_or_value cGroupDocument;

    cFilter = bsoncxx::builder::stream::document{} << compareattribute
        << bsoncxx::builder::stream::open_document
        << "$lt" << bsoncxx::types::b_date(chrono::time_point_cast<chrono::milliseconds>(cConvertedTimeBefore))
        << bsoncxx::builder::stream::close_document
        << bsoncxx::builder::stream::finalize;

    if (projectionattribute == "") {
        cProjectionDocument = bsoncxx::builder::stream::document{} << "_id" << 0 << bsoncxx::builder::stream::finalize;
    } else {
        cProjectionDocument = bsoncxx::builder::stream::document{} << "_id" << 0 << projectionattribute << 1 << bsoncxx::builder::stream::finalize;
    }

    if (maxattribute == "") {
        cGroupDocument = bsoncxx::document::view_or_value();
    }
    else {
        cGroupDocument = bsoncxx::builder::stream::document{} << "_id" << "null" << "max" << bsoncxx::builder::stream::open_document << "$max" << "$" + maxattribute << bsoncxx::builder::stream::close_document << bsoncxx::builder::stream::finalize;
    }

    rResult = cmMongoDriver->fRunQuery(&cOutputList, database, collection,
        cFilter,
        cProjectionDocument,
        bsoncxx::document::view_or_value(),
        0,
        0,
        cGroupDocument
    );

    if (rResult < 0) {
        return(IMongoDriverAgentInterface::kQueryFails);
    }

    *output = (cOutputList.size() > 0 ? cOutputList.front() : string(""));
    return(IMongoDriverAgentInterface::kGetSuccess);
}

int IMongoDriverAgentInterface::fGetSumBetweenTimePoints(string* output, string database, string collection, string betweenattribute, chrono::time_point<chrono::high_resolution_clock> to, chrono::time_point<chrono::high_resolution_clock> from, string sumattribute,  string projectionattribute) {
    list<string> cOutputList;
    int rResult = 0;

    chrono::high_resolution_clock::time_point cEquality_highsystem = chrono::high_resolution_clock::now();
    chrono::system_clock::time_point cEquality_system = chrono::system_clock::now();
    auto cConvertedTimeTo = cEquality_system + (to - cEquality_highsystem);
    auto cConvertedTimeFrom = cEquality_system + (from - cEquality_highsystem);

    
    bsoncxx::document::view_or_value cFilter;

    bsoncxx::document::view_or_value cProjectionDocument;
    bsoncxx::document::view_or_value cGroupDocument;


    cFilter = bsoncxx::builder::stream::document{} << betweenattribute
        << bsoncxx::builder::stream::open_document
        << "$lt" << bsoncxx::types::b_date(chrono::time_point_cast<chrono::milliseconds>(cConvertedTimeTo))
        << "$gt" << bsoncxx::types::b_date(chrono::time_point_cast<chrono::milliseconds>(cConvertedTimeFrom))
        << bsoncxx::builder::stream::close_document
        << bsoncxx::builder::stream::finalize;

    

    if (projectionattribute == "") {
        cProjectionDocument = bsoncxx::builder::stream::document{} << "_id" << 0 << bsoncxx::builder::stream::finalize;
    } else {
        cProjectionDocument = bsoncxx::builder::stream::document{} << "_id" << 0 << projectionattribute << 1 << bsoncxx::builder::stream::finalize;
    }

    if (sumattribute == "") {
        cGroupDocument = bsoncxx::document::view_or_value();
    } else {
        cGroupDocument = bsoncxx::builder::stream::document{} << "_id" << "null" << "sum" << bsoncxx::builder::stream::open_document << "$sum" << 1 << bsoncxx::builder::stream::close_document << bsoncxx::builder::stream::finalize;
    }

    rResult = this->cmMongoDriver->fRunQuery(&cOutputList, database, collection,
        cFilter,
        cProjectionDocument,
        bsoncxx::document::view_or_value(),
        0,
        0,
        cGroupDocument
    );

    if (rResult < 0) {
        return(IMongoDriverAgentInterface::kQueryFails);
    }

    *output = (cOutputList.size() > 0 ? cOutputList.front() : string(""));
    return(IMongoDriverAgentInterface::kGetSuccess);
}

int IMongoDriverAgentInterface::fGetSumAfterTimePoint(string* output, string database, string collection, string compareattribute, chrono::time_point<chrono::high_resolution_clock> after, string sumattribute,  string projectionattribute) {
    list<string> cOutputList;
    int rResult = 0;

    chrono::high_resolution_clock::time_point cEquality_highsystem = chrono::high_resolution_clock::now();
    chrono::system_clock::time_point cEquality_system = chrono::system_clock::now();
    auto cConvertedTimeAfter = cEquality_system + (after - cEquality_highsystem);

    
    bsoncxx::document::view_or_value cFilter;

    bsoncxx::document::view_or_value cProjectionDocument;
    bsoncxx::document::view_or_value cGroupDocument;

    cFilter = bsoncxx::builder::stream::document{} << compareattribute
        << bsoncxx::builder::stream::open_document
        << "$gt" << bsoncxx::types::b_date(chrono::time_point_cast<chrono::milliseconds>(cConvertedTimeAfter))
        << bsoncxx::builder::stream::close_document
        << bsoncxx::builder::stream::finalize;

    

    

    if (projectionattribute == "") {
        cProjectionDocument = bsoncxx::builder::stream::document{} << "_id" << 0 << bsoncxx::builder::stream::finalize;
    }
    else {
        cProjectionDocument = bsoncxx::builder::stream::document{} << "_id" << 0 << projectionattribute << 1 << bsoncxx::builder::stream::finalize;
    }

    if (sumattribute == "") {
        cGroupDocument = bsoncxx::document::view_or_value();
    }
    else {
        cGroupDocument = bsoncxx::builder::stream::document{} << "_id" << "null" << "sum" << bsoncxx::builder::stream::open_document << "$sum" << "$" + sumattribute << bsoncxx::builder::stream::close_document << bsoncxx::builder::stream::finalize;
    }

    rResult = cmMongoDriver->fRunQuery(&cOutputList, database, collection,
        cFilter,
        cProjectionDocument,
        bsoncxx::document::view_or_value(),
        0,
        0,
        cGroupDocument
    );

    if (rResult < 0) {
        return(IMongoDriverAgentInterface::kQueryFails);
    }

    *output = (cOutputList.size() > 0 ? cOutputList.front() : string(""));
    return(IMongoDriverAgentInterface::kGetSuccess);
}
int IMongoDriverAgentInterface::fGetSumBeforeTimePoint(string* output, string database, string collection, string compareattribute, chrono::time_point<chrono::high_resolution_clock> before, string sumattribute,  string projectionattribute) {
    list<string> cOutputList;
    int rResult = 0;

    chrono::high_resolution_clock::time_point cEquality_highsystem = chrono::high_resolution_clock::now();
    chrono::system_clock::time_point cEquality_system = chrono::system_clock::now();
    auto cConvertedTimeBefore = cEquality_system + (before - cEquality_highsystem);


    
    bsoncxx::document::view_or_value cFilter;

    bsoncxx::document::view_or_value cProjectionDocument;
    bsoncxx::document::view_or_value cGroupDocument;

    cFilter = bsoncxx::builder::stream::document{} << compareattribute
        << bsoncxx::builder::stream::open_document
        << "$lt" << bsoncxx::types::b_date(chrono::time_point_cast<chrono::milliseconds>(cConvertedTimeBefore))
        << bsoncxx::builder::stream::close_document
        << bsoncxx::builder::stream::finalize;

    

    

    if (projectionattribute == "") {
        cProjectionDocument = bsoncxx::builder::stream::document{} << "_id" << 0 << bsoncxx::builder::stream::finalize;
    }
    else {
        cProjectionDocument = bsoncxx::builder::stream::document{} << "_id" << 0 << projectionattribute << 1 << bsoncxx::builder::stream::finalize;
    }

    if (sumattribute == "") {
        cGroupDocument = bsoncxx::document::view_or_value();
    }
    else {
        cGroupDocument = bsoncxx::builder::stream::document{} << "_id" << "null" << "sum" << bsoncxx::builder::stream::open_document << "$sum" << "$" + sumattribute << bsoncxx::builder::stream::close_document << bsoncxx::builder::stream::finalize;
    }

    rResult = cmMongoDriver->fRunQuery(&cOutputList, database, collection,
        cFilter,
        cProjectionDocument,
        bsoncxx::document::view_or_value(),
        0,
        0,
        cGroupDocument
    );

    if (rResult < 0) {
        return(IMongoDriverAgentInterface::kQueryFails);
    }

    *output = (cOutputList.size() > 0 ? cOutputList.front() : string(""));
    return(IMongoDriverAgentInterface::kGetSuccess);
}

int IMongoDriverAgentInterface::fGetAvgBetweenTimePoints(string* output, string database, string collection, string betweenattribute, chrono::time_point<chrono::high_resolution_clock> to, chrono::time_point<chrono::high_resolution_clock> from, string avgattribute,  string projectionattribute) {
    list<string> cOutputList;
    int rResult = 0;

    chrono::high_resolution_clock::time_point cEquality_highsystem = chrono::high_resolution_clock::now();
    chrono::system_clock::time_point cEquality_system = chrono::system_clock::now();
    auto cConvertedTimeTo = cEquality_system + (to - cEquality_highsystem);
    auto cConvertedTimeFrom = cEquality_system + (from - cEquality_highsystem);

    
    bsoncxx::document::view_or_value cFilter;

    bsoncxx::document::view_or_value cProjectionDocument;
    bsoncxx::document::view_or_value cGroupDocument;


    cFilter = bsoncxx::builder::stream::document{} << betweenattribute
        << bsoncxx::builder::stream::open_document
        << "$lt" << bsoncxx::types::b_date(chrono::time_point_cast<chrono::milliseconds>(cConvertedTimeTo))
        << "$gt" << bsoncxx::types::b_date(chrono::time_point_cast<chrono::milliseconds>(cConvertedTimeFrom))
        << bsoncxx::builder::stream::close_document
        << bsoncxx::builder::stream::finalize;

    

    

    if (projectionattribute == "") {
        cProjectionDocument = bsoncxx::builder::stream::document{} << "_id" << 0 << bsoncxx::builder::stream::finalize;
    }
    else {
        cProjectionDocument = bsoncxx::builder::stream::document{} << "_id" << 0 << projectionattribute << 1 << bsoncxx::builder::stream::finalize;
    }

    if (avgattribute == "") {
        cGroupDocument = bsoncxx::document::view_or_value();
    }
    else {
        cGroupDocument = bsoncxx::builder::stream::document{} << "_id" << "null" << "avg" << bsoncxx::builder::stream::open_document << "$avg" << "$" + avgattribute << bsoncxx::builder::stream::close_document << bsoncxx::builder::stream::finalize;
    }

    rResult = this->cmMongoDriver->fRunQuery(&cOutputList, database, collection,
        cFilter,
        cProjectionDocument,
        bsoncxx::document::view_or_value(),
        0,
        0,
        cGroupDocument
    );

    if (rResult < 0) {
        return(IMongoDriverAgentInterface::kQueryFails);
    }

    *output = (cOutputList.size() > 0 ? cOutputList.front() : string(""));
    return(IMongoDriverAgentInterface::kGetSuccess);
}
int IMongoDriverAgentInterface::fGetAvgAfterTimePoint(string* output, string database, string collection, string compareattribute, chrono::time_point<chrono::high_resolution_clock> after, string avgattribute,  string projectionattribute) {
    list<string> cOutputList;
    int rResult = 0;

    chrono::high_resolution_clock::time_point cEquality_highsystem = chrono::high_resolution_clock::now();
    chrono::system_clock::time_point cEquality_system = chrono::system_clock::now();
    auto cConvertedTimeAfter = cEquality_system + (after - cEquality_highsystem);


    
    bsoncxx::document::view_or_value cFilter;

    bsoncxx::document::view_or_value cProjectionDocument;
    bsoncxx::document::view_or_value cGroupDocument;

    cFilter = bsoncxx::builder::stream::document{} << compareattribute
        << bsoncxx::builder::stream::open_document
        << "$gt" << bsoncxx::types::b_date(chrono::time_point_cast<chrono::milliseconds>(cConvertedTimeAfter))
        << bsoncxx::builder::stream::close_document
        << bsoncxx::builder::stream::finalize;

    

    

    if (projectionattribute == "") {
        cProjectionDocument = bsoncxx::builder::stream::document{} << "_id" << 0 << bsoncxx::builder::stream::finalize;
    }
    else {
        cProjectionDocument = bsoncxx::builder::stream::document{} << "_id" << 0 << projectionattribute << 1 << bsoncxx::builder::stream::finalize;
    }

    if (avgattribute == "") {
        cGroupDocument = bsoncxx::document::view_or_value();
    }
    else {
        cGroupDocument = bsoncxx::builder::stream::document{} << "_id" << "null" << "avg" << bsoncxx::builder::stream::open_document << "$avg" << "$" + avgattribute << bsoncxx::builder::stream::close_document << bsoncxx::builder::stream::finalize;
    }

    rResult = cmMongoDriver->fRunQuery(&cOutputList, database, collection,
        cFilter,
        cProjectionDocument,
        bsoncxx::document::view_or_value(),
        0,
        0,
        cGroupDocument
    );

    if (rResult < 0) {
        return(IMongoDriverAgentInterface::kQueryFails);
    }

    *output = (cOutputList.size() > 0 ? cOutputList.front() : string(""));
    return(IMongoDriverAgentInterface::kGetSuccess);
}
int IMongoDriverAgentInterface::fGetAvgBeforeTimePoint(string* output, string database, string collection, string compareattribute, chrono::time_point<chrono::high_resolution_clock> before, string avgattribute,  string projectionattribute) {
    list<string> cOutputList;
    int rResult = 0;

    chrono::high_resolution_clock::time_point cEquality_highsystem = chrono::high_resolution_clock::now();
    chrono::system_clock::time_point cEquality_system = chrono::system_clock::now();
    auto cConvertedTimeBefore = cEquality_system + (before - cEquality_highsystem);


    
    bsoncxx::document::view_or_value cFilter;

    bsoncxx::document::view_or_value cProjectionDocument;
    bsoncxx::document::view_or_value cGroupDocument;

    cFilter = bsoncxx::builder::stream::document{} << compareattribute
        << bsoncxx::builder::stream::open_document
        << "$lt" << bsoncxx::types::b_date(chrono::time_point_cast<chrono::milliseconds>(cConvertedTimeBefore))
        << bsoncxx::builder::stream::close_document
        << bsoncxx::builder::stream::finalize;

    

    

    if (projectionattribute == "") {
        cProjectionDocument = bsoncxx::builder::stream::document{} << "_id" << 0 << bsoncxx::builder::stream::finalize;
    }
    else {
        cProjectionDocument = bsoncxx::builder::stream::document{} << "_id" << 0 << projectionattribute << 1 << bsoncxx::builder::stream::finalize;
    }

    if (avgattribute == "") {
        cGroupDocument = bsoncxx::document::view_or_value();
    }
    else {
        cGroupDocument = bsoncxx::builder::stream::document{} << "_id" << "null" << "avg" << bsoncxx::builder::stream::open_document << "$avg" << "$" + avgattribute << bsoncxx::builder::stream::close_document << bsoncxx::builder::stream::finalize;
    }

    rResult = cmMongoDriver->fRunQuery(&cOutputList, database, collection,
        cFilter,
        cProjectionDocument,
        bsoncxx::document::view_or_value(),
        0,
        0,
        cGroupDocument
    );

    if (rResult < 0) {
        return(IMongoDriverAgentInterface::kQueryFails);
    }

    *output = (cOutputList.size() > 0 ? cOutputList.front() : string(""));
    return(IMongoDriverAgentInterface::kGetSuccess);
}



int IMongoDriverAgentInterface::fGetCountBetweenTimePoints(string* output, string database, string collection, string betweenattribute, chrono::time_point<chrono::high_resolution_clock> to, chrono::time_point<chrono::high_resolution_clock> from,  string projectionattribute) {
    list<string> cOutputList;
    int rResult = 0;

    chrono::high_resolution_clock::time_point cEquality_highsystem = chrono::high_resolution_clock::now();
    chrono::system_clock::time_point cEquality_system = chrono::system_clock::now();
    auto cConvertedTimeTo = cEquality_system + (to - cEquality_highsystem);
    auto cConvertedTimeFrom = cEquality_system + (from - cEquality_highsystem);

    
    bsoncxx::document::view_or_value cFilter;

    bsoncxx::document::view_or_value cProjectionDocument;
    bsoncxx::document::view_or_value cGroupDocument;


    cFilter = bsoncxx::builder::stream::document{} << betweenattribute
        << bsoncxx::builder::stream::open_document
        << "$lt" << bsoncxx::types::b_date(chrono::time_point_cast<chrono::milliseconds>(cConvertedTimeTo))
        << "$gt" << bsoncxx::types::b_date(chrono::time_point_cast<chrono::milliseconds>(cConvertedTimeFrom))
        << bsoncxx::builder::stream::close_document
        << bsoncxx::builder::stream::finalize;


    if (projectionattribute == "") {
        cProjectionDocument = bsoncxx::builder::stream::document{} << "_id" << 0 << bsoncxx::builder::stream::finalize;
    }
    else {
        cProjectionDocument = bsoncxx::builder::stream::document{} << "_id" << 0 << projectionattribute << 1 << bsoncxx::builder::stream::finalize;
    }

   
    cGroupDocument = bsoncxx::builder::stream::document{} << "_id" << "null" << "count" << bsoncxx::builder::stream::open_document << "$sum" << 1 << bsoncxx::builder::stream::close_document << bsoncxx::builder::stream::finalize;

    rResult = this->cmMongoDriver->fRunQuery(&cOutputList, database, collection,
        cFilter,
        cProjectionDocument,
        bsoncxx::document::view_or_value(),
        0,
        0,
        cGroupDocument
    );

    if (rResult < 0) {
        return(IMongoDriverAgentInterface::kQueryFails);
    }

    *output = (cOutputList.size() > 0 ? cOutputList.front() : string(""));
    return(IMongoDriverAgentInterface::kGetSuccess);
}
int IMongoDriverAgentInterface::fGetCountAfterTimePoint(string* output, string database, string collection, string compareattribute, chrono::time_point<chrono::high_resolution_clock> after,  string projectionattribute) {
    list<string> cOutputList;
    int rResult = 0;

    chrono::high_resolution_clock::time_point cEquality_highsystem = chrono::high_resolution_clock::now();
    chrono::system_clock::time_point cEquality_system = chrono::system_clock::now();
    auto cConvertedTimeAfter = cEquality_system + (after - cEquality_highsystem);


    
    bsoncxx::document::view_or_value cFilter;

    bsoncxx::document::view_or_value cProjectionDocument;
    bsoncxx::document::view_or_value cGroupDocument;

    cFilter = bsoncxx::builder::stream::document{} << compareattribute
        << bsoncxx::builder::stream::open_document
        << "$gt" << bsoncxx::types::b_date(chrono::time_point_cast<chrono::milliseconds>(cConvertedTimeAfter))
        << bsoncxx::builder::stream::close_document
        << bsoncxx::builder::stream::finalize;


    if (projectionattribute == "") {
        cProjectionDocument = bsoncxx::builder::stream::document{} << "_id" << 0 << bsoncxx::builder::stream::finalize;
    }
    else {
        cProjectionDocument = bsoncxx::builder::stream::document{} << "_id" << 0 << projectionattribute << 1 << bsoncxx::builder::stream::finalize;
    }

   
    cGroupDocument = bsoncxx::builder::stream::document{} << "_id" << "null" << "count" << bsoncxx::builder::stream::open_document << "$sum" << 1 << bsoncxx::builder::stream::close_document << bsoncxx::builder::stream::finalize;
  

    rResult = cmMongoDriver->fRunQuery(&cOutputList, database, collection,
        cFilter,
        cProjectionDocument,
        bsoncxx::document::view_or_value(),
        0,
        0,
        cGroupDocument
    );

    if (rResult < 0) {
        return(IMongoDriverAgentInterface::kQueryFails);
    }

    *output = (cOutputList.size() > 0 ? cOutputList.front() : string(""));
    return(IMongoDriverAgentInterface::kGetSuccess);
}


int IMongoDriverAgentInterface::fGetCountBeforeTimePoint(string* output, string database, string collection, string compareattribute, chrono::time_point<chrono::high_resolution_clock> before,  string projectionattribute) {
    list<string> cOutputList;
    int rResult = 0;

    chrono::high_resolution_clock::time_point cEquality_highsystem = chrono::high_resolution_clock::now();
    chrono::system_clock::time_point cEquality_system = chrono::system_clock::now();
    auto cConvertedTimeBefore = cEquality_system + (before - cEquality_highsystem);


    
    bsoncxx::document::view_or_value cFilter;

    bsoncxx::document::view_or_value cProjectionDocument;
    bsoncxx::document::view_or_value cGroupDocument;

    cFilter = bsoncxx::builder::stream::document{} << compareattribute
        << bsoncxx::builder::stream::open_document
        << "$lt" << bsoncxx::types::b_date(chrono::time_point_cast<chrono::milliseconds>(cConvertedTimeBefore))
        << bsoncxx::builder::stream::close_document
        << bsoncxx::builder::stream::finalize;

    

    

    if (projectionattribute == "") {
        cProjectionDocument = bsoncxx::builder::stream::document{} << "_id" << 0 << bsoncxx::builder::stream::finalize;
    }
    else {
        cProjectionDocument = bsoncxx::builder::stream::document{} << "_id" << 0 << projectionattribute << 1 << bsoncxx::builder::stream::finalize;
    }

    cGroupDocument = bsoncxx::builder::stream::document{} << "_id" << "null" << "min" << bsoncxx::builder::stream::open_document << "$sum" << 1 << bsoncxx::builder::stream::close_document << bsoncxx::builder::stream::finalize;

    rResult = cmMongoDriver->fRunQuery(&cOutputList, database, collection,
        cFilter,
        cProjectionDocument,
        bsoncxx::document::view_or_value(),
        0,
        0,
        cGroupDocument
    );

    if (rResult < 0) {
        return(IMongoDriverAgentInterface::kQueryFails);
    }

    *output = (cOutputList.size() > 0 ? cOutputList.front() : string(""));
    return(IMongoDriverAgentInterface::kGetSuccess);
}
















                
                

