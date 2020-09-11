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
        Bnode(Bnode *node) : identity(node->identity), has_files(node->has_files), is_file(node->is_file) {}
    //    Bnode(Bnode *child) : identity(child->file->path), children{child} { add_child(this, child); }
        ~Bnode();

        string identity;
        Bnode *father = nullptr;
        bool has_files = false;
        bool is_file = false;
        Vfile *file = nullptr;
        vector<Bnode *> children;

        static size_t add_child(Bnode *father, Bnode *child);
        static Bnode *remove_child(Bnode *father, const string identity);
        static bool is_oversize(const Bnode *node, size_t size);
        static size_t get_size(const Bnode *node);
        static Bnode *split_node(Bnode *node);
        static Bnode *find_node(Bnode *node, string identity);



    //    static bool is_file(Bnode *node);
    //    static bool has_files(Bnode *node);
        static Bnode *get_mid(const Bnode *node);
        static void update_identity(Bnode *start);

        static bool is_greater_or_eq(const Bnode *a, const Bnode *b);
    };
}


#endif // BNODE_H
