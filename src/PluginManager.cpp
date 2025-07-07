#include "Plugin.hpp"
#include <filesystem>
#if defined(_WIN32)
#include <windows.h>
#else
#include <dlfcn.h>
#endif

void PluginManager::load(const std::string& dir) {
    for (const auto& p: std::filesystem::directory_iterator(dir)) {
        auto ext = p.path().extension().string();
#if defined(_WIN32)
        if (ext != ".dll") continue;
        HMODULE handle = LoadLibraryA(p.path().string().c_str());
        if (!handle) continue;
        auto create = reinterpret_cast<Plugin*(*)()>(GetProcAddress(handle, "create_plugin"));
#else
        if (ext != ".so" && ext != ".dylib") continue;
        void* handle = dlopen(p.path().c_str(), RTLD_NOW);
        if (!handle) continue;
        auto create = reinterpret_cast<Plugin*(*)()>(dlsym(handle, "create_plugin"));
#endif
        if (create) {
            std::shared_ptr<Plugin> plugin(create(), [handle](Plugin* p) {
                delete p;
#if defined(_WIN32)
                FreeLibrary(handle);
#else
                dlclose(handle);
#endif
            });
            _plugins.emplace_back(std::move(plugin));
        }
    }
}

std::vector<std::shared_ptr<Plugin>> PluginManager::plugins() const { return _plugins; } 