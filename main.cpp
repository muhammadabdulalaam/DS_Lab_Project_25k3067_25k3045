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
#include <vector>//ONLY FOR INITIALLY storing folder filenames, then array is used
#include <thread>//thread and chrono are both just to give processor small breaks while playing song to not max usage
#include <chrono>
#include <fstream>//file handling of saved folderpath name
#include <conio.h>//getch() for reading keypress 
namespace fs= std::filesystem;
using namespace std;




/*NOTES VVVVVVV
1.  _getch() and spacebar pausing and playing
2. next previous arrowkeys playlist
 */



//NON DATA STRUCTURE FUNCTIONS START:
void clearScreen() {
    std::cout << "\033[2J\033[1;1H";
}





void directPlay(string folder, string FileNamesArray[], int nfiles, ma_engine* engine){//choice=1 (play a song directly)
	cout<<"All Songs:"<<endl<<endl;//list song s
	for (int i=0; i<nfiles; i++){
		cout<<i+1<<"-"<<FileNamesArray[i]<<endl;
	}
	cout<<nfiles+1<<"- RETURN TO MAIN MENU"<<endl;
	
	int songchoice;//select song to play from lisr
	cout<<"Enter song number to play"<<endl;
	cin>>songchoice;
	getchar();//remov \n leftover from cin
	if (songchoice<1||songchoice>nfiles){
		return;
	}
	string filename= FileNamesArray[songchoice-1];
	string filepath = folder +"/"+filename; //make full path
	ma_sound sound;
    ma_result result = ma_sound_init_from_file(engine, filepath.c_str(), 0, NULL, NULL, &sound);
    if (result != MA_SUCCESS) {
        return;
    }
    ma_sound_start(&sound);
    bool currentlyPlaying= true;
    clearScreen();
    cout << "Playing '"+filename+"'"<<endl;
	cout<<"Press spacebar to pause/resume"<<endl;
	cout<<"Press Q to stop playing"<<endl;
	while(!ma_sound_at_end(&sound)){
    	this_thread::sleep_for(chrono::milliseconds(50)); //pause processing for small breaks or cpu will be burdened
    	if (kbhit()){
    		char keypress= _getch();
    		if (keypress==' '){
    			if (currentlyPlaying){
    				ma_sound_stop(&sound);
    				currentlyPlaying=false;
				}else{
					ma_sound_start(&sound);
					currentlyPlaying=true;
				}	
			}else if(keypress=='q'||_getch()=='Q'){
				break;
			}
		}
		
	}
	ma_sound_uninit(&sound);
	
}

string getFolderPath(){
	clearScreen();
	//FOLDER INITIALILIZING START
	string folder; //store folder path
	do{
		cout<<"Enter Music Folder Path:"<<endl;
		getline(cin, folder);
		folder.erase(remove(folder.begin(), folder.end(), '"'), folder.end());//get rid of quotes when copypasting path
	    if (!fs::exists(folder)||!fs::is_directory(folder)){//folder not exist/folder is not folder
    		cout<<"Invalid or does not exit"<<endl;
		}
	}while(!fs::exists(folder)||!fs::is_directory(folder));
	return folder;
}

string saveFolder(){
	string folder= getFolderPath();
	ofstream FolderPathTxt("MusicFolderPath.txt");
	if (FolderPathTxt.is_open()){
		FolderPathTxt << folder;
		FolderPathTxt.close();
		cout<<"Folder path saved"<<endl;
		FolderPathTxt.close();
	}else{
		cout<<"Folder path failed to save"<<endl;
	}
	return folder;
}

string checkFolderSaved(){//if at start of program, cant find folder path previously saved ask for path and save. If found return saved path
	ifstream FolderPathTxt("MusicFolderPath.txt");
	string folder;
	if (FolderPathTxt.is_open()){
		getline(FolderPathTxt, folder);
		FolderPathTxt.close();
	}
	if (folder.empty() || !fs::exists(folder) || !fs::is_directory(folder)){
		folder= saveFolder();
	}
	return folder;
}
//FUNCTIONS END


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
			if (index<=0){//if use enters position 1 then for loop does not run as index=0, i<index-1=-1
				temp->next= head;
				temp->prev= tail;
				head->prev= temp;
				tail->next= temp;
				head= temp;
				size++;
				return;
			}
			PNode* curr= head;
			for (int i=0; i<index-1; i++){
			
				if (curr->next==head){//if user entered index bigger than list size just insert at end rather than throw "invalid index"
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
			if (curr==tail) tail= temp;
			size++;
		}
		
		void remove(int index){
			if (head==NULL){
				return;
			}
			PNode* curr= head;
			for (int i=0; i<index; i++){
				if (curr->next==head){
					break;
				}
				curr= curr->next;
			}
			
			if (size==1){
				delete curr;
				head=NULL;
				tail= NULL;
				size= 0;
				return;
			}
			if (curr==head){
				head= curr->next;
				curr->prev->next= curr->next;
				curr->next->prev= curr->prev;
				curr->prev=NULL;
				curr->next=NULL;
				delete curr;
			}if (curr==tail){
				tail= curr->prev;
				curr->prev->next= curr->next;
				curr->next->prev= curr->prev;
				curr->prev=NULL;
				curr->next=NULL;
				delete curr;
			}
			size--;
		}
		
		void display(){
			clearScreen();
			PNode* curr=head;
			if (head!=NULL){
				cout<<"Current Playlist:"<<endl;
				for (int i=0; i<size; i++){
					cout<<i+1<<"-"<<curr->songname<<endl;
					curr=curr->next;
				}
			}else{
				cout<<"Playlist Empty"<<endl;
			}
		}	
		
		void play(string folder,ma_engine* engine){
			PNode* curr=head;
			char keypress='\0';
			do{
				bool auto_forward=true;
				string filepath = folder +"/"+curr->songname; //make full path
				ma_sound sound;
			    ma_result result = ma_sound_init_from_file(engine, filepath.c_str(), 0, NULL, NULL, &sound);
			    if (result != MA_SUCCESS) {
			        return;
			    }
			    ma_sound_start(&sound);
			    bool currentlyPlaying= true;
			    clearScreen();
			    cout << "Playing '"+curr->songname+"'"<<endl;
				cout<<"Press spacebar to pause/resume"<<endl;
				cout<<"Press Q to stop playing"<<endl;
				cout<<"Press Arrow keys: < for previous song, > for next song"<<endl;
				while(!ma_sound_at_end(&sound)){
			    	this_thread::sleep_for(chrono::milliseconds(50)); //pause processing for small breaks or cpu will be burdened
			    	if (kbhit()){
			    		keypress= _getch();
			    		if (keypress==' '){
			    			if (currentlyPlaying){
			    				ma_sound_stop(&sound);
			    				currentlyPlaying=false;
							}else{
								ma_sound_start(&sound);
								currentlyPlaying=true;
							}	
						}else if(keypress=='q'||keypress=='Q'){
							auto_forward=false;
							break;
						}else if(keypress==(char)0||keypress==(char)224){//since normal keys are between, must mean special key, so theres 2 bytes instead of 1
							char spkey= _getch();//read 2ns byte
							if (spkey==(char)75){//left arrow
								curr=curr->prev;
								auto_forward=false;
								break;
							}else if (spkey==(char)77){//right arrow
								curr=curr->next;
								auto_forward= false;
								break;
							}
						}
					}
					
				}
				ma_sound_uninit(&sound);
				if (auto_forward)curr=curr->next;
			}while (keypress!='Q' && keypress!='q');
			
		}
};

Playlist PL;//GLOBAL PLAYLIST OBJECT
void savePlaylist(){
	ofstream PlaylistTxt("Playlist.txt");
	if (PlaylistTxt.is_open()){
		PNode* curr=PL.head;
		while (curr!=PL.tail){
			PlaylistTxt << curr->songname <<endl;
			curr= curr->next;
		}
		PlaylistTxt << curr->songname <<endl; //tail
	}
}
void getExistingPlaylist(){
	if (PL.head==NULL){
		ifstream PlaylistTxt("Playlist.txt");
		if (PlaylistTxt.is_open()){
			string line;
			int index=0;
			while (getline(PlaylistTxt, line)){
				PL.insert(index, line);
			}
		}
	}
}
  

void  beginPlaylist(string folder, string FileNamesArray[], int nfiles, ma_engine* engine){
	int playlistChoice=0;
	do{                                                                 
		clearScreen();
		cout<<"Select playlist function:"<<endl;
		cout<<"1. Play Playlist"<<endl;
		cout<<"2. Add to playlist"<<endl;
		cout<<"3. Remove from playlist"<<endl;
		cout<<"4. RETURN TO MAIN MENU"<<endl;
		cin>>playlistChoice;
		getchar();
		switch (playlistChoice){
			case 1:   
				PL.play(folder, engine);  
			case 2:
				cout<<"All Songs:"<<endl<<endl;//list song s
				for (int i=0; i<nfiles; i++){
					cout<<i+1<<"-"<<FileNamesArray[i]<<endl;
				}
				cout<<nfiles+1<<"- <<< BACK"<<endl;
				
				int songchoice;//select song to play from lisr
				cout<<"Enter song number to add to Playlist"<<endl;
				cin>>songchoice;
				getchar();//remov \n leftover from cin
				if (songchoice<1||songchoice>nfiles){
					break;
				}
				PL.display();
				int index;
				cout<<"Enter position to insert song"<<endl;
				cin>>index;
				getchar();
				PL.insert(index-1, FileNamesArray[songchoice-1]);
				savePlaylist();
				break;
			case 3:
				PL.display();
				cout<<"Enter song number to delete"<<endl;
				cin>>index;
				getchar();
				PL.remove(index-1);
				savePlaylist();
				break;
			
			                                  
		}           
	}while(playlistChoice!=4);
}
	             
//PLAYLIST STRUCTURE END



//25K-3045 RAHIM NAWAB'S SECTION START

void merge(string arr[], int left, int mid, int right) {

    int n = right - left + 1;
    string* temp = new string[n];

    int i = left;
    int j = mid + 1;
    int k = 0;

    while (i <= mid && j <= right) {
        if (arr[i] <= arr[j]) {
            temp[k] = arr[i];
            i++;
        }
        else {
            temp[k] = arr[j];
            j++;
        }
        k++;
    }

    while (i <= mid) {
        temp[k] = arr[i];
        i++;
        k++;
    }

    while (j <= right) {
        temp[k] = arr[j];
        j++;
        k++;
    }

    for (int x = 0; x < n; x++) {
        arr[left + x] = temp[x];
    }

    delete[] temp;
}

void mergeSort(string arr[], int left, int right) {

    if (left >= right) {
        return;
    }

    int mid = left + (right - left) / 2;

    mergeSort(arr, left, mid);
    mergeSort(arr, mid + 1, right);

    merge(arr, left, mid, right);
}



//RAHIM's SECTION END





int main() {
	ma_engine engine;
    if (ma_engine_init(NULL, &engine) != MA_SUCCESS) {
        exit(1);
	}
	                                                     
folder_initialization://label so if user changes music folder program can jump back here and reinitialise
	string folder= checkFolderSaved();
	vector<string> mp3files;
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
	for (int i=0; i<nfiles; i++){
		FileNamesArray[i]= mp3files[i];
	}
	//FOLDER INITIALIZING END (filenames extracted into array now we only use that)
	
	getExistingPlaylist();
	int choice=0;
	cout<<"Welcome to Structured Sound!"<<endl<<endl;
	
	do {
		clearScreen();
		cout<<"1. Play a song directly"<<endl;
		cout<<"2. Playlist"<<endl;
		cout<<"3. Change Music Folder"<<endl;
		cout<<"4. Sort By Name"<<endl;
		cout<<"5. EXIT"<<endl;
		cout<<endl<<"Enter choice: ";
		cin>>choice;
		getchar();
		cout<<endl;
		
		switch (choice){
			case 1:
				directPlay(folder, FileNamesArray, nfiles, &engine);
				break;
			case 2:
				beginPlaylist(folder,FileNamesArray, nfiles, &engine);
				break;
			case 3:
				saveFolder();
				goto folder_initialization;
				break;
			case 4:
				mergeSort(FileNamesArray, 0, nfiles - 1); // Merge sort will sort via filename
				cout<<"Sorted"<<endl;
		}
	}while (choice!=5);
	
	
	ma_engine_uninit(&engine);
}

                           