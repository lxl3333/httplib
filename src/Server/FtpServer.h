#ifndef FTPSERVER_H
#define FTPSERVER_H

#include "../base/Config.h"
#include "HandlerFactory.h"
#include "../include/httplib.h"

class FtpServer {
public:
    FtpServer():
      config_("../../config/config.json"),
      server_()
    {
        // Initialize your config as needed
        // config_ = ...
    }

    void start() {
        // Create and configure the server
        configureServer(server_);

        // Start the server
        server_.listen("localhost", 8080);
    }

private:
    Config config_;
    httplib::Server server_;
    void configureServer(httplib::Server& server) {
        // Set the server's run user
        if (config_.address()!="") {
            //server.set_user_and_group(config.runUser.c_str());
        }

        // Set the server's concurrency limit
        if (config_.concurrency_limit() > 0) {
            //server.set_max_threads(config.concurrencyLimit);
        }

        // Set the server's keep-alive limit
        if (config_.keepalive_limit() > 0) {
            server.set_keep_alive_max_count(config_.keepalive_limit());
        }

        // Define your routes and handlers here
        server.Get("/", [this](const httplib::Request& req, httplib::Response& res) {
            // Example handler
            res.set_content("Hello, World!", "text/plain");
        });

        // Create handlers using HandlerFactory
        HandlerFactory handlerFactory;

        // Get the handlers from the factory and register them with the server
        const std::vector<Config::Handler>& Handlers=config_.handlers();
        for (const auto& Handler :Handlers) {
            auto handler=handlerFactory.createHandler(Handler);
            server.Get(Handler.url, handler);
        }
    }
};

#endif


