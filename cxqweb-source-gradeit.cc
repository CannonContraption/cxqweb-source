#include<iostream>
#include<fstream>
#include"cppcgimanager/getquery.cpp"

using namespace std;
using namespace cgim;

int main(){
	cout<<"Content-type: text/plain\r\n\r\n"<<endl;
	parsekvstrings(true);
	fstream scorefile;
	string sfname = "quizresults/"+kvpairs[0].value+".txt";
	scorefile.open(sfname, fstream::out);
	string sfstring;
	string studentemail = kvpairs[3].value;
	scorefile<<kvpairs[2].value<<","<<kvpairs[1].value<<" ";
	cout<<"KVCOUNT: "<<kvcount<<endl;
	for(int i = 4; i<kvpaircount; i++){//4 components before questions, then dynamically created question answer list
		scorefile>>kvpairs[i].value;
		if(i%4 == 0) scorefile<<" ";
	}
	scorefile<<endl;
	return 0;
}
