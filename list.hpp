#pragma once

#include <iostream>
#include <stdexcept>

template<typename Item>
class List{
	private:
		typedef struct Node{
			Item __item;
			Node *__right;
			Node *__left;

			Node(const Item &item) : __item(item), __right(nullptr), __left(nullptr) {}
		}Node;

	Node *__begin;
	Node *__end;
	unsigned __size;

	public:
		List() : __begin(nullptr), __end(nullptr), __size(0) {}
		~List() {
			Node *temp=__begin;
			while(temp!=nullptr){
				temp=__begin->__right;
				delete __begin;
				__begin=temp;
			}
		}

		Item& operator[](int index){
			index=index<0 ? __size+index : index;
			if((index>=__size)||(index<0))
				throw std::logic_error("List index out of range");
			Node *temp=__begin;
			int i=0;
			while(i++!=index)
				temp=temp->__right;
			return temp->__item;
		}

		void append(const Item& item){
			if(__size==0)
				__begin=new Node(item);
			else if(__size==1){
				__end=new Node(item);
				__end->__left=__begin;
				__begin->__right=__end;
			}
			else{
				Node *temp=new Node(item);
				__end->__right=temp;
				temp->__left=__end;
				__end=temp;
			}
			__size+=1;
		}

		void insert(const Item& item, int index) {
			if (index < 0 || index > __size) {
				throw std::out_of_range("Index out of range");
			}
			index=index<0 ? __size+index : index;
			Node* newNode = new Node(item);

			if (index == 0) {
				if (__begin == nullptr) {
					__begin = newNode;
					__end = newNode;
				} else {
					newNode->__right = __begin;
					__begin->__left = newNode;
					__begin = newNode;
				}
			} else if (index == __size) {
				newNode->__left = __end;
				if (__end != nullptr) {
					__end->__right = newNode;
				}
				__end = newNode;
			} else {
				Node* temp = __begin;
				for (int i = 0; i < index; ++i) {
					temp = temp->__right;
				}
				newNode->__left = temp->__left;
				newNode->__right = temp;
				if (temp->__left != nullptr) {
					temp->__left->__right = newNode;
				}
				temp->__left = newNode;
			}
			__size+=1;
		}

		Item pop(int index) {
			if (index < 0 || index >= __size) {
				throw std::out_of_range("Index out of range");
			}

			Node* temp = __begin;

			for (int i = 0; i < index; ++i) {
				temp = temp->__right;
			}

			Item item = temp->__item;

			if (temp->__left != nullptr)
				temp->__left->__right = temp->__right;
			else
				__begin = temp->__right;

			if (temp->__right != nullptr)
				temp->__right->__left = temp->__left;
			else
				__end = temp->__left;

			delete temp;
			__size--;

			return item;
		}

		int size(){
			return __size;
		}

		void free(){
			Node *temp=__begin;
			while(temp!=nullptr){
				temp=__begin->__right;
				delete __begin->__item;
				delete __begin;
				__begin=temp;
			}
			__size=0;
		}

		void clear(){
			Node *temp=__begin;
			while(temp!=nullptr){
				temp=__begin->__right;
				delete __begin;
				__begin=temp;
			}
			__size=0;
		}

		void show(){
			std::cout << '[';
			int i=0;
			for(Node* it=__begin; it!=nullptr; it=it->__right, i++){
				std::cout << it->__item ;
				if((i+1)!=__size) std::cout << ", ";
			}
			std::cout<<']'<<std::endl;
		}

		class Iterator{
			private:
				List<Item> *__list;
				Node *__node;

				Iterator(List<Item> *list, Node *node) : __list(list), __node(node) {}
			public:
				using value_type =Item;
				using reference = value_type&;

				Iterator& operator++(){ if(__node!=nullptr) __node=__node->__right; return *this; }
				Iterator& operator--(){ if(__node!=nullptr) __node=__node->__left; return *this; }

				bool operator!=(const Iterator& other) { return __node!=other.__node; }
				bool operator==(const Iterator& other) { return __node==other.__node; }

				reference operator*(){ return __node->__item; }

				friend List<Item>::Iterator List<Item>::begin();
				friend List<Item>::Iterator List<Item>::end();
		};

		Iterator begin(){ return Iterator(this, __begin); }
		Iterator end(){ return Iterator(this, nullptr); }

		friend std::ostream& operator<<(std::ostream& out, const List& other){
			out << '[';
			int i=0;
			for(Node* it=other.__begin; it!=nullptr; it=it->__right, i++){
				out << it->__item ;
				if((i+1)!=other.__size) out << ", ";
			}
			out<<']';
			return out;
		}
};