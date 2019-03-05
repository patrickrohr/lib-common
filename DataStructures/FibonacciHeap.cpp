#include <algorithm>
#include <memory>
#include <list>
#include <unordered_map>

// For testing
#include <cstdlib>
#include <ctime>
#include <iostream> 
#include <vector>

/*!************************************************************
 * @brief      Class for fibonacci heap.
 * @details    The min pointer is always kept at the front of the rootList.
 *             This way, we don't have to keep around another variable and management gets a lot simpler.
 * @tparam     IdT   { description }
 * @tparam     KeyT  { description }
 * @date       March 4, 2019
 * @author     Patrick Rohr
**************************************************************/
template<typename KeyT, typename Comp = std::less<KeyT>>
class FibonacciHeap
{
private:
    struct Node
    {
        Node(KeyT _key) : key(std::move(_key)), parent(), children(), isLoser()
        {}

        KeyT key; // used for sorting
        std::weak_ptr<Node> parent;
        std::list<std::shared_ptr<Node>> children;
        bool isLoser;
    };

public:
    // Types
    using key_type = KeyT;
    using node_ref = std::weak_ptr<Node>;

public:
    FibonacciHeap() :
        m_rootList()
    {}

    node_ref Push(key_type key)
    {
        auto node = std::make_shared<Node>(std::move(key));
        auto it = m_rootList.insert(m_rootList.end(), node);
        UpdateFront(it);

        return node;
    }

    key_type Front() const
    {
        return m_rootList.front()->key;
    }

    void Pop()
    {
        // Promote node's children
        for (auto& child : m_rootList.front()->children)
        {
            // save (and more efficient) to be moved
            m_rootList.push_back(std::move(child));
        }
        // Pop Min
        m_rootList.pop_front();

        // Consolidate
        Consolidate();
    }

    bool Empty() const
    {
        return m_rootList.empty();
    }

    void DecreaseKey(const node_ref& ref, key_type key)
    {
        auto node = ref.lock();
        if (!node || Comp()(node->key, key))
        {
            // DecreaseKey may not be a good name for this method.
            // This FibonacciHeap supports std::greater sorting, and in this case,
            // this method should be called IncreaseKey.
            // These errors are probably best to ignore.
            return;
        }

        node->key = std::move(key);

        auto parent = node->parent.lock();
        if (parent && Comp()(node->key, parent->key))
        {
            bool wasLoser = true;
            while (parent && wasLoser)
            {

                // we have work to do
                // First remove node from parents child list
                parent->children.remove(node);
                bool wasLoser = parent->isLoser;
                parent->isLoser = true;

                // Then move node to rootlist and clear isLoser flag
                node->isLoser = false;
                node->parent.reset();
                auto it = m_rootList.insert(m_rootList.end(), node);
                UpdateFront(it);

                node = parent;
                parent = node->parent.lock();
            }

            // Check if node is in root list
            if (!parent)
            {
                node->isLoser = false;
            }
        }
        else if (Comp()(node->key, m_rootList.front()->key))
        {
            auto it = std::find_if(m_rootList.begin(), m_rootList.end(), [&node](const std::shared_ptr<Node>& other) {
                return !Comp()(node->key, other->key) && !Comp()(other->key, node->key);
            });

            if (it != m_rootList.end())
            {
                UpdateFront(it);
            }
            else
            {
                // should never happen
                throw std::runtime_error("Invalid item. Are parent pointers corrupt?");
            }
        }
    }

private:
    void UpdateFront(typename std::list<std::shared_ptr<Node>>::iterator it)
    {
        if (m_rootList.cbegin() == it)
        {
            // it already is in the front.
            // since we are inserting in the back, this should only happen for the first element.
            return;
        }

        if(Comp()((*it)->key, m_rootList.front()->key))
        {
            m_rootList.splice(m_rootList.begin(), m_rootList, it);
        }
    }

    void Consolidate()
    {
        std::unordered_map<size_t, std::shared_ptr<Node>> buckets;

        // Merge all root nodes with an equal amount of children
        while (!m_rootList.empty())
        {
            auto rootNode = m_rootList.front();
            m_rootList.pop_front();

            while (true)
            {
                size_t degree = rootNode->children.size();
                auto& bucketNode = buckets[degree];
                if (!bucketNode)
                {
                    // found an empty bucket, sets rootNode to nullptr and exits the loop
                    bucketNode.swap(rootNode);
                    break;
                }
                else
                {
                    // Merge these two elements
                    if (Comp()(bucketNode->key, rootNode->key))
                    {
                        bucketNode->children.push_back(rootNode);
                        rootNode->parent = bucketNode;
                        rootNode.swap(bucketNode);
                    }
                    else
                    {
                        rootNode->children.push_back(bucketNode);
                        bucketNode->parent = rootNode;
                    }
                    // Remove the node from the bucket entirely
                    bucketNode.reset();
                    buckets.erase(degree);
                }
            }
        }

        // Dump buckets back into root node list and update front value
        for (const auto& item : buckets)
        {
            auto it = m_rootList.insert(m_rootList.end(), item.second);
            UpdateFront(it);
        }
    }

private:
    std::list<std::shared_ptr<Node>> m_rootList;
};


int main()
{
    FibonacciHeap<int> heap;
    for (int i = -500; i < -100; ++i)
    {
        heap.Push(i);
    }
    std::vector<decltype(heap)::node_ref> vRef;
    for (int i = 0; i > -100; --i)
    {
        vRef.push_back(heap.Push(i));
    }

    std::srand(std::time(nullptr));
    for (int i = 0; i < 100; ++i)
    {
        if (std::rand() % 2 == 0)
        {
            heap.Push(i);
        }
        else
        {
            heap.Pop();
        }
    }

    std::cout << heap.Front() << std::endl;

    for (const auto& item : vRef)
    {
        if (std::rand() % 3 == 0)
        {
            heap.DecreaseKey(item, -300);
        }
    }
    for (int i = 0; i < 10; ++i)
    {
        heap.Pop();
    }


    for (const auto& item : vRef)
    {
        if (std::rand() % 7 == 0)
        {
            heap.DecreaseKey(item, -300);
        }
    }

    while (!heap.Empty())
    {
        std::cout << heap.Front() << std::endl;
        heap.Pop();
    }

    return 0;
}
