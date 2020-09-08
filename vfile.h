#ifndef VFILE_H
#define VFILE_H

#include <string>

namespace File_process {
    using namespace std;

    // unfinished.
    class Vfile
    {
    public:
        Vfile(const string p, long s, long o, unsigned long long u, const string md5) :
            path{p}, size(s), offset(o), utc_time(u), md5_val(md5){}

    //    bool operator==(Vfile &f);
        const string path;
        const long size;
        /*const*/ long offset;
        const unsigned long long utc_time;
        const string md5_val;


    };
}


#endif // VFILE_H
