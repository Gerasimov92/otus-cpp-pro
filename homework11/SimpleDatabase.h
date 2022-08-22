#pragma once

#include <map>
#include <mutex>
#include <set>
#include <string>

struct Record
{
    int id;
    std::string name;

    bool operator<(const Record &other) const { return id < other.id; }
    operator int() const { return id; }
};

using Table = std::set<Record>;

class SimpleDatabase
{
public:
    SimpleDatabase();

    std::string insert(const std::string &tableName, int id, const std::string &name);
    std::string truncate(const std::string &tableName);
    std::map<int, std::pair<std::string, std::string>> intersection();
    std::map<int, std::pair<std::string, std::string>> symmetricDifference();

private:
    std::map<int, std::pair<std::string, std::string>> findRecords(const std::set<int> &ids, const Table &A, const Table &B);

    std::mutex m_mutex;
    std::map<std::string, Table> m_tables;
};

