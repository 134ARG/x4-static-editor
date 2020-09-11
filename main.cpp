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



    // Search test - reverse order.
    cout << "File sreach test starts." << endl;
    vector<File_process::Vfile *> search_test = cat_file->files_seq;
    reverse(search_test.begin(), search_test.end());
    int search_successes = 0;
    for (auto j : search_test) {
        cout << endl << "Finding file : " <<  j->path << endl;

        auto start2 = chrono::steady_clock::now();
        auto file = cat_file->find_file(j->path);
        auto end2 = chrono::steady_clock::now();
        auto time2  = chrono::duration_cast<chrono::milliseconds>(end2 - start2);

        if (file->path == j->path) search_successes++;

        time_sum += time2.count();
        cout << "Result : " << file->path << " " << file->size << " " << file->offset << " " << file->utc_time << endl;
    }



    // linked list test.
    for (auto k : cat_file->files_seq) {
        if (k->next == nullptr) {

            cout << "Null next found." << endl;
            cout << k->path << endl;
        } else if (k->prev == nullptr || k->prev == 0) {
            cout << "Null prev found." << endl;
            cout << k->path << endl;
        }
    }

    // partial find test
    auto result_bound = cat_file->find_partial("assets/structures/buildmodule/");
    cout << "Partial match results : " << endl;
    int idx = 1;
    for (auto i = result_bound[0]; i != result_bound[1]; i = i->next) {
        cout << idx++ << " : " << i->path << " " << i->size << " " << i->offset << endl;
    }


    // overall report.
    cout << "Read time :" << time.count() << endl;
    cout << "All serach trials : " << cat_file->files_seq.size() << endl;
    cout << "Successful : " << search_successes << endl;
    cout << "Reverse search time : " << time_sum << endl;
    cout << fixed << "Average search time: " << (double)time_sum/(double)cat_file->files_seq.size() << endl;
    cout << "Partial match found : " << idx-1 << " results." << endl;

    cout << "Finished. Enter to exit." << endl;
    cin.get();
    delete cat_file;
    return 0;
}
