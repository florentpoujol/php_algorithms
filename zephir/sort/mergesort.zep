namespace Sort;

class MergeSort
{
	public static function make(array! left, uint! size = 0) -> array
    {
        if size <= 0 {
            let size = count(left);
        }

        if size === 1 {
            return left;
        }

        int middleId = (int)(size / 2); // the cast also floors the value when size is odd
       
        // get all the elements to the right of middleId (inclusive)
        array right = (array)array_splice(left, middleId);
        
        // no need to compute again the size of the array, nor to call merge_sort() if the size is 1
        uint leftSize = middleId;
        if leftSize > 1 {
            let left = self::make(left, leftSize);
        }

        uint rightSize = size - middleId;
        if rightSize > 1 {
            let right = self::make(right, rightSize);
        }

        // sort content of left and right
        array newArray = [];
        int leftId = 0;
        int rightId = 0;

        while leftId < leftSize && rightId < rightSize {
            if left[leftId] <= right[rightId] {
                let newArray[] = left[leftId];
                let leftId++;
            } else {
                let newArray[] = right[rightId];
                let rightId++;
            }
        }
        
        // add the remaining values from left and right in the new sorted array
        // we know that by that time, the remaining values in left are all smaller than the ones in right
        int i = leftId;
        while i < leftSize {   
            let newArray[] = left[i];
            let i++;
        }
        let i = rightId;
        while i < rightSize {   
            let newArray[] = right[i];
            let i++;
        }
        return newArray;
    }
}
