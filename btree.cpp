#include "btree.h"
#include <algorithm>
#include <iostream>
#include <string>

namespace File_process {
    Btree::~Btree()
    {
        delete root;
        for (auto i : files_seq) {
            delete i;
        }
    }

    void Btree::add_file(array<string, 4> raw_data)
    {
        string path = raw_data[0];
        long size = stol(raw_data[1]);
        long offset = current_bytes;
        unsigned long long time = stoull(raw_data[2]);
        string md5 = raw_data[3];
        add_file(new Vfile(path, size, offset, time, md5));
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


    void Btree::add_file(Vfile *file_obj)
    {
        if (root) {
            files_seq.push_back(file_obj);
            const string &key = file_obj->path;
            vector<Bnode *> routine = find_routine(key);
            current_bytes += file_obj->size;

            if (is_identity_exceeded(key)) {
                update_greatest_identity(key, routine);
            }

            Bnode::add_child(routine.back(), new Bnode(file_obj));
            for (auto i = routine.rbegin(); i < routine.rend(); i++) {
                rearrange_by_order(*i);
            }
        } else {
            root = new Bnode("");
            Bnode::add_child(root, new Bnode(file_obj));
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

        while (pos && !pos->has_files) {
            pos = binary_find(pos, real_key);
            routine.push_back(pos);
        }

        // to be refactored
        return routine;
    }

    Bnode *Btree::binary_find(Bnode *node, const string &key)
    {
        size_t mid = node->children.size()/2;
        size_t start = 0;
        size_t end = node->children.size() - 1;

        while (start == end) {
            if (key > node->children[mid]->identity) {
                start = mid + 1;
            } else {
                end = mid;
            }
            mid = (start + end) / 2;
        }

        return node->children[start];
    }


    void Btree::print_ele()
    {
        int idx = 1;
        for (auto i : files_seq) {
            cout << fixed << idx++ << " " << i->path << " " << (double)i->size/1e6 << "MB " << (double)i->offset/1e6 << "MB " << i->utc_time << " " << i->md5_val << endl;
        }

        cout << enum_idx(root) << endl;
    }

    int Btree::enum_idx(Bnode *start)
    {
        int i = Bnode::get_size(start);
        if (i) {
            for (auto j : start->children) {
                i += enum_idx(j);
            }
        }

        return i;
    }


    void Btree::rearrange_by_order(Bnode *start)
    {
        if (start && Bnode::is_oversize(start, max_size)) {
            if (!start->father) {
                start->father = new Bnode("");
                Bnode::add_child(start->father, root);
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

