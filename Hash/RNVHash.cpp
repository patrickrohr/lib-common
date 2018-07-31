#include <cstdint>
#include <iostream> // Just for testing

/*!************************************************************
 * @brief      Class for constexpr RNV hashing strings.
 * @details    This can be incredibly useful if you want to use a string as an index in a switch / case statement,
 *             since std::hash<std::string> is not constexpr....
 * @requires   C++11
 * @date       July 30, 2018
 * @author     Patrick Rohr
**************************************************************/
class RNVHash
{
public:
    static constexpr uint32_t Hash(const char* strIn)
    {
        return Hash(strIn, g_uFNVOffsetBasis);
    }

private:
    static constexpr uint32_t Hash(const char* cstrIn, uint32_t uHash)
    {
        return (*cstrIn == '\0') ? uHash : Hash(cstrIn + 1, (uHash * g_uFNVPrime) ^ static_cast<uint32_t>(*cstrIn));
    }

    static constexpr uint32_t g_uFNVPrime = 16777619;
    static constexpr uint32_t g_uFNVOffsetBasis = 2166136261;
};


int main()
{
    switch(RNVHash::Hash("This is a test"))
    {
        case RNVHash::Hash("This is a test"):
            std::cout << "It works";
            break;
        default:
            std::cout << "It doesn't work";
    }

    return 0;
}