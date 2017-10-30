namespace ZephirAlgos;

class MergeSort
{
	public static function simple(array! left) -> array
    {
        uint size = count(left);
        if size <= 1 {
            return left;
        }

        int middleId = (int)(size / 2); // the cast also floors the value when size is odd
       
        array right = (array)array_splice(left, middleId);
        
        let left = self::simple(left);
        let right = self::simple(right);

        array newArray = [];
        int leftId = 0;
        int leftSize = count(left);
        int rightId = 0;
        int rightSize = count(right);

        while (leftId < leftSize && rightId < rightSize) {
            if (left[leftId] <= right[rightId]) {
                let newArray[] = left[leftId];
                let leftId++;
            } else {
                let newArray[] = right[rightId];
                let rightId++;
            }
        }
        
        return array_merge(
            newArray,
            array_slice(left, leftId),
            array_slice(right, rightId)
        );
    }

    public static function opti(array! _array, int startId = 0, int size = -1) -> array
    {
        if (size == -1) {
            let size = count(_array);
        }
        if size <= 1 {
            return [_array[startId]];
        }

        int middleId = startId + (int)(size / 2); // the cast also floors the value when size is odd
       
        // no need to compute again the size of the array, nor to call merge_sort() if the size is 1
        array left = [_array[startId]];
        uint leftSize = middleId - startId;
        if leftSize > 1 {
            let left = (array) self::opti(_array, startId, leftSize);
        }

        array right = [_array[middleId]];
        uint rightSize = size - (middleId - startId);
        if rightSize > 1 {
            let right = (array) self::opti(_array, middleId, rightSize);
        }

        // sort content of left and right
        array newArray = [];
        int leftId = 0;
        int rightId = 0;
        int l, r;

        while leftId < leftSize && rightId < rightSize {
            let l = (int)left[leftId];
            let r = (int)right[rightId];
            if l <= r {
                let newArray[] = l;
                let leftId++;
            } else {
                let newArray[] = r;
                let rightId++;
            }
        }
        
        // add the remaining values from left and right in the new sorted array
        // we know that by that time, the remaining values in left are all smaller than the ones in right
        while leftId < leftSize {   
            let newArray[] = left[leftId];
            let leftId++;
        }
        while rightId < rightSize {   
            let newArray[] = right[rightId];
            let rightId++;
        }
        return newArray;
    }
}
