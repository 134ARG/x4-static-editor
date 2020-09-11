#include "bnode.h"

namespace File_process {
    using namespace std;

    Bnode::~Bnode()
    {
        for (auto i : children){
            delete i;
        }
    }

    size_t Bnode::add_child(Bnode *father, Bnode *child)
    {
        child->father = father;
        update_identity(child);

//        father->children.push_back(child);
        size_t idx = 0;
        while (get_size(father)
               && idx < father->children.size()
               && is_greater_or_eq(child, father->children[idx])) {
            idx++;
        }
//            if (child->is_file) {
//                father->has_files = true;

//                if (idx == father->children.end()) {
//                    Vfile::insert(child->file, (*(idx-1))->file, nullptr);
//                } else {
//                    Vfile::insert(child->file, nullptr, (*idx)->file);
//                }
//            }

//        } else {
//            if (child->is_file) father->has_files = true;

        father->children.insert(father->children.begin() + idx, child);

        if (child->is_file) father->has_files = true;
        return idx;


    //        sort(father->children.begin(), father->children.end(), is_smaller);
    }

    Bnode *Bnode::remove_child(Bnode *father, const string identity)
    {
        for (auto i = father->children.begin(); i < father->children.end(); i++) {
            if ((*i)->identity == identity) {
                delete *i;
                father->children.erase(i);
                return father;
            }
        }
        return nullptr;
    }

    bool Bnode::is_oversize(const Bnode *node, size_t size)
    {
        return size < node->children.size();
    }

    size_t Bnode::get_size(const Bnode *node)
    {
        return node->children.size();
    }

    Bnode *Bnode::get_mid(const Bnode *node)
    {
        return node->children[node->children.size()/2];
    }

    void Bnode::update_identity(Bnode *start)
    {
        while (start->father) {
            if (start->father->identity < start->identity) {
                start->father->identity = start->identity;
            }
            start = start->father;
        }

    }

    bool Bnode::is_greater_or_eq(const Bnode *a, const Bnode *b)
    {
        return a->identity >= b->identity;
    }

    Bnode *Bnode::split_node(Bnode *node)
    {
        Bnode *left_half = new Bnode(/*get_mid(node)->identity*/"");

        for (size_t i = 0; i <= get_size(node)/2; i++){
            add_child(left_half, node->children.front());
            node->children.erase(node->children.begin());
        }

        return left_half;
    }

    Bnode *Bnode::find_node(Bnode *node, string identity)
    {
        for (auto i : node->children) {
            if (i->identity == identity) {
                return i;
            }
        }

        return nullptr;
    }

}


