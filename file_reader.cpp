#include "file_reader.h"
#include <fstream>
#include <sstream>
#include <vector>
#include <algorithm>

namespace File_process {
    using namespace std;

    File_reader::File_reader()
    {

    }

    // unfinished. to be refactored.
    Btree *File_reader::construct_btree(const string &filename)
    {
        string line;
        string tmp;
        vector<string> entries;
        Btree *tree = new Btree();

        int line_num = 0;
        ifstream in;
        in.open("/home/xen134/01.cat");
        if (in.is_open()) {
            while (getline(in, line)) {
                line_num++;
                istringstream one_file(line);
                while (one_file) {
                    one_file >> tmp;
                    entries.push_back(tmp);
                }
    //            reverse(entries.begin(), entries.end());
                tree->add_file(new Vfile(entries[0], entries[1], entries[2], entries[3]));
                entries.clear();
            }
        }

        return tree;
    }

}
