#ifndef VOXELGAME_OPENGL_QT_MULTIMAP3D_H
#define VOXELGAME_OPENGL_QT_MULTIMAP3D_H

#include <cstddef>
#include <map>
#include <utility>
#include <array>
#include <list>


template<typename Key, typename T>
class multimap3d {
public:
	struct s_pos_wrapper;

	using inner_map = std::multimap<Key, s_pos_wrapper>;
	using value_type = std::pair<const std::array<Key, 3>, T>;
	using iterator = inner_map::iterator;

	struct t_pos_wrapper {
		T value;
		std::array<iterator, 3> pos;
	};

	using storage_t = std::list<t_pos_wrapper>;

	struct s_pos_wrapper {
		T& value;
		storage_t::iterator pos;
	};

	void insert(value_type&& value)
	{
		auto [key_x, key_y, key_z] = value.first;

		storage.push_front({std::move(value.second)});

		auto& ref = storage.front().value;
		auto s_pos = storage.begin();

		auto pos_x = inner_maps[0].insert({key_x, {ref, s_pos}});
		auto pos_y = inner_maps[1].insert({key_y, {ref, s_pos}});
		auto pos_z = inner_maps[2].insert({key_z, {ref, s_pos}});

		std::array t_pos{pos_x, pos_y, pos_z};

		storage.front().pos = t_pos;
	}

	template <size_t D>
	iterator begin()
	{
		return inner_maps[D].begin();
	}

	template <size_t D>
	iterator lower_bound(const Key& key)
	{
		return inner_maps[D].lower_bound(key);
	}

	template <size_t D>
	iterator upper_bound(const Key& key)
	{
		return inner_maps[D].upper_bound(key);
	}

	template <size_t D>
	iterator end()
	{
		return inner_maps[D].end();
	}

	template <size_t D>
	void erase(iterator first, iterator last)
	{
		for (auto it = first; it != last; ++it) {
			auto s_pos = it->second.pos;
			auto& t_pos = s_pos->pos;

			if constexpr (D != 0) inner_maps[0].erase(t_pos[0]);
			if constexpr (D != 1) inner_maps[1].erase(t_pos[1]);
			if constexpr (D != 2) inner_maps[2].erase(t_pos[2]);

			storage.erase(s_pos);
		}

		inner_maps[D].erase(first, last);
	}

private:
	std::array<inner_map, 3> inner_maps;
	storage_t storage;
};

#endif //VOXELGAME_OPENGL_QT_MULTIMAP3D_H
