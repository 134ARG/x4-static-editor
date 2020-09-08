#include "btree.h"
#include <algorithm>
#include <iostream>

namespace File_process {
    Btree::~Btree()
    {
        delete root;
        for (auto i : files_seq) {
            delete i;
        }
    }

    void Btree::add_file(Vfile *file_obj)
    {

        if (root) {
            files_seq.push_back(file_obj);
            const string &key = file_obj->path;
            vector<Bnode *> routine = find_routine(key);

            if (is_identity_exceeded(key)) {
                update_greatest_identity(key, routine);
            }

            Bnode::add_child(routine.back(), new Bnode(file_obj));
            for (auto i : routine) {
                rearrange_by_order(i);
            }
        } else {
            root = new Bnode("");
            add_file(file_obj);
        }
    }


    Vfile *Btree::find_file(const string &id)
    {
        Bnode *file_hub = find_routine(id).back();
        for (auto i : file_hub->children) {
            if (i->identity == id) {
                return i->file;
            }
        }

        return nullptr;
    }

    Btree *Btree::remove_file(const string &id)
    {
        Bnode *file_node = find_routine(id).back();
        if (Bnode::remove_child(file_node, id)) {
            return this;
        } else {
            return nullptr;
        }

    }

    vector<Bnode *>Btree::find_routine(const string &key)
    {
        // to be refactored.
        Bnode *pos = root;
        vector<Bnode *> routine{pos};
        string real_key;

        if (is_identity_exceeded(key)) {
            real_key = greatest_identity();
        } else {
            real_key = key;
        }

        while (pos && Bnode::get_size(pos)) {
            for (auto i : pos->children) {
                if (i->identity >= real_key) {
                    pos = i;
                }
            }
            routine.push_back(pos);
        }

        // to be refactored
        return routine;
    }

    void Btree::print_ele()
    {
        for (auto i : files_seq) {
            cout << i->path << i->md5_val << endl;
        }
    }

    void Btree::rearrange_by_order(Bnode *start)
    {
        if (start && Bnode::is_oversize(start, max_size)) {
            if (!start->father) {
                start->father = new Bnode("");
                root = start->father;
            }
            Bnode *left_half = Bnode::split_node(start);
            Bnode::add_child(start->father, left_half);

        }
    }

    string Btree::greatest_identity()
    {
        return root->identity;
    }

    bool Btree::is_identity_exceeded(const string &key)
    {
        return key > greatest_identity();
    }

    void Btree::update_greatest_identity(const string &new_id, vector<Bnode *> &routine)
    {
        for (auto i : routine) {
            i->identity = new_id;
        }
    }
}

