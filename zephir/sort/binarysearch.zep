namespace Sort;

class BinarySearch
{
    public static function make(const array! theArray, const int! target) -> bool
    {
        uint size = count(theArray);

        if (size <= 0) {
            return false;
        }
        if (size == 1) {
            return (target == theArray[0]);
        }

        int middleId = (int)(size / 2);
        int middleValue = (int)theArray[middleId];

        if (target == middleValue) {
            return true;
        }

        if (target < middleValue) {
            return self::make( array_slice(theArray, 0, middleId), target);
        }
        
        return self::make( array_slice(theArray, middleId), target);
    }


    public static function make2(const array theArray, const int start, const int end, const int target) -> bool
    {
        int size = end - start + 1;
        if (size <= 0) {
            return false;
        }
        if (size == 1) {
            return (target == theArray[start]);
        }

        int middleId = start + (int)(size / 2);
        int middleValue = (int)theArray[middleId];

        if (target == middleValue) {
            return true;
        }

        if (target < middleValue) {
            return self::make2(theArray, start, middleId - 1, target);
        }
        
        return self::make2(theArray, middleId + 1, end, target);
    }


    public static function make3(const array theArray, const int start, const int size, const int target) -> bool
    {
        if (size <= 0) {
            return false;
        }
        if (size == 1) {
            return (target == theArray[start]);
        }

        int middleId = start + (int)(size / 2);
        int middleValue = (int)theArray[middleId];

        if (target == middleValue) {
            return true;
        }

        if (target < middleValue) {
            return self::make3(theArray, start, middleId - start, target);
        }
        
        return self::make3(theArray, middleId + 1, size - (middleId - start + 1), target);
    }


    public static function make4(const array theArray, int start, int size, const int target) -> bool
    {
        while (size > 0) {
            if (size == 1) {
                return (target == theArray[start]);
            }

            int middleId = start + (int)(size / 2);
            int middleValue = (int)theArray[middleId];

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
