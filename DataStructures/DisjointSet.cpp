/**************************************************************
 *                                                            *
 *               Copyright (c) 2019 - Patrick Rohr            *
 *                      All Rights Reserved                   *
 *                                                            *
 *************************************************************/

#include <unordered_map>
#include <vector>

#include <iostream> // for testing

/*!************************************************************
 * @brief      Disjoint set supporting union by rank and path compression.
 * @tparam     Value Type
 * @date       March 3, 2019
 * @author     Patrick Rohr
**************************************************************/
template<typename T>
class DisjointSet
{
public:
    using value_type = T;

private:
    struct Node
    {
        size_t parent;
        size_t rank;

        Node(size_t parent) : parent(parent), rank(0)
        {}
    };

public:
    DisjointSet() :
        m_setData(),
        m_valueMap()
    {}

    /*!************************************************************
     * @brief      Inserts a new value into disjoint set.
     *             Values are unique.
     * @param[in]  obj   The object
     * @date       March 3, 2019
     * @author     Patrick Rohr
    **************************************************************/
    void Insert(const value_type& obj)
    {
        if (!Contains(obj))
        {
            auto index = m_setData.size();
            m_setData.emplace_back(index);
            m_valueMap[obj] = index;
        }
    }

    /*!************************************************************
     * @brief      Checks if the set already contains a specified value.
     * @param[in]  obj   The object
     * @return     { description_of_the_return_value }
     * @date       March 3, 2019
     * @author     Patrick Rohr
    **************************************************************/
    bool Contains(const value_type& obj)
    {
        return m_valueMap.count(obj);
    }

    /*!************************************************************
     * @brief      Performs a union operation.
     * @param[in]  a     { parameter_description }
     * @param[in]  b     { parameter_description }
     * @date       March 3, 2019
     * @author     Patrick Rohr
    **************************************************************/
    void Union(const value_type& a, const value_type& b)
    {
        auto itA = FindRoot(a);
        auto itB = FindRoot(b);


        if (itA->rank > itB->rank)
        {
            itB->parent = std::distance(m_setData.begin(), itA);
        }
        else
        {
            itA->parent = std::distance(m_setData.begin(), itB);
            if (itA->rank == itB->rank)
            {
                ++itB->rank;
            }
        }
    }

    /*!************************************************************
     * @brief      Checks if both elements are part of the same set.
     * @param[in]  a     { parameter_description }
     * @param[in]  b     { parameter_description }
     * @return     True if both elements are part of the same set; false otherwise.
     * @date       March 3, 2019
     * @author     Patrick Rohr
    **************************************************************/
    bool Find(const value_type& a, const value_type& b)
    {
        return FindRoot(a) == FindRoot(b);
    }

private:
    /*!************************************************************
     * @brief      Convenience wrapper of FindRoot taking in value type and returning an iterator.
     * @param[in]  obj   The object
     * @return     Root node iterator
     * @date       March 3, 2019
     * @author     Patrick Rohr
    **************************************************************/
    typename std::vector<Node>::iterator FindRoot(const value_type& obj)
    {
        auto it = m_setData.begin();
        std::advance(it, FindRoot(m_valueMap.at(obj)));
        return it;
    }

    /*!************************************************************
     * @brief      Finds the root node of any given node and performs path compression.
     * @param[in]  index  The index
     * @return     Root node index
     * @date       March 3, 2019
     * @author     Patrick Rohr
    **************************************************************/
    size_t FindRoot(size_t index)
    {
        auto& el = m_setData.at(index);
        if (index != el.parent)
        {
            el.parent = FindRoot(el.parent);
        }
        return el.parent;
    }

private:
    std::vector<Node> m_setData;
    std::unordered_map<value_type, size_t> m_valueMap;
};


int main()
{
    DisjointSet<int> set;

    set.Insert(55);
    set.Insert(59);
    set.Insert(51);
    set.Insert(52);
    set.Insert(101);
    set.Insert(107);

    set.Union(55, 59);
    set.Union(51, 52);
    set.Union(52, 55);

    set.Union(101, 107);

    std::cout << set.Find(59, 52) << std::endl;
    std::cout << set.Contains(62) << std::endl;

    return 0;
}
