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

int max_options;

/*----------------------------------------
return codes:
1: invalid [quiz] level key/value pair
2: invalid [question] level key/value pair
3: invalid [] section
4: file open issue
----------------------------------------*/
int get_quiz_results(string filename){
	ifstream quizfile;
	max_options = 0;
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
		int options = 0;
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
					options++;
					if(options>max_options) max_options = options;
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
