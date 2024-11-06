#include <iostream>
#include <algorithm>
#include <string>
#include <queue>

class AVLL {
public:
    AVLL()
    	: root(nullptr) {}

    void insert(int i, int a) {
    	root = insert(i, a, root);
    }

    void delete_(int i) {
        root = delete_(i, root);
    }

    void sum_of_every_fourth() {
        if (!root) return;
        std::cout << "S[1]: " << root->soef[1] << "\n";
        std::cout << "S[2]: " << root->soef[2] << "\n";
        std::cout << "S[3]: " << root->soef[3] << "\n";
        std::cout << "S[0]: " << root->soef[0] << "\n";
        std::cout << "\n----------------------\n\n";
    }

    void print(int id) {
    	std::cout << "[" << id << "] {tree size: " << root->size << "} : \n\n";
        print_i(root);
        std::cout << "\n----------------------\n\n";
    }

private:
    struct Node {
        int value;
        int size;
        int height;
        Node *left;
        Node *right;
        Node *father;
        int soef[4]; // sum of every fourth element, counting from the vertex as a root of a subtree

        Node(int n)
            : value(n), size(1), height(0), left(nullptr), right(nullptr), father(nullptr) {
            for (int i = 0; i < 4; ++i)
                soef[i] = 0;
            soef[1] = n;
        }
    };

    Node *root;

    // get & set height
    inline int height(Node *v) { return v ? v->height : -1; }
    inline void set_height(Node *v) { if (v) v->height = 1 + std::max(height(v->left), height(v->right)); }

    // get & set size
    inline int size(Node *v) { return v ? v->size : 0; }
    inline void set_size(Node *v) { if (v) v->size = 1 + size(v->left) + size(v->right); }

    // balance factor: + (↘︎), - (↙︎), · (=)
    inline int balance_factor(Node *v) { return v ? height(v->right) - height(v->left) : 0; }

    void update_sums(Node *v) {
        for (int j = 0; j < 4; j++) {
            v->soef[j] =
                (v->left ? v->left->soef[j] : 0)
                + (x(v) % 4 == j ? v->value : 0)
                + (v->right ? v->right->soef[(4 + j - (x(v) % 4)) % 4] : 0);
        }
    }

    Node* rotate_right(Node *y) {
        /*
    	       y                x
    	      / \              / \
    	     x   t3    =>     t1  y
    	    / \                  / \
    	   t1  t2               t2  t3
        */
        // "rotate in x""
    	Node *x = y->left;
        Node *t2 = x->right;

        x->right = y;
        y->left = t2;

        if (t2) t2->father = y;
        x->father = y->father;
        y->father = x;

        set_height(y);
        set_height(x);
        set_size(y);
        set_size(x);

        update_sums(y);
        update_sums(x);

        return x;
    }

    Node* rotate_left(Node *x) {
        /*
   	        x                 y
    	   / \               / \
     	  t1  y     =>      x   t3
  	         / \           / \
       	    t2  t3        t1  t2
        */
    	// "rotate in y"
        Node *y = x->right;
        Node *t2 = y->left;

        y->left = x;
        x->right = t2;

        if (t2) t2->father = x;
        y->father = x->father;
        x->father = y;

        set_height(x);
        set_height(y);
        set_size(x);
        set_size(y);

        update_sums(x);
        update_sums(y);

        return y;
    }

    Node *balance(Node *v) {
        if (balance_factor(v) > 1) {
            if (balance_factor(v->right) < 0) {
                v->right = rotate_right(v->right);
            }
            return rotate_left(v);
        }
        else if (balance_factor(v) < -1) {
            if (balance_factor(v->left) > 0) {
                v->left = rotate_left(v->left);
            }
            return rotate_right(v);
        }

        return v;
    }

    // auxiliary parameter: x = "left size" including root
    inline int x(Node *v) { return 1 + size(v->left); }

    Node* insert(int i, int a, Node *v) {
    	if (!v) {
            return new Node(a);
        }

        if (i <= x(v)) {  // ↙︎
            // if i == x --> insert(x, a, v->left)
            v->left = insert(i, a, v->left);
            v->left->father = v;

            set_size(v->left);
        }

        else {  // if (i > x(v)) ↘︎
        	v->right = insert(i - x(v), a, v->right);
            v->right->father = v;

            set_size(v->right);
        }

        set_height(v);
        set_size(v);

        update_sums(v);

        return balance(v);
    }

    bool is_leaf(Node *v) {
        if (!v) return false;
        else if (!v->left && !v->right) return true;
        return false;
    }

    Node* delete_(int i, Node * v) {
        if (!v) return nullptr;

        if (i < x(v)) {
            v->left = delete_(i, v->left);
            if (v->left) v->left->father = v;
        }
        else if (i > x(v)) {
            v->right = delete_(i - x(v), v->right);
            if (v->right) v->right->father = v;
        }
        else {
            if (is_leaf(v)) {
                delete v;
                return nullptr;
            }
            else if (!v->left) {
                Node *tmp = v->right;
                delete v;
                return tmp;
            }
            else if (!v->right) {
                Node *tmp = v->left;
                delete v;
                return tmp;
            }
            else {
                Node *tmp = v->right;
                while (tmp->left) {
                    tmp = tmp->left;
                }
                v->value = tmp->value;
                v->right = delete_(x(tmp), v->right);
                if (v->right) v->right->father = v;
            }
        }

        set_height(v);
        set_size(v);

        update_sums(v);

        return balance(v);
    }

    void print_i(Node* v) {
        if (!v)
            return;

        int h = v->height;

        // spaces between vertices
        int gap[h+1];
        gap[h] = 3;
        for (int i = h-1; i > 0; i--)
            gap[i] = gap[i+1] * 2 + 1;
        gap[0] = 0;

        // leading spaces
        int offset[h+1];
        offset[h] = 0;
        for (int i = h-1; i >= 0; i--)
            offset[i] = offset[i+1] * 2 + 2;

        std::queue<Node *> pq;
        pq.push(v);

        int rowsize = 1;

        for (int i = 0; i <= h; i++) {
            std::cout << std::string(offset[i], ' ');
            for (int j = 0; j < rowsize; j++) {
                Node *u = pq.front();
                pq.pop();
                if (u) {
                    std::cout << u->value;
                    pq.push(u->left);
                    pq.push(u->right);
                } else {
                    std::cout << " ";
                    pq.push(nullptr);
                    pq.push(nullptr);
                }
                std::cout << std::string(gap[i], ' ');
            }
            std::cout << std::endl;
            rowsize *= 2;
        }
    }
};


int main() {
    AVLL avll;

    while (true) {
        unsigned int op_id = 1;
        int i, a;
        char op;
        std::cin >> op;
        if (op == 'I') {
            std::cin >> i >> a;
            avll.insert(i, a);
            avll.print(op_id);
        }
        else if (op == 'D') {
            std::cin >> i;
            avll.delete_(i);
            avll.print(op_id);
        }
        else if (op == 'S') {
            avll.sum_of_every_fourth();
        }
        op_id++;
    }

    return 0;
}
