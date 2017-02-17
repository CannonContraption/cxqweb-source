#include<iostream>
#include<fstream>
#include"listhelper/listhelper.cc"
using namespace std;

struct option{
	string identifier;
	string description;
};

struct question{
	string description;
	singlelinked<option> optionlist;
};

struct quiz{
	string name;
	singlelinked<question> questionlist;
} master_quiz;

/* return codes:
1: invalid [quiz] level key/value pair
2: invalid [question] level key/value pair
3: invalid [] section
4: file open issue
*/
int getQuizResults(string filename){
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
			}
			if(key == "[quiz]") currentlevel = quizlevel;
			else if(key == "[question]") currentlevel = questionlevel;
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
Return codes:
0 - success
1 - no test id
2 - problem parsing quiz file (io, syntax, etc.)
*/
int main(int argc, char* argv[]){
	bool good = true;
	cout<<"<html>"<<endl<<"<head>"<<endl;
	if(argc<2){
		good = false;
	}
	if(!good) cout<<"<title>Invalid quiz ID.</title>"<<endl;
	else{
		cout<<"<title>Take quiz "<<argv[1]<<endl;
	}
	cout<<"</head>"<<endl<<"<body>"<<endl;
	if(good){
		cout<<"<h1>Take a Quiz</h1>"<<endl;
	}
	else{
		cout<<"<h1>No quiz ID.</h1>"<<endl;
		cout<<"<p>You have not supplied an appropriate quiz id. Please ask your instructor for a new link.</p>"<<endl;
		return 1;
	}
	string name = argv[1];
	name += ".ini";
	int quizresultgrab = getQuizResults(name);
	switch(quizresultgrab){
	case 0:
		break;
	case 1:
		cout<<"<h1>Question file syntax error.</h1>"<<endl<<"<p>Please alert your instructor, there is an error inside the [quiz] section.</p>"<<endl;
		break;
	case 2:
		cout<<"<h1>Question file syntax error.</h1>"<<endl<<"<p>Please alert your instructor, there is an error inside the [question] section.</p>"<<endl;
		break;
	case 3:
		cout<<"<h1>Question file syntax error.</h1>"<<endl<<"<p>Please alert your instructor, there is an extra [bracketed] set in the question file.</p>"<<endl;
		break;
	case 4:
		cout<<"<h1>Bad quiz ID.</h1>"<<endl<<"<p>The quiz ID you have supplied is wrong. Please ask your instructor for a new one.</p>"<<endl;
		break;
	}
	if(quizresultgrab != 0){
		cout<<"</body>"<<endl<<"</html>"<<endl;
		return 2;
	}
	return 0;
}
