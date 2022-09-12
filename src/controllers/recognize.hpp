//
// Created by Burenin Artem on 12.09.2022.
//

#pragma once

#include <drogon/HttpController.h>
#include "../business/recognize_digit.hpp"
#include "../dto/recognize_request.hpp"


namespace api {

    using namespace drogon;

    Json::Value jsonFromException(std::exception& exc, HttpStatusCode code = HttpStatusCode::k422UnprocessableEntity) {
        Json::Value body;
        body["ok"] = false;
        body["why"] = exc.what();
        auto response = HttpResponse::newHttpJsonResponse(body);
        response->setStatusCode(HttpStatusCode::k422UnprocessableEntity);
        return body;
    }

    class Recognize : public drogon::HttpController<Recognize> {
    public:

        METHOD_LIST_BEGIN
            METHOD_ADD(Recognize::recognize, "/", Post);
        METHOD_LIST_END

    public:
        void recognize(const HttpRequestPtr& request, std::function<void(const HttpResponsePtr&)>&& callback);
    };

    void Recognize::recognize(const HttpRequestPtr& request, std::function<void(const HttpResponsePtr&)>&& callback) {
        try {
            auto json = request->getJsonObject();
            auto data = dto::Recognize::fromJson(*json);
            auto result = business::recognizeDigit(data.vectorize());

            Json::Value responseBody;
            responseBody["ok"] = true;
            responseBody["result"] = result;

            LOG_DEBUG << "Response body ready";
            auto response = HttpResponse::newHttpJsonResponse(responseBody);
            response->setStatusCode(HttpStatusCode::k200OK);
            callback(response);

        } catch (dto::ParseError& error) {
            auto body = jsonFromException(error);
            callback(HttpResponse::newHttpJsonResponse(body));
        } catch (Json::RuntimeError& error) {
            auto body = jsonFromException(error);
            callback(HttpResponse::newHttpJsonResponse(body));
        }
    }
}
