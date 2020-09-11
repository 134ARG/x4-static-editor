#include "btree.h"
#include <algorithm>
#include <vector>
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
//            cout << i->identity << endl;
            if (i->identity == id) {
                return i->file;
            }
        }

        return nullptr;
    }


    void Btree::remove_file(const string &id)
    {
        Bnode *file_node = find_routine(id).back();
        Bnode::remove_child(file_node, id);

    }


    void Btree::add_file(Vfile *file_obj)
    {
        if (root) {
            files_seq.push_back(file_obj);
            const string &key = file_obj->path;

            current_bytes += file_obj->size;

//            if (is_identity_exceeded(key)) {
//                update_greatest_identity(key);
//            }
            vector<Bnode *> routine = find_routine(key);
            Bnode *file_hub = routine.back();

            size_t file_node_idx = Bnode::add_child(file_hub, new Bnode(file_obj));
//            file_hub->has_files = true;

            if (Bnode::get_size(file_hub) > 1) {
                if (file_node_idx != file_hub->children.size() - 1) {
                    Vfile::insert_to_llist(file_obj, nullptr, file_hub->children[file_node_idx+1]->file);
                } else {
                    Vfile::insert_to_llist(file_obj, file_hub->children[file_node_idx-1]->file, nullptr);
                }
            }

            for (auto i = routine.rbegin(); i < routine.rend(); i++) {
                rearrange_by_order(*i);
            }
        } else {
            root = new Bnode(file_obj->path);
            Bnode::add_child(root, new Bnode(file_obj));
        }
    }


    vector<Bnode *>Btree::find_routine(const string &key)
    {
        // to be refactored.
        Bnode *pos = root;
        vector<Bnode *> routine{pos};
//        string real_key;

//        if (is_identity_exceeded(key)) {
//            real_key = greatest_identity();
//        } else {
//            real_key = key;
//        }

        if (is_identity_exceeded(key)) {
            while (pos && !pos->has_files) {
                pos = pos->children.back();
                routine.push_back(pos);
            }
        } else {
            while (pos && !pos->has_files) {
                for (size_t i = 0; i < pos->children.size(); i++) {
                    if (pos->children[i]->identity >= key) {
                        pos = pos->children[i];
                        break;
                    }

//                    if (pos->children[i] == pos->children.back()) cout << "ERROR!ERROR!ERROR!ERROR!ERROR!ERROR!ERROR!ERROR!ERROR!ERROR!" << endl;
                }
                routine.push_back(pos);
            }
        }


        // to be refactored
        return routine;
    }

    Bnode *Btree::binary_find(Bnode *node, const string &key)
    {
        size_t mid = (node->children.size() - 1) / 2;
        size_t start = 0;
        size_t end = node->children.size() - 1;


        // to be refactored
//        while (start != end) {
//            if (key > node->children[mid]->identity) {
//                start = mid + 1;
//            } else {
//                end = mid;
//            }
//            mid = (start + end) / 2;
//        }

        for (start = 0; start <= end; start++) {
            if (key <= node->children[start]->identity) {
                break;
            }
            if(start == end) cout << "ERROR!ERROR!ERROR!ERROR!ERROR!ERROR!ERROR!ERROR!ERROR!ERROR!" << endl;
        }
        if (start > end) start = end;
//        bool res = node->children[start]->identity >= key;
//        bool res2 = node->children[start]->children.back()->identity >= key;
//        if (!res && ! res2){
//            cout << "both invalid" << endl;
//            cout << node->children[start]->identity << endl;
//            cout << node->children[start]->children.back()->identity << endl;
//        } else if (!res2) {
//            cout << endl << endl << "children invalid" << endl;
//        }
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


    void Btree::rearrange_by_order(Bnode *node)
    {
        if (node && Bnode::is_oversize(node, order)) {
            if (!node->father) {
                node->father = new Bnode(node->identity);
                Bnode::add_child(node->father, node);
                root = node->father;
            }
            Bnode *left_half = Bnode::split_node(node);
            Bnode::add_child(node->father, left_half);

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


    void Btree::update_greatest_identity(const string &new_id)
    {
        Bnode *pos = root;
        while (!pos->is_file) {
            pos->identity = new_id;
            pos = pos->children.back();
        }
    }

    string Btree::generate_upper_bound(const string &partial)
    {
        string upper_bound = partial;
        *(upper_bound.end()-1) += 1;
        return upper_bound;
    }
}

