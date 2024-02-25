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

namespace sjtu
{
	template <typename T>
	class vector
	{
	private:
		T *data;
		size_t size_;
		size_t capacity;
		std::allocator<T> alloc;

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
		/**
		 * returns an iterator to the beginning.
		 */
		iterator begin() {}
		const_iterator cbegin() const {}
		/**
		 * returns an iterator to the end.
		 */
		iterator end() {}
		const_iterator cend() const {}

		bool empty() const
		{
			return (size_ == 0);
		}

		size_t size_() const
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
		iterator insert(iterator pos, const T &value) {}
		/**
		 * inserts value at index ind.
		 * after inserting, this->at(ind) == value
		 * returns an iterator pointing to the inserted value.
		 * throw index_out_of_bound if ind > size (in this situation ind can be size because after inserting the size will increase 1.)
		 */
		iterator insert(const size_t &ind, const T &value) {}
		/**
		 * removes the element at pos.
		 * return an iterator pointing to the following element.
		 * If the iterator pos refers the last element, the end() iterator is returned.
		 */
		iterator erase(iterator pos) {}
		/**
		 * removes the element with index ind.
		 * return an iterator pointing to the following element.
		 * throw index_out_of_bound if ind >= size
		 */
		iterator erase(const size_t &ind) {}

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
