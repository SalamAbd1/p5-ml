#include "BinarySearchTree.hpp"
#include "Map.hpp"
#include "unit_test_framework.hpp"

using namespace std;

TEST(test_empty_map) {
    Map<string, int> map;

    // make sure map is empty with size 0

    ASSERT_TRUE(map.empty());
    ASSERT_EQUAL(map.size(), 0);

    // make sure find() on an element will return null iterator
    ASSERT_EQUAL(map.find("0"), map.end());

    // make sure find() didn't add the element
    ASSERT_TRUE(map.empty());
    ASSERT_EQUAL(map.size(), 0);
}

TEST(test_string_map) {
    Map<string, int> map;
    // adding elements using []

    map["1"] = 1;
    ASSERT_EQUAL(map["1"], 1)

    map["2"] = 2;
    ASSERT_EQUAL(map["2"], 2);

    map["3"] = 3;
    ASSERT_EQUAL(map["3"], 3);

    // map is now {"1", 1} R({"2", 2} R{"3", 3})

    // make sure map is not empty, size is 3

    ASSERT_FALSE(map.empty());

    // testing begin()

    ASSERT_EQUAL((*map.begin()).first, "1");
    ASSERT_EQUAL((*map.begin()).second, 1);

    ASSERT_EQUAL(map.size(), 3);

    // finding elements with find()

    // make sure elements are there
    ASSERT_EQUAL(map.find("1"), map.begin());
    ASSERT_EQUAL(map.find("2"), ++map.begin());
    ASSERT_EQUAL(map.find("3"), ++(++map.begin()));
    
    // make sure find() returns null iterator when not there
    ASSERT_EQUAL(map.find("4"), map.end());

    // adding elements using insert()

    // first check that insert returns false when adding same elements
    ASSERT_EQUAL(map.insert({"1", 1}), make_pair(map.begin(), false));
    ASSERT_EQUAL(map.insert({"2", 2}), make_pair(++map.begin(), false));
    ASSERT_EQUAL(map.insert({"3", 3}), make_pair(++(++map.begin()), false));

    // this should happen regardless of whether the value type of the inserted element 
    // is the same as the existing key_type's value_type
    ASSERT_EQUAL(map.insert({"1", 100}), make_pair(map.begin(), false));
    ASSERT_EQUAL(map.insert({"2", 100}), make_pair(++map.begin(), false));
    ASSERT_EQUAL(map.insert({"3", 100}), make_pair(++(++map.begin()), false));

    // now we can try adding new elements
    map.insert({"4", 4});
    map.insert({"5", 5});
    map.insert({"6", 6});
    ASSERT_EQUAL(map.size(), 6);

    // iterate through map to make sure it contains all added elements
    
    int i = 1;
    for(auto pair : map) {
        ASSERT_EQUAL(pair.first, to_string(i));
        ASSERT_EQUAL(pair.second, i);
        i++;
    }

    // test that [] gives assigns 0 to value by default if key not already in map
    ASSERT_EQUAL(map["0"], 0);

    // make sure begin() now points to {"0", 0} (new lowest value where iterator should begin)

    ASSERT_EQUAL((*map.begin()).first, "0");
    ASSERT_EQUAL((*map.begin()).second, 0);
}

TEST_MAIN()
