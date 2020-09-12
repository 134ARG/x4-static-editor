#ifndef BNODE_H
#define BNODE_H

#include <string>
#include <vector>
#include "vfile.h"

namespace File_process {
    using namespace std;

    class Bnode
    {
    public:
        Bnode(string id) : identity(id) {}
        Bnode(Vfile *file_obj) : identity(file_obj->path), is_file(true), file(file_obj) {}
//        Bnode(Bnode *node) : identity(node->identity), has_files(node->has_files), is_file(node->is_file) {}
    //    Bnode(Bnode *child) : identity(child->file->path), children{child} { add_child(this, child); }
        ~Bnode();

        string identity;
        Bnode *father = nullptr;
        bool has_files = false;
        bool is_file = false;
        Vfile *file = nullptr;
        vector<Bnode *> children;

        vector<Bnode *>::const_iterator add_child(Bnode *child);
        Bnode *remove_child(const string identity);
        Bnode *remove_child(const Bnode *node);
        Bnode *remove_children(size_t i, size_t j);
        bool is_oversize(size_t size);
        size_t get_size();
        Bnode *split_to_left();
        static Bnode *merge_to_left(Bnode *left, Bnode *right);
        Bnode *find_node(string identity);

        Bnode *find_floor(const string &id);


    //    static bool is_file(Bnode *node);
    //    static bool has_files(Bnode *node);
        static Bnode *get_mid(const Bnode *node);
        static void update_identity(Bnode *start);

        static bool is_greater(const Bnode *a, const Bnode *b);
    };
}


#endif // BNODE_H
