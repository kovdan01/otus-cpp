#ifndef STATS_H
#define STATS_H

#include <map>
#include <thread>

namespace my
{

class LogStats
{
public:
    LogStats(const LogStats&) = delete;
    LogStats& operator=(const LogStats&) = delete;
    LogStats(LogStats&&) = delete;
    LogStats& operator=(LogStats&&) = delete;

    struct Stats
    {
        std::size_t blocks_count = 0;
        std::size_t commands_count = 0;
    };

    static LogStats* get_instance()
    {
        static LogStats instance;
        return &instance;
    }

    std::map<std::thread::id, Stats> stats;

private:
    LogStats() = default;
};

class MainStats
{
public:
    MainStats(const MainStats&) = delete;
    MainStats& operator=(const MainStats&) = delete;
    MainStats(MainStats&&) = delete;
    MainStats& operator=(MainStats&&) = delete;

    std::size_t lines_count = 0;
    std::size_t blocks_count = 0;
    std::size_t commands_count = 0;

    static MainStats* get_instance()
    {
        static MainStats instance;
        return &instance;
    }

private:
    MainStats() = default;
};

} // namespace my

#endif // STATS_H
