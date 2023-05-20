#include <cassert>
#include <climits>
#include <iostream>
#include <cassert>
#include <sstream>
#include "skiplist.h"

using namespace std;

// void test1() {
//   // setting seed for easy testing
//   srand(10000);
//   SkipList skp(5, 80);
//   cout << boolalpha;
//   skp.add(3);
//   skp.add(5);
//   skp.add(vector<int>{1, 7, 9});
//   // cannot add existing
//   assert(!skp.add(vector<int>{1, 7}));
//   cout << skp << endl;
//   cout << "7 ? " << skp.contains(7) << endl;
//   cout << "3 ? " << skp.contains(3) << endl;
//   cout << "-100 ? " << skp.contains(-100) << endl;
//   cout << "100 ? " << skp.contains(100) << endl;
//   bool result = skp.remove(5);
//   assert(result);
//   result = skp.remove(5);
//   assert(!result);
//   cout << skp << endl;
// }

// void test2() {
//   srand(1000);
//   SkipList skp(5, 50);
//   cout << boolalpha;
//   cout << skp << endl;
//   skp.add(3);
//   skp.add(8);
//   skp.add(5);
//   skp.add(9);
//   skp.contains(8);
//   skp.contains(5);
//   skp.contains(9);
//   skp.contains(3);
//   bool result = skp.add(INT_MAX);
//   assert(!result);
//   result = skp.add(INT_MIN);
//   assert(!result);
//   cout << skp << endl;
// }

void test1()
{
  stringstream outSS;
  SkipList skp;
  skp.add(3);
  outSS << skp;
  cout << outSS.str() << endl;
  assert(outSS.str() == "[level: 1] 3-->nullptr\n");

  skp.add(9);
  outSS.str("");
  outSS << skp;
  cout << "skp; " << outSS.str() << endl;
  assert(outSS.str() == "[level: 1] 3-->9-->nullptr\n");

  skp.add(1);
  outSS.str("");
  outSS << skp;
  cout << outSS.str() << endl;
  assert(outSS.str() == "[level: 1] 1-->3-->9-->nullptr\n");

  skp.add(vector<int>{7, 5});
  outSS.str("");
  outSS << skp;
  assert(outSS.str() == "[level: 1] 1-->3-->5-->7-->9-->nullptr\n");

  assert(skp.contains(1) && skp.contains(7) && skp.contains(9));
  assert(!skp.contains(0) && !skp.contains(20));

  // TODO(student) check there are no memory leaks after test completed
  cout << "test1 done." << endl;
}

// testing the copy constructor for the SkipList
void test2()
{
  stringstream outSS;
  SkipList *skp1 = new SkipList;
  skp1->add(vector<int>{9, 1, 7, 5});

  outSS.str("");
  outSS << *skp1;
  // cout << "skp1: " << outSS.str() << endl;
  assert(outSS.str() == "[level: 1] 1-->5-->7-->9-->nullptr\n");

  SkipList *skp2 = new SkipList(*skp1);
  delete skp1;
  outSS << *skp2;
  // cout << "skp2: " << outSS.str() << endl;
  assert(outSS.str() == "[level: 1] 1-->5-->7-->9-->nullptr\n");
  delete skp2;
  // TODO(student) check there are no memory leaks after test completed
  cout << "test2 done." << endl;
}

// testing SkipList with multiple levels
void test3()
{
  stringstream outSS;
  SkipList skp(3, 80);
  skp.add(vector<int>{9, 1, 7, 5, 3, 20});

  outSS << skp;
  cout << "test 3 skp: \n"
       << outSS.str() << endl;
  assert(outSS.str() == "[level: 3] 7-->nullptr\n"
                        "[level: 2] 3-->7-->nullptr\n"
                        "[level: 1] 1-->3-->5-->7-->9-->20-->nullptr\n");

  skp.add(vector<int>{-20, 100});
  outSS.str("");
  outSS << skp;
  assert(outSS.str() ==
         "[level: 3] -20-->7-->100-->nullptr\n"
         "[level: 2] -20-->3-->7-->100-->nullptr\n"
         "[level: 1] -20-->1-->3-->5-->7-->9-->20-->100-->nullptr\n");

  // TODO(student) check that contains searches from top level down
  assert(skp.contains(1) && skp.contains(7) && skp.contains(9));
  assert(!skp.contains(0) && !skp.contains(200));
  assert(skp.contains(-20) && skp.contains(100));

  SkipList skp2(3, 30);
  skp2.add(vector<int>{9, 1, 7, 5, 3, 20});
  outSS.str("");
  outSS << skp2;
  assert(outSS.str() == "[level: 3] nullptr\n"
                        "[level: 2] 3-->5-->nullptr\n"
                        "[level: 1] 1-->3-->5-->7-->9-->20-->nullptr\n");

  assert(skp2.contains(3) && skp2.contains(5) && skp2.contains(20));
  assert(!skp2.contains(-3) && !skp2.contains(4) && !skp2.contains(200));
  // TODO(student) check there are no memory leaks after test completed
  cout << "test3 done." << endl;
}

// removing from multi-level SkipList
void test4()
{
  // stringstream outSS;
  // SkipList skp(3, 50);
  // skp.add(vector<int>{9, 1, 7, 5, 3, 20});

  // outSS << skp;
  // assert(outSS.str() == "[level: 3] 20-->nullptr\n"
  //                       "[level: 2] 3-->5-->7-->9-->20-->nullptr\n"
  //                       "[level: 1] 1-->3-->5-->7-->9-->20-->nullptr\n");
  // SkipList skp2(skp);
  // assert(skp.remove(1));
  // assert(!skp.remove(100));
  // assert(skp.remove(9));

  // outSS.str("");
  // outSS << skp;
  // assert(outSS.str() == "[level: 3] 20-->nullptr\n"
  //                       "[level: 2] 3-->5-->7-->20-->nullptr\n"
  //                       "[level: 1] 3-->5-->7-->20-->nullptr\n");

  // // skp2 should be unchanged
  // outSS.str("");
  // outSS << skp2;
  // assert(outSS.str() == "[level: 3] 20-->nullptr\n"
  //                       "[level: 2] 3-->5-->7-->9-->20-->nullptr\n"
  //                       "[level: 1] 1-->3-->5-->7-->9-->20-->nullptr\n");
  // // TODO(student) check there are no memory leaks after test completed
  // cout << "test4 done." << endl;
}

int main()
{

  test1();
  test2();
  test3();

  cout << "Done." << endl;
  return 0;
}