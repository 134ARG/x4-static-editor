#include "./bnode.h"

namespace File_process {
    using namespace std;

    Bnode::~Bnode()
    {
        for (auto i : children){
            delete i;
        }
    }

    vector<Bnode *>::const_iterator Bnode::add_child(Bnode *child)
    {
        child->father = this;
        update_identity(child);             // to be refactored.
        children.push_back(child);

        size_t idx = get_size() - 1;
        while (idx && is_greater(children[idx-1], children[idx])) {
            swap(children[idx-1], children[idx]);
            idx--;
        }
        if (child->is_file) has_files = true;

        return children.cbegin() + idx;


    //        sort(father->children.begin(), father->children.end(), is_smaller);
    }

    Bnode *Bnode::remove_child(const string identity)
    {
        for (auto i = children.begin(); i < children.end(); i++) {
            if ((*i)->identity == identity) {
                delete *i;
                children.erase(i);
                return this;
            }
        }
        return nullptr;
    }

    Bnode *Bnode::remove_child(const Bnode *node)
    {
        for (auto i = children.begin(); i != children.end(); i++) {
            if (*i == node) {
                delete *i;
                children.erase(i);
                return this;
            }
        }
        return nullptr;
    }

    Bnode *Bnode::remove_children(size_t i, size_t j)
    {
        if (i < get_size() && j < get_size()) {
            children.erase(children.cbegin()+i, children.cbegin()+j);
        }

        return this;
    }

    bool Bnode::is_oversize(size_t size)
    {
        return size < children.size();
    }

    size_t Bnode::get_size()
    {
        return children.size();
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

    bool Bnode::is_greater(const Bnode *a, const Bnode *b)
    {
        return a->identity > b->identity;
    }

    Bnode *Bnode::split_to_left()
    {
        Bnode *left_half = new Bnode(/*get_mid(node)->identity*/"");

        for (size_t i = 0; i <= get_size()/2; i++){
            left_half->add_child(children.front());
            children.erase(children.begin());
        }

        return left_half;
    }

    Bnode *Bnode::merge_to_left(Bnode *left, Bnode *right)
    {
        for (auto i : right->children) {
            left->children.push_back(i);
        }
        left->identity = right->identity;

        return left;
    }

    Bnode *Bnode::find_node(string identity)
    {
        for (auto i : children) {
            if (i->identity == identity) {
                return i;
            }
        }

        return nullptr;
    }

    Bnode *Bnode::find_floor(const string &id)
    {
        auto i = children.cbegin();
        while (i != children.cend() && (*i)->identity < id) {
            i++;
        }

        if (i == children.cend()) {
            return nullptr;
        } else {
            return *i;
        }
    }
}

