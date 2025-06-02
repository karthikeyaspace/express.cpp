// /example/handler.cpp

#include "express.h"

using namespace express;


// handlers

void get_landing(Request req, Response &res) {
    res.status(200);
    res.message("Welcome to the landing page!");
}

void get_json(Request req, Response &res) {
    res.status(200);
    res.json({
        {"message", "This is a sample JSON response"},
        {"status", "success"},
    });
}

void api_submit(Request req, Response &res) {
    auto data = req.body;
    res.status(200);
    res.message(data);
}

void api_data(Request req, Response &res) {
    res.status(200);
    res.json({
        {"message", "This is a sample API response"},
        {"status", "success"},
    });
}

void redirect(Request req, Response &res) {
    res.redirect("/");
}

