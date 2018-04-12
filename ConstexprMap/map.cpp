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
        key{_key},
        val{_val}
    {}

    const key_type key;
    const value_type val;
};


using size_type = unsigned;
template<typename K, typename V, size_type index>
struct const_map
{
    // using value_type = std::pair<K, V>;
    
    // static constexpr size_type size = sizeof...(Args);

    // static constexpr std::array<value_type, N> data;
    // static constexpr value_type data[size] = { Args... };
    
    using value_type = map_entry<K, V>;

    template<class ...Ts>
    constexpr const_map(value_type _data, Ts... params) :
        data{_data.key, _data.val},
        tail{params...}
    {
    }

    constexpr V find(const K& key) const
    {
        return data.key == key ? data.val : tail.find(key);
    }

    value_type data;
    const_map<K, V, index - 1> tail;
};

template<typename K, typename V>
struct const_map<K, V, 1>
{
    constexpr const_map(map_entry<K, V> _data) :
        data{_data}
    {}

    void compile_time_error()
    {
        // do nothing
    }

    constexpr V find(const K& key) const
    {
        return data.key == key ? data.val : throw std::out_of_range("key not found");
    }

    map_entry<K, V> data;
};


int main()
{
    const_map<int, int, 2> map{ map_entry<int, int>({1, 2}), map_entry<int, int>({2, 3})};
    std::cout << map.find(1) << std::endl;
}