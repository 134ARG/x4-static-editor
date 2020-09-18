#include <iostream>
#include "./file_reader.h"
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

    string file_path = "./test_samples/01.cat";

    auto start = chrono::steady_clock::now();
    auto cat_file = File_process::File_reader::construct_btree(file_path);
    auto end = chrono::steady_clock::now();
    auto time  = chrono::duration_cast<chrono::milliseconds>(end - start);
    cout << "Initializing finished." << endl;
    cout << "Reading time :" << time.count() << endl << endl;
    //i->print_ele();


    // Search test - reverse order.
    long time_sum = 0;

    cout << "File sreach test starts." << endl;
    vector<File_process::Vfile *> search_test = cat_file->files_seq;
    reverse(search_test.begin(), search_test.end());
    int search_successes = 0;
    for (auto j : search_test) {

        auto start2 = chrono::steady_clock::now();
        auto file = cat_file->find_file(j->path);
        auto end2 = chrono::steady_clock::now();
        auto time2  = chrono::duration_cast<chrono::milliseconds>(end2 - start2);

        if (file->path == j->path) search_successes++;

        time_sum += time2.count();
    }
    cout << "All serach trials : " << cat_file->files_seq.size() << endl;
    cout << "Successful : " << search_successes << endl;
    cout << "Reverse search time : " << time_sum << endl;
    cout << fixed << "Average search time: " << (double)time_sum/(double)cat_file->files_seq.size() << endl;
    cout << endl;


    // linked list test.
    for (auto k : cat_file->files_seq) {
        if (k->next == nullptr) {

            cout << "Null next found." << endl;
            cout << k->path << endl;
        } else if (k->prev == nullptr || k->prev == 0) {
            cout << "Null prev found." << endl;
            cout << k->path << endl;
        } else if (k->path > k->next->path) {
            throw "error";
        }
    }
    // linked list test 2
    int num_forward = 0;
    int num_backward = 0;
    File_process::Vfile *c = cat_file->head;
    while (c != nullptr) {
        num_forward++;
        c = c->next;
    }
    c = cat_file->tail;
    while (c != nullptr) {
        num_backward++;
        c = c->prev;
    }
    cout << "vfiles in linked list (forward) : " << num_forward << endl;
    cout << "vfiles in linked list (backward) : " << num_backward << endl << endl;


    // depth
    cout << " depth : " << cat_file->depth() << endl << endl;

    // partial find test
    string partial = "b";

    auto result_bound = cat_file->find_partial(partial);
    cout << "Partial match keyword : " << partial << endl;
    cout << "Partial match results : " << endl;
    int idx = 1;
    for (auto i = result_bound[0]; i && i->path <= result_bound[1]->path; i = i->next) {
        cout << idx++ << " : " << i->path << " " << i->size << " " << i->offset << endl;
    }
    cout << "Partial match found : " << idx-1 << " results." << endl << endl;


    cout << "Finished. Enter to exit." << endl;
    cin.get();
    delete cat_file;
    return 0;
}
