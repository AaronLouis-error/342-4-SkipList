//
// Created by Yusuf Pisan on 4/26/18.
//

#include <cassert>
#include <climits>
#include <cstdlib>
#include <iostream>

#include "skiplist.h"

using namespace std;

ostream &operator<<(ostream &out, const SkipList &skip)
{
  for (int d = skip.maxLevel - 1; d >= 0; d--)
  {
    out << "[level: " << d + 1 << "] ";
    auto *curr = skip.head->forward[d];
    if (curr->forward[d] != nullptr)
    {
      // cout << curr->value;
      out << curr->value;
      curr = curr->forward[d];
    }

    while (curr != nullptr && curr->forward[d] != nullptr)
    {
      out << "-->" << curr->value;
      curr = curr->forward[d];
    }

    if (curr->forward[d] == nullptr)
    {
      out << "-->nullptr\n";
    }

    // out << endl;
  }
  return out;
}

SNode::SNode(int value) : value{value} {}

// copy constructor
SkipList::SkipList(const SkipList &other)
{
  // cout << "constructor 2 called" << endl;
  this->maxLevel = other.maxLevel - 1;
  this->head = new SNode(0);

  for (int d = other.maxLevel - 1; d >= 0; d--)
  {
    SNode *curr = other.head->forward[d];
    SNode *thisCurr = this->head;

    if (curr->forward[d] != nullptr)
    {
      SNode *temp = new SNode(curr->value);
      thisCurr->forward.push_back(temp);
      curr = curr->forward[d];
      thisCurr = thisCurr->forward[d];
    }
    // cout << "before while loop: " << endl;
    while (curr != nullptr && curr->forward[d] != nullptr)
    {
      SNode *temp = new SNode(curr->value);
      thisCurr->forward.push_back(temp);
      curr = curr->forward[d];
      thisCurr = thisCurr->forward[d];
    }
    // cout << "after while loop" << endl;
    if (curr->forward[d] == nullptr)
    {
      thisCurr->forward.push_back(nullptr);
    }
  }
  this->probability = other.probability;
  // cout << "copy finished" << endl;
}

SkipList::SkipList(int maxLevel, int probability)
    : maxLevel{maxLevel}, probability{probability}
{
  assert(maxLevel > 0 && probability >= 0 && probability < 100);
  head = new SNode(INT_MIN);
  tail = new SNode(INT_MAX);
  for (int currLevel = 0; currLevel < maxLevel; currLevel++)
  {
    head->forward.push_back(tail);
    head->backward.push_back(nullptr);
    tail->backward.push_back(head);
    tail->forward.push_back(nullptr);
  }
}

bool SkipList::shouldInsertAtHigher() const
{
  return rand() % 100 < probability;
}

SNode *SkipList::getNode(int value) const
{
  SNode *temp = head;
  for (int currLevel = maxLevel - 1; currLevel >= 0; currLevel--)
  {
    // while (temp->forward[currLevel] != nullptr &&
    //        temp->forward[currLevel]->value <= value)
    // {
    // }
    while (temp->value != tail->value &&
           temp->forward[currLevel]->value <= value)
    {
      temp = temp->forward[currLevel];
      if (temp->value == value)
      {
        return temp;
      }
    }
  }
  return temp;
}

vector<SNode *> SkipList::getBeforeNodes(int value) const
{
  vector<SNode *> ret;
  SNode *temp = head;
  for (int currLevel = maxLevel - 1; currLevel >= 0; currLevel--)
  {
    while (temp->value != tail->value &&
           temp->forward[currLevel]->value <= value)
    {
      temp = temp->forward[currLevel];
    }
    ret.insert(ret.begin(), temp);
  }
  return ret;
}

bool SkipList::add(int value)
{
  if (value == head->value || value == tail->value)
  {
    return false;
  }
  int insertLevel = 0;
  auto insert = new SNode(value);
  vector<SNode *> vec = getBeforeNodes(value);
  if (vec[0]->value == value)
  {
    delete insert;
    return false;
  }
  do
  {
    SNode *keep = vec[insertLevel]->forward[insertLevel];
    insert->forward.push_back(vec[insertLevel]->forward[insertLevel]);
    vec[insertLevel]->forward[insertLevel] = insert;
    insert->backward.push_back(vec[insertLevel]);
    keep->backward[insertLevel] = insert;
    insertLevel++;
  } while (shouldInsertAtHigher() && insertLevel < maxLevel);
  return true;
}

bool SkipList::add(const vector<int> &values)
{
  bool ret = true;
  for (int value : values)
  {
    if (!add(value))
    {
      ret = false;
    }
  }
  return ret;
}

SkipList::~SkipList()
{
  // need to delete individual nodes
  SNode *grimReaper = head;
  while (grimReaper != nullptr)
  {
    SNode *next = grimReaper->forward[0];
    delete grimReaper;
    grimReaper = next;
  }
}

bool SkipList::remove(int data)
{
  SNode *temp = getNode(data);
  if (temp->value != data)
  {
    return false;
  }
  for (int currLevel = temp->forward.size() - 1; currLevel >= 0; currLevel--)
  {
    temp->forward[currLevel]->backward[currLevel] = temp->backward[currLevel];
    temp->backward[currLevel]->forward[currLevel] = temp->forward[currLevel];
  }
  delete temp;
  return true;
}

bool SkipList::contains(int data) const { return getNode(data)->value == data; }
