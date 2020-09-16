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
            path{p}, size(s), offset(o), utc_time(u), md5_val(md5) {}
        Vfile(const Vfile *vfile) :
            path(vfile->path), size(vfile->size), offset(vfile->offset), utc_time(vfile->utc_time), md5_val(vfile->md5_val) {}

        Vfile(const Vfile &) = delete;
        Vfile &operator=(Vfile &) = delete;
    //    bool operator==(Vfile &f);
        const string path;
        const long size;
        /*const*/ long offset;
        const unsigned long long utc_time;
        const string md5_val;

        Vfile *prev = nullptr;
        Vfile *next = nullptr;

        static void insert_to_llist(Vfile *file, Vfile *front, Vfile *behind);
        static Vfile *remove_from_llist(Vfile *file);
    };
}


#endif // VFILE_H
