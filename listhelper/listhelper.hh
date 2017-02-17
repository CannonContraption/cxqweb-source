/*
 * Be it noted that under normal circumstances, it would be
 * bad practice to write something like this. The correct
 * solution would be to use C++'s internal linked list class
 * template. Since this is, however, for a data structures
 * class, I have taken the time to implement my own. It is
 * complete with exactly as many features as I need and no 
 * more.
 */

template <class T>
class singlelinked{
	struct chainlink{
		T data;
		chainlink * next;
	};
	chainlink * head;
public:
	void push_back(T newdata);
	int size();
	T * getelement(int index);
	T& operator[] (int index);
	singlelinked();
	void clearlist();
	void detachhead();
};
