/*
 * ============ AABBTree2D ================
 * Author: Sunil Sapkota
 * Description: AABBTree2D is a spatial partitioning data structure similar to AABBTree but is
 * applied only in 2d space.
 */


#pragma once

#include "core/utils.h"
#include "math/types.h"
#include <vector>


namespace Everest {

    struct AABB2D {
        vec2 min, max;


        // returns true if the AABB2D overlaps with other
        bool overlaps(const AABB2D& other) const;
        // returns the area of the AABB2D
        f32 area() const;
        // returns the new area increment after merging to other AABB2D
        inline f32 getDiff(const AABB2D& other){
            return AABB2D::merge(*this, other).area() - area();}
        // merges two AABBs and returns the new one
        static AABB2D merge(const AABB2D& a, const AABB2D& b);
    };

    template<typename ObjectType>
    struct AABBTreeNode {
        ref<AABBTreeNode> left, right;
        ref<ObjectType> object;
        AABB2D bounds;

        // returns true if the node is a leaf node
        inline bool isLeaf(){return object != nullptr;}
    };


    template<typename ObjectType>
    class AABBTree2D {
        using nodetype = AABBTreeNode<ObjectType>;
        public:
            // inserts object with the provided bounds in the tree
            inline void insert(ref<ObjectType>& object, const AABB2D& bounds){
                root = insert(root, object, bounds) ; }
            // returns the vector of objects which overlaps with the provided bounds
            inline void query(const AABB2D& qBounds, std::vector<ref<ObjectType>>& results){
                query(root, qBounds, results); }
         private:
            ref<nodetype> root;

            ref<nodetype> insert(ref<nodetype>& node, ref<ObjectType>& object, const AABB2D& bounds){
                if(!node){
                    ref<nodetype> leaf = createRef<nodetype>();
                    leaf->object = object;
                    leaf->bounds = bounds;
                    return leaf;
                }

                if(node->isLeaf()){
                    ref<nodetype> nnode = createRef<nodetype>();
                    nnode->bounds = AABB2D::merge(node->bounds, bounds);
                    nnode->left = node;
                    
                    ref<nodetype> rnode = createRef<nodetype>();
                    rnode->object = object;
                    rnode->bounds = bounds;

                    nnode->right = rnode;
                    return nnode;
                }

                f32 ldiff = node->left->bounds.getDiff(bounds);
                f32 rdiff = node->right->bounds.getDiff(bounds);

                if(ldiff < rdiff){
                    node->left = insert(node->left, object, bounds);
                } else {
                    node->right = insert(node->right, object, bounds);
                }
                node->bounds = AABB2D::merge(node->left->bounds, node->right->bounds);
                return node;
            }

            void query(ref<nodetype>& node, const AABB2D& qBounds, std::vector<ref<nodetype>>& results){
                if(!node) return;
                if(!node->overlaps(qBounds)) return;
                if(node->isLeaf()) results.push_back(root->object);
                else {
                    query(node->left, qBounds, results);
                    query(node->right, qBounds, results);
                }
            }
    };

}

