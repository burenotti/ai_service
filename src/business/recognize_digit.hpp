//
// Created by Burenin Artem on 12.09.2022.
//

#pragma once


#include <boost/numeric/ublas/matrix.hpp>
#include "../neural/neural.hpp"

namespace business {

    using boost::numeric::ublas::vector;

    int recognizeDigit(const vector<double>& pixels) {
        static auto network = neural::NeuralNetwork::fromFile("weights.json");

        auto results = network.predict(pixels);
        auto digit = std::distance(
                results.begin(),
                std::max_element(results.begin(), results.end())
        );

        return static_cast<int>(digit);
    }
}