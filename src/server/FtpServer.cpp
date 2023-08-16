#include "FtpServer.h"

#include "../base/Logger.h"
#include "../base/Singleton.h"
#include "LoginManager.h"

FtpServer::FtpServer(std::string path) : server_(),
                                         config_(path)
{
}

void FtpServer::start()
{
    // Create and configure the server
    LOG_Debug("Create and configure the server");
    configureServer(server_);

    // Start the server
    LOG_Debug("Start the server");
    server_.listen(config_.address(), 8080);
}

void FtpServer::configureServer(httplib::Server &server)
{
    // Set the server's run user

    uid_t currentUserId = getuid();

    if (currentUserId == 0 && config_.run_user() != "")
    {
        // Get the user ID for the specified user
        struct passwd *pw = getpwnam(config_.run_user().c_str());
        if (pw == nullptr)
        {
            std::string errorMsg = "Error:User '" + config_.run_user() + "' not found.";
            LOG_Debug(errorMsg);
            exit(1);
        }
        else
        {
            // Change the user ID of the process
            if (setuid(pw->pw_uid) != 0)
            {
                std::string errorMsg = "Error: Failed to set user ID for '" + config_.run_user() + "'.";
                LOG_Debug(errorMsg);
                exit(1);
            }
        }
    }

    if (config_.credentials().size() != 0)
    {
        const std::vector<Config::Credential> &credentials = config_.credentials();
        for (const auto &credential : credentials)
        {
            Singleton<LoginManager>::getInstance().set_user_credentials(credential.username, credential.password);
        }
    }

    // Set the server's concurrency limit
    if (config_.concurrency_limit() > 0)
    {
        int concurrency_limit = config_.concurrency_limit();
        server_.new_task_queue = [concurrency_limit]()
        { return new httplib::ThreadPool(concurrency_limit); };
    }

    // Set the server's keep-alive limit
    if (config_.keepalive_limit() > 0)
    {
        server_.set_keep_alive_max_count(config_.keepalive_limit());
    }

    // Define your routes and handlers here
    server_.Get("/", [this](const httplib::Request &req, httplib::Response &res)
                {
        // Example handler
        res.set_content("Hello, World!", "text/plain"); });

    // Create handlers using HandlerFactory
    HandlerFactory handlerFactory;

    // Get the handlers from the factory and register them with the server
    LOG_Debug("Get the handlers from the factory and register them with the server");
    const std::vector<Config::Handler> &Handlers = config_.handlers();
    for (const auto &Handler : Handlers)
    {
        HandlerInfo handler_info = handlerFactory.createHandler(Handler);

        if (handler_info.http_method == HttpMethod::GET)
        {
            server_.Get(Handler.url.c_str(), handler_info.handler_function);
        }
        else if (handler_info.http_method == HttpMethod::POST)
        {
            server_.Post(Handler.url.c_str(), handler_info.handler_function);
        }
        else if (handler_info.http_method == HttpMethod::PUT)
        {
            server_.Put(Handler.url.c_str(), handler_info.handler_function);
        }
        else if (handler_info.http_method == HttpMethod::DELETE)
        {
            server_.Delete(Handler.url.c_str(), handler_info.handler_function);
        }
    }

    server_.Get("/", [](const httplib::Request &req, httplib::Response &res)
                { res.status = 200; });
}
