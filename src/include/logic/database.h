#ifndef DATABASE_H
#define DATABASE_H

#include "message.h"

#include <map>
#include <string>
#include <optional>
#include <shared_mutex>

namespace my
{

class Database
{
public:
    using Index = int;
    using Data = std::string;

    Database(const Database&) = delete;
    Database& operator=(const Database&) = delete;
    Database(Database&&) = delete;
    Database& operator=(Database&&) = delete;

    static Database* get_instance();

    Message insert(const std::string& table, Index id, Data data);
    Message truncate(const std::string& table);

    class SymmetricDifference
    {
    public:
        std::size_t size() const;

        class Iterator
        {
        public:
            struct Value
            {
                Index id;
                const std::optional<Data>& first;
                const std::optional<Data>& second;
            };

            Value operator*() const;
            Iterator& operator++();
            bool operator==(const Iterator& other) const;
            bool operator!=(const Iterator& other) const;

        private:
            friend class SymmetricDifference;
            using map_it = std::map<Index, std::pair<std::optional<Data>, std::optional<Data>>>::const_iterator;

            Iterator(const SymmetricDifference* instance, map_it iterator);

            const SymmetricDifference* m_instance;
            map_it m_iterator;
        };

        Iterator begin() const;
        Iterator end() const;

    private:
        friend class Database;

        std::map<Index, std::pair<std::optional<Data>, std::optional<Data>>> m_data;
    };

    class Intersection
    {
    public:
        std::size_t size() const;

        class Iterator
        {
        public:
            struct Value
            {
                Index id;
                const Data& first;
                const Data& second;
            };

            Value operator*() const;
            Iterator& operator++();
            bool operator==(const Iterator& other) const;
            bool operator!=(const Iterator& other) const;

        private:
            friend class Intersection;
            using map_it = std::map<Index, std::pair<Data, Data>>::const_iterator;

            Iterator(const Intersection* instance, map_it iterator);

            const Intersection* m_instance;
            map_it m_iterator;
        };

        Iterator begin() const;
        Iterator end() const;

    private:
        friend class Database;

        std::map<Index, std::pair<Data, Data>> m_data;
    };

    std::pair<Message, SymmetricDifference>
    symmetric_difference(const std::string& table1_name, const std::string& table2_name) const;

    std::pair<Message, Intersection>
    intersection(const std::string& table1_name, const std::string& table2_name) const;


private:
    Database();
    ~Database() = default;

    using Table = std::map<Index, Data>;
    mutable std::shared_mutex m_mutex;

    std::map<std::string, Table> m_tables;
};

} // namespace my

#endif // DATABASE_H
