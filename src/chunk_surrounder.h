#ifndef VOXELGAME_OPENGL_QT_CHUNK_SURROUNDER_H
#define VOXELGAME_OPENGL_QT_CHUNK_SURROUNDER_H


#include <optional>
#include <cmath>
#include <unordered_set>
#include "geometry.h"
#include "chunk.h"


class CenterChangeListener {
public:
	CenterChangeListener() = default;
	CenterChangeListener(CenterChangeListener&) = delete;
	CenterChangeListener(CenterChangeListener&&) = delete;
	CenterChangeListener& operator=(CenterChangeListener&) = delete;
	CenterChangeListener& operator=(CenterChangeListener&&) = delete;
	virtual ~CenterChangeListener() = default;

	virtual void on_center_changed(const ChunkPosition& center) = 0;
};

class ChunkRequester {
public:
	ChunkRequester() = default;
	ChunkRequester(ChunkRequester&) = delete;
	ChunkRequester(ChunkRequester&&) = delete;
	ChunkRequester& operator=(ChunkRequester&) = delete;
	ChunkRequester& operator=(ChunkRequester&&) = delete;
	virtual ~ChunkRequester() = default;

	virtual void request_chunk(const ChunkPosition& center) = 0;
};

class ChunkSurrounder {
public:
	void set_center(const Point& center);
	ChunkSurrounder(
			unsigned int view_distance,
			ChunkRequester& chunk_requester, CenterChangeListener& center_change_listener
	);

private:
	unsigned int view_distance;
	CenterChangeListener& center_change_listener;
	ChunkRequester& chunk_requester;
	std::optional<ChunkPosition> last_center;
};


#endif //VOXELGAME_OPENGL_QT_CHUNK_SURROUNDER_H
