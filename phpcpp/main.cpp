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

        return extension;
    }
}