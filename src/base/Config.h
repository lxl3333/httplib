#ifndef CONFIG_H
#define CONFIG_H

#include <string>
#include <vector>

class Config
{
public:
    struct Handler
    {
        std::string url;
        std::string method;
        std::string type;
        std::string dir;
    };

    struct Credential
    {
        std::string username;
        std::string password;
    };

    explicit Config(const std::string &filename);

    const std::string &address() const;
    const std::string &run_user() const;
    int concurrency_limit() const;
    int keepalive_limit() const;
    const std::vector<Handler> &handlers() const;
    const std::vector<Credential> &credentials() const;

private:
    std::string address_;
    std::string run_user_;
    int concurrency_limit_;
    int keepalive_limit_;
    std::vector<Handler> handlers_;
    std::vector<Credential> credentials_;
};

#endif // CONFIG_H
