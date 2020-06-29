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

/**
 * @brief Calculates distance between two apartments
 * @param lhs[in] First apartment
 * @param rhs[in] Second apartment
 * @return Distance between the apartments
 */
double distance(const Apartment& lhs, const Apartment& rhs);

/**
 * @brief Normalize apartment data to range [0, 1]
 * @param data[in] Apartment instance to normalize
 * @return A matrix row with normalized data
 */
dlib::matrix<double, 7, 1> normalize_apartment(const Apartment& data);

/**
 * @brief Effectively calls `normalize_apartment` for each instance from data
 * @param data[in] Vector of apartments to normalize
 * @return Vector of matrix rows with normalized data
 */
std::vector<dlib::matrix<double, 7, 1>> normalize_apartments(const std::vector<Apartment>& data);

struct EmptyLineException : std::exception
{
};

/**
 * @brief Write apartment data to an output stream in csv format
 * @param stream[in,out] Stream to write apartment data to
 * @param apartment[in] Apartment with data to write
 */
void to_csv(std::ostream& stream, const Apartment& apartment);

/**
 * @brief Convert one csv line to an apartment instance
 * @param stream[in,out] Input stream with csv line
 * @param separator[in] Csv format separator
 * @return Apartment instance with data from csv
 */
Apartment read_csv_line(std::istream& stream, char separator = ';');

/**
 * @brief Effectively calls `read_csv_line` for
 * each line from the given input stream
 * @param stream[in,out] Input stream with csv data
 * @param separator[in] Csv format separator
 * @return Vector of apartment instances with data from csv
 */
std::vector<Apartment> read_data_from_csv(std::istream& stream, char separator = ';');

} // namespace my

#endif // APARTMENT_H
