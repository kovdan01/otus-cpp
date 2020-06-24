#include "apartment.h"

#include <dlib/clustering.h>
#include <dlib/svm/kkmeans.h>

#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <map>
#include <algorithm>
#include <cassert>

using sample_type = dlib::matrix<double, 7, 1>;
using kernel_type = dlib::radial_basis_kernel<sample_type>;

// label to element indexes
using clusters_type = std::map<std::size_t, std::vector<std::size_t>>;

/**
 * @brief Gets vector of elements from the same cluster as the
 * given element and sorts this vector according to distance
 * @param apartment[in] The element to search close elements for
 * @param samples[in] Elements used to train the clustering model
 * @param model[in] Clustering model
 * @param clusters[in] Clusters in format: cluster label -> vector of indexes
 * @return Vector of indexes corresponding to the apartments
 * from the same cluster that are sorted by distance
 */
std::vector<std::size_t> get_close_elements(const my::Apartment& apartment,
                                            const std::vector<my::Apartment>& samples,
                                            const dlib::kkmeans<kernel_type>& model,
                                            const clusters_type& clusters)
{
    sample_type sample = my::normalize_apartment(apartment);
    std::size_t cluster_label = model(sample);
    std::vector<std::size_t> elements = clusters.at(cluster_label);
    std::sort(elements.begin(), elements.end(), [&](std::size_t lhs, std::size_t rhs)
    {
        return my::distance(apartment, samples[lhs]) < my::distance(apartment, samples[rhs]);
    });

    return elements;
}

int main(int argc, char* argv[]) try
{
    if (argc != 2)
        throw std::runtime_error("You must pass the model filename as the only argument");

    std::string filename = std::string(argv[1]);

    // create and load model from file
    dlib::kcentroid<kernel_type> kc(kernel_type(0.1), 0.01, 8);
    dlib::kkmeans<kernel_type> model(kc);
    {
        std::ifstream model_file(filename + ".model", std::ios::in | std::ios::binary);
        deserialize(model, model_file);
    }
    // load raw data from file
    std::vector<my::Apartment> apartments;
    {
        std::ifstream data_file(filename + ".data", std::ios::in);
        apartments = my::read_data_from_csv(data_file);
    }
    // load cluster labels from file
    std::vector<std::size_t> cluster_labels(apartments.size());
    {
        std::ifstream labels_file(filename + ".labels", std::ios::in);
        for (std::size_t& elem : cluster_labels)
            labels_file >> elem;
    }
    assert(apartments.size() == cluster_labels.size());

    clusters_type clusters; ///< Map from cluster label to vector of indexes of elements in this cluster
    for (std::size_t i = 0; i < apartments.size(); ++i)
        clusters[cluster_labels[i]].emplace_back(i);

    try
    {
        my::Apartment apartment;
        while (std::cin)
        {
            apartment = my::read_csv_line(std::cin);
            std::vector<std::size_t> close_elements = get_close_elements(apartment, apartments, model, clusters);
            for (std::size_t i : close_elements)
                my::to_csv(std::cout, apartments[i]);
        }
    }
    catch (const my::EmptyLineException&)
    {
        // it's okay
    }

}
catch (const std::exception& e)
{
    std::cerr << e.what() << '\n';
    return 1;
}

