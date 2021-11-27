
//
// Created by Tom Guy on 11/18/2021.
//
#include <cmath>
#include <cstdio>
#include <exception>
#include <memory>
#include <iostream>

#ifndef AVLTREE_H_
#define AVLTREE_H_

namespace wet1_dast
{

    const int NOT_EXIST = -1;

    template<class T>
    class AVLTree
    {

    private:
        class Node
        {
            friend class AVLTree<T>;

            T* value;
            Node *father;
            Node *right_son;
            Node *left_son;
            int height;

            explicit Node(T* val = nullptr, Node *father = nullptr) : value(val), father(father), right_son(nullptr),
                                                                      left_son(nullptr),
                                                                      height(0)
            {
                left_son=nullptr;
                right_son=nullptr;
            }
            ~Node()
            {

                father = nullptr;
                right_son = nullptr;
                left_son = nullptr;
                value = nullptr;
//                delete this;
            }

            bool operator==(Node *other)
            {
                return (*value == *(other->value));
            }

            bool operator<=(Node *other)
            {
                return (*value <= *(other->value));
            }

           bool Tree_Creator_AUX(int *size_of_tree, int height_of_tree);

        };

        Node *root; //belongs to AVLTree
        int size;
       //@param loc and father_of_loc are getting changed in this function according to the situaton.
       //@ver - the current vertex we are looking at.
       //@val - the value we are looking for.

        T *find_in_tree(Node *ver, const T &val, Node **loc, Node **father_of_loc)
        {
            if (*(ver->value) == val) //if we're in the location
            {
                //problem here.
                //valgrind says loc isn't inited.
                *loc = ver; // mark this as the location.
                *father_of_loc = ver->father;//mark father.
                return ver->value; // return us the value.
            }
            //if left son doesn't exist , and the value we look for is lesser than the current vertex(meaning he doesn't;t exist in the tree according ot convention)
            //or alternatively ,right son doesn't exist and the current value we're looking for is larger than the current vertex value(meaning we want to go right according to convention,meaning he doesn't exist).
            if ((!ver->left_son && val <= *(ver->value)) || (!ver->right_son && *(ver->value) <= val))
            {
               *loc = nullptr; //the place doesn't exist.
               *father_of_loc = ver; //mark him as his father.
               return nullptr; //return nullptr,we couldn't find him
            }

            if (val <= *(ver->value) && ver->left_son) //if the value we are looking for is lesser than the current vertex ,go left.
            {
                return find_in_tree(ver->left_son, val, loc, father_of_loc);
            } else if(*(ver->value) <= val && ver->right_son)
            {
                //else go right,because he's larger than him.
                return find_in_tree(ver->right_son, val, loc, father_of_loc);
            }
            *loc = nullptr; //the place doesn't exist.
            *father_of_loc = ver; //mark him as his father.
            return nullptr; //return nullptr,we couldn't find him
        }

        int getHeight(Node *ver);
        int balance(Node *ver);

        void correctHeight(Node *ver);

        Node *easyCaseRemove(Node *loc, bool &easy_case, T* value);

        T* rootCaseRemove();

        Node *swapWithNext(Node *loc);

        void checkForRolls(Node *start);

        void ll_roll(Node *first_ubl);

        void lr_roll(Node *first_ubl);

        void rl_roll(Node *first_ubl);

        void rr_roll(Node *first_ubl);

        void fixFatherAfterRoll(Node *old_son, Node *new_son);

        void inorderHelper(T values[], int index, Node *ver);

        void inorderIn(T* values, int &index, Node *ver);

        void createEmptyTree(int size_of_tree);

        void postOrderDelete(Node* ver);

        void printHelper(Node* ver);


    public:
        friend void combineTrees(AVLTree<T>& to_insert, AVLTree<T> &to_delete)
        {
            T* array_to_insert = to_insert.inorderOut();
            T* array_to_delete = to_delete.inorderOut();
            T* all_players = new T[to_insert.size + to_delete.size];
            int i = 0, j = 0, k;
            for (k = 0; k < to_insert.size + to_delete.size; k++)
            {
                if (to_insert.size == i || to_delete.size == j) break;
                if (array_to_delete[j] == array_to_insert[i])
                {
                    throw AVLTree<T>::ItemInBothTrees();
                }
                if (array_to_insert[i] <= array_to_delete[j])
                {
                    all_players[k] = array_to_insert[i++];
                    continue;
                }
                if (array_to_delete[j] <= array_to_insert[i])
                {
                    all_players[k] = array_to_delete[j++];
                }
            }
            for (; i < to_insert.size; i++)
            {
                all_players[k++] = array_to_insert[i];

            }
            for (; j < to_delete.size; j++)
            {
                all_players[k++] = array_to_delete[j];
            }
            int full_size = to_delete.size + to_insert.size;
            delete &to_delete;
            delete array_to_delete;
            delete array_to_insert;
            to_insert.createEmptyTree(full_size);
            int index = 0;
            to_insert.inorderIn(all_players, index ,to_insert.root);
        }
        int getSize() const;
        AVLTree() : size(0)
        {
            root=nullptr;
        }

        AVLTree(const AVLTree<T> &other) = delete;
        ~AVLTree()
        {
            postOrderDelete(root);
        }

        AVLTree &operator=(const AVLTree<T> &other) = delete;

        T* find(const T &val);

        void insert(T &val);

        T* remove(const T &val);

        T * inorderOut() ;

        void inorderInsert(T* values);

        class exceptions : public std::exception
        {
        };

        class ItemExist : public exceptions
        {
        };

        class ItemNotExist : public exceptions
        {
        };

        class ItemInBothTrees : public exceptions
        {
        public:
            const char *what() const noexcept override
            {
                return "The same item is in both trees";
            }
        };

        T* findClosest(const T &value); // return thr closest from below to the value in the tree

        void printTree();

        T* GetRootValue();

    };
    static int max(int a, int b);

    template<class T>
    bool AVLTree<T>::Node::Tree_Creator_AUX(int *size_of_tree, int height_of_tree)
    {
        if (*size_of_tree == 0 || height_of_tree == -1)
        {
            return false;
        }
        father = nullptr;
        value = nullptr;
        (*size_of_tree)--;
        if(!left_son)
        {
            left_son = new typename AVLTree<T>::Node();
        }
        if(!right_son)
        {
            right_son = new typename AVLTree<T>::Node();
        }
        if(!(left_son->Tree_Creator_AUX(size_of_tree, height_of_tree - 1)))
            left_son = nullptr;
        if(!(right_son->Tree_Creator_AUX(size_of_tree, height_of_tree - 1)))
            right_son = nullptr;
        if (left_son)
            left_son->father = this;
        if (right_son)
            right_son->father = this;
        return true;
    }

    template<class T>
    void AVLTree<T>::createEmptyTree(int size_of_tree)
    {
        //calculate the height based on size.
        //create the tree, for each conjunction , alloc node, perfferably in recursive way.
        int height_of_new_tree = (int) ceil(log2(size_of_tree + 1)) - 1;
        root->Tree_Creator_AUX(&size_of_tree, height_of_new_tree);
    }

    template<class T>
    int AVLTree<T>::getHeight(Node *ver)
    {
        return (!ver) ? NOT_EXIST : ver->height;
    }

    template<class T>
    int AVLTree<T>::balance(Node *ver)
    {
        if (!ver)
        {
            return 0;
        }
        return (this->getHeight(ver->left_son) - this->getHeight(ver->right_son));
    }

    template<class T>
    void AVLTree<T>::correctHeight(Node *ver)
    {
        while (ver)
        {
            ver->height = max(getHeight(ver->right_son), getHeight(ver->left_son)) + 1;
            ver = ver->father;
        }
    }


    template<class T>
    typename AVLTree<T>::Node *AVLTree<T>::easyCaseRemove(AVLTree<T>::Node *loc, bool &easy_case, T* value)
    {
        if (!(loc->right_son || loc->left_son))
        {
            if (*(loc->value) <= *(loc->father->value))
            {
                loc->father->left_son = nullptr;
            } else
            {
                loc->father->right_son = nullptr;
            }
            Node *ver = loc->father;
            value = loc->value;
            delete loc;
            easy_case = true;
            return ver;
        }

        if (loc->left_son && !loc->right_son)
        {
            if (loc->value <= loc->father->value)
            {
                loc->father->left_son = loc->left_son;
            } else
            {
                loc->father->right_son = loc->left_son;
            }
            Node *ver = loc->father;
            value = loc->value;
            delete loc;
            easy_case = true;
            return ver;
        }

        if (!loc->left_son && loc->right_son)
        {
            if (loc->value <= loc->father->value)
            {
                loc->father->left_son = loc->right_son;
            } else
            {
                loc->father->right_son = loc->right_son;
            }
            Node *ver = loc->father;
            value = loc->value;
            delete loc;
            easy_case = true;
            return ver;
        }
        easy_case = false;
        return nullptr;
    }


    template<class T>
    T* AVLTree<T>::rootCaseRemove()
    {
        T* value = root->value;
        if (!(root->right_son) && !(root->left_son))
        {
            delete root;
            root = nullptr;
            return value;
        } else if (!(root->right_son) && root->left_son)
        {
            Node *temp = root->left_son;
            delete root;
            root = temp;
            return value;
        } else if (root->right_son && !(root->left_son))
        {
            Node *temp = root->right_son;
            delete root;
            root = temp;
            return value;
        }
        Node *ver = swapWithNext(root);
        bool easyCase;
        easyCaseRemove(ver, easyCase, value);
        correctHeight(ver);
        checkForRolls(ver);
        return value;
    }

    template<class T>
    typename AVLTree<T>::Node *AVLTree<T>::swapWithNext(Node *loc)
    {
        Node *temp = loc->right_son;
        while (temp->left_son)
        {
            temp = temp->left_son;
        }
        T* temp_value = temp->value;
        temp->value = loc->value;
        loc->value = temp_value;
        return temp;
    }


    template<class T>
    void AVLTree<T>::checkForRolls(Node *start)
    {
        while (start)
        {
            if (balance(start) > -2 && balance(start) < 2)
            {
                start = start->father;
                continue;
            } else
            {
                switch (balance(start))
                {
                    case 2:
                    {
                        if (balance(start->left_son) >= 0)
                        {
                            ll_roll(start);
                            return;
                        }
                        lr_roll(start);
                        return;
                    }
                    case -2:
                    {
                        if (balance(start->right_son) <= 0)
                        {
                            rr_roll(start);
                            return;
                        }
                        rl_roll(start);
                        return;
                    }
                    default: // not supposed to get here
                        rr_roll(start);
                }
            }
        }
    }

    static int max(int a, int b)
    {
        return (a >= b) ? a : b;
    }

    template<class T>
    void AVLTree<T>::ll_roll(Node *first_ubl)
    {
        //todo
        Node *temp = first_ubl->left_son;
        first_ubl->left_son = temp->right_son;
        temp->right_son = first_ubl;
        first_ubl->height = max(getHeight(first_ubl->left_son), getHeight(first_ubl->right_son)) + 1;
        temp->height = max(getHeight(temp->left_son), getHeight(temp->right_son)) + 1;
        fixFatherAfterRoll(first_ubl, temp);
    }

    template<class T>
    void AVLTree<T>::lr_roll(Node *first_ubl)
    {
        //todo
        Node *son = first_ubl->left_son;
        Node *grandson = son->right_son;
        first_ubl->left_son = grandson->right_son;
        son->right_son = grandson->left_son;
        grandson->left_son = son;
        grandson->right_son = first_ubl;
        first_ubl->height = max(getHeight(first_ubl->left_son), getHeight(first_ubl->right_son)) + 1;
        son->height = max(getHeight(son->left_son), getHeight(son->right_son)) + 1;
        grandson->height = max(getHeight(grandson->left_son), getHeight(grandson->right_son)) + 1;
        fixFatherAfterRoll(first_ubl, grandson);
    }


    template<class T>
    void AVLTree<T>::rl_roll(Node *first_ubl)
    {
        //todo
        Node *son = first_ubl->right_son;
        Node *grandson = son->left_son;
        first_ubl->right_son = grandson->left_son;
        son->left_son = grandson->right_son;
        grandson->right_son = son;
        grandson->left_son = first_ubl;
        first_ubl->height = max(getHeight(first_ubl->left_son), getHeight(first_ubl->right_son)) + 1;
        son->height = max(getHeight(son->left_son), getHeight(son->right_son)) + 1;
        grandson->height = max(getHeight(grandson->left_son), getHeight(grandson->right_son)) + 1;
        fixFatherAfterRoll(first_ubl, grandson);
    }


    template<class T>
    void AVLTree<T>::rr_roll(Node *first_ubl)
    {
        //todo
        Node *temp = first_ubl->right_son;
        first_ubl->right_son = temp->left_son;
        temp->left_son = first_ubl;
        first_ubl->height = max(getHeight(first_ubl->left_son), getHeight(first_ubl->right_son));
        temp->height = max(getHeight(temp->left_son), getHeight(temp->right_son));
        fixFatherAfterRoll(first_ubl, temp);
    }

    template<class T>
    void AVLTree<T>::fixFatherAfterRoll(Node *old_son, Node *new_son)
    {
        if (old_son == root)
        {
            root = new_son;
            return;
        }
        Node *father = old_son->father;
        old_son == father->left_son ? father->left_son = new_son : father->right_son = new_son;
    }

    template<class T>
    T* AVLTree<T>::find(const T &val)
    {
        if(root==nullptr) //if root is nullptr whats the point in checking the whole tree.
        {
            return nullptr;
        }
        Node *loc;
        Node *father_of_loc;
        T* to_return= find_in_tree(root, val, &loc, &father_of_loc);
        return to_return;
    }

    template<class T>
    void AVLTree<T>::insert(T &val)
    {
        if (root == nullptr)
        {
            root = new Node(&val);
            size++;
            return;
        }
        Node *loc;
        Node *father_of_loc;
        if (find_in_tree(root, val, &loc, &father_of_loc))
        {

            throw ItemExist();
        }


        if (val <= *(father_of_loc->value))
        {
            father_of_loc->left_son =new Node(&val);
            correctHeight(father_of_loc->left_son);
            checkForRolls(father_of_loc->left_son);
        } else
        {
            father_of_loc->right_son = new Node(&val);
            correctHeight(father_of_loc->right_son);
            checkForRolls(father_of_loc->right_son);
        }
        size++;
    }

    template<class T>
    T* AVLTree<T>::remove(const T &val)
    {
        Node *loc;
        Node *father_of_loc;
        if (!find_in_tree(root, val, &loc, &father_of_loc))
        {

            throw ItemNotExist();
        }
        size--;
        if (loc == root)
        {
            return rootCaseRemove();
        }
        bool easy_case = false;
        T* value;
        Node *ver = easyCaseRemove(loc, easy_case, value);
        if (easy_case)
        {
            correctHeight(ver);
            checkForRolls(ver);
            return value;
        }
        ver = swapWithNext(loc);
        easyCaseRemove(loc, easy_case, value);
        correctHeight(ver);
        checkForRolls(ver);
        return value;
    }

    template<class T>
    T * AVLTree<T>::inorderOut()
    {
        if (!root)
            return nullptr;
        T* values = new T[size];
        inorderHelper(values, 0, root);
        return values;
    }

    template<class T>
    void AVLTree<T>::inorderHelper(T values[] , int index, Node *ver)
    {
        if (!ver)
            return;
        inorderHelper(values, index, ver->left_son);
        values[index++] = *(ver->value); //need to implement an assignment operator for class Player
        inorderHelper(values, index, ver->right_son);
    }


    template<class T>
    void AVLTree<T>::inorderIn(T* values, int &index, Node *ver)
    {
        if (!ver)
            return;
        inorderIn(values, index, ver->left_son);
        ver->value = &(values[index++]); //need to implement a assignment operator for Player. TODO
        inorderIn(values, index, ver->right_son);
    }

    template<class T>
    T* AVLTree<T>::findClosest(const T &value)
    {
        if (*(root->value) == value)
        {
            return nullptr;
        }
        Node *loc;
        Node *father_of_loc;
        find_in_tree(root, value, &loc, &father_of_loc);
        if (loc->right_son)
        {
            return loc->right_son->value;
        }
        if (father_of_loc)
        {
            return father_of_loc->value;
        }
        return nullptr;
    }

    template<class T>
    int AVLTree<T>::getSize() const{
        return size;
    }

    template<class T>
    void AVLTree<T>::postOrderDelete(Node* ver)
    {
        if(!ver)
            return;
        postOrderDelete(ver->left_son);
        postOrderDelete(ver->right_son);
        delete ver;
    }

    template<class T>
    void AVLTree<T>::printTree()
    {
        printHelper(root);
    }

    template<class T>
    void AVLTree<T>::printHelper(Node* ver)
    {
        if(!ver)
            return;
        printHelper(ver->left_son);
        std::cout<<*(ver->value)<<std::endl;
        printHelper(ver->right_son);
    }

    template<class T>
    void AVLTree<T>::inorderInsert(T* values)
    {
        int index = 0;
        inorderIn(values, index, root);
    }

    template<class T>
    T *AVLTree<T>::GetRootValue()
    {
        return root->value;
    }
}


#endif // AVLTREE_H_