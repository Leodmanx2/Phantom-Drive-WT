#ifndef PD_RESOURCECACHE_HPP
#define PD_RESOURCECACHE_HPP

#include <chrono>
#include <memory>
#include <string>
#include <unordered_map>

template <typename T>
class ResourceCache {
	struct CacheEntry {
		std::shared_ptr<T>                                 resource;
		std::chrono::time_point<std::chrono::system_clock> accessed;
	};

	private:
	std::unordered_map<std::string, CacheEntry> m_map;

	void removeExpiredEntries();

	public:
	// TODO: Construct with default/placeholder resource
	std::shared_ptr<T> get(const std::string& key);
	void               put(const std::string& key, std::shared_ptr<T> resource);
};

template <typename T>
void ResourceCache<T>::removeExpiredEntries() {}

template <typename T>
std::shared_ptr<T> ResourceCache<T>::get(const std::string& key) {
	auto it = m_map.find(key);
	if(it != m_map.end()) { return it->second.resource; }
	return nullptr;
}

template <typename T>
void ResourceCache<T>::put(const std::string& key,
                           std::shared_ptr<T> resource) {
	CacheEntry entry{resource, std::chrono::system_clock::now()};
	m_map.insert({key, entry});
}

#endif
