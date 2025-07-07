#pragma once
#include <string>
#include <vector>
#include <memory>

class Plugin {
public:
    virtual ~Plugin() = default;
    virtual std::string name() const = 0;
};

class PluginManager {
public:
    void load(const std::string& dir);
    std::vector<std::shared_ptr<Plugin>> plugins() const;
private:
    std::vector<std::shared_ptr<Plugin>> _plugins;
}; 