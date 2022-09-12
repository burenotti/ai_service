//
// Created by Burenin Artem on 12.09.2022.
//

#pragma once

#include <string>
#include <boost/numeric/ublas/matrix.hpp>
#include <json/json.h>
#include <exception>
#include <drogon/utils/Utilities.h>

namespace dto {

    using boost::numeric::ublas::vector;


    struct ParseError : public std::runtime_error {
        explicit ParseError(const std::string& msg) : std::runtime_error(msg) {
        }
    };

    struct Recognize {
        int width;
        int height;
        vector<double> data;

        [[nodiscard]] vector<double> vectorize() const;

        Recognize() : width(0), height(0) {
        }

        static Recognize fromJson(const Json::Value& source) {
            Recognize r;
            r.data.resize(28 * 28);

            if (!source.isMember("width"))
                throw ParseError("Width is required parameter");

            if (!source.isMember("height"))
                throw ParseError("Height is required parameter");

            r.width = source["width"].asInt();
            r.height = source["height"].asInt();

            if (r.width != 28 || r.height != 28) {
                throw ParseError("Image must be (28x28) pixels");
            }

            auto rawData = drogon::utils::base64Decode(source["body"].asString());
            int i = 0;
            std::string buf;

            for (char ch: rawData) {
                if (ch != ',') {
                    buf += ch;
                } else {
                    double color = std::stoi(buf);

                    if (color < 0 || color > 255)
                        throw ParseError("Each pixel color must be in range 0..255");

                    r.data[i] = color / 255.0;
                    ++i;
                    buf = "";
                }
            }

            if (i + 1 != 28 * 28)
                throw ParseError("Pixel vector must be of size 28x28=784. But "
                                 + std::to_string(i + 1) + " were given");

            return r;
        }
    };

    vector<double> Recognize::vectorize() const {
        return data;
    }

}