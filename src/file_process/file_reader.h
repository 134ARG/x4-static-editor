#ifndef FILE_READER_H
#define FILE_READER_H

#include <string>
#include <array>
#include "vfile.h"
#include "btree.h"

#define EXTENSION_LEN 4
#define DATA_LEN 4
#define DAT_EXT ".dat"
#define SPACE_C ' '

namespace File_process {
    using namespace std;

    class File_reader
    {
    public:
        File_reader();

        static Btree *construct_btree(const string &filename);

    private:
        static array<string, 4> split(string str);
        static string remove_tail_spaces(string &str);
        static string dat_path(const string &path);
    };
}

#endif // FILE_READER_H
