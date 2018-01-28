#include "learn_stl/detail/tree.h"

#include <gtest/gtest.h>
#include <gmock/gmock.h>

TEST(Tree, Construction) {
    Tree<int, std::less<int>> tree;
    
}