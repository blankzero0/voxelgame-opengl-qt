#ifndef VOXELGAME_OPENGL_QT_INPUT_H
#define VOXELGAME_OPENGL_QT_INPUT_H

#include <optional>

class Input {
public:
	using setter = void (Input::*)(bool);

	[[nodiscard]] bool is_forward() const;
	void set_forward(bool forward);
	[[nodiscard]] bool is_backward() const;
	void set_backward(bool backward);
	[[nodiscard]] bool is_right() const;
	void set_right(bool right);
	[[nodiscard]] bool is_left() const;
	void set_left(bool left);
	[[nodiscard]] bool is_up() const;
	void set_up(bool up);
	[[nodiscard]] bool is_down() const;
	void set_down(bool down);
	[[nodiscard]] bool is_boost() const;
	void set_boost(bool boost);
	[[nodiscard]] bool is_destroy() const;
	void set_destroy(bool destroy);
	[[nodiscard]] bool is_place() const;
	void set_place(bool place);

	[[nodiscard]] static std::optional<Input::setter> get_setter_from_keycode(int key);

private:
	bool forward, backward, right, left, up, down, boost, destroy, place;
};


#endif //VOXELGAME_OPENGL_QT_INPUT_H
