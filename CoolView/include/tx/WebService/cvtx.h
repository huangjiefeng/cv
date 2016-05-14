namespace tx_soap {
//gsoap ns service name: CvTx
//gsoap ns service style: rpc
//gsoap ns service encoding: encoded
//gsoap ns service namespace: http://127.0.0.1:8091/CvTx.wsdl
//gsoap ns service location: http://127.0.0.1:8091/CvTx
//gsoap ns schema  namespace:    urn:CvTx
#import "stlvector.h"

typedef std::string xsd__string;

//
// db query
//

class ns__DBConferenceNode {
public:
  int cid;
  xsd__string title;
};

class ns__QueryDBConferenceResult {
public:
  int total;
  int offset;
  std::vector<ns__DBConferenceNode> conferences;
};

class ns__DBTerminalNode {
public:
  xsd__string uri;
  xsd__string name;
};

class ns__QueryDBTerminalResult {
public:
  std::vector<ns__DBTerminalNode> terminals;
};

class ns__QueryDBEpisodeDatesResult {
public:
  std::vector<short> dates;
};

class ns__DBEpisodeNode {
public:
  long long id;
  xsd__string terminal_uri;
  xsd__string title;
  xsd__string requester;
  long long start_time;
  long long duration;
  xsd__string participants;
  xsd__string keywords;
  xsd__string description;
};

class ns__QueryDBEpisodeResult {
public:
  int total;
  int offset;
  std::vector<ns__DBEpisodeNode> episodes;
};

class ns__QueryDBEpisodeByTimeResult {
public:
  long long start;
  long long end;
  std::vector<ns__DBEpisodeNode> episodes;
};

class ns__DBViewNode {
public:
  int id;
  short camera_index;
  long long start_time;
  long long duration;
  xsd__string format;
  xsd__string file_path;
  int status;
};

class ns__QueryDBViewResult {
public:
  std::vector<ns__DBViewNode> views;
};

int ns__QueryDBConference(int offset, int count, ns__QueryDBConferenceResult *result);

int ns__QueryDBTerminal(int cid, ns__QueryDBTerminalResult *result);

int ns__QueryDBEpisodeDates(int cid, xsd__string uri, ns__QueryDBEpisodeDatesResult *result);

int ns__QueryDBEpisode(int cid, xsd__string uri, int offset, int count, 
                   ns__QueryDBEpisodeResult *result);

int ns__QueryDBEpisodeByTime(int cid, xsd__string uri, long long start, long long end, 
                         ns__QueryDBEpisodeResult *result);

int ns__QueryDBView(long long eid, ns__QueryDBViewResult *result);


//
// Record control
//

class ns__StartRecordParam {
public:
  xsd__string focus;
  xsd__string requester_uri;
  xsd__string target_uri;
  xsd__string title;
  xsd__string participants;
  xsd__string keywords;
  xsd__string description;
};

class ns__StopRecordParam {
public:
  xsd__string focus;
  xsd__string target_uri;
};

class ns__RecordTerminalStatus {
public:
  xsd__string vuri;
  xsd__string name;
  xsd__string status;
  int virtual_count;
  xsd__string requester_uri;
  unsigned int duration;
  unsigned long long recorded_bytes;
};

class ns__GetRecordStatusResult {
public:
  std::vector<ns__RecordTerminalStatus> terminals;
};

int ns__StartRecord(ns__StartRecordParam param, int *result);

int ns__StopRecord(ns__StopRecordParam param, int *result);

int ns__GetRecordStatus(xsd__string focus, xsd__string uri, ns__GetRecordStatusResult *result);

}
