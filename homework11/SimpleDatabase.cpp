#include "SimpleDatabase.h"

#include <algorithm>

SimpleDatabase::SimpleDatabase()
    : m_tables{{"A", {}}, {"B", {}}}
{
}

std::string SimpleDatabase::insert(const std::string &tableName, int id, const std::string &name)
{
    std::lock_guard lock{m_mutex};

    if (m_tables.count(tableName) == 0)
        return "unknown table " + tableName;

    auto &table = m_tables[tableName];

    auto it = std::find_if(table.cbegin(), table.cend(), [id](const Record &r) { return r.id == id; });
    if (it != table.end())
        return "duplicate " + std::to_string(id);

    table.emplace(Record{id, name});

    return "";
}

std::string SimpleDatabase::truncate(const std::string &tableName)
{
    std::lock_guard lock{m_mutex};

    if (m_tables.count(tableName) == 0)
        return "unknown table " + tableName;

    m_tables[tableName].clear();

    return "";
}

std::map<int, std::pair<std::string, std::string>> SimpleDatabase::intersection()
{
    std::lock_guard lock{m_mutex};

    const auto &A = m_tables.at("A");
    const auto &B = m_tables.at("B");
    std::set<int> ids;

    std::set_intersection(A.cbegin(), A.cend(), B.cbegin(), B.cend(), std::inserter(ids, ids.end()));

    return findRecords(ids, A, B);
}

std::map<int, std::pair<std::string, std::string>> SimpleDatabase::symmetricDifference()
{
    std::lock_guard lock{m_mutex};

    const auto &A = m_tables.at("A");
    const auto &B = m_tables.at("B");
    std::set<int> ids;

    std::set_symmetric_difference(A.cbegin(), A.cend(), B.cbegin(), B.cend(), std::inserter(ids, ids.end()));

    return findRecords(ids, A, B);
}

std::map<int, std::pair<std::string, std::string>> SimpleDatabase::findRecords(const std::set<int> &ids, const Table &A, const Table &B)
{
    std::map<int, std::pair<std::string, std::string>> result;

    for (int id : ids)
    {
        std::string n1, n2;

        auto it = A.find({id, ""});
        if (it != A.end())
            n1 = it->name;

        it = B.find({id, ""});
        if (it != B.end())
            n2 = it->name;

        result[id] = {n1, n2};
    }

    return result;
}
