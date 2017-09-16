namespace Sort;

class QuickSort
{
    private static function make(const array! theArray, uint! size = 0) -> array
    {
        if size <= 0 {
            let size = count(theArray);
        }

        if size === 1 {
            return theArray;
        }

        // select pivot (median of three)
        int first = (int)theArray[0];
        int last = (int)theArray[size - 1];
        if first > last {
            int temp = first;
            let first = last;
            let last = temp;
        }

        if size === 2 {
            return [first, last];
        }

        int pivot = (int)theArray[ (int)(size/2) ];
        if pivot < first {
            let pivot = first;
        } elseif (pivot > last) {
            let pivot = last;
        }
        
        // moves values
        array left = [];
        array middles = [];
        array right = [];
        var value;
        for value in theArray { // a standard for loop is 50% slower 
            if (int)value < pivot {
                let left[] = value;
            } elseif value > pivot {
                let right[] = value;
            } else {
                let middles[] = value;
            }        
        }

        uint leftSize = count(left);
        if leftSize > 1 {
            let left = self::make(left, leftSize);
        }

        uint rightSize = count(right);
        if rightSize > 1 {
            let right = self::make(right, rightSize);
        }

        return array_merge(left, middles, right);
    }
}
