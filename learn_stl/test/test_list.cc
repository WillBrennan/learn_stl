#include "learn_stl/list.h"

#include "learn_stl/array.h"

#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "helpers.h"

template <typename TypeT>
class ListTest : public ::testing::Test {};

using Types = testing::Types<double, float, int, unsigned char, learn::array<double, 3>>;
TYPED_TEST_CASE(ListTest, Types);

TYPED_TEST(ListTest, DefaultCtor) { 
	using List = learn::list<TypeParam>;
	
	ASSERT_NO_THROW({ List list; });
}

TYPED_TEST(ListTest, EmplaceBack) { 
	using List = learn::list<TypeParam>;
	
	List list;
	ASSERT_NO_THROW({ list.emplace_back(helpers::generate<TypeParam>()); });
}

TYPED_TEST(ListTest, Empty) { 
	using List = learn::list<TypeParam>;

	List list;
	
	ASSERT_TRUE(list.empty());

	list.emplace_back(helpers::generate<TypeParam>());

	ASSERT_FALSE(list.empty());
}

TYPED_TEST(ListTest, Size) {
	using List = learn::list<TypeParam>;

	List list; 

	ASSERT_EQ(list.size(), 0);

	list.emplace_back(helpers::generate<TypeParam>());

	ASSERT_EQ(list.size(), 1);

	list.emplace_back(helpers::generate<TypeParam>());
	list.emplace_back(helpers::generate<TypeParam>());

	ASSERT_EQ(list.size(), 3);
}

TYPED_TEST(ListTest, ConstEmpty) {
	using List = learn::list<TypeParam>;
	
	List list;

	const List& list_a = list;
	ASSERT_TRUE(list_a.empty());

	list.emplace_back(helpers::generate<TypeParam>());

	ASSERT_FALSE(list_a.empty());
}


TYPED_TEST(ListTest, ConstSize) {
	using List = learn::list<TypeParam>;

	List list;

	const List& list_a = list;

	ASSERT_EQ(list_a.size(), 0);

	list.emplace_back(helpers::generate<TypeParam>());

	ASSERT_EQ(list_a.size(), 1);

	list.emplace_back(helpers::generate<TypeParam>());
	list.emplace_back(helpers::generate<TypeParam>());

	ASSERT_EQ(list_a.size(), 3);
	ASSERT_EQ(list.size(), list_a.size());
}

TEST(ListTest, EmptyIterators) {
	using List = learn::list<char>;
	
	List list;
	ASSERT_EQ(list.begin(), list.end());
}

TEST(ListTest, EmptyConstIterators) {
	using List = learn::list<char>;

	const List list;
	ASSERT_EQ(list.begin(), list.end());
}

TEST(ListTest, ForwardIterators) {
	using List = learn::list<char>;
	
	List list;

	list.emplace_back('a');
	list.emplace_back('b');
	list.emplace_back('d');
	list.emplace_back('c');

	auto iter = list.begin();
	
	ASSERT_EQ(*iter, 'a');
	ASSERT_EQ(iter++, list.begin());
	
	ASSERT_EQ(*iter++, 'b');
	ASSERT_EQ(*iter++, 'd');

	ASSERT_EQ(*iter++, 'c');
	ASSERT_EQ(iter, list.end());	
}

TEST(ListTest, ConstForwardIterators) {
	using List = learn::list<char>;

	const List list = []() -> List {
		List list;
		list.emplace_back('a');
		list.emplace_back('b');
		list.emplace_back('d');
		list.emplace_back('c');

		return list;
	}();

	auto iter = list.begin();

	ASSERT_EQ(*iter, 'a');
	ASSERT_EQ(iter++, list.begin());

	ASSERT_EQ(*iter++, 'b');
	ASSERT_EQ(*iter++, 'd');
	
	ASSERT_EQ(*iter++, 'c');
	ASSERT_EQ(iter, list.end());
}


TEST(ListTest, BackwardsIterators) { FAIL(); }
TEST(ListTest, ConstBackwardsIterators) { FAIL(); }
