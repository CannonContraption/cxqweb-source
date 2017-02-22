#include<iostream>
#include<fstream>
#include"cppcgimanager/getquery.cpp"

using namespace std;
using namespace cgim;

int main(){
	cout<<"Content-type: text/html\r\n\r\n"<<endl;
	cout<<"<html><head>"<<endl;
	parsekvstrings(true);
	cout<<"<title>Results from "<<kvpairs[0].value<<"</title>"<<endl;
	cout<<"</head><body>"<<endl;
	cout<<"<h1>Your quiz has been submitted</h1>"<<endl;
	fstream scorefile;
	string sfname = "quizresults/"+kvpairs[0].value+".txt";
	scorefile.open(sfname, fstream::app);
	string sfstring;
	string studentemail = kvpairs[3].value;
	scorefile<<kvpairs[2].value<<","<<kvpairs[1].value<<"	";
	//cout<<"KVCOUNT: "<<kvpaircount<<endl;
	for(int i = 4; i<kvpaircount; i++){//4 components before questions, then dynamically created question answer list
		scorefile<<kvpairs[i].value;
		if((i+1)%4 == 0) scorefile<<" ";
	}
	scorefile<<endl;
	cout<<"<h2>What now?</h2>"<<endl;
	cout<<"<p>That's it, close your browser window. When everyone has finished, the professor will pull your answers from the server, and you will be notified when the results are ready.</p>"<<endl;
	cout<<"</body></html>"<<endl;
	return 0;
}
