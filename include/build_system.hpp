#include <unordered_map>
#include <filesystem>

class BuildSystem {
public:
    explicit BuildSystem(const std::string& sourceDir);
    void build();

private:
    std::string sourceDirectory;
    std::unordered_map<std::string, std::filesystem::file_time_type> cache;

    std::vector<std::string> getCppFiles();
    void compileFile(const std::string& filepath);
    void linkObjects(const std::vector<std::string>& objects);

    // BuildCache
    void loadCache();
    void saveCache();
    bool needsRebuild(const std::string& filepath);
};
