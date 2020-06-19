#ifndef APARTMENT_H
#define APARTMENT_H

#include <dlib/matrix.h>

#include <vector>
#include <iostream>

namespace my
{

struct Apartment
{
    double longitude;
    double latitude;
    std::size_t rooms_number;
    double price;
    double area;
    double kitchen;
    std::size_t floor;
    std::size_t floors_num;
};

double distance(const Apartment& lhs, const Apartment& rhs);

dlib::matrix<double, 7, 1> normalize_apartment(const Apartment& data);
std::vector<dlib::matrix<double, 7, 1>> normalize_apartments(const std::vector<Apartment>& data);

struct EmptyLineException : std::exception
{
};

void to_csv(std::ostream& stream, const Apartment& apartment);

Apartment read_csv_line(std::istream& stream, char separator = ';');
std::vector<Apartment> read_data_from_csv(std::istream& stream, char separator = ';');

} // namespace my

#endif // APARTMENT_H
