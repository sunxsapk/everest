/*
 * ============ aabb2d_tree_t ================
 * Author: Sunil Sapkota
 * Description: aabb2d_tree_t is a spatial partitioning data structure similar to AABBTree but is
 * applied only in 2d space.
 */


#pragma once

#include "core/utils.h"
#include "math/types.h"
#include <vector>


namespace Everest {

    struct aabb2d_t {
        vec2 min, max;


        // returns true if the aabb2d_t overlaps with other
        bool overlaps(const aabb2d_t& other) const;
        // returns the area of the aabb2d_t
        f32 area() const;
        // returns the new area increment after merging to other aabb2d_t
        inline f32 getDiff(const aabb2d_t& other){
            return aabb2d_t::merge(*this, other).area() - area();}
        // merges two AABBs and returns the new one
        static aabb2d_t merge(const aabb2d_t& a, const aabb2d_t& b);
    };

    template<typename object_t>
    struct aabb2d_tree_node_t {
        ref<aabb2d_tree_node_t> left = nullptr, right = nullptr;
        ref<object_t> object = nullptr;
        aabb2d_t bounds;

        // returns true if the node is a leaf node
        inline bool isLeaf(){return object != nullptr;}
        inline bool overlaps(aabb2d_t other){return bounds.overlaps(other);}
    };


    template<typename object_t>
    class aabb2d_tree_t {
        using nodetype = aabb2d_tree_node_t<object_t>;
        public:
            // inserts object with the provided bounds in the tree
            inline void insert(ref<object_t>& object, const aabb2d_t& bounds){
                root = insert(root, object, bounds); }
            // returns the vector of objects which overlaps with the provided bounds
            inline void query(const aabb2d_t& qBounds, std::vector<ref<object_t>>& results){
                query(root, qBounds, results); }

            inline void clear() {
                root.reset();
                root = nullptr;
            }
         private:
            ref<nodetype> root;

            ref<nodetype> insert(ref<nodetype>& node, ref<object_t>& object, const aabb2d_t& bounds){
                if(!node){
                    ref<nodetype> leaf = createRef<nodetype>();
                    leaf->object = object;
                    leaf->bounds = bounds;
                    return leaf;
                }

                if(node->isLeaf()){
                    ref<nodetype> nnode = createRef<nodetype>();
                    nnode->bounds = aabb2d_t::merge(node->bounds, bounds);
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
                node->bounds = aabb2d_t::merge(node->left->bounds, node->right->bounds);
                return node;
            }

            void query(ref<nodetype>& node, const aabb2d_t& qBounds, std::vector<ref<object_t>>& results){
                if(!node) return;
                if(!node->overlaps(qBounds)) return;
                if(node->isLeaf()){
                    results.push_back(node->object);
                } else {
                    query(node->left, qBounds, results);
                    query(node->right, qBounds, results);
                }
            }
    };
}

