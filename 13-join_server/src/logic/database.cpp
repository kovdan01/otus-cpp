#include "logic/database.h"

#include <stdexcept>
#include <tuple>

namespace my
{

Database::Database()
{
    m_tables.emplace("A", Table{});
    m_tables.emplace("B", Table{});
}

Database* Database::get_instance()
{
    static Database instance;
    return &instance;
}

// ---------------------------------------------------------------------------
//                                   INSERT
// ---------------------------------------------------------------------------

Message Database::insert(const std::string& table_name, Index id, Data data)
{
    std::lock_guard m_lock(m_mutex);
    try
    {
        auto table_it = m_tables.find(table_name);
        if (table_it == m_tables.end())
            return Message{Message::Code::ERROR, std::string{"No such table "} + table_name};

        Table& table = table_it->second;
        if (table.find(id) != table.end())
            return Message{Message::Code::ERROR, std::string{"Duplicate "} + std::to_string(id)};

        table.emplace(id, std::move(data));
        return Message{Message::Code::OK, ""};
    }
    catch (const std::exception&)
    {
        return Message{Message::Code::ERROR, "Unknown error"};
    }
}

// ---------------------------------------------------------------------------
//                                 TRUNCATE
// ---------------------------------------------------------------------------

Message Database::truncate(const std::string& table)
{
    std::lock_guard m_lock(m_mutex);
    try
    {
        auto table_it = m_tables.find(table);
        if (table_it == m_tables.end())
            return Message{Message::Code::ERROR, std::string{"No such table "} + table};

        table_it->second = {};
        return Message{Message::Code::OK, ""};
    }
    catch (...)
    {
        return Message{Message::Code::ERROR, "Unknown error"};
    }
}

// ---------------------------------------------------------------------------
//                            SYMMETRIC DIFFERENCE
// ---------------------------------------------------------------------------

std::pair<Message, Database::SymmetricDifference>
Database::symmetric_difference(const std::string& table1_name, const std::string& table2_name) const
{
    std::shared_lock m_lock(m_mutex);
    try
    {
        auto table1_it = m_tables.find(table1_name);
        if (table1_it == m_tables.end())
            return {Message{Message::Code::ERROR, std::string{"No such table "} + table1_name}, {}};

        auto table2_it = m_tables.find(table2_name);
        if (table2_it == m_tables.end())
            return {Message{Message::Code::ERROR, std::string{"No such table "} + table2_name}, {}};

        const Table& table1 = table1_it->second;
        const Table& table2 = table2_it->second;

        SymmetricDifference diff;
        for (const auto& [index, data] : table1)
            if (table2.find(index) == table2.end())
                diff.m_data.emplace(index, std::pair{data, std::nullopt});
        for (const auto& [index, data] : table2)
            if (table1.find(index) == table1.end())
                diff.m_data.emplace(index, std::pair{std::nullopt, data});

        return {Message{Message::Code::OK, ""}, diff};
    }
    catch (...)
    {
        return {Message{Message::Code::ERROR, "Unknown error"}, {}};
    }
}

std::size_t Database::SymmetricDifference::size() const
{
    return m_data.size();
}

Database::SymmetricDifference::Iterator Database::SymmetricDifference::begin() const
{
    return Iterator(this, m_data.begin());
}

Database::SymmetricDifference::Iterator Database::SymmetricDifference::end() const
{
    return Iterator(this, m_data.end());
}

Database::SymmetricDifference::Iterator::Iterator(const SymmetricDifference* instance, map_it iterator)
    : m_instance(instance)
    , m_iterator(iterator)
{
}

Database::SymmetricDifference::Iterator::Value Database::SymmetricDifference::Iterator::operator*() const
{
    return Value{m_iterator->first, m_iterator->second.first, m_iterator->second.second};
}

bool Database::SymmetricDifference::Iterator::operator==(const Iterator &other) const
{
    return (std::tie(m_instance, m_iterator) ==
            std::tie(other.m_instance, other.m_iterator));
}

bool Database::SymmetricDifference::Iterator::operator!=(const Iterator &other) const
{
    return !(*this == other);
}

Database::SymmetricDifference::Iterator& Database::SymmetricDifference::Iterator::operator++()
{
    ++m_iterator;
    return *this;
}

// ---------------------------------------------------------------------------
//                                 INTERSECTION
// ---------------------------------------------------------------------------

std::pair<Message, Database::Intersection>
Database::intersection(const std::string& table1_name, const std::string& table2_name) const
{
    std::shared_lock m_lock(m_mutex);
    try
    {
        auto table1_it = m_tables.find(table1_name);
        if (table1_it == m_tables.end())
            return {Message{Message::Code::ERROR, std::string{"No such table "} + table1_name}, {}};

        auto table2_it = m_tables.find(table2_name);
        if (table2_it == m_tables.end())
            return {Message{Message::Code::ERROR, std::string{"No such table "} + table2_name}, {}};

        const Table *table1, *table2;

        bool need_swap;
        if (table1_it->second.size() < table2_it->second.size())
        {
            table1 = &table1_it->second;
            table2 = &table2_it->second;
            need_swap = false;
        }
        else
        {
            table1 = &table2_it->second;
            table2 = &table1_it->second;
            need_swap = true;
        }

        Intersection intersection;
        for (const auto& [index1, data1] : *table1)
        {
            auto it2 = table2->find(index1);
            if (it2 != table2->end())
            {
                if (!need_swap)
                    intersection.m_data.emplace(index1, std::pair<Data, Data>{data1, it2->second});
                else
                    intersection.m_data.emplace(index1, std::pair<Data, Data>{it2->second, data1});
            }
        }

        return {Message{Message::Code::OK, ""}, intersection};
    }
    catch (...)
    {
        return {Message{Message::Code::ERROR, "Unknown error"}, {}};
    }
}

std::size_t Database::Intersection::size() const
{
    return m_data.size();
}

Database::Intersection::Iterator Database::Intersection::begin() const
{
    return Iterator(this, m_data.begin());
}

Database::Intersection::Iterator Database::Intersection::end() const
{
    return Iterator(this, m_data.end());
}

Database::Intersection::Iterator::Iterator(const Intersection* instance, map_it iterator)
    : m_instance(instance)
    , m_iterator(iterator)
{
}

Database::Intersection::Iterator::Value Database::Intersection::Iterator::operator*() const
{
    return Value{m_iterator->first, m_iterator->second.first, m_iterator->second.second};
}

bool Database::Intersection::Iterator::operator==(const Iterator &other) const
{
    return (std::tie(m_instance, m_iterator) ==
            std::tie(other.m_instance, other.m_iterator));
}

bool Database::Intersection::Iterator::operator!=(const Iterator &other) const
{
    return !(*this == other);
}

Database::Intersection::Iterator& Database::Intersection::Iterator::operator++()
{
    ++m_iterator;
    return *this;
}

} // namespace my
