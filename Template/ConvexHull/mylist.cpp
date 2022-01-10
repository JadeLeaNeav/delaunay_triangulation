#include "mylist.h"

MyList::MyList()
{
    // allocation of memory
    this->tab = (QPointF * ) malloc(MyList::FIRST_SIZE * sizeof(QPointF));

    // no element in the array
    this->points_number = 0;

    this->current_size = MyList::FIRST_SIZE;
}


// add a point at the end of the list
void MyList::addPoint(QPointF new_point) {

    // reallocate memory if the array is full
    if (this->points_number == this->current_size) {

        int new_size = this->current_size + MyList::FIRST_SIZE;
        QPointF *new_tab = (QPointF*) realloc (this->tab, new_size * sizeof(QPointF));

            if (new_tab != NULL) {

              this->tab = new_tab;

              // modificate the size of the memory available
              this->current_size = new_size;

            }

            // if realloc did not work
            else {
              free (new_tab);
              puts ("Error (re)allocating memory");
              exit (1);
            }
    }

    // add the point at this end
    this->tab[this->points_number] = new_point;
    this->points_number++;
}

// return the k-th element of the list
QPointF MyList::getPoint(int k){

    if ( (0 <= k) && (k < this->points_number)) {
        return this->tab[k];
    } else {
        perror("Index out of range");
        exit (1);
    }
}

// return the number of points in the array
int MyList::getPointsNumber() {
    return this->points_number;
}


// delete the penultimate point of the list
void MyList::deletePenultimate(){
    int n = this->points_number;

    // need at last two points
    if (n > 2) {

        // decrementing the number of points
        this->points_number = n -1;

        // rewritting the last point on the penultimate point
        this->tab[n -2] =  this->tab[n -1];
    }
}

// concatenate l2 at the end of the list
void MyList::concatenate(MyList l2) {
    int n2 = l2.points_number;
    QPointF new_point;
    for (int k = 0 ; k < n2 ; k++ ) {
        new_point = l2.tab[k];
        this->addPoint(new_point);
    }
}


// return the minimal lexicographically point
int MyList::minPoint() {
    if (this->points_number > 0) {
        QPointF current_min = this->tab[0];
        float xmin = current_min.x();
        float ymin = current_min.y();
        int index = 0;
        QPointF current_point;
        for (int k = 1 ; k < this->points_number ; k++) {
            current_point = this->tab[k];
            if (current_point.y() <= ymin) {
                    current_min = current_point;
                    xmin = current_min.x();
                    ymin = current_min.y();
                    index = k;
             } else if (current_point.y() == ymin) {
                if (current_point.x() < xmin) {
                    current_min = current_point;
                    xmin = current_min.x();
                    ymin = current_min.y();
                    index = k;
                }
            }
        }
        return index;
    } else {
        perror("Liste vide, pas de min");
        exit (1);
    }
}

void MyList::merge(int start1, int start2, int end1, int end2) {
    int k1 = start1;
    int k2 = start2;
    int k = 0;
    QPointF * new_tab = (QPointF *) malloc((end2 - start1 + 1) * sizeof(QPointF));
    while (k1 <= end1 || k2 <= end2 ) {

        // we already put all the elements of tab1
        if (k1 > end1) {
            new_tab[k] = this->tab[k2];
            qDebug() << new_tab[k];

            k2++;
            k++;


        // we already put all the elements of tab2
        } else if (k2 > end2) {
            new_tab[k] = this->tab[k1];
            qDebug() << new_tab[k];
            k1++;
            k++;

        // the next element of tab1 is smaller than the next element of tab2
        } else if (this->tab[k1].x() < this->tab[k2].x()) {
            new_tab[k] = this->tab[k1];
            qDebug() << new_tab[k];
            k1++;
            k++;

        // the next element of tab1 and the next element of tab2 have the same x
        } else if (this->tab[k1].x() < this->tab[k2].x()) {

            // choosing the highest
            if (this->tab[k1].y() >= this->tab[k2].y()) {
            new_tab[k] = this->tab[k1];
            qDebug() << new_tab[k];
            k1++;
            k++;
            } else {
                new_tab[k] = this->tab[k2];
                qDebug() << new_tab[k];
                k2++;
                k++;

            }

        // the next element of tab2 is smaller than the next element of tab1
        }  else {

            qDebug() << new_tab[k];
            new_tab[k] = this->tab[k2];

            k2++;
            k++;
        }

    }
    qDebug() << "new tab";
    for (int i = 0 ; i < k ; i++) {
        this->tab[start1 + i] = new_tab[i];
        qDebug() << new_tab[i];
    }

    free(new_tab);

}
// sort the list using the fusion sort
void MyList::mergeSort(){
    int n = this->points_number;
    if (n > 1) {
        mergeSortIndex(0, n - 1);
    }


}

void MyList::mergeSortIndex(int start, int end){
    if (end != start) {
        int middle = (start + end) / 2;
        qDebug() << "debut 1 ";
        qDebug() << start;
        qDebug() << middle;
        qDebug() << "\n";
        mergeSortIndex(start, middle);
        qDebug() << "debut 2 ";
        qDebug() << middle+1;
        qDebug() << end;
        qDebug() << "\n";
        mergeSortIndex(middle +1, end);
        qDebug() << "merge ";
        qDebug() << start;
        qDebug() << end;
        qDebug() << "\n";
        merge(start, middle +1, middle, end);
    }
}
