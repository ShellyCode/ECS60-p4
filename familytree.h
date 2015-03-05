#ifndef FAMILYTREE_H
#define	FAMILYTREE_H

#include "familyRunner.h"
#include "QuadraticProbing.h"
#include <cstdlib>

using namespace std;

class FamilyTree {
private:
    QuadraticHashTable<Person2*> personTable;
    Person2** findAllAncester1(Person2* person, bool personsAccessed[], Person2 **queue);
    Person findAllAncester2(Person2* person, bool personsAccessed[], Person2 **queue);
    Person getLCA(Person2** ancestors1, Person2** ancestors2);
    bool personEqual(Person & lhs, Person & rhs);
public:
  FamilyTree(Family *families, int familyCount);
  void runQueries(Query *queries, Person *answers, int queryCount);
    static const Person NOBODY;
    int totalPeople;

};

int personCmp(const void* p1, const void* p2);

#endif	/* FAMILYTREE_H */

