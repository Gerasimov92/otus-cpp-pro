#pragma once

#include <memory>

template<class T>
struct ForwardListNode
{
    ForwardListNode()
    {}

    ForwardListNode(const T &value) :
        value(value)
    {}

    T value;
    ForwardListNode *next = nullptr;
};

template<class T>
struct ForwardListIterator
{
    using _self = ForwardListIterator<T>;

    using value_type = T;
    using pointer = T*;
    using reference = T&;
    using difference_type = std::ptrdiff_t;

    ForwardListIterator() :
        m_node()
    {}

    ForwardListIterator(ForwardListNode<T> *node) :
        m_node(node)
    {}

    reference operator*() const
    {
        return m_node->value;
    }

    pointer operator->() const
    {
        return &m_node->value;
    }

    _self &operator++()
    {
        m_node = m_node->next;
        return *this;
    }

    _self operator++(int)
    {
        _self tmp(*this);
        m_node = m_node->next;
        return tmp;
    }

    friend bool operator==(const _self &x, const _self &y)
    {
        return x.m_node == y.m_node;
    }

    friend bool operator!=(const _self &x, const _self &y)
    {
        return x.m_node != y.m_node;
    }

    ForwardListNode<T> *m_node;
};

template<class T>
struct ForwardListConstIterator
{
    using _self = ForwardListConstIterator<T>;

    using value_type = T;
    using pointer = const T*;
    using reference = const T&;
    using difference_type = std::ptrdiff_t;

    ForwardListConstIterator() :
        m_node()
    {}

    ForwardListConstIterator(const ForwardListNode<T> *node) :
        m_node(node)
    {}

    reference operator*() const
    {
        return m_node->value;
    }

    pointer operator->() const
    {
        return &m_node->value;
    }

    _self &operator++()
    {
        m_node = m_node->next;
        return *this;
    }

    _self operator++(int)
    {
        _self tmp(*this);
        m_node = m_node->next;
        return tmp;
    }

    friend bool operator==(const _self &x, const _self &y)
    {
        return x.m_node == y.m_node;
    }

    friend bool operator!=(const _self &x, const _self &y)
    {
        return x.m_node != y.m_node;
    }

    const ForwardListNode<T> *m_node;
};

template<class T, class Allocator = std::allocator<T>>
class ForwardList
{
public:
    using value_type = T;
    using reference = value_type&;
    using iterator = ForwardListIterator<T>;
    using const_iterator = ForwardListConstIterator<T>;

    ForwardList() = default;
    ForwardList(const ForwardList &other)
    {
        m_nodeAlloc = ForwardList::NodeAllocTraits::select_on_container_copy_construction(other.m_nodeAlloc);

        NodeType *currNode;

        for (const auto &value : other)
        {
            if (m_head == nullptr)
            {
                m_head = createNode(value);
                currNode = m_head;
            }
            else
            {
                NodeType *node = createNode(value);
                currNode->next = node;
                currNode = node;
            }
        }
    }

    reference front()
    {
        return m_head->value;
    }

    void push_front(const T &value)
    {
        NodeType *node = createNode(value);
        node->next = m_head;
        m_head = node;
    }

    void pop_front()
    {
        NodeType *node = m_head;
        m_head = node->next;
        destroyNode(node);
    }

    void clear()
    {
        while (!empty())
            pop_front();
    }

    bool empty() const
    {
        return m_head == nullptr;
    }

    iterator begin()
    {
        return iterator(m_head);
    }

    const_iterator begin() const
    {
        return const_iterator(m_head);
    }

    const_iterator cbegin() const
    {
        return const_iterator(m_head);
    }

    iterator end()
    {
        return iterator(nullptr);
    }

    const_iterator end() const
    {
        return const_iterator(nullptr);
    }

    const_iterator cend() const
    {
        return const_iterator(nullptr);
    }

private:
    using NodeType = ForwardListNode<T>;
    using NodeAllocType = std::__alloc_rebind<Allocator, NodeType>;
    using NodeAllocTraits = std::allocator_traits<NodeAllocType>;

    NodeType *m_head = nullptr;
    NodeAllocType m_nodeAlloc;

    NodeType *createNode(const T &value)
    {
        NodeType *ptr = NodeAllocTraits::allocate(m_nodeAlloc, 1);
        NodeAllocTraits::construct(m_nodeAlloc, ptr, value);
        return ptr;
    }

    void destroyNode(NodeType *node)
    {
        NodeAllocTraits::destroy(m_nodeAlloc, node);
        NodeAllocTraits::deallocate(m_nodeAlloc, node, 1);
    }
};
