//
// Created by Yusuf Pisan on 4/26/18.
//

#include <cassert>
#include <climits>
#include <cstdlib>
#include <iostream>
#include "random.h"

#include "skiplist.h"

using namespace std;

ostream &operator<<(ostream &out, const SkipList &skip)
{
  for (int d = skip.levels - 1; d >= 0; d--)
  {
    out << "[level: " << d + 1 << "] ";
    auto *curr = skip.head->next[d];
    if (curr->next[d] != nullptr)
    {
      // cout << curr->val;
      out << curr->val;
      curr = curr->next[d];
    }

    while (curr != nullptr && curr->next[d] != nullptr)
    {
      out << "-->" << curr->val;
      curr = curr->next[d];
    }

    if (curr->next[d] == nullptr)
    {
      out << "-->nullptr\n";
    }

    // out << endl;
  }
  return out;
}

SNode::SNode(int val) : val{val} {}

// copy constructor
SkipList::SkipList(const SkipList &other)
{
  // cout << "constructor 2 called" << endl;
  this->levels = other.levels - 1;
  this->head = new SNode(0);

  for (int d = other.levels - 1; d >= 0; d--)
  {
    SNode *curr = other.head->next[d];
    SNode *thisCurr = this->head;

    if (curr->next[d] != nullptr)
    {
      SNode *temp = new SNode(curr->val);
      thisCurr->next.push_back(temp);
      curr = curr->next[d];
      thisCurr = thisCurr->next[d];
    }
    // cout << "before while loop: " << endl;
    while (curr != nullptr && curr->next[d] != nullptr)
    {
      SNode *temp = new SNode(curr->val);
      thisCurr->next.push_back(temp);
      curr = curr->next[d];
      thisCurr = thisCurr->next[d];
    }
    // cout << "after while loop" << endl;
    if (curr->next[d] == nullptr)
    {
      thisCurr->next.push_back(nullptr);
    }
  }
  this->probability = other.probability;
  // cout << "copy finished" << endl;
}

SkipList::SkipList(int levels, int probability)
    : levels{levels}, probability{probability}
{
  assert(levels > 0 && probability >= 0 && probability < 100);
  head = new SNode(INT_MIN);
  tail = new SNode(INT_MAX);
  for (int currLevel = 0; currLevel < levels; currLevel++)
  {
    head->next.push_back(tail);
    head->backward.push_back(nullptr);
    tail->backward.push_back(head);
    tail->next.push_back(nullptr);
  }
  this->probability = probability;
}

bool SkipList::shouldInsertAtHigher() const
{
  return probability >= Random::random() % 100;
}

SNode *SkipList::getNode(int val) const
{
  SNode *temp = head;
  for (int currLevel = levels - 1; currLevel >= 0; currLevel--)
  {
    // while (temp->next[currLevel] != nullptr &&
    //        temp->next[currLevel]->val <= val)
    // {
    // }
    while (temp->val != tail->val &&
           temp->next[currLevel]->val <= val)
    {
      temp = temp->next[currLevel];
      if (temp->val == val)
      {
        return temp;
      }
    }
  }
  return temp;
}

vector<SNode *> SkipList::getBeforeNodes(int val) const
{
  vector<SNode *> ret;
  SNode *temp = head;
  for (int currLevel = levels - 1; currLevel >= 0; currLevel--)
  {
    while (temp->next[currLevel] != nullptr &&
           temp->next[currLevel]->val <= val)
    // while (temp->val != tail->val &&
    //        temp->next[currLevel]->val <= val)
    {
      temp = temp->next[currLevel];
    }
    ret.insert(ret.begin(), temp);
  }
  return ret;
}

void SkipList::add(int val)
{
  if (!(val == head->val || val == tail->val))
  {

    int insertLevel = 0;
    auto insert = new SNode(val);
    vector<SNode *> vec = getBeforeNodes(val);
    if (vec[0]->val == val)
    {
      delete insert;
    }
    else
    {
      do
      {
        SNode *keep = vec[insertLevel]->next[insertLevel];
        insert->next.push_back(vec[insertLevel]->next[insertLevel]);
        vec[insertLevel]->next[insertLevel] = insert;
        insert->backward.push_back(vec[insertLevel]);
        keep->backward[insertLevel] = insert;
        insertLevel++;
      } while (shouldInsertAtHigher() && insertLevel < levels);
    }
  }
}

void SkipList::add(const vector<int> &vals)
{
  for (int val : vals)
  {
    add(val);
  }
}

SkipList::~SkipList()
{
  // need to delete individual nodes
  SNode *grimReaper = head;
  while (grimReaper != nullptr)
  {
    SNode *next = grimReaper->next[0];
    delete grimReaper;
    grimReaper = next;
  }
}

bool SkipList::remove(int data)
{
  SNode *temp = getNode(data);
  if (temp->val != data)
  {
    return false;
  }
  for (int currLevel = temp->next.size() - 1; currLevel >= 0; currLevel--)
  {
    temp->next[currLevel]->backward[currLevel] = temp->backward[currLevel];
    temp->backward[currLevel]->next[currLevel] = temp->next[currLevel];
  }
  delete temp;
  return true;
}

bool SkipList::contains(int data) const { return getNode(data)->val == data; }
