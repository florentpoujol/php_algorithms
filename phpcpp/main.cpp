#include <phpcpp.h>

class BinarySearch : public Php::Base
{

public:
    /**
     *  C++ constructor and destructor
     */
    // BinarySearch() = default;
    // virtual ~BinarySearch() = default;

    /**
     * keep the array as a Php::Value
     * Same code as in the Zephir make3()
     * @param array
     * @param int startId
     * @param int endId
     * @param int target
     * @return int
     */
    static Php::Value opti_recursive(Php::Parameters &params) {
        return BinarySearch::_opti_recursive(params[0], params[1], params[2], params[3]);
    }

    static bool _opti_recursive(Php::Value array, int startId, int size, int target) {
        if (size <= 0) {
            return false;
        }
        if (size == 1) {
            return (target == (int)array[startId]);
        }

        int middleId = startId + (int)(size / 2);
        int middleValue = (int)array[middleId];

        if (target == middleValue) {
            return true;
        }

        if (target < middleValue) {
            return BinarySearch::_opti_recursive(array, startId, middleId - startId, target);
        }
        
        return BinarySearch::_opti_recursive(array, middleId + 1, size - (middleId - startId + 1), target);
    }


    static Php::Value opti(Php::Parameters &params) {
        Php::Value array = params[0]; 
        // the cast to a std::vector<int> takes more than 40 seconds for 4 millions elements
        int startId = params[1];
        int size = params[2];
        int target = params[3];

        while (size > 0) {
            if (size == 1) {
                return (target == (int)array[startId]);
            }

            int middleId = startId + (int)(size / 2);
            int middleValue = (int)array[middleId];
            
            if (target == middleValue) {
                return true;
            }

            if (target < middleValue) {
                size = middleId - startId;
            } else {
                size -= (middleId - startId + 1);
                startId = middleId + 1;
            }
        }

        return false;
    }
    
};
/*
class MergeSort : public Php::Base
{

public:
    static Php::Value simple(Php::Parameters &params) {
        return MergeSort::_simple(params[0]);
    }

    static Php::Value _simple(Php::Value left) {
        int size = call("count", left);
        if (size <= 1) {
            return left;
        }

        int middleId = (int)(size / 2); // the cast also floors the value when size is odd
       
        Php::Value right = call("array_splice", &left, middleId);
        
        left = MergeSort::_simple(left);
        right = MergeSort::_simple(right);

        Php::Value newArray;
        int leftId = 0;
        int leftSize = call("count", left);
        int rightId = 0;
        int rightSize = call("count", right);
        int arrayId = 0;

        while (leftId < leftSize && rightId < rightSize) {
            if (left[leftId] <= right[rightId]) {
                newArray[ arrayId++ ] = left[leftId];
                leftId++;
            } else {
                newArray[ arrayId++ ] = right[rightId];
                rightId++;
            }
        }
        
        return call("array_merge",
            newArray,
            call("array_slice", left, leftId),
            call("array_slice", right, rightId)
        );
    }


    static Php::Value opti(Php::Parameters &params) {
        return MergeSort::_opti(&params[0]);
    }

    static Php::Value _opti(Php::Value _array, int startId = 0, int size = -1) {
        if (size == -1) {
            size = call("count", _array);
        }
        if (size <= 1) {
            Php::Value array;
            array[0] = _array[startId];
            return array;
        }

        int middleId = startId + (int)(size / 2); // the cast also floors the value when size is odd
       
        // no need to compute again the size of the array, nor to call merge_sort() if the size is 1
        Php::Value left;
        left[0] = _array[startId];
        int leftSize = middleId - startId;
        if (leftSize > 1) {
            left = MergeSort::_opti(_array, startId, leftSize);
        }

        Php::Value right;
        right[0] = _array[middleId];
        int rightSize = size - (middleId - startId);
        if (rightSize > 1) {
            right = MergeSort::_opti(_array, middleId, rightSize);
        }

        // sort content of left and right
        Php::Value newArray;
        int leftId = 0;
        int rightId = 0;
        int arrayId = 0;
        int l, r;

        while (leftId < leftSize && rightId < rightSize) {
            l = left[leftId];
            r = right[rightId];
            if (l <= r) {
                newArray[ arrayId++ ] = l;
                leftId++;
            } else {
                newArray[ arrayId++ ] = r;
                rightId++;
            }
        }
        
        // add the remaining values from left and right in the new sorted array
        // we know that by that time, the remaining values in left are all smaller than the ones in right
        while (leftId < leftSize) {   
            newArray[ arrayId++ ] = left[leftId];
            leftId++;
        }
        while (rightId < rightSize) {   
            newArray[ arrayId++ ] = right[rightId];
            rightId++;
        }
        return newArray;
    }
    
};
*/

extern "C" {
    PHPCPP_EXPORT void *get_module() 
    {
        static Php::Extension extension("cppalgos", "1.0");

        Php::Class<BinarySearch> binarySearchDesc("CppAlgos\\BinarySearch");

        binarySearchDesc.method<&BinarySearch::opti_recursive>("opti_recursive", {
            Php::ByVal("array", Php::Type::Array),
            Php::ByVal("startId", Php::Type::Numeric),
            Php::ByVal("size", Php::Type::Numeric),
            Php::ByVal("target", Php::Type::Numeric)
        }); // static method

        binarySearchDesc.method<&BinarySearch::opti>("opti", {
            Php::ByVal("array", Php::Type::Array),
            Php::ByVal("startId", Php::Type::Numeric),
            Php::ByVal("size", Php::Type::Numeric),
            Php::ByVal("target", Php::Type::Numeric)
        }); // static method

        extension.add(std::move(binarySearchDesc));

        /*
        // merge sort ---------------
        Php::Class<MergeSort> mergeSortDesc("CppAlgos\\MergeSort");

        mergeSortDesc.method<&MergeSort::simple>("simple", {
            Php::ByVal("array", Php::Type::Array)
        }); // static method

        mergeSortDesc.method<&MergeSort::opti>("opti", {
            Php::ByVal("array", Php::Type::Array),
            Php::ByVal("startId", Php::Type::Numeric, false),
            Php::ByVal("size", Php::Type::Numeric, false)
        }); // static method

        extension.add(std::move(mergeSortDesc));*/

        return extension;
    }
}
