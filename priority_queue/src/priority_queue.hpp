#ifndef SJTU_PRIORITY_QUEUE_HPP
#define SJTU_PRIORITY_QUEUE_HPP

#include <cstddef>
#include <functional>
#include "exceptions.hpp"

// 参考资料：oi wiki及csdn
namespace sjtu
{
	template <typename T, class Compare = std::less<T>>
	class priority_queue
	{
	private:
		struct Node
		{
			T value;
			int dist;
			Node *ls;
			Node *rs;

			Node() : value(0), dist(0), ls(nullptr), rs(nullptr) {}
			Node(T value_, int dist_ = 0) : value(value_), dist(dist_), ls(nullptr), rs(nullptr) {}
			bool operator<(const Node &b) const
			{
				return Compare()(value, b.value);
			}
		};

		Node *copy_Node(Node *a) // 递归创建树
		{
			if (!a)
			{
				return nullptr;
			}
			Node *new_node = new Node(a->value);
			new_node->ls = copy_Node(a->ls);
			new_node->rs = copy_Node(a->rs);
			new_node->dist = a ? a->dist : 0;
			return new_node;
		}

		void delete_Node(Node *a) // 递归删除树
		{
			if (!a)
			{
				return;
			}
			delete_Node(a->ls);
			delete_Node(a->rs);
			delete a;
		}

		Node *merge_Node(Node *a, Node *b)
		{
			if (!a)
			{
				return b;
			}
			if (!b)
			{
				return a;
			}
			// if (Compare()(a->value, b->value))
			if (*a < *b)
			{
				std::swap(a, b);
			}
			a->rs = merge_Node(a->rs, b);
			if (!a->ls || a->ls->dist < a->rs->dist) // 考虑a->ls为空
			{
				std::swap(a->ls, a->rs);
			}
			a->dist = a->rs ? a->rs->dist + 1 : 0;
			return a;
		}

		Node *root;
		size_t size_;

	public:
		priority_queue() : root(nullptr), size_(0) {}
		priority_queue(const priority_queue &other) : size_(other.size_)
		{
			root = copy_Node(other.root);
		}

		~priority_queue()
		{
			delete_Node(root);
		}

		priority_queue &operator=(const priority_queue &other)
		{
			if (this == &other)
			{
				return *this;
			}
			delete_Node(root);
			root = copy_Node(other.root);
			size_ = other.size_;
			return *this;
		}

		const T &top() const
		{
			if (size_ == 0)
			{
				throw container_is_empty();
			}
			return root->value;
		}

		void push(const T &e)
		{
			Node *new_Node = new Node(e);
			try
			{
				root = merge_Node(root, new_Node);
				++size_;
			}
			catch (...)
			{
				delete new_Node;
			}
		}

		void pop()
		{
			if (size_ == 0)
			{
				throw container_is_empty();
			}
			Node *tmp = merge_Node(root->ls, root->rs);
			delete root;
			// root = nullptr;
			// delete_Node(root);
			root = tmp;
			--size_;
		}

		size_t size() const
		{
			return size_;
		}

		bool empty() const
		{
			return !size_;
		}

		void merge(priority_queue &other)
		{
			root = merge_Node(root, other.root);
			size_ += other.size_;
			// delete other.root;
			other.root = nullptr;
			// delete_Node(other.root);
			// other.root = nullptr;
			other.size_ = 0;
		}
	};

}

#endif
