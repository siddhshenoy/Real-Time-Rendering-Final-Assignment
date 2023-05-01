#pragma once

#include <unordered_map>
#include <map>
template<class K, class V>
class Map
{
private:
	std::map<K, V> map;
public:
	Map();
	V Get(K k);
	void Add(K k, V v);
	bool HasKey(K k);
	void Set(K k, V v);
	~Map();
};

template<class K, class V>
Map<K, V>::Map() {
}
template<class K, class V>
V Map<K, V>::Get(K k) {
	if (map.find(k) != map.end()) return map[k];
	else return (V)nullptr;
}
template<class K, class V>
void Map<K, V>::Add(K k, V v) {
	map.insert(std::pair<K, V>(k, v));
}

template<class K, class V>
bool Map<K, V>::HasKey(K k) {

	return (map.find(k) != map.end());
}

template<class K, class V>
inline void Map<K, V>::Set(K k, V v)
{
	if (HasKey(k))
		map[k] = v;
	else
		Add(k, v);
}

template<class K, class V>
Map<K, V>::~Map() {
	map.erase(map.begin(), map.end());
}