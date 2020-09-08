#ifndef BTREE_H
#define BTREE_H

#define DEFAULT_SIZE 5

#include "bnode.h"
#include "vfile.h"

namespace File_process {
    using namespace std;

    class Btree
    {
    public:
        Btree() : root(nullptr), max_size(DEFAULT_SIZE) {}
        ~Btree();

        Bnode *root;
        vector<Vfile *> files_seq;
        const size_t max_size;

        void add_file(Vfile *file_obj);
    //    void add_file(string line);

        Vfile *find_file(const string &id);
    //    Bnode *select_branch(const string &key, const Bnode *start);

        Btree *remove_file(const string &id);
    //    Bnode *find_file_hubs(const string &key, Bnode* start);

        void print_ele();

    private:
        vector<Bnode *> find_routine(const string &key);
        void rearrange_by_order(Bnode *start);
        inline string greatest_identity();
        inline bool is_identity_exceeded(const string &key);
        void update_greatest_identity(const string &new_id, vector<Bnode *> &routine);
    };
}


#endif // BTREE_H
