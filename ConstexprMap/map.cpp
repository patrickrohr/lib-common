/**************************************************************
 *                                                            *
 *               Copyright (c) 2018 - Patrick Rohr            *
 *                      All Rights Reserved                   *
 *                                                            *
 *************************************************************/

#include <stdexcept>
#include <iostream>

template<typename K, typename V>
struct map_entry
{
    // Member Types
    using key_type = K;
    using value_type = V;
 
    // Member Functions
    constexpr map_entry(const K& _key, const V& _val) :
        key(_key),
        val(_val)
    {
    }

    const key_type key;
    const value_type val;
};

template<typename K, typename V, unsigned index>
struct const_map
{    
    using value_type = map_entry<K, V>;

    template<class ...Ts>
    constexpr const_map(value_type _data, Ts... params) :
        data(_data.key, _data.val),
        tail(params...)
    {
    }

    constexpr V find(const K& key) const
    {
        return data.key == key ? data.val : tail.find(key);
    }

    const value_type data;
    const const_map<K, V, index - 1> tail;
};

template<typename K, typename V>
struct const_map<K, V, 1>
{
    constexpr const_map(map_entry<K, V> _data) :
        data{_data}
    {
    }
    
    constexpr V find(const K& key) const noexcept
    {
        return data.key == key ? data.val : 0; // throw 0; // Somehow this is not working with clang...TODO: fail
    }

    const map_entry<K, V> data;
};


int main()
{
    map_entry<int, int>(1, 2);


    constexpr const_map<int, int, 1> map(map_entry<int, int>(1, 2));
    std::integral_constant<int, map.find(2)> c;
    std::integral_constant<int, map.find(3)> c2;
}