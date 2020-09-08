#include <iostream>
#include "file_reader.h"
#include <string>
#include <chrono>
#include <vector>
#include <algorithm>

using namespace std;

int main()
{
//    string path2;

//    cout << "Enter Path:";
//    cin >> path2;

    auto start = chrono::steady_clock::now();
    auto i = File_process::File_reader::construct_btree("/home/xen134/01.cat");
    auto end = chrono::steady_clock::now();

    auto start2 = chrono::steady_clock::now();
    auto file = i->find_file(string("assets/props/StorageModules/sm_gen_wpsmod_common_01_data/assets_props_storagemodules_sm_gen_wpsmod_common_01-lod0.xmf"));
    auto end2 = chrono::steady_clock::now();

    i->print_ele();
    vector<int> test{1, 2, 3, 4};
    sort(test.begin(), test.end());
    auto time  = chrono::duration_cast<chrono::milliseconds>(end - start);
    auto time2  = chrono::duration_cast<chrono::milliseconds>(end2 - start2);
    cout << "time :" << time.count() << endl;
    cout << "find time: " << time2.count() << endl;
    //cout << file->path << " " << file->size << endl;

    delete i;
    return 0;
}
