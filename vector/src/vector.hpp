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
	/**
	 * a data container like std::vector
	 * store data in a successive memory and support random access.
	 */
	template <typename T>
	class vector
	{
	private:
	T* data;
	size_t size;
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
			/**
			 * TODO add data members
			 *   just add whatever you want.
			 */
		public:
			/**
			 * return a new iterator which pointer n-next elements
			 * as well as operator-
			 */
			iterator operator+(const int &n) const
			{
				// TODO
			}
			iterator operator-(const int &n) const
			{
				// TODO
			}
			// return the distance between two iterators,
			// if these two iterators point to different vectors, throw invaild_iterator.
			int operator-(const iterator &rhs) const
			{
				// TODO
			}
			iterator &operator+=(const int &n)
			{
				// TODO
			}
			iterator &operator-=(const int &n)
			{
				// TODO
			}
			/**
			 * TODO iter++
			 */
			iterator operator++(int) {}
			/**
			 * TODO ++iter
			 */
			iterator &operator++() {}
			/**
			 * TODO iter--
			 */
			iterator operator--(int) {}
			/**
			 * TODO --iter
			 */
			iterator &operator--() {}
			/**
			 * TODO *it
			 */
			T &operator*() const {}
			/**
			 * a operator to check whether two iterators are same (pointing to the same memory address).
			 */
			bool operator==(const iterator &rhs) const {}
			bool operator==(const const_iterator &rhs) const {}
			/**
			 * some other operator for iterator.
			 */
			bool operator!=(const iterator &rhs) const {}
			bool operator!=(const const_iterator &rhs) const {}
		};

		/**
		 * TODO
		 * has same function as iterator, just for a const object.
		 */
		class const_iterator
		{
		public:
			using difference_type = std::ptrdiff_t;
			using value_type = T;
			using pointer = T *;
			using reference = T &;
			using iterator_category = std::output_iterator_tag;

		private:
			/*TODO*/
		};
		
		
		vector() : data(nullptr), size(0), capacity(0) {}
		vector(const vector &other) : size(other.size), capacity(other.capacity)
		{
			data = alloc.allocate(capacity); //分配
			for (size_t i = 0; i < size; ++i)
			{
				//data[i] = other.data[i];
				alloc.construct(data + i, other.data[i]); //构造
			}
		}
		
		~vector()  
		{
			for (size_t i = 0; i < size; ++i)
			{
				alloc.destroy(data + i); //析构
			}
			alloc.deallocate(data, capacity); //释放
			size = 0;
			capacity = 0;
			data = nullptr;
		}
		
		vector &operator=(const vector &other) 
		{
			if (this = &other)
			{
				return *this;
			}
			for (size_t i = 0; i < size; ++i)
			{
				alloc.destroy(data + i); //析构
			}
			alloc.deallocate(data, capacity); //释放
			size = other.size;
			capacity = other.capacity;
			data = alloc.allocate(capacity); //分配
			for (size_t i = 0; i < size; ++i)
			{
				//data[i] = other.data[i];
				alloc.construct(data + i, other.data[i]); //构造
			}
			return *this;
		}
		
		void Double()
		{
			capacity *= 2;
			T *tmp = alloc.allocate(2 * capacity);
			for (size_t i = 0; i < size; ++i)
			{
				alloc.construct(tmp + i, data[i]);
			}
			for (size_t i = 0; i < size; ++i)
			{
				alloc.destroy(data + i);
			}
			alloc.deallocate(data, capacity);
			data = tmp;
			tmp = nullptr;
		}

		T &at(const size_t &pos) 
		{
			if (pos >= size || pos < 0)
			{
				throw index_out_of_bound();
			}
			return data[pos];
		}
		const T &at(const size_t &pos) const 
		{
			//return const_cast<vector<T>*>(this)->at(pos); //不确定风险
			if (pos >= size || pos < 0)
			{
				throw index_out_of_bound();
			}
			return data[pos];
		}
		
		T &operator[](const size_t &pos) 
		{
			if (pos >= size || pos < 0)
			{
				throw index_out_of_bound();
			}
			return data[pos];
		}
		const T &operator[](const size_t &pos) const 
		{
			if (pos >= size || pos < 0)
			{
				throw index_out_of_bound();
			}
			return data[pos];
		} 
		
		const T &front() const 
		{
			if (size == 0)
			{
				throw container_is_empty();
			}
			return data[0];
		}
		const T &back() const 
		{
			if (size == 0)
			{
				throw container_is_empty();
			}
			return data[size - 1];
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
			return (size == 0);
		}
		
		size_t size() const 
		{
			return size;
		}
		
		void clear() 
		{
			for (size_t i = 0; i < size; ++i)
			{
				alloc.destroy(data + i);
			}
			alloc.deallocate(data, capacity);
			size = 0;
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
			if (size >= capacity)
			{
				Double();
			}
			alloc.construct(data + size, value);
			++size;
		}
		
		void pop_back() 
		{
			if (size == 0)
			{
				throw container_is_empty();
			}
			alloc.destroy(data + size - 1);
			--size;
		}
	};

}

#endif
