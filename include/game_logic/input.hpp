#ifndef PD_INPUT_HPP
#define PD_INPUT_HPP

struct KeyEvent {
	int key;
	int action;
	int modifiers;
};

// MouseMovementEvent is effectively a displacement vector. Its member 'dx' is
// the horizontal change in position. Similarly, 'dy' is the vertical component
struct MouseMovementEvent {
	double dx;
	double dy;
};

struct MouseButtonEvent {
	int button;
	int action;
	int modifiers;
};

#endif
