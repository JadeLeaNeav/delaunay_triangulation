#ifndef MYLIST_H
#define MYLIST_H
#include <QFuture>


class MyList
{
public:
    MyList();

    // add a point at the end of the list
    void addPoint(QPointF new_point);

    // sort the list using the merge sort
    void mergeSort();

    // return the k-th element of the list
    QPointF getPoint(int k);

    // return the minimal lexicographically point
    int minPoint();

    // delete the penultimate point of the list
    void deletePenultimate();

    // concatenate l2 at the end of the list
    void concatenate(MyList l2);

    // return the number of points of the list
    int getPointsNumber();


private:

    // merge two sorted sub arrays of tab
    // these arrays should be next to each other (start2 = end1 +1)
    // start1 : first index of the first sub array
    // start2 : first index of the second sub array
    // end1 : last index of the first sub array
    // end2 : last index of the second sub array
    void merge(int start1, int start2, int end1, int end2);

    // recursive function sorting an array from start index to end
    void mergeSortIndex(int start, int end);

    // constante used to modifiy the size of the memory available for the array
    static const int FIRST_SIZE = 100;

    // array of points
    QPointF * tab;

    // size of the memory available
    int current_size;

    // number of points in the array
    int points_number;

};

#endif // MYLIST_H
