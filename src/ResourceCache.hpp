#ifndef RESOURCECACHE_H
#define RESOURCECACHE_H

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
	std::shared_ptr<T> get(std::string key);
	void               put(std::string key, std::shared_ptr<T> resource);
};

#endif
