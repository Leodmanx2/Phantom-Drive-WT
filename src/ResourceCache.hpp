#ifndef PD_RESOURCECACHE_HPP
#define PD_RESOURCECACHE_HPP

#include <chrono>
#include <memory>
#include <string>
#include <unordered_map>

// TODO: Implement ResourceCache

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
	std::shared_ptr<T> get(std::string key);
	void               put(std::string key, std::shared_ptr<T> resource);
};

#endif
