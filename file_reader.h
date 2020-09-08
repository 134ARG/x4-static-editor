#ifndef FILE_READER_H
#define FILE_READER_H

#include <string>
#include <array>
#include "vfile.h"
#include "btree.h"

namespace File_process {
    using namespace std;

    class File_reader
    {
    public:
        File_reader();

        static Btree *construct_btree(const string &filename);

    private:
        array<string, 4> split(string str);
    };
}

#endif // FILE_READER_H
