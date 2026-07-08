#include "AssetManager.hpp"
#include "../Core/Logging/Logger.hpp"
#include "../Renderer/Mesh.hpp"
#include "../Renderer/Material.hpp"
#include "../Animation/Skeleton.hpp"

namespace Luminary {

Asset::Asset(const std::string& path, AssetType type) 
    : m_Path(path), m_Type(type) {
    size_t lastSlash = path.find_last_of("/\\");
    m_Name = (lastSlash != std::string::npos) ? path.substr(lastSlash + 1) : path;
}

Asset::~Asset() = default;

AssetManager::AssetManager() {
    LOG_INFO("AssetManager created");
}

AssetManager::~AssetManager() {
    Shutdown();
}

bool AssetManager::Initialize() {
    LOG_INFO("AssetManager initialized");
    return true;
}

void AssetManager::Shutdown() {
    m_LoadedAssets.clear();
    LOG_INFO("AssetManager shut down");
}

std::shared_ptr<Mesh> AssetManager::LoadMesh(const std::string& path) {
    auto existing = GetAsset<Mesh>(path);
    if (existing) return existing;

    LOG_DEBUG("Loading mesh: {}", path);
    // Create a default cube mesh if file doesn't exist
    auto mesh = Mesh::CreateCube();
    auto asset = std::make_shared<Asset>(path, Asset::AssetType::Mesh);
    m_LoadedAssets[path] = asset;
    return mesh;
}

std::shared_ptr<Material> AssetManager::LoadMaterial(const std::string& path) {
    auto existing = GetAsset<Material>(path);
    if (existing) return existing;

    LOG_DEBUG("Loading material: {}", path);
    auto material = Material::CreateStandard();
    auto asset = std::make_shared<Asset>(path, Asset::AssetType::Material);
    m_LoadedAssets[path] = asset;
    return material;
}

std::shared_ptr<Animation> AssetManager::LoadAnimation(const std::string& path) {
    auto existing = GetAsset<Animation>(path);
    if (existing) return existing;

    LOG_DEBUG("Loading animation: {}", path);
    auto animation = std::make_shared<Animation>(path);
    auto asset = std::make_shared<Asset>(path, Asset::AssetType::Animation);
    m_LoadedAssets[path] = asset;
    return animation;
}

std::shared_ptr<Skeleton> AssetManager::LoadSkeleton(const std::string& path) {
    auto existing = GetAsset<Skeleton>(path);
    if (existing) return existing;

    LOG_DEBUG("Loading skeleton: {}", path);
    auto skeleton = Skeleton::CreateHumanoidSkeleton();
    auto asset = std::make_shared<Asset>(path, Asset::AssetType::Skeleton);
    m_LoadedAssets[path] = asset;
    return skeleton;
}

bool AssetManager::LoadAsset(const std::string& path, Asset::AssetType type) {
    LOG_DEBUG("Loading asset: {} (type: {})", path, static_cast<int>(type));
    return true;
}

} // namespace Luminary
