#include <iostream>
#include "BinarySearchTree.hpp"
#include "unit_test_framework.hpp"

using namespace std;

class Duck {
 public:
    Duck() : wealth(0) {} // tree requires default-constructible
    Duck(int wealth_in) : wealth(wealth_in) {}
    int getWealth() const { return wealth; }
 private:
    int wealth;
 };
class DuckWealthLess {
 public:
    bool operator() (const Duck &d1, const Duck &d2) const {
        return d1.getWealth() < d2.getWealth();
    }

};

ostream &operator<<(ostream &os, const Duck &duck) {
     return os << "Duck: $" << duck.getWealth();
}


TEST(test_compile_check) {
  // make tree
  BinarySearchTree<int> tree;
  const BinarySearchTree<int> &const_tree = tree;

  // make duck tree
  BinarySearchTree<Duck, DuckWealthLess> duck_tree;
  const BinarySearchTree<Duck, DuckWealthLess> &const_duck_tree = duck_tree;

  // Insert elements into trees to prevent breaking requires clauses
  tree.insert(100);
  tree.insert(1000);
  tree.insert(10000);
  duck_tree.insert(Duck(100));
  duck_tree.insert(Duck(1000));
  duck_tree.insert(Duck(10000));

  // Dummy variables
  bool b;
  size_t i;
  size_t st;
  string s;

  // Dummy iterators - should be default constructible as end iterator
  BinarySearchTree<int>::Iterator it;
  BinarySearchTree<Duck, DuckWealthLess>::Iterator duck_it;

  // Big Three
  BinarySearchTree<int> tree_copy(const_tree);
  BinarySearchTree<Duck, DuckWealthLess> duck_tree_copy(const_duck_tree);

  tree_copy = const_tree;
  duck_tree_copy = const_duck_tree;

  // destructor tested implicitly at end of function


  // Functions that can be applied to a const tree
  b = const_tree.empty();
  b = const_duck_tree.empty();

  i = const_tree.height();
  i = const_duck_tree.height();

  st = const_tree.size();
  st = const_duck_tree.size();
  
  ostringstream output;
  const_tree.traverse_inorder(output);
  ASSERT_EQUAL(output.str(), "100 1000 10000 "); 
  output.str("");
  const_duck_tree.traverse_inorder(output);
  ASSERT_EQUAL(output.str(), "Duck: $100 Duck: $1000 Duck: $10000 "); 

  ostringstream output2;
  const_tree.traverse_preorder(output2);
  ASSERT_EQUAL(output2.str(), "100 1000 10000 "); 
  output2.str("");
  const_duck_tree.traverse_preorder(output2);
  ASSERT_EQUAL(output2.str(), "Duck: $100 Duck: $1000 Duck: $10000 "); 

  b = const_tree.check_sorting_invariant();
  b = const_duck_tree.check_sorting_invariant();

  it = const_tree.begin();
  duck_it = const_duck_tree.begin();

  it = const_tree.end();
  duck_it = const_duck_tree.end();

  it = const_tree.min_element();
  duck_it = const_duck_tree.min_element();

  it = const_tree.max_element();
  duck_it = const_duck_tree.max_element();
  
  it = const_tree.find(0);
  duck_it = const_duck_tree.find(Duck());

  s = const_tree.to_string();
  s = const_duck_tree.to_string();

  it = const_tree.min_greater_than(0);
  duck_it = const_duck_tree.min_greater_than(Duck());

  // Functions that can't be called on a const tree
  it = tree.insert(0);
  duck_it = duck_tree.insert(Duck());

  // Using iterators

  it = tree.begin();
  duck_it = duck_tree.begin();

  *it = 50;
  *duck_it = Duck(50);

  ++++it;
  ++++duck_it;

  it = it++;
  duck_it = duck_it++;

  b = tree.end() == tree.end();
  b = duck_tree.end() == duck_tree.end();

  b = tree.end() != tree.end();
  b = duck_tree.end() != duck_tree.end();

  const auto &const_it = it;
  const auto &const_duck_it = duck_it;

  BinarySearchTree<int>::Iterator it_copy(const_it);
  BinarySearchTree<Duck, DuckWealthLess>::Iterator duck_it_copy(const_duck_it);
  ASSERT_FALSE(b);
  ASSERT_EQUAL(i, 3);
  ASSERT_EQUAL(st, 3);
  ASSERT_TRUE(s == duck_tree_copy.to_string());
  ASSERT_EQUAL(*it_copy, 1000);
  ASSERT_EQUAL((*duck_it_copy).getWealth(), 1000);

}


TEST_MAIN()
