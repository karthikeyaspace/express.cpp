// example/handler.h
#pragma once

#include "express.h"

using namespace express;

void get_landing(Request req, Response &res);
void get_json(Request req, Response &res);
void api_submit(Request req, Response &res);
void api_data(Request req, Response &res);
void redirect(Request req, Response &res);
