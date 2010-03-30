#include "QueryTree.h"
#include "dbCatalog.h"
#include <boost/variant.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/variant.hpp>
#include <boost/any.hpp>
/*
Need the following Macros
�C Select: linear search, binary search and search using a B+tree index or a Hash-based Key.
�C Join: Nested-loop join, Single-loop Join and Partition Hash Join (or Hybrid Hash Join).
�C Sort: Merge-Sort
*/

#define LINEAR_SEARCH 0
#define BINARY_SEARCH 1
#define BTREE_SEARCH 2
#define HASH_KEY 3
#define NESTED_LOOP_JOIN 4
#define SINGLE_LOOP_JOIN 5
#define PARTITION_HASH_JOIN 6
#define MERGE_SORT 7

int CostCalcTree(client::QueryTreeNodePtr root, DbCatalog * dbCatalog);
client::QueryTreeNodePtr JoinOptTree(const client::QueryTreeNodePtr root);