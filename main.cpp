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
    long time_sum = 0;
    string file_path = "assets/structures/buildmodule/buildmodule_arg_equip_l_data/assets_structures_buildmodule_buildmodule_arg_equip_l-lod0.xmf";

    auto start = chrono::steady_clock::now();
    auto cat_file = File_process::File_reader::construct_btree("/home/xen134/01.cat");
    auto end = chrono::steady_clock::now();
    auto time  = chrono::duration_cast<chrono::milliseconds>(end - start);
    //i->print_ele();

    cout << "File sreach test starts." << endl;
    vector<File_process::Vfile *> search_test = cat_file->files_seq;
    reverse(search_test.begin(), search_test.end());
    for (auto j : search_test) {
        cout << "Finding file : " <<  j->path << endl;

        auto start2 = chrono::steady_clock::now();
        auto file = cat_file->find_file(j->path);
        auto end2 = chrono::steady_clock::now();
        auto time2  = chrono::duration_cast<chrono::milliseconds>(end2 - start2);
//        cout << "time: " << time2.count() << endl;
        time_sum += time2.count();
        cout << "Result : " << file->path << " " << file->size << " " << file->offset << " " << file->utc_time << endl;
    }

    cout << "Read time :" << time.count() << endl;
    cout << "Search consumed : " << time_sum << endl;
    cout << fixed << "Average : " << (double)time_sum/(double)cat_file->files_seq.size() << endl;

//    cout << "Test for generating upper bound : " << cat_file->generate_upper_bound("/usr/bin/1") << endl;


    delete cat_file;
    return 0;
}
