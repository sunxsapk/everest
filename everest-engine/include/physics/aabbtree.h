/*
 * ============ AABBTree ================
 * Author: Sunil Sapkota
 * Description: AABBTree is a spatial partitioning data structure, which puts objects inside the tree
 * composed of AABBs which are used for coarse collision check.
 */


#pragma once

#include "core/utils.h"
#include "math/types.h"
#include <vector>


namespace Everest {

    struct aabb_t {
        vec3 min, max;


        // returns true if the aabb_t overlaps with other
        bool overlaps(const aabb_t& other) const;
        // returns the volume of the aabb_t
        f32 volume() const;
        // returns the new volume increment after merging to other aabb_t
        inline f32 getDiff(const aabb_t& other){
            return aabb_t::merge(*this, other).volume() - volume();}
        // merges two AABBs and returns the new one
        static aabb_t merge(const aabb_t& a, const aabb_t& b);
    };

    template<typename ObjectType>
    struct AABBTreeNode {
        ref<AABBTreeNode> left, right;
        ref<ObjectType> object;
        aabb_t bounds;

        // returns true if the node is a leaf node
        inline bool isLeaf(){return object != nullptr;}
    };


    template<typename ObjectType>
    class AABBTree {
        using nodetype = AABBTreeNode<ObjectType>;
        public:
            // inserts object with the provided bounds in the tree
            inline void insert(ref<ObjectType>& object, const aabb_t& bounds){
                root = insert(root, object, bounds) ; }
            // returns the vector of objects which overlaps with the provided bounds
            inline void query(const aabb_t& qBounds, std::vector<ref<ObjectType>>& results){
                query(root, qBounds, results); }

            inline void clear() {
                root.reset();
                root = nullptr;
            }
         private:
            ref<nodetype> root;

            ref<nodetype> insert(ref<nodetype>& node, ref<ObjectType>& object, const aabb_t& bounds){
                if(!node){
                    ref<nodetype> leaf = createRef<nodetype>();
                    leaf->object = object;
                    leaf->bounds = bounds;
                    return leaf;
                }

                if(node->isLeaf()){
                    ref<nodetype> nnode = createRef<nodetype>();
                    nnode->bounds = aabb_t::merge(node->bounds, bounds);
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
                node->bounds = aabb_t::merge(node->left->bounds, node->right->bounds);
                return node;
            }

            void query(ref<nodetype>& node, const aabb_t& qBounds, std::vector<ref<nodetype>>& results){
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
