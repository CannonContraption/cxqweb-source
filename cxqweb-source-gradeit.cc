#include<iostream>
#include<fstream>
#include"cppcgimanager/getquery.cpp"

/*
 * CxqWeb-Source-GradeIt
 * 
 * The Most Misleading Source File Name Ever(tm)
 * 
 * This program is the side which tallies the results of a student's quiz when they hit "submit".
 * It is responsible for parsing key/value pairs and then deciding which answer goes where in the
 * answers file. It also accepts the student's email as an argument, but as of now does nothing
 * with it.
 * 
 * This program DOES NOT GRADE ANYTHING! It is merely created to tally results for analysis later.
 * 
 * This program will output submitquiz.cgi.
 */

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
		for(int j = 4; j<kvpaircount; j++){
			if(stoi(kvpairs[j].key)==i-4){
				scorefile<<kvpairs[j].value;
				break;
			}
		}
		if((i+1)%4 == 0) scorefile<<" ";
	}
	scorefile<<endl;
	cout<<"<h2>What now?</h2>"<<endl;
	cout<<"<p>That's it, close your browser window. When everyone has finished, the professor will pull your answers from the server, and you will be notified when the results are ready.</p>"<<endl;
	cout<<"</body></html>"<<endl;
	return 0;
}
