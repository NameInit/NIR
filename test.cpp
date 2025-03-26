#include <iostream>
#include <cstring>

class Letter{
    private:
        char* symbols;
        int frequency;
    public:
        Letter() : symbols(nullptr), frequency(0) {}
        Letter(char* s, int f) : symbols(strdup(s)), frequency(f) {}
        Letter(const Letter& other) : symbols(strdup(other.symbols)), frequency(other.frequency) {}
        Letter& operator=(const Letter& other) {if (symbols!=nullptr) delete symbols; symbols=strdup(other.symbols); return *this;}
        Letter operator+(const Letter& other) {
            Letter temp(symbols, frequency+other.frequency);
            strcat(temp.symbols, other.symbols);
            return temp;
        }

        int operator<(const Letter& other){ return frequency<other.frequency; }

        friend std::ostream& operator<<(std::ostream& out,const Letter& other){
            out << '(' <<other.symbols <<"; "<< other.frequency<<')';
            return out;
        }
        ~Letter(){ delete symbols; }
};

template<typename Item>
class BTree {
private:
    struct Node {
        Item item;
        Node* left;
        Node* right;

        Node(Item& value) : item(value), left(nullptr), right(nullptr) {}
        int operator<(const Node& other) { return item<other.item; }
        friend std::ostream& operator<<(std::ostream& out, const Node& other){
            out<<other.item;
            return out;
        }
    };

    Node* __root;

public:
    BTree() : __root(nullptr) {}

    BTree(Item& value) : __root(new Node(value)) {}

    int operator<(const BTree& other) { return *__root<*other.__root; }
    BTree operator+(const BTree& other){
        Item temp_item=__root->item+other.__root->item;
        BTree temp(temp_item);
        if(__root<other.__root){
            temp.__root->left=__root;
            temp.__root->right=other.__root;
        }
        else{
            temp.__root->left=other.__root;
            temp.__root->right=__root;
        }
        return temp;
    }

    void destroy(Node* node) {
        if (node) {
            destroy(node->left);
            destroy(node->right);
            node->left=nullptr;
            node->right=nullptr;
            delete node;
        }
    }

    Node* get_root(){
        return __root;
    }

    void show(){
        ;
    }

    friend std::ostream& operator<<(std::ostream& out, const BTree& other){
        out<<*other.__root;
        return out;
    }
};

// void foo(){
//     auto foo1 = [](int a){
//         std::cout << a << std::endl;
//     };
//     for(int i=0; i<10; i++)
//         foo1(i);
// }

int main() {
    Letter i1((char*)"a", 10);
    Letter item2((char*)"b", 9);
    BTree<Letter> Huffman1(i1);
    BTree<Letter> Huffman2(item2);
    BTree<Letter> Huffman3=Huffman1+Huffman2;
    std::cout << Huffman3<<std::endl;
    Huffman3.destroy(Huffman3.get_root());
    return 0;
}