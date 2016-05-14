#pragma  once
#include <string>
#include <map>
using namespace std;

#define COMMENT_CHAR '#'

class ManualConfig
{
public:
	static ManualConfig* getInstance()
	{
		static ManualConfig ipconf;
		return &ipconf;
	}

	bool ReadConfig(const string & filename);
	string getResolution(){return resolution;}
	string getcrosstype1(){return crosstype1;}
	string getcrosstype2(){return crosstype2;}
	string getcrosstype3(){return crosstype3;}

protected:
	bool AnalyseLine(const string & line, string & key, string & value);
	void Trim(string & str);
	bool IsCommentChar(char c);
	bool IsSpace(char c);

private:
	map<string, string> m;
	string resolution;
	string crosstype1;
	string crosstype2;
	string crosstype3;
};