#ifndef CONFIG_H
#define CONFIG_H

#include <string>
#include <vector>

class Config {
public:
    struct Handler {
        std::string url;
        std::string type;
        std::string dir;
    };

    explicit Config(const std::string& filename);

    const std::string& address() const;
    const std::string& run_user() const;
    int concurrency_limit() const;
    int keepalive_limit() const;
    const std::vector<Handler>& handlers() const;

private:
    std::string address_;
    std::string run_user_;
    int concurrency_limit_;
    int keepalive_limit_;
    std::vector<Handler> handlers_;
};

#endif  // CONFIG_H
