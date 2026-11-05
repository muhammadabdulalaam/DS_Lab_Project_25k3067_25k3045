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
namespace fs= std::filesystem;
using namespace std;

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
	cout << "Playing audio";

    cin.get();

    ma_sound_uninit(&sound);
    ma_engine_uninit(&engine);


   
}
