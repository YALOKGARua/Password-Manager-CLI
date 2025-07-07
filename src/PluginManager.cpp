#include "Plugin.hpp"
#include <filesystem>
#include <dlfcn.h>

void PluginManager::load(const std::string& dir) {
    for(auto& p: std::filesystem::directory_iterator(dir)) {
        if(p.path().extension() == ".so") {
            void* handle = dlopen(p.path().c_str(), RTLD_LAZY);
            if(handle) {
                using create_t = Plugin*();
                auto create = (create_t*)dlsym(handle, "create_plugin");
                if(create) {
                    std::shared_ptr<Plugin> plugin(create(), [handle](Plugin* p){ delete p; dlclose(handle); });
                    _plugins.push_back(plugin);
                }
            }
        }
    }
}

std::vector<std::shared_ptr<Plugin>> PluginManager::plugins() const { return _plugins; } 