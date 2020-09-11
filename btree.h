#ifndef BTREE_H
#define BTREE_H

#define BTREE_ORDER 20

#include <array>
#include <vector>
#include "bnode.h"
#include "vfile.h"

namespace File_process {
    using namespace std;

    class Btree
    {
    public:
        Btree(string path) : root(nullptr), dat_path(path), order(BTREE_ORDER) {}
        ~Btree();

        Bnode *root;
        vector<Vfile *> files_seq;
        const string dat_path;
        const size_t order;

        void add_file(array<string, 4> raw_data);
    //    void add_file(string line);

        Vfile *find_file(const string &id);
        array<Vfile *, 2> find_range(const string &id_start, const string &id_end);
        array<Vfile *, 2> find_partial(const string &partial);
    //    Bnode *select_branch(const string &key, const Bnode *start);

        void remove_file(const string &id);
    //    Bnode *find_file_hubs(const string &key, Bnode* start);

//        void print_ele();

        int enum_index_node(Bnode *start);

        string generate_upper_bound(const string &partial);

    private:
        long current_bytes = 0;

        void add_file(Vfile *file_obj);
        Bnode *find_hub(const string &key);

//        Bnode *binary_find(Bnode *node, const string &key);

        void update_node_chain_after_add(Bnode *start);
        bool update_routine_after_del(Bnode *start);

        inline bool is_identity_exceeded(const string &key);
        Bnode *update_greatest_identity_to_hub(const string &new_id);


    };
}


#endif // BTREE_H
