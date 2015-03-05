#include "QuadraticProbing.h"
#include <cstring>
/*

*/
/**
 * Internal method to test if a positive number is prime.
 * Not an efficient algorithm.
 */
template <class HashedObj>
bool QuadraticHashTable<HashedObj>::isPrime( int n ) const
{
    if( n == 2 || n == 3 )
        return true;
    
    if( n == 1 || n % 2 == 0 )
        return false;
    
    for( int i = 3; i * i <= n; i += 2 )
        if( n % i == 0 )
            return false;
    
    return true;
}

/**
 * Internal method to return a prime number at least as large as n.
 * Assumes n > 0.
 */
template <class HashedObj>
int QuadraticHashTable<HashedObj>::nextPrime( int n ) const
{
    if( n % 2 == 0 )
        n++;
    
    for( ; !isPrime( n ); n += 2 )
        ;
    
    return n;
}

/**
 * Construct the hash table.
 */
template <class HashedObj>
QuadraticHashTable<HashedObj>::QuadraticHashTable( const HashedObj & notFound, int size )
:  ITEM_NOT_FOUND( notFound )
{
    tableSize = nextPrime(size);
    array= new HashEntry[tableSize];
    
    
    
    makeEmpty( );
}



/**
 * Expand the hash table.
 */
template <class HashedObj>
void QuadraticHashTable<HashedObj>::rehash( )
{
    //            vector<HashEntry> oldArray = array;
    
    int oldSize = tableSize;
    HashEntry* oldArray;
    oldArray=array;
    int newSize= nextPrime( 2 * oldSize);
    tableSize = newSize;
    HashEntry* newArray=new HashEntry[newSize];
    
    array=newArray;
    // Create new double-sized, empty table
    //            array.resize( nextPrime( 2 * oldArray.size( ) ) );
    for( int j = 0; j < tableSize; j++ )
        //            for( int j = 0; j < array.size( ); j++ )
        array[ j ].info = EMPTY;
    
    // Copy table over
    currentSize = 0;
    //            for( int i = 0; i < oldArray.size( ); i++ )
    for( int i = 0; i < oldSize; i++ )
        if( oldArray[ i ].info == ACTIVE )
            insert( oldArray[ i ].element );
    delete [] oldArray;
    
}

/**
 * Make the hash table logically empty.
 */
template <class HashedObj>
void QuadraticHashTable<HashedObj>::makeEmpty( )
{
    currentSize = 0;
    //            for( int i = 0; i < array.size( ); i++ )
    for( int i = 0; i < tableSize; i++ )
        array[ i ].info = EMPTY;
}

/**
 * Deep copy.
 */
template <class HashedObj>
const QuadraticHashTable<HashedObj> & QuadraticHashTable<HashedObj>::
operator=( const QuadraticHashTable<HashedObj> & rhs )
{
    if( this != &rhs )
    {
        array = rhs.array;
        currentSize = rhs.currentSize;
    }
    return *this;
}


/**
 * Return true if currentPos exists and is active.
 */
template <class HashedObj>
bool QuadraticHashTable<HashedObj>::isActive( int currentPos ) const
{
    return array[ currentPos ].info == ACTIVE;
}

/**
 * A hash routine for string objects.
 */
template <class HashedObj>
int QuadraticHashTable<HashedObj>::hash( const string & key, int tableSize ) const
{
    int hashVal = 0;
    
    for( int i = 0; i < key.length( ); i++ )
        hashVal = 37 * hashVal + key[ i ];
    
    hashVal %= tableSize;
    if( hashVal < 0 )
        hashVal += tableSize;
    
    return hashVal;
}


/**
 * A hash routine for ints.
 */
template <class HashedObj>
int QuadraticHashTable<HashedObj>::hash( int key, int tableSize ) const
{
    if( key < 0 ) key = -key;
    return key % tableSize;
}







/**
 * Remove item x from the hash table.
 */
template <class HashedObj>
void QuadraticHashTable<HashedObj>::remove( Person2* x )
{
    int currentPos = findPos( x->me );
    if( isActive( currentPos ) )
        array[ currentPos ].info = DELETED;
}

/**
 * Find item x in the hash table.
 * Return the matching item, or ITEM_NOT_FOUND, if not found.
 */
template <class HashedObj>
const HashedObj & QuadraticHashTable<HashedObj>::find( Person & x )
{
    int currentPos = findPos( x );
    return isActive( currentPos ) ? array[ currentPos ].element : ITEM_NOT_FOUND;
}


/**
 * Insert item x into the hash table. If the item is
 * already present, then do nothing.
 */
template <class HashedObj>
void QuadraticHashTable<HashedObj>::insert( Person2* x )
{
    // Insert x as active
    int currentPos = findPos(x->me);
    if( isActive( currentPos ) )
        return;
    array[ currentPos ] = HashEntry( x, ACTIVE );
    x->ID = currentSize;
    
    // Rehash; see Section 5.5
    //    if( ++currentSize > array.size( ) / 2 )
    if( ++currentSize > tableSize / 2 + 1 )
        rehash( );
}


template <class HashedObj>
int QuadraticHashTable<HashedObj>::findPos( Person &  person )
{
    /* 1*/      int collisionNum = 0;
    //    /* 2*/      int currentPos = hash( person, array.size( ) );
    int currentPos = hash( person, tableSize );
    
    /* 3*/      while( array[ currentPos ].info != EMPTY &&
                      !personEqual(person, (array[ currentPos ].element)->me) )
    {
        /* 4*/          currentPos += 2 * ++collisionNum - 1;  // Compute ith probe
        //        /* 5*/          if( currentPos >= array.size( ) )
        if( currentPos >= tableSize )
            //        /* 6*/              currentPos -= array.size( );
            currentPos -= tableSize;
    }
    
    /* 7*/      return currentPos;
}


/**
 * A hash routine for Person2* .
 */
template <class HashedObj>
int QuadraticHashTable<HashedObj>::hash( Person & me, int tableSize )
{
    
    int hashVal = 0;
    hashVal = 37 * hashVal + hashArr(me.firstName);
    hashVal = 37 * hashVal + hashArr(me.lastName);
    hashVal = 37 * hashVal + me.gender;
    hashVal = 37 * hashVal + me.year;
    
    hashVal %= tableSize;
    if( hashVal < 0 )
        hashVal += tableSize;
    
    return hashVal;
    
}

template <class HashedObj>
bool QuadraticHashTable<HashedObj>::personEqual(Person & lhs, Person & rhs){
    if (strcmp(lhs.firstName, rhs.firstName) != 0 || strcmp(lhs.lastName, rhs.lastName) != 0 || lhs.gender != rhs.gender || lhs.year != rhs.year){
        return false;
    }
    return true;
}

template <class HashedObj>
int QuadraticHashTable<HashedObj>::hashArr(char* arr){
    int result = 0;
    for (int i = 0; arr[i]; i++){
        result = 37 * result + arr[i];
    }
    return result;
}

template <class HashedObj>
int QuadraticHashTable<HashedObj>::getSize(){
    return currentSize;
}