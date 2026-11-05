//Playlist Linked List: Muhammad Abdul Alaam
//Search as typed Trie: Muhammad Abdul Alaam
//main() : Muhammad Abdul Alaam
//Playback Queue: Rahim Nawab
//History Stack: Rahim Nawab
//Merge Sort: Rahim Nawab
#include <iostream>
#define MINIAUDIO_IMPLEMENTATION
#include "miniaudio.h"
#include <string>
#include <algorithm>//for remove() to cleanup filename
using namespace std;

int main() {
	string filename;
	cout<<"ENter filename:"<<endl;
	getline(cin, filename);
	filename.erase(remove(filename.begin(), filename.end(), '"'), filename.end());

    cout<<filename<<endl;
    
    ma_result result;
    ma_engine engine;
    result = ma_engine_init(NULL, &engine);
    if (result != MA_SUCCESS) {
        return -1;
    }
    ma_sound sound;
    result = ma_sound_init_from_file(&engine, filename.c_str(), 0, NULL, NULL, &sound);
    if (result != MA_SUCCESS) {
        ma_engine_uninit(&engine);
        return -1;
    }

    ma_sound_start(&sound);
	cout << "Playing audio";

    cin.get();

    ma_sound_uninit(&sound);
    ma_engine_uninit(&engine);


    return 0;
}
