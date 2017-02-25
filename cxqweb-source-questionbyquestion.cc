#include<iostream>
#include<fstream>
#include<random>
#include"listhelper/listhelper.cc"
#include"cppcgimanager/getquery.cpp"

/*
 * CxqWeb-Source-Test
 * 
 * For When One Question At A Time Really Is Enough(tm)
 * 
 * This version of the web interface generates one question at a time,
 * displaying only the one the user is working on and no more.
 * 
 * This has the advantage of working a little more towards the goal of 
 * making a testing environment which has good integrity.
 */


using namespace std;

#include "questiongrabber.cc"

/*----------------------
Return codes:
0: success
1: no questions to print
----------------------*/
int format_php_quiz(string quizname, string purename, bool randomize){
	if(master_quiz.questionlist.size()<1) return 1;
	cout<<"<h2>"<<master_quiz.name<<"</h2>"<<endl;
	cout<<"<h3>"<<master_quiz.questionlist.size()<<" questions."<<endl;
	cout<<"<form id=quiz action=submitquiz.cgi method=POST>"<<endl;
	cout<<"<input type=hidden name=quizname value=\""<<purename<<"\"></input>"<<endl;
	cout<<"<table><tr><td>First Name</td><td>Last Name</td><td>E-Mail</td></tr><tr><td><input type=text name=fname class=textfield></input></td><td><input type=text name=lname class=textfield></input></td><td><input class=textfield type=text name=email></input></td></tr></table>"<<endl;
	int optionlistsize;
	int questionlistsize = master_quiz.questionlist.size();
	question currentquestion;
	option currentoption;
	if(randomize) randomize_list(master_quiz.questionlist.size());
	else order_list(master_quiz.questionlist.size());
	for(int questionid = 0; questionid<questionlistsize; questionid++){
		currentquestion = master_quiz.questionlist[randomlist[questionid]];
		cout<<"<p class=questiontext>"<<currentquestion.description<<"</p>"<<endl;
		optionlistsize = master_quiz.questionlist[randomlist[questionid]].optionlist.size();
		for(int optionid = 0; optionid<optionlistsize; optionid++){
			currentoption = currentquestion.optionlist[optionid];
			if(currentoption.identifier == "") continue;
			cout<<"<input type=radio name="<<randomlist[questionid]<<" value="<<currentoption.identifier<<">"<<currentoption.description<<"</input><br>"<<endl;
		}
	}
	cout<<"<br><br><input type=submit value=\"Submit Quiz\"></input>"<<endl;
	cout<<"</form>"<<endl;
	return 0;
}

/*----------------------------------------------
Return codes:
0 - success
1 - no test id
2 - problem parsing quiz file (io, syntax, etc.)
----------------------------------------------*/
int main(){
	bool good = true;
	cout<<"Content-type: text/html\r\n\r\n"<<endl;
	cout<<"<html>"<<endl<<"<head>"<<endl;
	cgim::parsekvstrings(false);
	int quizresultgrab = 255;
	string name = "";
	string purename = "";
	if(cgim::kvpaircount<0){
		good = false;
		cout<<"<title>Invalid quiz ID.</title>"<<endl;
	}
	else{
		name = cgim::kvpairs[0].value;
		purename = name;
		name += ".ini";
		quizresultgrab = get_quiz_results(name);
		switch(quizresultgrab){
		case 0:
			break;
		case 1:
			cout<<"</head><body><h1>Question file syntax error.</h1>"<<endl<<"<p>Please alert your instructor, there is an error inside the [quiz] section.</p>"<<endl;
			break;
		case 2:
			cout<<"</head><body><h1>Question file syntax error.</h1>"<<endl<<"<p>Please alert your instructor, there is an error inside the [question] section.</p>"<<endl;
			break;
		case 3:
			cout<<"</head><body><h1>Question file syntax error.</h1>"<<endl<<"<p>Please alert your instructor, there is an extra [bracketed] set in the question file.</p>"<<endl;
			break;
		case 4:
			cout<<"</head><body><h1>Bad quiz ID.</h1>"<<endl<<"<p>The quiz ID you have supplied is wrong. Please ask your instructor for a new one.</p>"<<endl;
			break;
		case 255:
		default:
			cout<<"</head><body><h1>Internal Server Error</h1><h2>Please report a bug.</h2>"<<endl;
			cout<<"<p>Email <a href=\"mailto:jread@student.framingham.edu\">Jim Read</a> with details of what happened after notifying your instructor.</p>"<<endl;
			break;
		}
		if(quizresultgrab != 0){
			cout<<"</body>"<<endl<<"</html>"<<endl;
			return 2;
		}
		cout<<"<title>Take Quiz: "<<master_quiz.name<<"</title>"<<endl;
		cout<<"<link rel=stylesheet type=text/css href=master.css>"<<endl;
	}
	cout<<"</head>"<<endl<<"<body>"<<endl;
	if(!good){
		cout<<"<h1>No quiz ID.</h1>"<<endl;
		cout<<"<p>You have not supplied an appropriate quiz id. Please ask your instructor for a new link.</p>"<<endl;
		return 1;
	}
	cout<<"<h1>Take a Quiz</h1>"<<endl;
	int quizprintgrab = format_php_quiz(name, purename, true);
	cout<<"</body>"<<endl<<"</html>"<<endl;
	return 0;
}
