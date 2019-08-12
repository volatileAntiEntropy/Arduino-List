#pragma once

#ifndef _List_h_
#define _List_h_

#if ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#include "stdlib.h"
#include "new.h"
#endif

template<typename T>
class List
{
private:
	class ListNode {
	public:
		T value;
		ListNode* previousNode;
		ListNode* nextNode;
		ListNode(const T& object = T(), ListNode* previous = nullptr, ListNode* next = nullptr) :
			value(object), previousNode(previous), nextNode(next) {}
	};
	size_t thisSize;
	ListNode* head;
	ListNode* tail;
	void initialize() {
		thisSize = 0U;
		head = new ListNode();
		tail = new ListNode();
		head->nextNode = tail;
		tail->previousNode = head;
	}
public:
	class ConstIterator {
	protected:
		ListNode* currentNode;
		ConstIterator(ListNode* node) :currentNode(node) {}
		T& get() const {
			return currentNode->value;
		}
		friend class List<T>;
	public:
		ConstIterator() :currentNode(nullptr) {}
		const T& operator*() const {
			return currentNode->value;
		}
		ConstIterator& operator++() {
			currentNode = currentNode->nextNode;
			return (*this);
		}
		ConstIterator& operator--() {
			currentNode = currentNode->previousNode;
			return (*this);
		}
		bool operator==(const ConstIterator& iter) const {
			return currentNode == iter.currentNode;
		}
		bool operator!=(const ConstIterator& iter) const {
			return currentNode != iter.currentNode;
		}
	};
	class Iterator :public ConstIterator {
	protected:
		Iterator(ListNode* node) :ConstIterator(node) {}
		friend class List<T>;
	public:
		Iterator() :ConstIterator() {}
		T& operator*() {
			return get();
		}
		const T& operator*() const {
			return ConstIterator::operator*();
		}
		Iterator& operator++() {
			this->currentNode = this->currentNode->value;
			return (*this);
		}
		Iterator& operator--() {
			this->currentNode = this->currentNode->previousNode;
			return (*this);
		}
	};
	List() { initialize(); }
	Iterator begin() {
		return Iterator(head->nextNode);
	}
	ConstIterator begin() const {
		return ConstIterator(head->nextNode);
	}
	Iterator end() {
		return Iterator(tail);
	}
	ConstIterator end() const {
		return ConstIterator(tail);
	}
	size_t size() const {
		return thisSize;
	}
	bool empty() const {
		return size() == 0U;
	}
	T& front() {
		return *(this->begin());
	}
	const T& front() const {
		return *begin();
	}
	T& back() {
		return *(--(this->end()));
	}
	const T& back() const {
		return *--end();
	}
	Iterator insert(Iterator iter, const T& object) {
		ListNode* ptr = iter.currentNode;
		thisSize++;
		/*
		 	Node *np = Node(x,p->prev,p);
			this means that np->prev = p->prev,
		    and np->next = p;
			update the p->prev and p->prev->next;
			*p->prev->next = np;
			*p->prev = np;
		*/
		return Iterator(ptr->previousNode = ptr->previousNode->nextNode = new ListNode(object, ptr->previousNode, ptr));
	}
	Iterator erase(Iterator iter){
		/*得到当前迭代器的指针*/
		ListNode* ptr = iter.currentNode;
		/*得到新的迭代器，并初始化*/
		Iterator returnValue(ptr->nextNode);
		/*更新链表的链接关系*/
		ptr->previousNode->nextNode = ptr->nextNode;
		ptr->nextNode->previousNode = ptr->previousNode;
		/*删除对象*/
		delete ptr;
		/*使得对象数减少*/
		thisSize--;
		/*返回新的迭代器*/
		return returnValue;
	}
	Iterator erase(Iterator start, Iterator end){
		/*for中不使用++iter的原因是erase之后
		 *就是下一个迭代器，因此不需要++操作
		*/
		Iterator iter;
		for (iter = start; iter != end; ){
			/*该操作会导致迭代器更新到下一个*/
			iter = erase(iter);
		}
		return iter;
	}
	void popFront() {
		erase(begin());
	}
	void popBack() {
		erase(--end());
	}
	void pushFront(const T& object) {
		insert(begin(), object);
	}
	void pushBack(const T& object) {
		insert(end(), object);
	}
	void clear(){
		while (!empty()) {
			popFront();
		}
	}
	const List& operator=(const List& _list){
		if (this == &_list)
			return (*this);
		/*清除原有的信息*/
		clear();
		/*添加新的对象*/
		for (ConstIterator iter = _list.begin(); iter != _list.end(); ++iter) {
			pushBack(*iter);
		}
		return (*this);
	}
	List(const List& _list){
		/*创建哨兵节点*/
		initialize();
		/*复制数据*/
		(*this) = _list;
	}
	~List() {
		clear();
		delete head;
		delete tail;
	}
};

#endif