#pragma once

#include <algorithm>
#include <list>

template<typename T, T DefaultValue>
class Matrix
{
private:
    using Element = std::tuple<size_t, size_t, T>;

public:
    using iterator = typename std::list<Element>::iterator;
    using const_iterator = typename std::list<Element>::const_iterator;

    class ValueProxy
    {
    public:
        ValueProxy(Matrix &m, size_t row, size_t column);
        ValueProxy &operator=(const T &other);
        operator T() const;
    private:
        Matrix &m_matrix;
        size_t m_row;
        size_t m_column;
    };

    class Row
    {
    public:
        Row(Matrix &m, size_t rowIndex);
        ValueProxy operator[](size_t columnIndex);
    private:
        Matrix &m_matrix;
        size_t m_rowIndex;
    };

    size_t size() const { return m_data.size(); };
    Row operator[](size_t index) { return Row(*this, index); };

    iterator begin() { return m_data.begin(); }
    const_iterator begin() const { return m_data.begin(); }
    const_iterator cbegin() const { return m_data.cbegin(); }
    iterator end() { return m_data.end(); }
    const_iterator end() const { return m_data.end(); }
    const_iterator cend() const { return m_data.cend(); }

private:
    auto findElement(size_t rowIndex, size_t columnIndex);

    friend ValueProxy;
    std::list<Element> m_data;
};

template<typename T, T DefaultValue>
auto Matrix<T, DefaultValue>::findElement(size_t rowIndex, size_t columnIndex)
{
    return std::find_if(m_data.begin(), m_data.end(), [rowIndex, columnIndex](Element &e) {
        return std::get<0>(e) == rowIndex && std::get<1>(e) == columnIndex;
    });
}

template<typename T, T DefaultValue>
Matrix<T, DefaultValue>::ValueProxy::ValueProxy(Matrix &m, size_t row, size_t column) :
    m_matrix(m), m_row(row), m_column(column)
{
}

template<typename T, T DefaultValue>
typename Matrix<T, DefaultValue>::ValueProxy &Matrix<T, DefaultValue>::ValueProxy::operator=(const T &other)
{
    auto it = m_matrix.findElement(m_row, m_column);

    if (other == DefaultValue)
    {
        if (it != m_matrix.m_data.end())
            m_matrix.m_data.erase(m_matrix.findElement(m_row, m_column));
    }
    else
    {
        if (it == m_matrix.m_data.end())
            m_matrix.m_data.push_back({m_row, m_column, other});
        else
            std::get<2>(*it) = other;
    }

    return *this;
}

template<typename T, T DefaultValue>
Matrix<T, DefaultValue>::ValueProxy::operator T() const
{
    auto it = m_matrix.findElement(m_row, m_column);
    if (it != m_matrix.m_data.end())
        return std::get<2>(*it);
    else
        return DefaultValue;
}

template<typename T, T DefaultValue>
Matrix<T, DefaultValue>::Row::Row(Matrix &m, size_t rowIndex) :
    m_matrix(m), m_rowIndex(rowIndex) {}

template<typename T, T DefaultValue>
typename Matrix<T, DefaultValue>::ValueProxy Matrix<T, DefaultValue>::Row::operator[](size_t columnIndex)
{
    return ValueProxy(m_matrix, m_rowIndex, columnIndex);
}
