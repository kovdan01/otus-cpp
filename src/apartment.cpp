#include "apartment.h"

#include <stdexcept>
#include <cmath>

namespace my
{

namespace detail
{

constexpr double MAX_LONGITUDE = 180;
constexpr double MAX_LATITUDE = 90;
constexpr double MAX_ROOMS_NUMBER = 10;
constexpr double MAX_PRICE = 10'000'000;
constexpr double MAX_AREA = 150;
constexpr double MAX_KITCHEN = 25;

} // namespace detail

double distance(const Apartment& lhs, const Apartment& rhs)
{
    double x1 = lhs.longitude / detail::MAX_LONGITUDE;
    double y1 = lhs.latitude / detail::MAX_LATITUDE;
    double x2 = rhs.longitude / detail::MAX_LONGITUDE;
    double y2 = rhs.latitude / detail::MAX_LATITUDE;

    return std::sqrt((x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2));
}

dlib::matrix<double, 7, 1> normalize_apartment(const Apartment& data)
{
    return dlib::matrix<double, 7, 1>
    ({
        data.longitude / detail::MAX_LONGITUDE,
        data.latitude / detail::MAX_LATITUDE,
        data.rooms_number > detail::MAX_ROOMS_NUMBER ? 1 : data.rooms_number / detail::MAX_ROOMS_NUMBER,
        data.price > detail::MAX_PRICE ? 1 : data.price / detail::MAX_PRICE,
        data.area > detail::MAX_AREA ? 1 : data.area / detail::MAX_AREA,
        data.kitchen > detail::MAX_KITCHEN ? 1 : data.kitchen / detail::MAX_KITCHEN,
        data.floor == 1 || data.floor == data.floors_num ? 1. : 0
    });
}

std::vector<dlib::matrix<double, 7, 1>> normalize_apartments(const std::vector<Apartment>& data)
{
    std::vector<dlib::matrix<double, 7, 1>> samples;
    samples.reserve(data.size());

    for (const Apartment& elem : data)
        samples.emplace_back(normalize_apartment(elem));

    return samples;
}

Apartment read_csv_line(std::istream& stream, char separator)
{
    double longitude;
    double latitude;
    std::size_t rooms_number;
    double price;
    double area;
    double kitchen;
    std::size_t floor;
    std::size_t floors_num;

    auto check_separator = [&stream, separator]()
    {
        if (stream.peek() != separator)
            throw std::runtime_error("Invalid csv");
        stream.ignore(1);
    };

    auto input = [&stream, separator, &check_separator](auto& elem)
    {
        check_separator();
        if (stream.peek() == separator)
        {
            elem = 0;
            return;
        }
        stream >> elem;
    };

    stream >> longitude;
    if (!stream)
        throw EmptyLineException{};
    input(latitude);
    input(rooms_number);
    input(price);
    input(area);
    input(kitchen);
    input(floor);
    input(floors_num);

    return my::Apartment{longitude, latitude, rooms_number, price, area, kitchen, floor, floors_num};
}

std::vector<Apartment> read_data_from_csv(std::istream& stream, char separator)
{
    std::vector<Apartment> samples;

    try
    {
        while (stream)
        {
            samples.emplace_back(read_csv_line(stream, separator));
        }
    }
    catch (const EmptyLineException&)
    {
        // it's okay
    }

    return samples;
}

void to_csv(std::ostream& stream, const Apartment& apartment)
{
    stream << apartment.longitude << ';'
           << apartment.latitude << ';'
           << apartment.rooms_number << ';'
           << apartment.price << ';'
           << apartment.area << ';'
           << apartment.kitchen << ';'
           << apartment.floor << ';'
           << apartment.floors_num << '\n';
}

} // namespace my
