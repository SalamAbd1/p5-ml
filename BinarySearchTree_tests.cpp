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

using BST = BinarySearchTree<int>; // making my life easier...
using Iterator = BinarySearchTree<int>::Iterator; // continuing to make my life easier...

TEST(test_empty_and_size_and_copy) {
   BST tree;
   ASSERT_TRUE(tree.empty());
   ASSERT_EQUAL(tree.size(), 0);
   tree.insert(10); ASSERT_EQUAL(tree.size(), 1); // 10
   ASSERT_FALSE(tree.empty());
   tree.insert(8); ASSERT_EQUAL(tree.size(), 2); // 10 L8
   tree.insert(12); ASSERT_EQUAL(tree.size(), 3); // 10 L8 R12
   tree.insert(11); ASSERT_EQUAL(tree.size(), 4); // 10 L8 R(12 L11)
   tree.insert(9); ASSERT_EQUAL(tree.size(), 5); // 10 L(8 R9) R(12 L11)
   ASSERT_FALSE(tree.empty());

   BST copy(tree);
   ASSERT_EQUAL(tree.size(), copy.size());
   ostringstream oss;
   ostringstream oss_copy;
   tree.traverse_inorder(oss);
   copy.traverse_inorder(oss_copy);
   ASSERT_EQUAL(oss.str(), oss_copy.str());

   ASSERT_EQUAL(*tree.min_element(), 8);
   ASSERT_EQUAL(*tree.max_element(), 12);
   
   ASSERT_TRUE(tree.check_sorting_invariant());
}

TEST(test_grand_tree) {
   BST tree;
   tree.insert(50); // 50
   tree.insert(60); // 50 R60
   tree.insert(70); // 50 R(60 R70)
   tree.insert(40); // 50 L40 R(60 R70);
   tree.insert(30); // 50 L(40 L30) R(60 R70)
   tree.insert(65); // 50 L(40 L30) R(60 R(70 L65))
   tree.insert(35); // 50 L(40 L(30 R35)) R(60 R(70 L65))
   Iterator root_iter(tree.find(50));

   // testing find()
   ASSERT_EQUAL(*tree.find(50), 50); // root
   ASSERT_EQUAL(*tree.find(30), 30); // middle
   ASSERT_EQUAL(*tree.find(65), 65); // leaf
   ASSERT_EQUAL(tree.find(75), tree.end()); // none

   // testing min_element() & max_element()
   ASSERT_EQUAL(*tree.min_element(), 30);
   ASSERT_EQUAL(*tree.max_element(), 70);

   // testing traverse_inorder() & traverse_preorder()
   ostringstream oss;
   tree.traverse_inorder(oss);
   ASSERT_EQUAL(oss.str(), "30 35 40 50 60 65 70 ");
   oss.str("");
   tree.traverse_preorder(oss);
   ASSERT_EQUAL(oss.str(), "50 40 30 35 60 70 65 ");

   // testing min_greater_than()
   ASSERT_EQUAL(*tree.min_greater_than(25), 30); // lowest val
   ASSERT_EQUAL(*tree.min_greater_than(66), 70); // highest val
   ASSERT_EQUAL(*tree.min_greater_than(40), 50); // middle val
   ASSERT_EQUAL(tree.min_greater_than(70), tree.end()); // none val
   ASSERT_EQUAL(*tree.min_greater_than(35), 40); // same val

   // testing check_sorting_invariant()
   ASSERT_TRUE(tree.check_sorting_invariant());
}

TEST(bst_public_test) {
  BinarySearchTree<int> tree;

  tree.insert(5);

  ASSERT_TRUE(tree.size() == 1);
  ASSERT_TRUE(tree.height() == 1);

  ASSERT_TRUE(tree.find(5) != tree.end());

  tree.insert(7);
  tree.insert(3);

  ASSERT_TRUE(tree.check_sorting_invariant());
  ASSERT_TRUE(*tree.max_element() == 7);
  ASSERT_TRUE(*tree.min_element() == 3);
  ASSERT_TRUE(*tree.min_greater_than(5) == 7);

  cout << "cout << tree.to_string()" << endl;
  cout << tree.to_string() << endl << endl;

  cout << "cout << tree" << endl << "(uses iterators)" << endl;
  cout << tree << endl << endl;

  ostringstream oss_preorder;
  tree.traverse_preorder(oss_preorder);
  cout << "preorder" << endl;
  cout << oss_preorder.str() << endl << endl;
  ASSERT_TRUE(oss_preorder.str() == "5 3 7 ");

  ostringstream oss_inorder;
  tree.traverse_inorder(oss_inorder);
  cout << "inorder" << endl;
  cout << oss_inorder.str() << endl << endl;
  ASSERT_TRUE(oss_inorder.str() == "3 5 7 ");
}

TEST(test_empty_tree) {
   BST tree;
   ASSERT_EQUAL(tree.find(0), tree.end());
   ASSERT_EQUAL(tree.min_element(), tree.end());
   ASSERT_EQUAL(tree.max_element(), tree.end());
   ASSERT_EQUAL(tree.min_greater_than(0), tree.end());
   ostringstream oss;
   tree.traverse_inorder(oss);
   ASSERT_EQUAL(oss.str(), "");
   tree.traverse_preorder(oss);
   ASSERT_EQUAL(oss.str(), "");
   ASSERT_TRUE(tree.check_sorting_invariant());
   ASSERT_TRUE(tree.empty());
   BST copy(tree);
   ASSERT_TRUE(copy.empty());
}

TEST(test_one_element_tree) {
   BST tree;
   tree.insert(0);
   ASSERT_EQUAL(*tree.find(0), 0); 
   ASSERT_EQUAL(*tree.min_element(), 0);
   ASSERT_EQUAL(*tree.max_element(), 0);
   ASSERT_EQUAL(*tree.min_greater_than(-1), 0);
   ASSERT_EQUAL(tree.min_greater_than(0), tree.end());
   ostringstream oss;
   tree.traverse_inorder(oss);
   ASSERT_EQUAL(oss.str(), "0 ");
   oss.str("");
   tree.traverse_preorder(oss);
   ASSERT_EQUAL(oss.str(), "0 "); 
   ASSERT_TRUE(tree.check_sorting_invariant());
   ASSERT_FALSE(tree.empty());
   BST copy(tree);
   ASSERT_FALSE(copy.empty());

}

TEST_MAIN()
