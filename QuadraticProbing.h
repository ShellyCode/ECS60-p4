        #ifndef _QUADRATIC_PROBING_H_
        #define _QUADRATIC_PROBING_H_

//        #include "vector.h"
        #include "mystring.h"
#include "familyRunner.h"


    class Person2
    {
    public:
        Person me;
        Person2 *spouse;
        Person2 *dad;
        Person2 *mom;
        int ID;
    
        Person2(){
            me.year = -1;
            ID = -1;
        }
    
    
    Person2(Person& it){
        me = it;
        dad = 0;
        mom = 0;
        }
    
    }; // class Person2

        // QuadraticProbing Hash table class
        //
        // CONSTRUCTION: an initialization for ITEM_NOT_FOUND
        //               and an approximate initial size or default of 101
        //
        // ******************PUBLIC OPERATIONS*********************
        // void insert( x )       --> Insert x
        // void remove( x )       --> Remove x
        // Hashable find( x )     --> Return item that matches x
        // void makeEmpty( )      --> Remove all items
        // int hash( String str, int tableSize )
        //                        --> Static method to hash strings

        template <class HashedObj>
        class QuadraticHashTable
        {
          public:
            explicit QuadraticHashTable( const HashedObj & notFound, int size = 500000 );
            QuadraticHashTable( const QuadraticHashTable & rhs )
              : ITEM_NOT_FOUND( rhs.ITEM_NOT_FOUND ),
                array( rhs.array ), currentSize( rhs.currentSize ) { }

//            const HashedObj & find( const HashedObj & x ) const;

            void makeEmpty( );
//            void insert( const HashedObj & x );
//            void remove( const HashedObj & x );
            
            void insert( Person2* x );
            void remove( Person2* x );
            const HashedObj & find( Person & x );

            const QuadraticHashTable & operator=( const QuadraticHashTable & rhs );
            
            int getSize();

            enum EntryType { ACTIVE, EMPTY, DELETED };
          private:
            
            struct HashEntry
            {
                HashedObj element;
                EntryType info;

                HashEntry( const HashedObj & e = HashedObj( ), EntryType i = EMPTY )
                  : element( e ), info( i ) { }
            };
//----vector
//            vector<HashEntry> array;
            HashEntry *array;
            int tableSize;
            int currentSize;
            
            
            const HashedObj ITEM_NOT_FOUND;
            bool isPrime( int n ) const;
            int nextPrime( int n ) const;
            bool isActive( int currentPos ) const;
            //int findPos( const HashedObj & x ) const;
            int hash( const string & key, int tableSize ) const;
            int hash( int key, int tableSize ) const;
            void rehash( );
            
            int findPos(Person & person);
            int hash( Person & key, int tableSize );
            
            bool personEqual(Person & lhs, Person & rhs);
            int hashArr(char* arr);
        };

        #include "QuadraticProbing.cpp"
        #endif
