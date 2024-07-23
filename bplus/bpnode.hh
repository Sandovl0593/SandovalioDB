#ifndef BPBPNODE_HH
#define BPBPNODE_HH

#include <functional>
#include <iostream>
#include <string>
using namespace std;

// @param K: tipo de dato de las keys
template<typename K>
struct BPNode {
    K *keys;             // Array[K] de keys
    int num_keys;        // Numero de keys
    BPNode<K> **children;// Array[BPNode<K>*] de nodos hijos
    bool is_leaf;        // Indica si el nodo es hoja

    // Constructor de BPNode, por defecto no es hoja y no tiene keys
    explicit BPNode(int order, bool is_leaf = false, int num_keys = 0);

    // Elimina todos los nodos de la sub-arbol comenzando con los nodos hijos 
    // y terminando con el nodo actual
    virtual void killSelf();

    virtual void split(BPNode<K> *&father, int i, int order, int m) = 0;

    void reAssign(BPNode<K> *&father, BPNode<K> *sibling, int i, int m);

    // En caso valide si children[j] tiene hijos, asigna a node el nodo hijo
    bool hasChildren(int j, BPNode<K> *&node);

    K *maxKey();

    virtual ~BPNode();
};

#endif