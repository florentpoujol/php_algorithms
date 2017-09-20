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
}
