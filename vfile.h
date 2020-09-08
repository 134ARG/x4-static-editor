#ifndef VFILE_H
#define VFILE_H

#include <string>

namespace File_process {
    using namespace std;

    // unfinished.
    class Vfile
    {
    public:
        Vfile(const string p, const string cat_p, int s, int o, long u, const string md5) :
            path{p}, dat_path(cat_p), size(s), offset(o), utc_time(u), md5_val(md5){}
        Vfile(const string p, /*const string cat_p,*/ const string s, /*const string o, */const string u, const string md5) :
            path(p), /*dat_path(cat_p),*/ size(stoull(s)), /*offset(stoull(o)), */utc_time(stoull(u)), md5_val(md5) {}


    //    bool operator==(Vfile &f);
        const string path;
        /*const*/ string dat_path;
        const unsigned long long size;
        /*const*/ unsigned long long offset;
        const unsigned long long utc_time;
        const string md5_val;
    };
}


#endif // VFILE_H
