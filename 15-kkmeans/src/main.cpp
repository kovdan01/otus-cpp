#include <iostream>
#include <vector>

#include <dlib/clustering.h>

template <typename Number, std::size_t ColumnsNumber>
std::vector<dlib::matrix<Number, ColumnsNumber, 1>> read_numbers_from_csv(std::istream& stream, char separator = ';')
{
    if (ColumnsNumber == 0)
        return {};

    Number num;
    dlib::matrix<Number, ColumnsNumber, 1> row;
    std::vector<dlib::matrix<Number, ColumnsNumber, 1>> samples;

    while (stream >> num)
    {
        row(0) = num;
        for (std::size_t i = 1; i < ColumnsNumber; ++i)
        {
            if (stream.peek() != separator)
                throw std::runtime_error("Invalid csv");
            stream.ignore(1);
            stream >> num;
            row(i) = num;
        }
        samples.emplace_back(row);
    }

    return samples;
}

int main(int argc, char* argv[]) try
{
    if (argc != 2)
        throw std::runtime_error("You must pass number of clusters as an only argument");

    unsigned long clusters_number = std::stoul(argv[1]);

    using sample_type = dlib::matrix<double, 2, 1>;
    using kernel_type = dlib::radial_basis_kernel<sample_type>;

    dlib::kcentroid<kernel_type> kc(kernel_type(0.1), 0.01, 8);
    dlib::kkmeans<kernel_type> test(kc);

    std::vector<sample_type> samples;
    samples = read_numbers_from_csv<double, 2>(std::cin);

    std::vector<sample_type> initial_centers;
    test.set_number_of_centers(clusters_number);
    pick_initial_centers(clusters_number, initial_centers, samples, test.get_kernel());

    test.train(samples, initial_centers);

    for (unsigned long i = 0; i < samples.size(); ++i)
        std::cout << samples[i](0) << ';' << samples[i](1) << ';' << test(samples[i]) << '\n';
}
catch (const std::exception& e)
{
    std::cerr << e.what() << '\n';
    return 1;
}
