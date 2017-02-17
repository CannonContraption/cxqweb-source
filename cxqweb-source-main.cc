#include<iostream>
#include<fstream>
#include"listhelper/listhelper.cc"
using namespace std;

struct question{
	string identifier;
	string description;
};

struct quiz{
	string name;
	singlelinked<question> questionlist;
};

int getQuizResults(string filename){
	ifstream quizfile;
	quizfile.open(filename.c_str());
	if(!quizfile.is_open()){
		cout<<"<h1>Bad quiz ID.</h1>"<<endl;
		cout<<"<p>The quiz ID you have supplied is wrong. Please ask your instructor for a new one.</p>"<<endl;
		cout<<"</body>"<<endl<<"</html>"<<endl;
		return 2;
	}
	enum level{
		quizlevel,
		questionlevel
	};
	string key;
	string value;
	string currentline;
	int spaces;
	level currentlevel;
	currentlevel = quizlevel;
	string values[4]; //common values label.
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
				//I don't think we need anything on the quiz level
			} else if (currentlevel == questionlevel){
				//do question-level stuff
			}
		}
		else{
			if(currentlevel == quizlevel){
				//do key/value parsing
			}
			if(currentlevel == questionlevel){
				//do question level stuff
			}
		}
	}
	//repeat key/value parsing code here
	quizfile.close();
}

/*
Return codes:
0 - success
1 - no test id
2 - test ID not found
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
		cout<<"<h1>Bad quiz ID.</h1>"<<endl;
		cout<<"<p>The quiz ID you have supplied is wrong. Please ask your instructor for a new one.</p>"<<endl;
		cout<<"</body>"<<endl<<"</html>"<<endl;
		return 1;
	}
	string name = argv[1];
	name += ".ini";

}
