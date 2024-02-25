#ifndef SJTU_VECTOR_HPP
#define SJTU_VECTOR_HPP

#include "exceptions.hpp"

#include <climits>
#include <cstddef>
#include <cstdio>
#include <cstring>
#include <iostream>
#include <cmath>
#include <string>

// 参考资料：stl源码解析
namespace sjtu
{
	template <typename T>
	class vector
	{
	private:
		T *data;				 // 存储数据
		size_t size_;			 // 已占容量
		size_t capacity;		 // 总容量
		std::allocator<T> alloc; // 空间配置器

	public:
		/**
		 * TODO
		 * a type for actions of the elements of a vector, and you should write
		 *   a class named const_iterator with same interfaces.
		 */
		/**
		 * you can see RandomAccessIterator at CppReference for help.
		 */
		class const_iterator;
		class iterator
		{
			// The following code is written for the C++ type_traits library.
			// Type traits is a C++ feature for describing certain properties of a type.
			// For instance, for an iterator, iterator::value_type is the type that the
			// iterator points to.
			// STL algorithms and containers may use these type_traits (e.g. the following
			// typedef) to work properly. In particular, without the following code,
			// @code{std::sort(iter, iter1);} would not compile.
			// See these websites for more information:
			// https://en.cppreference.com/w/cpp/header/type_traits
			// About value_type: https://blog.csdn.net/u014299153/article/details/72419713
			// About iterator_category: https://en.cppreference.com/w/cpp/iterator
		public:
			using difference_type = std::ptrdiff_t;
			using value_type = T;
			using pointer = T *;
			using reference = T &;
			using iterator_category = std::output_iterator_tag;

		private:
			size_t pos;	 // 迭代器位置
			vector *vec; // 指向容器
		public:
			iterator() : pos(0), vec(nullptr) {}
			iterator(size_t position, vector *v) : pos(position), vec(v) {}
			iterator(const iterator &other) : pos(other.pos), vec(other.vec) {}

			iterator operator+(const int &n) const
			{
				if (pos + n >= vec->size_)
				{
					throw index_out_of_bound();
				}
				return iterator(pos + n, vec);
			}
			iterator operator-(const int &n) const
			{
				if (pos - n < 0)
				{
					throw index_out_of_bound();
				}
				return iterator(pos - n, vec);
			}

			int operator-(const iterator &rhs) const
			{
				if (vec != rhs.vec)
				{
					throw invalid_iterator();
				}
				return pos - rhs.pos;
			}
			iterator &operator+=(const int &n)
			{
				// return (*this + n);
				if (pos + n >= vec->size_)
				{
					throw index_out_of_bound();
				}
				pos += n;
				return *this;
			}
			iterator &operator-=(const int &n)
			{
				// return (*this - n);
				if (pos - n < 0)
				{
					throw index_out_of_bound();
				}
				pos -= n;
				return *this;
			}

			iterator operator++(int)
			{
				if (pos + 1 >= vec->size_)
				{
					throw index_out_of_bound();
				}
				return iterator(pos + 1, vec);
			}

			iterator &operator++()
			{
				if (pos + 1 >= vec->size_)
				{
					throw index_out_of_bound();
				}
				++pos;
				return *this;
			}

			iterator operator--(int)
			{
				if (pos < 1)
				{
					throw index_out_of_bound();
				}
				return iterator(pos - 1, vec);
			}

			iterator &operator--()
			{
				if (pos < 1)
				{
					throw index_out_of_bound();
				}
				--pos;
				return *this;
			}

			T &operator*() const
			{
				return vec->data[pos];
			}

			bool operator==(const iterator &rhs) const
			{
				return (pos == rhs.pos) && (vec == rhs.vec);
			}
			bool operator==(const const_iterator &rhs) const
			{
				return (pos == rhs.pos) && (vec == rhs.vec);
			}

			bool operator!=(const iterator &rhs) const
			{
				return !(*this == rhs);
			}
			bool operator!=(const const_iterator &rhs) const
			{
				return !(*this == rhs);
			}
		};

		class const_iterator
		{
		public:
			using difference_type = std::ptrdiff_t;
			using value_type = T;
			using pointer = T *;
			using reference = T &;
			using iterator_category = std::output_iterator_tag;

		private:
			size_t pos;
			const vector *vec;

		public:
			const_iterator() : pos(0), vec(nullptr) {}
			const_iterator(size_t position, vector *v) : pos(position), vec(v) {}
			const_iterator(const const_iterator &other) : pos(other.pos), vec(other.vec) {}

			const_iterator operator+(const int &n) const
			{
				if (pos + n >= vec->size_)
				{
					throw index_out_of_bound();
				}
				return const_iterator(pos + n, vec);
			}
			const_iterator operator-(const int &n) const
			{
				if (pos - n < 0)
				{
					throw index_out_of_bound();
				}
				return const_iterator(pos - n, vec);
			}

			int operator-(const const_iterator &rhs) const
			{
				if (vec != rhs.vec)
				{
					throw invalid_iterator();
				}
				return pos - rhs.pos;
			}
			const_iterator &operator+=(const int &n)
			{
				// return (*this + n);
				if (pos + n >= vec->size_)
				{
					throw index_out_of_bound();
				}
				pos += n;
				return *this;
			}
			const_iterator &operator-=(const int &n)
			{
				// return (*this - n);
				if (pos - n < 0)
				{
					throw index_out_of_bound();
				}
				pos -= n;
				return *this;
			}

			const_iterator operator++(int)
			{
				if (pos + 1 >= vec->size_)
				{
					throw index_out_of_bound();
				}
				return const_iterator(pos + 1, vec);
			}

			const_iterator &operator++()
			{
				if (pos + 1 >= vec->size_)
				{
					throw index_out_of_bound();
				}
				++pos;
				return *this;
			}

			const_iterator operator--(int)
			{
				if (pos < 1)
				{
					throw index_out_of_bound();
				}
				return const_iterator(pos - 1, vec);
			}

			const_iterator &operator--()
			{
				if (pos < 1)
				{
					throw index_out_of_bound();
				}
				--pos;
				return *this;
			}

			T &operator*() const
			{
				return vec->data[pos];
			}

			bool operator==(const iterator &rhs) const
			{
				return (pos == rhs.pos) && (vec == rhs.vec);
			}
			bool operator==(const const_iterator &rhs) const
			{
				return (pos == rhs.pos) && (vec == rhs.vec);
			}

			bool operator!=(const iterator &rhs) const
			{
				return !(*this == rhs);
			}
			bool operator!=(const const_iterator &rhs) const
			{
				return !(*this == rhs);
			}
		};

		vector() : data(nullptr), size_(0), capacity(0) {}
		vector(const vector &other) : size_(other.size_), capacity(other.capacity)
		{
			data = alloc.allocate(capacity); // 分配
			for (size_t i = 0; i < size_; ++i)
			{
				// data[i] = other.data[i];
				alloc.construct(data + i, other.data[i]); // 构造
			}
		}

		~vector()
		{
			for (size_t i = 0; i < size_; ++i)
			{
				alloc.destroy(data + i); // 析构
			}
			alloc.deallocate(data, capacity); // 释放
			size_ = 0;
			capacity = 0;
			data = nullptr;
		}

		vector &operator=(const vector &other)
		{
			if (this == &other)
			{
				return *this;
			}
			for (size_t i = 0; i < size_; ++i)
			{
				alloc.destroy(data + i); // 析构
			}
			alloc.deallocate(data, capacity); // 释放
			size_ = other.size_;
			capacity = other.capacity;
			data = alloc.allocate(capacity); // 分配
			for (size_t i = 0; i < size_; ++i)
			{
				// data[i] = other.data[i];
				alloc.construct(data + i, other.data[i]); // 构造
			}
			return *this;
		}

		void Double()
		{
			capacity *= 2;
			T *tmp = alloc.allocate(2 * capacity);
			for (size_t i = 0; i < size_; ++i)
			{
				alloc.construct(tmp + i, data[i]);
			}
			for (size_t i = 0; i < size_; ++i)
			{
				alloc.destroy(data + i);
			}
			alloc.deallocate(data, capacity);
			data = tmp;
			tmp = nullptr;
		}

		T &at(const size_t &pos)
		{
			if (pos >= size_ || pos < 0)
			{
				throw index_out_of_bound();
			}
			return data[pos];
		}
		const T &at(const size_t &pos) const
		{
			// return const_cast<vector<T>*>(this)->at(pos); //不确定风险
			if (pos >= size_ || pos < 0)
			{
				throw index_out_of_bound();
			}
			return data[pos];
		}

		T &operator[](const size_t &pos)
		{
			if (pos >= size_ || pos < 0)
			{
				throw index_out_of_bound();
			}
			return data[pos];
		}
		const T &operator[](const size_t &pos) const
		{
			if (pos >= size_ || pos < 0)
			{
				throw index_out_of_bound();
			}
			return data[pos];
		}

		const T &front() const
		{
			if (size_ == 0)
			{
				throw container_is_empty();
			}
			return data[0];
		}
		const T &back() const
		{
			if (size_ == 0)
			{
				throw container_is_empty();
			}
			return data[size_ - 1];
		}

		iterator begin()
		{
			return iterator(0, this);
		}
		const_iterator cbegin() const
		{
			return const_iterator(0, this);
		}

		iterator end()
		{
			return iterator(size_ - 1, this);
		}
		const_iterator cend() const
		{
			return const_iterator(size_ - 1, this);
		}

		bool empty() const
		{
			return (size_ == 0);
		}

		size_t size() const
		{
			return size_;
		}

		void clear()
		{
			for (size_t i = 0; i < size_; ++i)
			{
				alloc.destroy(data + i);
			}
			alloc.deallocate(data, capacity);
			size_ = 0;
			capacity = 0;
			data = nullptr;
		}
		/**
		 * inserts value before pos
		 * returns an iterator pointing to the inserted value.
		 */
		iterator insert(iterator pos, const T &value)
		{
			size_t ind = pos - begin();
			return insert(ind, value);
		}
		/**
		 * inserts value at index ind.
		 * after inserting, this->at(ind) == value
		 * returns an iterator pointing to the inserted value.
		 * throw index_out_of_bound if ind > size (in this situation ind can be size because after inserting the size will increase 1.)
		 */
		iterator insert(const size_t &ind, const T &value)
		{
			if (ind < 0 || ind > size_)
			{
				throw index_out_of_bound();
			}
			if (size_ >= capacity)
			{
				Double();
			}
			const size_t elem_after = size_ - ind;
			iterator pos_old(ind, this);
			iterator pos_new(ind + 1, this);
			uninitialized_copy_n(pos_old, size_ - ind + 1, pos_new);
			alloc.construct(data + ind, value);
			++size_;
			return begin() + ind;

		}
		/**
		 * removes the element at pos.
		 * return an iterator pointing to the following element.
		 * If the iterator pos refers the last element, the end() iterator is returned.
		 */
		iterator erase(iterator pos) 
		{
			size_t ind = pos - begin();
			return erase(ind);
		}
		/**
		 * removes the element with index ind.
		 * return an iterator pointing to the following element.
		 * throw index_out_of_bound if ind >= size
		 */
		iterator erase(const size_t &ind) 
		{
			if (ind < 0 || ind >= size_)
			{
				throw index_out_of_bound();
			}
			iterator pos_old(ind + 1, this);
			iterator pos_new(ind, this);
			uninitialized_copy_n(pos_old, size_ - ind, pos_new);
			alloc.destroy(data + ind);
			--size_;
			return begin() + ind;
		}

		void push_back(const T &value)
		{
			if (size_ >= capacity)
			{
				Double();
			}
			alloc.construct(data + size_, value);
			++size_;
		}

		void pop_back()
		{
			if (size_ == 0)
			{
				throw container_is_empty();
			}
			alloc.destroy(data + size_ - 1);
			--size_;
		}
	};

}

#endif
