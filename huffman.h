#ifndef HUFFMAN
#define HUFFMAN



template<typename Item>
class Tree {
    private:
        struct Node {
            Item item;
            Node* left;
            Node* right;

            Node(Item value) : item(value), left(nullptr), right(nullptr) {}
        };

        Node* root;
    public:
        Tree() : root(nullptr) {}
        Tree(Item value) : root(value) {}

        __GetItem() { return root->item; }
};

#endif