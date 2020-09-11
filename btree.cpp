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
        Bnode *file_hub = find_hub(id);
        for (auto i : file_hub->children) {
//            cout << i->identity << endl;
            if (i->identity == id) {
                return i->file;
            }
        }

        return nullptr;
    }

    array<Vfile *, 2> Btree::find_range(const string &id_start, const string &id_end)
    {
        Vfile *start = find_hub(id_start)->get_smallest_file();
        Vfile *end = find_hub(id_end)->get_greatest_file();

        return array<Vfile *, 2>{start, end};
    }

    array<Vfile *, 2> Btree::find_partial(const string &partial)
    {
        return find_range(partial, generate_upper_bound(partial));
    }


    void Btree::remove_file(const string &id)
    {
        Bnode *file_node = find_hub(id);
        Bnode::remove_child(file_node, id);
        Vfile::remove_from_llist(Bnode::find_node(file_node, id)->file);

    }


    void Btree::add_file(Vfile *file)
    {
        files_seq.push_back(file);
        current_bytes += file->size;

        if (root) {
            const string &key = file->path;
            Bnode *files_hub = nullptr;

            if (is_identity_exceeded(key)) {
                files_hub = update_greatest_identity_to_hub(key);
            } else {
                files_hub =  find_hub(key);
            }

            size_t file_node_idx = Bnode::add_child(files_hub, new Bnode(file));

            if (Bnode::get_size(files_hub) > 1) {
                if (file_node_idx != files_hub->children.size() - 1) {
                    Vfile::insert_to_llist(file, nullptr, files_hub->children[file_node_idx+1]->file);
                } else {
                    Vfile::insert_to_llist(file, files_hub->children[file_node_idx-1]->file, nullptr);
                }
            }

            update_node_chain_after_add(files_hub);
        } else {
            root = new Bnode(file->path);
            Bnode::add_child(root, new Bnode(file));
        }
    }


    Bnode *Btree::find_hub(const string &key)
    {
        Bnode *pos = root;

        while (pos && !pos->has_files) {
            auto iter = pos->children.cbegin();
            while (iter != pos->children.cend() && (*iter)->identity < key) {
                iter++;
            }
            if (iter == pos->children.cend()) {
                return *iter;
            } else {
                pos = *iter;
            }
        }

        return pos;
    }

//    Bnode *Btree::binary_find(Bnode *node, const string &key)
//    {
//        size_t mid = (node->children.size() - 1) / 2;
//        size_t start = 0;
//        size_t end = node->children.size() - 1;


//        // to be refactored
////        while (start != end) {
////            if (key > node->children[mid]->identity) {
////                start = mid + 1;
////            } else {
////                end = mid;
////            }
////            mid = (start + end) / 2;
////        }

//        for (start = 0; start <= end; start++) {
//            if (key <= node->children[start]->identity) {
//                break;
//            }
//            if(start == end) cout << "ERROR!ERROR!ERROR!ERROR!ERROR!ERROR!ERROR!ERROR!ERROR!ERROR!" << endl;
//        }
//        if (start > end) start = end;
////        bool res = node->children[start]->identity >= key;
////        bool res2 = node->children[start]->children.back()->identity >= key;
////        if (!res && ! res2){
////            cout << "both invalid" << endl;
////            cout << node->children[start]->identity << endl;
////            cout << node->children[start]->children.back()->identity << endl;
////        } else if (!res2) {
////            cout << endl << endl << "children invalid" << endl;
////        }
//        return node->children[start];
//    }


//    void Btree::print_ele()
//    {
//        int idx = 1;
//        for (auto i : files_seq) {
//            cout << fixed << idx++ << " " << i->path << " " << (double)i->size/1e6 << "MB " << (double)i->offset/1e6 << "MB " << i->utc_time << " " << i->md5_val << endl;
//        }

//        cout << enum_index_node(root) << endl;
//    }

    int Btree::enum_index_node(Bnode *start)
    {
        int i = Bnode::get_size(start);
        if (i) {
            for (auto j : start->children) {
                i += enum_index_node(j);
            }
        }

        return i;
    }


    void Btree::update_node_chain_after_add(Bnode *node)
    {
        while (node && Bnode::is_oversize(node, order)) {
            if (!node->father) {
                node->father = new Bnode(node->identity);
                Bnode::add_child(node->father, node);
                root = node->father;
            }
            Bnode *left_half = Bnode::split_node(node);
            Bnode::add_child(node->father, left_half);

            node = node->father;
        }
//        if (node && Bnode::is_oversize(node, order)) {
//            if (!node->father) {
//                node->father = new Bnode(node->identity);
//                Bnode::add_child(node->father, node);
//                root = node->father;
//            }
//            Bnode *left_half = Bnode::split_node(node);
//            Bnode::add_child(node->father, left_half);

//            return true;
//        } else {
//            return false;
        //        }
    }

    bool Btree::update_routine_after_del(Bnode *start)
    {

    }



    bool Btree::is_identity_exceeded(const string &key)
    {
        return key > root->identity;
    }


    Bnode *Btree::update_greatest_identity_to_hub(const string &new_id)
    {
        Bnode *pos = root;
        while (!pos->is_file) {
            pos->identity = new_id;
            pos = pos->children.back();
        }

        return pos->father;
    }

    string Btree::generate_upper_bound(const string &partial)
    {
        string upper_bound = partial;
        *(upper_bound.end()-1) += 1;
        return upper_bound;
    }
}

