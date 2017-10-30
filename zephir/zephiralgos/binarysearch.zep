namespace ZephirAlgos;

class BinarySearch
{
    public static function simple(const array _array, const int target) -> bool
    {
        uint size = count(_array);

        if (size <= 0) {
            return false;
        }
        if (size == 1) {
            return (target == _array[0]);
        }

        int middleId = (int)(size / 2);
        int middleValue = (int)_array[middleId];

        if (target == middleValue) {
            return true;
        }

        if (target < middleValue) {
            return self::simple( array_slice(_array, 0, middleId), target);
        }
        
        return self::simple( array_slice(_array, middleId), target);
    }

    public static function opti(const array _array, int start, int size, const int target) -> bool
    {
        while (size > 0) {
            if (size == 1) {
                return (target == _array[start]);
            }

            int middleId = start + (int)(size / 2);
            int middleValue = (int)_array[middleId];

            if (target == middleValue) {
                return true;
            }

            if (target < middleValue) {
                let size = middleId - start;
            } else {
                let size -= (middleId - start + 1);
                let start = middleId + 1;
            }
        }

        return false;
    }
}
