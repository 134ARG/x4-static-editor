#include <iostream>
#include "file_reader.h"
#include <fstream>
#include <sstream>
#include <vector>
#include <algorithm>
#include <string>

using namespace std;

int main()
{
    auto i = File_process::File_reader::construct_btree(path2);
    i->print_ele();


    auto j = 1;
    delete i;
    return 0;
}
