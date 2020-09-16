#ifndef BTREE_H
#define BTREE_H

#define DEFAULT_BTREE_ORDER 20

#include <array>
#include <vector>
#include "bnode.h"
#include "vfile.h"

namespace File_process {
    using namespace std;

    class Btree
    {
    public:
        Btree(string path) : dat_path(path), order(DEFAULT_BTREE_ORDER) {}
        Btree(string path, size_t o) : dat_path(path), order(o) {}
        Btree(const Btree &) = delete;
        Btree &operator=(const Btree &) = delete;
        ~Btree();

        Bnode *root = nullptr;
        vector<Vfile *> files_seq;
        Vfile *head = nullptr;
        Vfile *tail = nullptr;
        const string dat_path;
        const size_t order;

        void add_file(array<string, 4> raw_data);
        Vfile *find_file(const string &id);
        array<Vfile *, 2> find_range(const string &id_start, const string &id_end);
        array<Vfile *, 2> find_partial(const string &partial);
        void remove_file(const string &id);

//        void print_ele();

        int enum_index_node(Bnode *start);

    private:
        long dat_size = 0;

        void add_file(Vfile *file_obj);
        Bnode *find_files_hub(const string &key);

//        Bnode *binary_find(Bnode *node, const string &key);

        string id_upper_bound();
        string id_lower_bound();

        void update_node_chain_after_add(Bnode *start);
        bool update_routine_after_del(Bnode *start);

        const string generate_upper_bound(const string &partial);

        inline bool is_identity_exceeded(const string &key);
        Bnode *update_greatest_identity_from_hub(const string &new_id);


    };
}


#endif // BTREE_H
