#ifndef DATABASE_H
#define DATABASE_H

#include "message.h"

#include <map>
#include <string>
#include <optional>
#include <shared_mutex>

namespace my
{

/**
 * @brief The Database class
 * A singletone representing a database of
 * two tables A and B with the following structure:
 * {
 *     int id;
 *     std::string name;
 * }
 * id is a primary key here
 */
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

    /**
     * @brief Insert row to table
     * @param table[in] Table to insert row into (A, B)
     * @param id[in] Id
     * @param data[in] Name
     * @return Message with code OK on success, code ERROR
     * and textual error description on failure
     */
    Message insert(const std::string& table, Index id, Data data);

    /**
     * @brief Truncate a table
     * @param table[in] table name to truncate (A, B)
     * @return Message with code OK on success, code ERROR
     * and textual error description on failure
     */
    Message truncate(const std::string& table);

    /**
     * @brief The SymmetricDifference class
     * Represents a symmetric difference of two tables
     */
    class SymmetricDifference
    {
    public:
        /**
         * @brief size
         * @return Number of rows in symmetric difference
         */
        [[nodiscard]] std::size_t size() const;

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

        [[nodiscard]] Iterator begin() const;
        [[nodiscard]] Iterator end() const;

    private:
        friend class Database;

        SymmetricDifference() = default;

        std::map<Index, std::pair<std::optional<Data>, std::optional<Data>>> m_data;
    };

    /**
     * @brief The Intersection class
     * Represents an intersection of two tables
     */
    class Intersection
    {
    public:
        /**
         * @brief size
         * @return Number of rows in intersection
         */
        [[nodiscard]] std::size_t size() const;

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

        [[nodiscard]] Iterator begin() const;
        [[nodiscard]] Iterator end() const;

    private:
        friend class Database;

        Intersection() = default;

        std::map<Index, std::pair<Data, Data>> m_data;
    };

    /**
     * @brief Create symmetric difference of two tables
     * @param table1_name[in] First table name (A, B)
     * @param table2_name[in] Second table name (A, B)
     * @return A pair of message and symmetric difference.
     * Message contains code OK on success, code ERROR
     * and textual error description on failure
     */
    std::pair<Message, SymmetricDifference>
    symmetric_difference(const std::string& table1_name, const std::string& table2_name) const;

    /**
     * @brief Create intersection of two tables
     * @param table1_name[in] First table name (A, B)
     * @param table2_name[in] Second table name (A, B)
     * @return A pair of message and intersection
     * Message contains code OK on success, code ERROR
     * and textual error description on failure
     */
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
