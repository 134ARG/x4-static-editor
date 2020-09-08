#include "file_reader.h"
#include <fstream>
#include <sstream>
#include <vector>
#include <algorithm>

#include <iostream>

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
        Btree *tree = new Btree(dat_path(filename));

        int line_num = 0;
        ifstream in;
        in.open(filename);
        if (in.is_open()) {
            while (getline(in, line)) {
                line_num++;
                cout << line_num << endl;
                tree->add_file(split(line));
            }
        }

        return tree;
    }

    array<string, 4> File_reader::split(string str)
    {
        array<string, 4> raw_data{};
        for(size_t i = DATA_LEN - 1; i > 0; i--) {
            size_t start = str.rfind(" ");
            raw_data[i] = str.substr(start);
            str.erase(start);
            remove_tail_spaces(str);
        }
        raw_data[0] = str;

        return raw_data;
    }

    string File_reader::remove_tail_spaces(string &str)
    {
        while (str.back() == SPACE_C) {
            str.pop_back();
        }

        return str;
    }

    string File_reader::dat_path(const string &path)
    {
        string dat_path = path;
        dat_path.erase(dat_path.size() - EXTENSION_LEN, EXTENSION_LEN);
        dat_path.append(DAT_EXT);

        return dat_path;
    }

}
