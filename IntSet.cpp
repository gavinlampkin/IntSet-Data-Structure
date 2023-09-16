// FILE: IntSet.cpp - header file for IntSet class
//       Implementation file for the IntStore class
//       (See IntSet.h for documentation.)
// INVARIANT for the IntSet class:
// (1) Distinct int values of the IntSet are stored in a 1-D,
//     dynamic array whose size is stored in member variable
//     capacity; the member variable data references the array.
// (2) The distinct int value with earliest membership is stored
//     in data[0], the distinct int value with the 2nd-earliest
//     membership is stored in data[1], and so on.
//     Note: No "prior membership" information is tracked; i.e.,
//           if an int value that was previously a member (but its
//           earlier membership ended due to removal) becomes a
//           member again, the timing of its membership (relative
//           to other existing members) is the same as if that int
//           value was never a member before.
//     Note: Re-introduction of an int value that is already an
//           existing member (such as through the add operation)
//           has no effect on the "membership timing" of that int
//           value.
// (4) The # of distinct int values the IntSet currently contains
//     is stored in the member variable used.
// (5) Except when the IntSet is empty (used == 0), ALL elements
//     of data from data[0] until data[used - 1] contain relevant
//     distinct int values; i.e., all relevant distinct int values
//     appear together (no "holes" among them) starting from the
//     beginning of the data array.
// (6) We DON'T care what is stored in any of the array elements
//     from data[used] through data[capacity - 1].
//     Note: This applies also when the IntSet is empry (used == 0)
//           in which case we DON'T care what is stored in any of
//           the data array elements.
//     Note: A distinct int value in the IntSet can be any of the
//           values an int can represent (from the most negative
//           through 0 to the most positive), so there is no
//           particular int value that can be used to indicate an
//           irrelevant value. But there's no need for such an
//           "indicator value" since all relevant distinct int
//           values appear together starting from the beginning of
//           the data array and used (if properly initialized and
//           maintained) should tell which elements of the data
//           array are actually relevant.
//
// DOCUMENTATION for private member (helper) function:
//   void resize(int new_capacity)
//     Pre:  (none)
//           Note: Recall that one of the things a constructor
//                 has to do is to make sure that the object
//                 created BEGINS to be consistent with the
//                 class invariant. Thus, resize() should not
//                 be used within constructors unless it is at
//                 a point where the class invariant has already
//                 been made to hold true.
//     Post: The capacity (size of the dynamic array) of the
//           invoking IntSet is changed to new_capacity...
//           ...EXCEPT when new_capacity would not allow the
//           invoking IntSet to preserve current contents (i.e.,
//           value for new_capacity is invalid or too low for the
//           IntSet to represent the existing collection),...
//           ...IN WHICH CASE the capacity of the invoking IntSet
//           is set to "the minimum that is needed" (which is the
//           same as "exactly what is needed") to preserve current
//           contents...
//           ...BUT if "exactly what is needed" is 0 (i.e. existing
//           collection is empty) then the capacity should be
//           further adjusted to 1 or DEFAULT_CAPACITY (since we
//           don't want to request dynamic arrays of size 0).
//           The collection represented by the invoking IntSet
//           remains unchanged.
//           If reallocation of dynamic array is unsuccessful, an
//           error message to the effect is displayed and the
//           program unconditionally terminated.

#include "IntSet.h"
#include <iostream>
#include <cassert>
using namespace std;

void IntSet::resize(int new_capacity)
{
    if(new_capacity <= 0) // Ensure the new capacity is an acceptable value.
        capacity = DEFAULT_CAPACITY;
    else if(new_capacity < used)
        capacity = used;
    else
        capacity = new_capacity;

    int* newData = new int[capacity]; // Dynamically allocate new array

    for(int i = 0;i < used;i++)
        newData[i] = data[i];

    delete[] data; // Delete old array
    data = newData; // Reassign invoking data array to newData array.
}

IntSet::IntSet(int initial_capacity) : capacity(initial_capacity), used(0)
{
    if(initial_capacity <= 0) // If the initial capacity passed is not
        capacity = DEFAULT_CAPACITY; // an acceptable value, we use the DEF_CAP.

    data = new int[capacity]; // Dynamically allocate memory of space capacity.
}

IntSet::IntSet(const IntSet& src) : capacity(src.capacity), used(src.used)
{
    data = new int[capacity];

    for(int i = 0;i < used; i++)
        data[i] = src.data[i]; // Copy data from the src up to used.
}

IntSet::~IntSet()
{
   delete[] data; // Deallocate memory
   data = NULL; // Ensure data is NULL after destructed.
}

IntSet& IntSet::operator=(const IntSet& rhs)
{
    // If object you pass is the same
    // return back the same object.
    if (this == &rhs)
        return *this;

    int* temp = new int[rhs.capacity]; // Temp dynamic array

    for (int i = 0; i < rhs.used; i++)
        temp[i] = rhs.data[i];

    delete [] data; // Deallocate original dynamic array.

    data = temp; // Reassign to temp dynamic array.
    capacity = rhs.capacity;
    used = rhs.used;

    return *this;
}

int IntSet::size() const
{
    return used; // Used is always updated when an
}                // an element is removed or added.

bool IntSet::isEmpty() const
{
    if(used == 0) // If the used index is zero, the data array is empty
        return true; // otherwise, it is not empty.
    else
        return false;
}

bool IntSet::contains(int anInt) const
{
    if(used > 0) // As long as we have elements in our IntSet
    {
        for(int i = 0;i < used; i++) // Traverse until the used index checking
        {                            // if the element exists within
            if(anInt == data[i])     // the invoking data array.
                return true;
        }
    }
    return false;
}
bool IntSet::isSubsetOf(const IntSet& otherIntSet) const
{
   if(isEmpty()) // If the invoking set is empty, it will
        return true; // always be a subset of otherIntSet.
   else
   {
       for(int i = 0;i < used; i++)
       {
           if(!otherIntSet.contains(data[i])) // If not every element of the
                return false; // invoking set is in otherIntSet, return false.
       }
       return true;
   }
}

void IntSet::DumpData(ostream& out) const
{  // already implemented ... DON'T change anything
    if (used > 0)
    {
        out << data[0];
        for (int i = 1; i < used; ++i)
            out << "  " << data[i];
    }
}

IntSet IntSet::unionWith(const IntSet& otherIntSet) const
{
   IntSet unionSet = (*this);
   int otherSize = otherIntSet.size(); // Safely store size

   for(int i = 0; i < otherSize; i++)
   {
       if(unionSet.contains(otherIntSet.data[i]) == false) // If both sets contain
            unionSet.add(otherIntSet.data[i]); // the value, add it to the union and return.
   }
   return unionSet;
}

IntSet IntSet::intersect(const IntSet& otherIntSet) const
{
    IntSet intersectSet = (*this);

    for(int i = 0; i < size(); i++) // Must use size of invoking set
    {                               // to reach every value of the initial set.
        if(otherIntSet.contains(data[i]) == false)
            intersectSet.remove(data[i]); // Remove element if it is
    }                                     // not contained in both sets.
    return intersectSet;
}

IntSet IntSet::subtract(const IntSet& otherIntSet) const
{
    IntSet subSet = (*this); // create a copy of the invoking intSet
    int otherSize = otherIntSet.size(); // using the operator= overloaded function.

    for(int i = 0; i < otherSize; i++)
    {
        if(subSet.contains(otherIntSet.data[i])) // if the copy contains an element
            subSet.remove(otherIntSet.data[i]); // from otherIntSet, remove from subSet.
    }
    return subSet;
}

void IntSet::reset()
{
    used = 0;
}

bool IntSet::add(int anInt)
{
    if(contains(anInt) == false)
    {
        if(used >= capacity)     // If the size is at capacity, resize
            resize(int(1.5 * capacity) + 1); // the entire array in resize().

        data[used] = anInt;
        used++; // Increment the used index to supplement the value added.
        return true;
    }
    return false;
}

bool IntSet::remove(int anInt)
{
    if(contains(anInt))
    {
        for(int i = 0; i < used; i++) // Traverse entire array up to used
        {                             // to try and find anInt.
            if(data[i] == anInt) // Side comment: if you could return the index of an element from
            {                    // the contains function, it would be much less expensive to remove.
                for(int j = i; j < used - 1; j++) // Move every element after anInt
                    data[j] = data[j + 1];        // back one index.
                used--;
                return true;
            }
        }
    }
    return false;
}

bool operator==(const IntSet& is1, const IntSet& is2)
{
    if(is1.size()!=is2.size()) // if they are not the same size,
        return false;          // they can not be logically equal.

    if(is1.isSubsetOf(is2) && is2.isSubsetOf(is1)) // if both sets are subsets
        return true;                               // of each other, they are equal.
    else
        return false;
}
