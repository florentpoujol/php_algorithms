#include <phpcpp.h>

class CppAlgos : public Php::Base
{

public:
    /**
     *  C++ constructor and destructor
     */
    CppAlgos() = default;
    virtual ~CppAlgos() = default;

    /**
     * keep the array as a Php::Value
     * Same code as in the Zephir make3()
     * @param array
     * @param int startId
     * @param int endId
     * @param int target
     * @return int
     */
    static Php::Value binarySearch3(Php::Parameters &params) {
        return CppAlgos::_binarySearch3(params[0], params[1], params[2], params[3]);
    }

    static bool _binarySearch3(Php::Value array, int startId, int size, int target) {
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
            return CppAlgos::_binarySearch3(array, startId, middleId - startId, target);
        }
        
        return CppAlgos::_binarySearch3(array, middleId + 1, size - (middleId - startId + 1), target);
    }


    static Php::Value binarySearch4(Php::Parameters &params) {
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

        Php::Class<CppAlgos> algosDescriptor("CppAlgos");

        algosDescriptor.method<&CppAlgos::binarySearch3>("binarySearch3", {
            Php::ByVal("array", Php::Type::Array),
            Php::ByVal("startId", Php::Type::Numeric),
            Php::ByVal("size", Php::Type::Numeric),
            Php::ByVal("target", Php::Type::Numeric)
        }); // static method

        algosDescriptor.method<&CppAlgos::binarySearch4>("binarySearch4", {
            Php::ByVal("array", Php::Type::Array),
            Php::ByVal("startId", Php::Type::Numeric),
            Php::ByVal("size", Php::Type::Numeric),
            Php::ByVal("target", Php::Type::Numeric)
        }); // static method

        extension.add(std::move(algosDescriptor));

        return extension;
    }
}