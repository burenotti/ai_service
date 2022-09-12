//
// Created by Burenin Artem on 12.09.2022.
//

#pragma once

#include <boost/numeric/ublas/matrix.hpp>
#include <filesystem>
#include <fstream>
#include <json/json.h>
#include <utility>

namespace neural {


    using boost::numeric::ublas::matrix;
    using boost::numeric::ublas::vector;
    using boost::numeric::ublas::prod;

    using activation = std::function<double(double)>;

    namespace detail {
        matrix<double> matrixFromJson(const Json::Value& source) {

            matrix<double> m(source.size(), source[0].size());
            for (int row = 0; row < m.size1(); ++row) {
                for (int col = 0; col < m.size2(); ++col) {
                    if (source[row].isArray())
                        m(row, col) = source[row][col].asDouble();
                    else
                        m(row, col) = source[row].asDouble();
                }
            }

            return m;
        }

        vector<double> vectorFromJson(const Json::Value& source) {
            vector<double> v(source.size());
            for (int i = 0; i < source.size(); ++i) {
                v[i] = source[i].asDouble();
            }
            return v;
        }

        activation activationByName(const std::string& name) {
            if (name == "relu")
                return [](double x) { return (x > 0) ? x : 0; };
            else // Always ReLu
                return [](double x) { return (x > 0) ? x : 0; };
        }

        Json::Value jsonFromFile(std::string path) {
            std::ifstream stream(path);
            Json::Value result;
            stream >> result;
            return result;
        }

    }


    struct Layer {
        matrix<double> weights;
        vector<double> bias;
        activation f;

        static Layer fromJson(const Json::Value& source) {
            Layer l;
            l.f = detail::activationByName(source["activation"].asString());
            l.weights = detail::matrixFromJson(source["kernel"]);
            l.bias = detail::vectorFromJson(source["bias"]);

            return l;
        }

        [[nodiscard]] vector<double> apply(const vector<double>& neurons) const {
            LOG_DEBUG << "Neurons shape: " << neurons.size();
            LOG_DEBUG << "Weights shape: " << weights.size1() << " " << weights.size2();
            vector<double> result = prod(neurons, weights) + bias;
            LOG_DEBUG << "Result shape: " << result.size();


            std::transform(result.begin(), result.end(), result.begin(), f);
            LOG_DEBUG << "Applied activation";
            return result;
        }
    };

    class NeuralNetwork {
    public:
        NeuralNetwork(std::initializer_list<Layer> layers) : _layers(layers) {
        }

        vector<double> predict(const vector<double>& inputs);

        static NeuralNetwork fromFile(std::string path);

    protected:
        NeuralNetwork() = default;

    private:
        std::vector<Layer> _layers;
    };

    vector<double> NeuralNetwork::predict(const vector<double>& inputs) {
        auto result = inputs;
        int i = 0;
        for (const auto& layer: _layers) {
            LOG_DEBUG << "Applying layer #" << ++i;
            result = layer.apply(result);
        }
        return result;
    }

    NeuralNetwork NeuralNetwork::fromFile(std::string path) {
        auto data = detail::jsonFromFile(std::move(path));
        NeuralNetwork nn;
        for (const auto& item: data) {
            nn._layers.push_back(Layer::fromJson(item));
        }
        return nn;
    }
}