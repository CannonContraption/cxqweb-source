#include "listhelper.hh"
template <class T>
void singlelinked<T>::push_back(T newdata){
	//create a new chainlink
	chainlink * current;
	current = new chainlink;
	current -> data = newdata;
	current -> next = nullptr;
	
	//find our last element
	chainlink * ptr = head;
	if(ptr == nullptr){
		head = current;
		return;
	}
	while(ptr->next != nullptr){
		ptr = ptr-> next;
	}
	
	//then attach our new chainlink to it
	ptr->next = current;
}

/*
 * this function literally just counts how many links and returns
 * a pointer to the current one after that time. I used pointers so
 * that data could be manipulated more directly.
 */
template <class T>
T * singlelinked<T>::getelement(int index){
	chainlink * current = head;
	for(int i = 0; i<index; i++){
		if(current->next==nullptr){
			break;
		}
		current = current->next;
	}
	return &current->data;
}

/*
 * This function traverses every element and returns how many there are.
 */
template <class T>
int singlelinked<T>::size(){
	chainlink * current = head;
	int counter = 0;
	while(current != nullptr){
		current = current->next;
		counter++;
	}
	return counter;
}

/*
 * so we don't segfault right away, the head should be assigned to
 * a nullptr.
 */
template <class T>
singlelinked<T>::singlelinked(){
	head = nullptr;
}

template <class T>
T& singlelinked<T>::operator[](int index){
	chainlink * current = head;
	for(int i = 0; i<index; i++){
		if(current->next==nullptr){
			break;
		}
		current = current->next;
	}
	return current->data;
}
