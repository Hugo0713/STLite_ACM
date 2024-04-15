#ifndef SJTU_MAP_HPP
#define SJTU_MAP_HPP

#include <functional>
#include <cstddef>
#include "utility.hpp"
#include "exceptions.hpp"

// 参考资料：https://www.cnblogs.com/komet/p/13736468.html
// https://www.cnblogs.com/leipDao/p/10097001.html
// github repository from ACMClassCourse-2022
namespace sjtu
{

    template <
        class Key,
        class T,
        class Compare = std::less<Key>>
    class map
    {
    public:
        typedef pair<const Key, T> value_type;
        class iterator;

    private:
        struct Node
        {
            value_type data;
            Node *ls;
            Node *rs;
            Node *f;
            size_t h;

            Node() : data(0), ls(nullptr), rs(nullptr), f(nullptr), h(0) {}
            Node(const value_type &data_, Node *l = nullptr, Node *r = nullptr, Node *f_ = nullptr, size_t h_ = 0) : data(data_), ls(l), rs(r), f(f_), h(h_) {}
            Node(const Node &other) : data(other.data), ls(other.ls), rs(other.rs), f(other.f), h(other.h) {}
        };

        size_t Size;
        Node *root;
        Compare compare; // 减少函数调用开销

        int max(int a, int b)
        {
            return a > b ? a : b;
        }
        size_t get_h(Node *node)
        {
            return (node ? node->h : 0);
        }
        int balance(Node *node)
        {
            return get_h(node->ls) - get_h(node->rs);
        }
        int update_h(Node *a)
        {
            return max(get_h(a->ls), get_h(a->rs)) + 1;
        }

        Node *copy_Node(Node *a)
        {
            if (a == nullptr)
            {
                return nullptr;
            }
            Node *new_node = new Node(a->data);
            new_node->h = a->h;
            new_node->ls = copy_Node(a->ls);
            new_node->rs = copy_Node(a->rs);
            if (new_node->ls != nullptr)
            {
                new_node->ls->f = new_node;
            }
            if (new_node->rs != nullptr)
            {
                new_node->rs->f = new_node;
            }
            return new_node;
        }
        void delete_Node(Node *a)
        {
            if (a == nullptr)
            {
                return;
            }
            delete_Node(a->ls);
            delete_Node(a->rs);
            delete a;
        }

        Node *find_Node(const Key &key)
        {
            Node *p = root;
            while (p != nullptr && (compare(p->data.first, key) || compare(key, p->data.first)))
            {
                if (compare(key, p->data.first))
                {
                    p = p->ls;
                }
                else
                {
                    p = p->rs;
                }
            }
            if (p == nullptr)
            {
                return nullptr;
            }
            return p;
        }

        void LL(Node *&x)
        {
            Node *y = x->ls;
            x->ls = y->rs;
            if (y->rs != nullptr)
            {
                y->rs->f = x;
            }
            y->rs = x;
            y->f = x->f;
            x->f = y;
            x->h = update_h(x);
            y->h = update_h(y);
            x = y;
        }
        void RR(Node *&x)
        {
            Node *y = x->rs;
            x->rs = y->ls;
            if (y->ls != nullptr)
            {
                y->ls->f = x;
            }
            y->ls = x;
            y->f = x->f;
            x->f = y;
            x->h = update_h(x);
            y->h = update_h(y);
            x = y;
        }
        void LR(Node *&x)
        {
            RR(x->ls);
            LL(x);
        }
        void RL(Node *&x)
        {
            LL(x->rs);
            RR(x);
        }

        pair<iterator, bool> insert_Node(const value_type &value, Node *&t, Node *&father)
        {
            if (t == nullptr)
            {
                t = new Node(value, nullptr, nullptr, father, 1);
                iterator it(t, this);
                return pair<iterator, bool>(it, true);
            }
            else if (compare(value.first, t->data.first))
            {
                father = t;
                pair<iterator, bool> tmp(insert_Node(value, t->ls, father));
                if (tmp.second && balance(t) == 2)
                {
                    if (compare(value.first, t->ls->data.first))
                    {
                        LL(t);
                    }
                    else
                    {
                        LR(t);
                    }
                }
                t->h = update_h(t);
                return tmp;
            }
            else if (compare(t->data.first, value.first))
            {
                father = t;
                pair<iterator, bool> tmp(insert_Node(value, t->rs, father));
                if (tmp.second && balance(t) == -2)
                {
                    if (compare(t->rs->data.first, value.first))
                    {
                        RR(t);
                    }
                    else
                    {
                        RL(t);
                    }
                }
                t->h = update_h(t);
                return tmp;
            }
            else // 插入失败
            {
                iterator it(t, this);
                return pair<iterator, bool>(it, false);
            }
        }

        bool adjust(Node *&t, int SubTree)
        {
            if (SubTree) // 右子树删除，使右子树变矮
            {
                if (balance(t) == 1)
                {
                    return true;
                }
                if (balance(t) == 0)
                {
                    --(t->h);
                    return false;
                }
                if (balance(t->ls) < 0)
                {
                    LR(t);
                    return false;
                }
                LL(t);
                if (balance(t) == 0)
                {
                    return false;
                }
                else
                {
                    return true;
                }
            }
            else
            {
                if (balance(t) == -1)
                {
                    return true;
                }
                if (balance(t) == 0)
                {
                    --(t->h);
                    return false;
                }
                if (balance(t->rs) > 0)
                {
                    RL(t);
                    return false;
                }
                RR(t);
                if (balance(t) == 0)
                {
                    return false;
                }
                else
                {
                    return true;
                }
            }
        }
        bool remove(const Key &key, Node *&t)
        {
            if (t == nullptr)
            {
                return true;
            }
            if (compare(key, t->data.first)) // 左子树删除
            {
                if (remove(key, t->ls)) // 没有变矮
                {
                    return true;
                }
                return adjust(t, 0);
            }
            else if (compare(t->data.first, key)) // 右子树删除
            {
                if (remove(key, t->rs)) // 没有变矮
                {
                    return true;
                }
                return adjust(t, 1);
            }
            else // 根节点
            {
                if (t->ls == nullptr || t->rs == nullptr) // 叶或一子
                {
                    Node *old_node = t;
                    t = (t->ls != nullptr) ? t->ls : t->rs;
                    if (t != nullptr)
                    {
                        t->f = old_node->f;
                    }
                    if (old_node != nullptr)
                    {
                        delete (old_node);
                    }
                    return false;
                }
                else // 两个儿子
                {
                    Node *tmp = t->rs;
                    while (tmp->ls != nullptr)
                    {
                        tmp = tmp->ls;
                    }
                    int tmp_h = tmp->h;
                    tmp->h = t->h;
                    t->h = tmp_h;
                    if (tmp == t->rs) // 特殊交换
                    {
                        Node *father = t->f;
                        Node *son_l = t->ls;
                        Node *son_r = tmp->rs;
                        t->f = tmp;
                        t->ls = nullptr;
                        t->rs = son_r;
                        tmp->f = father;
                        tmp->ls = son_l;
                        tmp->rs = t;

                        if (father != nullptr) // 处理老爹
                        {
                            if (father->ls == t)
                            {
                                father->ls = tmp;
                            }
                            else
                            {
                                father->rs = tmp;
                            }
                        }
                        if (son_l != nullptr)
                        {
                            son_l->f = tmp;
                        }
                        if (son_r != nullptr)
                        {
                            son_r->f = t;
                        }
                        t = tmp;
                    }
                    else // 交换
                    {
                        Node *father = tmp->f;
                        Node *son = tmp->rs;
                        Node *father_ = t->f;
                        Node *son_l = t->ls;
                        Node *son_r = t->rs;

                        tmp->f = father_;
                        tmp->ls = son_l;
                        tmp->rs = son_r;
                        t->ls = nullptr;
                        t->rs = son;
                        t->f = father;
                        if (son != nullptr)
                        {
                            son->f = t;
                        }
                        father->ls = t;
                        if (son_l != nullptr)
                        {
                            son_l->f = tmp;
                        }
                        son_r->f = tmp;
                        if (father_ != nullptr)
                        {
                            if (t == father_->ls)
                            {
                                father_->ls = tmp;
                            }
                            else
                            {
                                father_->rs = tmp;
                            }
                        }
                        t = tmp;
                    }
                    if (remove(key, t->rs))
                    {
                        return true;
                    }
                    return adjust(t, 1);
                }
            }
        }

    public:
        class const_iterator;
        class iterator
        {
        private:
        public:
            Node *pos;
            const map *container;
            bool flag = true; // end()
        public:
            iterator(Node *pos_ = nullptr, const map *container_ = nullptr, bool flag_ = true) : pos(pos_), container(container_), flag(flag_) {}
            iterator(const iterator &other)
            {
                pos = other.pos;
                container = other.container;
                flag = other.flag;
            }

            void increase()
            {
                if (pos == nullptr)
                {
                    throw invalid_iterator();
                }
                if (pos->rs != nullptr) // 右子树的最左节点
                {
                    Node *lls = pos->rs;
                    while (lls->ls != nullptr)
                    {
                        lls = lls->ls;
                    }
                    pos = lls;
                    return;
                }
                // 向上回溯，节点为父节点的左子节点
                Node *ff = pos->f;
                Node *cur = pos;
                while (ff != nullptr)
                {
                    if (ff->rs == cur)
                    {
                        cur = ff;
                        ff = ff->f;
                    }
                    else
                    {
                        break;
                    }
                }
                pos = ff;
            }
            iterator operator++(int)
            {
                iterator tmp(*this);
                increase();
                return tmp;
            }
            iterator &operator++()
            {
                increase();
                return *this;
            }

            void decrease()
            {
                if (pos == nullptr)
                {
                    if (flag)
                    {
                        throw invalid_iterator();
                    }
                    else
                    {
                        Node *p = container->root;
                        while (p->rs != nullptr)
                        {
                            p = p->rs;
                        }
                        pos = p;
                        flag = true;
                        return;
                    }
                }
                if (pos->ls != nullptr) // 左子树不为空
                {
                    Node *rrs = pos->ls;
                    while (rrs->rs != nullptr)
                    {
                        rrs = rrs->rs;
                    }
                    pos = rrs;
                    return;
                }
                // 左子树为空，向上回溯 该节点为父节点的右子节点
                Node *ff = pos->f;
                Node *cur = pos;
                while (ff != nullptr)
                {
                    if (ff->ls == cur)
                    {
                        cur = ff;
                        ff = ff->f;
                    }
                    else
                    {
                        break;
                    }
                }
                pos = ff;
            }
            iterator operator--(int)
            {
                iterator tmp(*this);
                decrease();
                return tmp;
            }
            iterator &operator--()
            {
                decrease();
                return *this;
            }

            value_type &operator*() const
            {
                return pos->data;
            }
            bool operator==(const iterator &rhs) const
            {
                return (pos == rhs.pos) && (container == rhs.container);
            }
            bool operator==(const const_iterator &rhs) const
            {
                return (pos == rhs.pos) && (container == rhs.container);
            }

            bool operator!=(const iterator &rhs) const
            {
                return !(*this == rhs);
            }
            bool operator!=(const const_iterator &rhs) const
            {
                return !(*this == rhs);
            }

            value_type *operator->() const noexcept
            {
                return &(pos->data);
            }
        };
        class const_iterator
        {
        public:
            const Node *pos;
            const map *container;
            bool flag = true;

        public:
            const_iterator(const Node *pos_ = nullptr, const map *container_ = nullptr, bool flag_ = true) : pos(pos_), container(container_), flag(flag_) {}
            const_iterator(const const_iterator &other)
            {
                pos = other.pos;
                container = other.container;
                flag = other.flag;
            }
            const_iterator(const iterator &other)
            {
                pos = other.pos;
                container = other.container;
                flag = other.flag;
            }

            void increase()
            {
                if (pos == nullptr)
                {
                    throw invalid_iterator();
                }
                if (pos->rs != nullptr) // 右子树的最左节点
                {
                    const Node *lls = pos->rs;
                    while (lls->ls != nullptr)
                    {
                        lls = lls->ls;
                    }
                    pos = lls;
                    return;
                }
                // 向上回溯，节点为父节点的左子节点
                const Node *ff = pos->f;
                const Node *cur = pos;
                while (ff != nullptr)
                {
                    if (ff->rs == cur)
                    {
                        cur = ff;
                        ff = ff->f;
                    }
                    else
                    {
                        break;
                    }
                }
                pos = ff;
            }
            const_iterator operator++(int)
            {
                const_iterator tmp(*this);
                increase();
                return tmp;
            }
            const_iterator &operator++()
            {
                increase();
                return *this;
            }

            void decrease()
            {
                if (pos == nullptr)
                {
                    if (flag)
                    {
                        throw invalid_iterator();
                    }
                    else
                    {
                        Node *p = container->root;
                        while (p->rs != nullptr)
                        {
                            p = p->rs;
                        }
                        pos = p;
                        flag = true;
                        return;
                    }
                }
                if (pos->ls != nullptr) // 左子树不为空
                {
                    const Node *rrs = pos->ls;
                    while (rrs->rs != nullptr)
                    {
                        rrs = rrs->rs;
                    }
                    pos = rrs;
                    return;
                }
                // 左子树为空，向上回溯 该节点为父节点的右子节点
                const Node *ff = pos->f;
                const Node *cur = pos;
                while (ff != nullptr)
                {
                    if (ff->ls == cur)
                    {
                        cur = ff;
                        ff = ff->f;
                    }
                    else
                    {
                        break;
                    }
                }
                pos = ff;
            }
            const_iterator operator--(int)
            {
                const_iterator tmp(*this);
                decrease();
                return tmp;
            }
            const_iterator &operator--()
            {
                decrease();
                return *this;
            }

            const value_type &operator*() const
            {
                return pos->data;
            }
            bool operator==(const iterator &rhs) const
            {
                return (pos == rhs.pos) && (container == rhs.container);
            }
            bool operator==(const const_iterator &rhs) const
            {
                return (pos == rhs.pos) && (container == rhs.container);
            }

            bool operator!=(const iterator &rhs) const
            {
                return !(*this == rhs);
            }
            bool operator!=(const const_iterator &rhs) const
            {
                return !(*this == rhs);
            }

            value_type *operator->() const noexcept
            {
                return const_cast<value_type *>(&(pos->data));
            }
        };

        map()
        {
            root = nullptr;
            Size = 0;
        }
        map(const map &other)
        {
            root = copy_Node(other.root);
            Size = other.Size;
        }
        map &operator=(const map &other)
        {
            if (&other == this)
            {
                return *this;
            }
            delete_Node(root);
            root = copy_Node(other.root);
            Size = other.Size;
            return *this;
        }

        ~map()
        {
            delete_Node(root);
            Size = 0;
        }

        T &at(const Key &key)
        {
            Node *target = find_Node(key);
            if (target == nullptr)
            {
                throw index_out_of_bound();
            }
            return target->data.second;
        }
        const T &at(const Key &key) const
        {
            Node *target = const_cast<sjtu::map<Key, T, Compare> *>(this)->find_Node(key);
            if (target == nullptr)
            {
                throw index_out_of_bound();
            }
            return target->data.second;
        }

        T &operator[](const Key &key) // 插入的另一种形式
        {
            return insert({key, {}}).first->second;
        }
        const T &operator[](const Key &key) const
        {
            Node *target = const_cast<sjtu::map<Key, T, Compare> *>(this)->find_Node(key);
            if (target == nullptr)
            {
                throw index_out_of_bound();
            }
            return target->data.second;
        }

        iterator begin()
        {
            Node *p = root;
            while (p != nullptr && p->ls != nullptr)
            {
                p = p->ls;
            }
            return iterator(p, this);
        }
        const_iterator cbegin() const
        {
            Node *p = root;
            while (p != nullptr && p->ls != nullptr)
            {
                p = p->ls;
            }
            return const_iterator(p, this);
        }

        iterator end()
        {
            return iterator(nullptr, this, false);
        }
        const_iterator cend() const
        {
            return const_iterator(nullptr, this, false);
        }

        bool empty() const
        {
            return Size == 0;
        }

        size_t size() const
        {
            return Size;
        }

        void clear()
        {
            delete_Node(root);
            root = nullptr;
            Size = 0;
        }

        pair<iterator, bool> insert(const value_type &value)
        {
            Node *father = nullptr;
            pair<iterator, bool> tmp(insert_Node(value, root, father));
            if (tmp.second)
            {
                ++Size;
            }
            return tmp;
        }

        void erase(iterator pos_)
        {
            Node *tmp = pos_.pos;
            if (tmp == nullptr || !count(tmp->data.first) || pos_.container != this)
            {
                throw invalid_iterator();
            }
            --Size;
            remove(tmp->data.first, root);
        }

        size_t count(const Key &key) const
        {
            Node *target = const_cast<sjtu::map<Key, T, Compare> *>(this)->find_Node(key);
            if (target == nullptr)
            {
                return 0;
            }
            return 1;
        }

        iterator find(const Key &key)
        {
            Node *target = find_Node(key);
            if (target == nullptr)
            {
                return end();
            }
            return iterator(target, this);
        }
        const_iterator find(const Key &key) const
        {
            Node *target = const_cast<sjtu::map<Key, T, Compare> *>(this)->find_Node(key);
            if (target == nullptr)
            {
                return cend();
            }
            return const_iterator(target, this);
        }
    };

}

#endif
