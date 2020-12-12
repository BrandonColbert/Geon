#include "engine.h"

using namespace std;
using Input = Engine::Input;
using Display = Engine::Display;

Property<Vector2> Input::mousePosition([]() {
	int x, y;
	SDL_GetMouseState(&x, &y);

	auto screenSize = Display::getScreenSize();

	return Vector2(x - screenSize.x / 2, screenSize.y / 2 - y);
});

bool Input::down(string key) {
	return Input::down(keyToCode[key]);
}

bool Input::held(string key) {
	return Input::held(keyToCode[key]);
}

bool Input::up(string key) {
	return Input::up(keyToCode[key]);
}

bool Input::down(int keyCode) {
	auto keys = Engine::getInstance().downKeys;
	return keys.find(keyCode) != keys.end();
}

bool Input::held(int keyCode) {
	auto keys = Engine::getInstance().heldKeys;
	return keys.find(keyCode) != keys.end();
}

bool Input::up(int keyCode) {
	auto keys = Engine::getInstance().upKeys;
	return keys.find(keyCode) != keys.end();
}

bool Input::IgnoreCaseComparator::operator()(const string &lhs, const string &rhs) const {
	return lexicographical_compare(
		lhs.begin(), lhs.end(),
		rhs.begin(), rhs.end(),
		[](char l, char r) {
			return tolower(l) < tolower(r);
		}
	);
}

map<string, int, Input::IgnoreCaseComparator> Input::keyToCode{
	{"LShift", 0x400000E1},
	{"RShift", 0x400000E5},
	{"LCtrl", 0x400000E0},
	{"RCtrl", 0x400000E4},
	{"LAlt", 0x400000E2},
	{"RAlt", 0x400000E6},
	{"Escape", 0x1B},
	{"Space", ' '},
	{"LeftArrow", 0x40000050},
	{"UpArrow", 0x40000052},
	{"RightArrow", 0x4000004F},
	{"DownArrow", 0x40000051},
	{"Minus", 0x2D},
	{"Equals", 0x3d},
	{"0", '0'},
	{"1", '1'},
	{"2", '2'},
	{"3", '3'},
	{"4", '4'},
	{"5", '5'},
	{"6", '6'},
	{"7", '7'},
	{"8", '8'},
	{"9", '9'},
	{"A", 'a'},
	{"B", 'b'},
	{"C", 'c'},
	{"D", 'd'},
	{"E", 'e'},
	{"F", 'f'},
	{"G", 'g'},
	{"H", 'h'},
	{"I", 'i'},
	{"J", 'j'},
	{"K", 'k'},
	{"L", 'l'},
	{"M", 'm'},
	{"N", 'n'},
	{"O", 'o'},
	{"P", 'p'},
	{"Q", 'q'},
	{"R", 'r'},
	{"S", 's'},
	{"T", 't'},
	{"U", 'u'},
	{"V", 'v'},
	{"W", 'w'},
	{"X", 'x'},
	{"Y", 'y'},
	{"Z", 'z'}
};