#include<iostream>
#include<fstream>

/*
Return codes:
0 - success
1 - bad test id
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
}
