#pragma once

#include <memory>
#include <string>
#include <map>
#include <vector>

namespace Luminary {

class Mesh;
class Material;
class Animation;
class Skeleton;
class Texture;

class Asset {
public:
    enum class AssetType {
        Mesh,
        Material,
        Texture,
        Animation,
        Skeleton,
        Model,
        Audio,
        Unknown
    };

    Asset(const std::string& path, AssetType type);
    virtual ~Asset();

    const std::string& GetPath() const { return m_Path; }
    AssetType GetType() const { return m_Type; }
    const std::string& GetName() const { return m_Name; }
    bool IsLoaded() const { return m_IsLoaded; }

private:
    std::string m_Path;
    std::string m_Name;
    AssetType m_Type;
    bool m_IsLoaded = false;
};

class AssetManager {
public:
    AssetManager();
    ~AssetManager();

    bool Initialize();
    void Shutdown();

    std::shared_ptr<Mesh> LoadMesh(const std::string& path);
    std::shared_ptr<Material> LoadMaterial(const std::string& path);
    std::shared_ptr<Animation> LoadAnimation(const std::string& path);
    std::shared_ptr<Skeleton> LoadSkeleton(const std::string& path);

    template<typename T>
    std::shared_ptr<T> GetAsset(const std::string& path) {
        auto it = m_LoadedAssets.find(path);
        if (it != m_LoadedAssets.end()) {
            return std::dynamic_pointer_cast<T>(it->second);
        }
        return nullptr;
    }

private:
    std::map<std::string, std::shared_ptr<Asset>> m_LoadedAssets;

    bool LoadAsset(const std::string& path, Asset::AssetType type);
};

} // namespace Luminary
