#include<iostream>
#include<fstream>
#include<random>
#include"listhelper/listhelper.cc"
#include"cppcgimanager/getquery.cpp"

/*
 * CxqWeb-Source-Main
 * 
 * We Make Quizzes and Annoy Students(tm)
 * 
 * This source file is made to read a .ini quiz file, determine what quiz is contained in it,
 * randomize the questions, and then display it in a nice HTML format. It is designed for use
 * in HTTP CGI, so as the program which generates page content for a student directly.
 * 
 * For more details, see the CMakeLists.txt file, which is used in the build process to determine
 * how to create the makefile. It has defined in the file the source/executable name pairings.
 * 
 * This file will output makequiz.cgi.
 */


using namespace std;

struct option{
	string identifier;
	string description;
};

struct question{
	int number;
	string description;
	singlelinked<option> optionlist;
};

struct quiz{
	string name;
	singlelinked<question> questionlist;
} master_quiz;

/*----------------------------------------
return codes:
1: invalid [quiz] level key/value pair
2: invalid [question] level key/value pair
3: invalid [] section
4: file open issue
----------------------------------------*/
int get_quiz_results(string filename){
	ifstream quizfile;
	quizfile.open(filename.c_str());
	if(!quizfile.is_open()){
		return 4;
	}
	enum level{
		quizlevel,
		questionlevel
	} currentlevel;
	string key;
	string value;
	string currentline;
	int spaces;
	currentlevel = quizlevel;
	string values[4]; //common values label.
	question currentquestion;
	option currentoption;
	int qnumber = 0;
	while(!quizfile.fail()){
		getline(quizfile, currentline);
		spaces = 0;
		key = "";
		value = "";
		for(char c : currentline){
			if(c == '\n') spaces = 0;
			if(c == ' '){
				if(spaces > 1) value += ' ';
				spaces++;
				continue;
			}
			if(spaces == 0){
				key += c;
			}
			else if(spaces > 1){
				value += c;
			}
		}
		if(key[0] == '['){
			if(currentlevel == quizlevel){
				//Quiz-level logic is not needed if we have one quiz per quiz file.
			} else if (currentlevel == questionlevel){
				master_quiz.questionlist.push_back(currentquestion);
				currentquestion.optionlist.detachhead();
			}
			if(key == "[quiz]") currentlevel = quizlevel;
			else if(key == "[question]"){
				currentlevel = questionlevel;
				qnumber++;
			}
			else return 3;
		}
		else{
			if(currentlevel == quizlevel){
				if(key == "name") master_quiz.name = value;
				else return 1;
			}
			if(currentlevel == questionlevel){
				if(key != "description"){
					currentoption.identifier = key;
					currentoption.description = value;
					currentquestion.optionlist.push_back(currentoption);
				} else if (key == "description") {
					currentquestion.description = value;
					currentquestion.number = qnumber;
				}
				else return 2;
			}
		}
	}
	master_quiz.questionlist.push_back(currentquestion);
	quizfile.close();
	return 0;
}

/*
 * In order to facilitate randomization, we have a randomlist pointer
 * which will eventually become an array of size (int max), which will
 * contain every number from 0 to the desired max -1, or a random list
 * of all numbers from 0 to max.
 */
int* randomlist;
void randomize_list(int max){
	default_random_engine genrn;
	uniform_int_distribution<int> distribution(0,max);
	randomlist = new int[max+1];
	for(int i = 0; i<=max; i++){
		randomlist[i] = distribution(genrn);
		for(int j = 0; j<i; j++){
			if(randomlist[i] == randomlist[j]){
				randomlist[i]++;
				if(randomlist[i]>max) randomlist[i]=0;
				j=-1;//resets loop until we found our value
				//will be set to 0 on loop end (postcommand=j++)
			}
		}
	}
}

void order_list(int max){
	randomlist = new int[max+1];
	for(int i = 0; i<=max; i++){
		randomlist[i]=i;
	}
}

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
			cout<<"<input type=radio name="<<randomlist[questionid]<<" value="<<currentoption.identifier<<">"<<currentoption.identifier<<") "<<currentoption.description<<"</input><br>"<<endl;
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
