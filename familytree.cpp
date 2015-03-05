// Author: Sean Davis

#include "familytree.h"

#include "QuadraticProbing.h"
#include <cstring>

const Person FamilyTree::NOBODY = {-1,"","",' '};

FamilyTree::FamilyTree(Family *families, int familyCount):personTable(new Person2(), familyCount * 20)
{
 //   CPUTimer ct;
 //   ct.reset();
    if (familyCount <= 0){
        return;
    }
    
    for (int i = 0; i < familyCount; i++) {
        Family family = families[i];
        Person me = family.person;
        personTable.insert(new Person2(me));
    }
    
    for (int i = 0; i < familyCount; i++) {
        Family family = families[i];
        if (family.spouse.year == -1){
            continue;
        }
        Person2* person = personTable.find(family.person);
        person->spouse = personTable.find(family.spouse);
        Person* children = family.children;
        for (int j = 0 ; j < family.childCount; j++){
            Person2* child = personTable.find(children[j]);
            if (person->me.gender == 'M'){
                child->dad = person;
            } else {
                child->mom = person;
            }
        }
    }
 //   cout<<"Create Family Tree: "<<ct.cur_CPUTime()<<endl;
    
    totalPeople = personTable.getSize() * 2;
    
        
//     
        

    
} // FamilyTree()

void FamilyTree::runQueries(Query *queries, Person *answers, int queryCount)
{
//    CPUTimer ct;
//    ct.reset();
    bool *personsAccessed = new bool[personTable.getSize()*2];
    Person2 **queue;
    queue = new Person2*[personTable.getSize()*2];
    for (int i = 0; i < queryCount; i++){
        Query query = queries[i];
        Person person1 = query.person1;
        Person person2 = query.person2;
        if (personEqual(person1, person2)){
            answers[i] = person1;
            continue;
        }
        Person2* person11 =personTable.find(person1);
        Person2* person22 =personTable.find(person2);
        
        memset(personsAccessed, 0, personTable.getSize() * sizeof(bool));
        
        findAllAncester1(person11, personsAccessed, queue);
        answers[i] = findAllAncester2(person22, personsAccessed, queue);
        
        
    }
//    cout<<"runQueries: "<<ct.cur_CPUTime()<<endl;
}  // runQueries()


Person2** FamilyTree::findAllAncester1(Person2* person, bool personsAccessed[], Person2 **ancestor){
    int head = 0;
    int tail = 0;
    Person2* front = 0;
    ancestor[tail++] = person;
    while(head != tail){
        front = ancestor[head++];
        personsAccessed[front->ID] = true;
        if (front->dad){
            ancestor[tail++] = front->dad;
        }
        if (front->mom){
            ancestor[tail++] = front->mom;
        }
    }
    
    
    return ancestor;
}


Person FamilyTree::findAllAncester2(Person2* person, bool personsAccessed[], Person2 **ancestor){
    int head = 0;
    int tail = 0;
    Person2* front = 0;

    int year = 0;
    Person2 *returnPerson = NULL;
    
    ancestor[tail++] = person;
    while(head != tail){
        front = ancestor[head++];
        if (personsAccessed[front->ID] && year < front->me.year)
        {
            returnPerson = front;
            year = front->me.year;
        }
        if (front->dad){
            ancestor[tail++] = front->dad;
        }
        if (front->mom){
            ancestor[tail++] = front->mom;
        }
    }
    
    if (returnPerson != NULL)
        return returnPerson->me;
    Person p;
    p.year = -1;
    return p;
}



Person FamilyTree::getLCA(Person2** ancestors1, Person2** ancestors2){
    if (!ancestors1 && !ancestors2){
        return NOBODY;
    }

    int i = 0;
    int j = 0;
    while (ancestors1[i] && ancestors2[j]) {
        Person2* p1 = ancestors1[i];
        Person2* p2 = ancestors2[j];
        if (personCmp(&p1, &p2) == 0) return p1->me;
        if (personCmp(&p1, &p2) < 0){
            i++;
        } else{
            j++;
        }
    }
    
    return NOBODY;
}


bool FamilyTree::personEqual(Person & lhs, Person & rhs){
    if (&lhs == 0 || &rhs == 0){
        return false;
    }
    if (strcmp(lhs.firstName, rhs.firstName) != 0 || strcmp(lhs.lastName, rhs.lastName) != 0 || lhs.gender != rhs.gender || lhs.year != rhs.year){
        return false;
    }
    return true;
}

int personCmp(const void* o1, const void* o2){
    Person p1 = (*(Person2**)o1)->me;
    Person p2 = (*(Person2**)o2)->me;
    if (p1.year < p2.year)  return 1;
    else if (p1.year > p2.year) return -1;
    else {
        int result1 = strcmp(p1.firstName, p2.firstName);
        if (result1 != 0)    return -result1;
        else {
            int result2 = strcmp(p1.lastName, p2.lastName);
            if (result2 != 0)   return -result2;
            else if (p1.gender == p2.gender)    return 0;
            else    return p1.gender < p2.gender;
        }
    }
}


