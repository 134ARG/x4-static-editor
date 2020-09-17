#include "./btree.h"
#include <algorithm>
#include <vector>
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
        long offset = dat_size;
        unsigned long long time = stoull(raw_data[2]);
        string md5 = raw_data[3];
        add_file(new Vfile(path, size, offset, time, md5));
    }


    Vfile *Btree::find_file(const string &id)
    {
        Bnode *file_hub = find_files_hub(id);
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
        if (id_start > id_end || id_start > id_upper_bound() || id_end < id_lower_bound()) {
            return array<Vfile *, 2>{};
        } else if (id_end > id_upper_bound()) {
            return find_range(id_start, root->identity);
        }
        else {
            Vfile *start = find_files_hub(id_start)->find_floor(id_start)->file;
            Vfile *end = find_files_hub(id_end)->find_floor(id_end)->file;

            return array<Vfile *, 2>{start, end};
        }
    }


    array<Vfile *, 2> Btree::find_partial(const string &partial)
    {
        return find_range(partial, generate_upper_bound(partial));
    }


    void Btree::remove_file(const string &id)
    {
        Bnode *file_node = find_files_hub(id);
        file_node->remove_child(id);
        Vfile::remove_from_llist(file_node->find_node(id)->file);

    }


    void Btree::add_file(Vfile *file)
    {
        files_seq.push_back(file);
        dat_size += file->size;

        if (root) {
            const string &key = file->path;
            Bnode *files_hub = nullptr;

            if (is_identity_exceeded(key)) {
                files_hub = update_greatest_identity_to_hub(key);
            } else {
                files_hub =  find_files_hub(key);
            }

            auto iter = files_hub->add_child(new Bnode(file));

            if (files_hub->get_size() > 1) {
                if (iter == files_hub->children.cbegin()) {
                    Vfile::insert_to_llist(file, nullptr, (*(iter+1))->file);
                } else if (iter == files_hub->children.cend() - 1) {
                    Vfile::insert_to_llist(file, (*(iter-1))->file, nullptr);
                } else {
                    Vfile::insert_to_llist(file, (*(iter-1))->file, (*(iter+1))->file);
                }
            }

            update_node_chain_after_add(files_hub);
        } else {
            root = new Bnode(file->path);
            root->add_child(new Bnode(file));
        }

        if (file->next == nullptr) tail = file;
        if (file->prev == nullptr) head = file;
    }


    Bnode *Btree::find_files_hub(const string &key)
    {
        Bnode *pos = root;

        while (pos && !pos->has_files) {
            auto iter = pos->children.cbegin();
            while (iter != pos->children.cend() && (*iter)->identity < key) {
                iter++;
            }
            if (iter == pos->children.cend()) {
                throw "Exceeded identity.";
            } else {
                pos = *iter;
            }
        }
        if (!pos->has_files) throw "error";
        return pos;
    }

    string Btree::id_upper_bound()
    {
        return root->identity;
    }

    string Btree::id_lower_bound()
    {
        return head->path;
    }

    int Btree::enum_index_node(Bnode *start)
    {
        int i = start->get_size();
        if (i) {
            for (auto j : start->children) {
                i += enum_index_node(j);
            }
        }

        return i;
    }


    void Btree::update_node_chain_after_add(Bnode *node)
    {
        while (node && node->is_oversize(order)) {
            if (!node->father) {
                node->father = new Bnode(node->identity);
                node->father->add_child(node);
                root = node->father;
            }
            Bnode *left_half = node->split_to_left();
            node->father->add_child(left_half);

            node = node->father;
        }
    }


    // unfinished
    bool Btree::update_routine_after_del(Bnode *start)
    {
        return true;
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

    const string Btree::generate_upper_bound(const string &partial)
    {
        string upper_bound = partial;
        *(upper_bound.end()-1) += 1;
        return upper_bound;
    }
}

