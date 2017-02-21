#include<iostream>
#include<fstream>
#include"cppcgimanager/getquery.cpp"

using namespace std;
using namespace cgim;

int main(){
	cout<<"Content-type: text/html\r\n\r\n"<<endl;
	parsekvstrings(true);
	fstream scorefile;
	string sfname = kvpairs[0].value+".txt";
	scorefile.open(sfname, fstream::out);
	string sfstring;
	
	return 0;
}
