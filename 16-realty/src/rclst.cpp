#include "apartment.h"

#include <dlib/clustering.h>
#include <dlib/svm/kkmeans.h>

#include <iostream>
#include <vector>
#include <string>
#include <fstream>

int main(int argc, char* argv[]) try
{
    if (argc != 3)
        throw std::runtime_error("You must pass number of clusters as the first argument "
                                 "and the model filename as the second argument");

    unsigned long clusters_number = std::stoul(argv[1]);
    std::string filename = std::string(argv[2]);

    using sample_type = dlib::matrix<double, 7, 1>;
    using kernel_type = dlib::radial_basis_kernel<sample_type>;

    dlib::kcentroid<kernel_type> kc(kernel_type(0.1), 0.01, 8);
    dlib::kkmeans<kernel_type> model(kc);

    // read data from stdin
    std::vector<my::Apartment> samples_raw = my::read_data_from_csv(std::cin);
    // normalize data to range from 0 to 1
    std::vector<sample_type> samples = my::normalize_apartments(samples_raw);

    // train model
    std::vector<sample_type> initial_centers;
    model.set_number_of_centers(clusters_number);
    pick_initial_centers(clusters_number, initial_centers, samples, model.get_kernel());

    model.train(samples, initial_centers);

    // save trained model to file
    {
        std::ofstream model_file(filename + ".model", std::ios::out | std::ios::binary);
        serialize(model, model_file);
    }
    // save raw data to file
    {
        std::ofstream data_file(filename + ".data", std::ios::out);
        for (const my::Apartment& apartment : samples_raw)
            my::to_csv(data_file, apartment);
    }
    // save cluster labels to file
    {
        std::ofstream labels_file(filename + ".labels", std::ios::out);
        for (const sample_type& sample : samples)
            labels_file << model(sample) << '\n';
    }
}
catch (const std::exception& e)
{
    std::cerr << e.what() << '\n';
    return 1;
}

