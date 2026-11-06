//Playlist Linked List: Muhammad Abdul Alaam
//Search as typed Trie: Muhammad Abdul Alaam
//main() : Muhammad Abdul Alaam
//Playback Queue: Rahim Nawab
//History Stack: Rahim Nawab
//Merge Sort: Rahim Nawab
#include <iostream>
#define MINIAUDIO_IMPLEMENTATION
#include "miniaudio.h"//handles playing audio
#include <string>//folder path string fixing
#include <algorithm>//for remove() to cleanup filename
#include <filesystem>//access music folder
#include <vector>//storing folder filenames w
#include <thread>//thread and chrono are both just to give processor small breaks while playing song to not max usage
#include <chrono>
namespace fs= std::filesystem;
using namespace std;


//PLAYLIST(DOUBLY LINKED CIRCULAr LINKEDLIST): 25K-3067 Muhammad Abdul Alaam
class PNode{
	public:
		string songname;
		PNode* prev;
		PNode* next;
		PNode(){
			songname= "NULL";
			prev= NULL;
			next=NULL;	
		}
		PNode(string name){
			songname= name;
			prev=NULL;
			next=NULL;
		}
};
class Playlist{
	public:
		PNode* head;
		PNode* tail;
		int size;
		Playlist(){
			head= NULL;
			tail= NULL;
			size=0;
		}
		
		void insert(int index, string name){
			PNode* temp= new PNode(name);	
			if (head==NULL){//if nothing in playlist ignore index and create at start
				head=temp;
				tail= temp;
				head->prev= tail;
				head->next=tail;
				tail->prev= head;
				tail->next= head;
				size=1;
				return;
			}
			PNode* curr= head;
			for (int i=0; i<index-1; i++){
				if (curr->next==head){
					tail->next= temp;
					temp->prev= tail;
					tail= tail->next;
					tail->next= head;
					head->prev= tail;
					size++;
					return;
				}
				curr=curr->next;
			}
			curr->next->prev=temp;
			temp->next= curr->next;
			temp->prev= curr;
			curr->next= temp;
			size++;
		}
		
		void remove(int index){
			if (head==NULL){
				return;
			}
			PNode* curr= head;
			for (int i=0; i<index; i++){
				if (curr->next==head){
					tail= tail->prev;
					tail->next=head;
					head->prev=tail;
					curr->prev=NULL;
					curr->next=NULL;
					delete curr;
					size--;
					return;
				}
				curr= curr->next;
			}
			curr->prev->next= curr->next;
			curr->next->prev= curr->prev;
			curr->prev=NULL;
			curr->next=NULL;
			delete curr;
			size--;
		}
		
		void display(){
			PNode* curr=head;
			for (int i=0; i<size; i++){
				cout<<i+1<<"-"<<curr->songname<<endl;
				curr=curr->next;
			}
		}	
};
//PLAYLIST STRUCTURE END



//25K-3045 RAHIM NAWAB'S SECTION START

void merge(vector<string>& arr, int left, int mid, int right) {
    vector<string> temp;

    int i = left;
    int j = mid + 1;

    while (i <= mid && j <= right) {
        if (arr[i] <= arr[j]) {
            temp.push_back(arr[i]);
            i++;
        }
        else {
            temp.push_back(arr[j]);
            j++;
        }
    }

    while (i <= mid) {
        temp.push_back(arr[i]);
        i++;
    }

    while (j <= right) {
        temp.push_back(arr[j]);
        j++;
    }

    for (int k = 0; k < temp.size(); k++) {
        arr[left + k] = temp[k];
    }
}

void mergeSort(vector<string>& arr, int left, int right) {
    if (left >= right) {
        return;
    }

    int mid = left + (right - left) / 2;

    mergeSort(arr, left, mid);
    mergeSort(arr, mid + 1, right);

    merge(arr, left, mid, right);
}



//END


void clearScreen() {
    std::cout << "\033[2J\033[1;1H";
}

int main() {
	string folder; //store folder path
	cout<<"Enter Music Folder Path:"<<endl;
	getline(cin, folder);
	folder.erase(remove(folder.begin(), folder.end(), '"'), folder.end());//get rid of quotes when copypasting path
    //cout<<folder<<endl;
    
    if (!fs::exists(folder)||!fs::is_directory(folder)){//folder not exit/folder is not folder
    	cout<<"Invalid or does not exit"<<endl;
    	exit(1);
	}
	vector<string> mp3files;//all filenames go here
	
	for (auto& file : fs::directory_iterator(folder)){//for each loop going over each file in folder
		if (file.is_regular_file()){
			string ext = file.path().extension().string();
			if (ext==".mp3"){
				mp3files.push_back(file.path().filename().string());//add filename to array
			}
		}
	}
	string FileNamesArray[mp3files.size()]; //<----------STRING ARR FOR DATA STRUCTURESS
	int nfiles= mp3files.size();
	mergeSort(mp3files, 0, nfiles - 1); //Merge sort will sort the mp3 files via there names
	for (int i=0; i<nfiles; i++){
		FileNamesArray[i]= mp3files[i];
	}
	
	cout<<"All Songs:"<<endl<<endl;//list song s
	for (int i=0; i<nfiles; i++){
		cout<<i+1<<"-"<<FileNamesArray[i]<<endl;
	}
	
	int choice;//select song to play from lisr
	cout<<"Enter song number to play"<<endl;
	cin>>choice;
	getchar();//remov \n leftover from cin
	if (choice<1||choice>nfiles){
		cout<<"Invalid"<<endl;
		exit(1);
	}
	string filename= FileNamesArray[choice-1];
	filename = folder +"/"+filename; //make full path
	
    ma_result result;
    ma_engine engine;
    result = ma_engine_init(NULL, &engine);
    if (result != MA_SUCCESS) {
        exit(1);
    }
    ma_sound sound;
    result = ma_sound_init_from_file(&engine, filename.c_str(), 0, NULL, NULL, &sound);
    if (result != MA_SUCCESS) {
        ma_engine_uninit(&engine);
        exit(1);
    }

    ma_sound_start(&sound);
    clearScreen();
    cout << "Playing audio";
    while(!ma_sound_at_end(&sound)){
    	this_thread::sleep_for(chrono::milliseconds(100)); //pause processing for small breaks or cpu will be burdened
	}
	cout<<"Ended."<<endl;

    ma_sound_uninit(&sound);
    ma_engine_uninit(&engine);


   
}
