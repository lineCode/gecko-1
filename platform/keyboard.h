
#pragma once

#include <functional>

namespace platform
{

////////////////////////////////////////

/// @brief Scan codes for keys
/// USB scan codes are used for keys
enum class scancode : uint32_t
{
	KEY_UNKNOWN = 0,

	KEY_A = 4,
	KEY_B = 5,
	KEY_C = 6,
	KEY_D = 7,
	KEY_E = 8,
	KEY_F = 9,
	KEY_G = 10,
	KEY_H = 11,
	KEY_I = 12,
	KEY_J = 13,
	KEY_K = 14,
	KEY_L = 15,
	KEY_M = 16,
	KEY_N = 17,
	KEY_O = 18,
	KEY_P = 19,
	KEY_Q = 20,
	KEY_R = 21,
	KEY_S = 22,
	KEY_T = 23,
	KEY_U = 24,
	KEY_V = 25,
	KEY_W = 26,
	KEY_X = 27,
	KEY_Y = 28,
	KEY_Z = 29,

	KEY_1 = 30,
	KEY_2 = 31,
	KEY_3 = 32,
	KEY_4 = 33,
	KEY_5 = 34,
	KEY_6 = 35,
	KEY_7 = 36,
	KEY_8 = 37,
	KEY_9 = 38,
	KEY_0 = 39,

	KEY_RETURN       = 40,
	KEY_ESCAPE       = 41,
	KEY_BACKSPACE    = 42,
	KEY_TAB          = 43,
	KEY_SPACE        = 44,
	KEY_MINUS        = 45,
	KEY_EQUALS       = 46,
	KEY_LEFTBRACKET  = 47,
	KEY_RIGHTBRACKET = 48,
	KEY_BACKSLASH    = 49,
	KEY_NONUSLASH    = 50,
	KEY_SEMICOLON    = 51,
	KEY_APOSTROPHE   = 52,
	KEY_GRAVE        = 53,
	KEY_COMMA        = 54,
	KEY_PERIOD       = 55,
	KEY_SLASH        = 56,
	KEY_CAPSLOCK     = 57,
	
	KEY_F1  = 58,
	KEY_F2  = 59,
	KEY_F3  = 60,
	KEY_F4  = 61,
	KEY_F5  = 62,
	KEY_F6  = 63,
	KEY_F7  = 64,
	KEY_F8  = 65,
	KEY_F9  = 66,
	KEY_F10 = 67,
	KEY_F11 = 68,
	KEY_F12 = 69,

	KEY_PRINTSCREEN = 70,
	KEY_SCROLLLOCK  = 71,
	KEY_PAUSE       = 72,
	KEY_INSERT      = 73,
	KEY_HOME        = 74,
	KEY_PAGEUP      = 75,
	KEY_DELETE      = 76,
	KEY_END         = 77,
	KEY_PAGEDOWN    = 78,
	KEY_RIGHT       = 79,
	KEY_LEFT        = 80,
	KEY_DOWN        = 81,
	KEY_UP          = 82,

	KEY_NUMLOCK     = 83,
	KEY_KP_DIVIDE   = 84,
	KEY_KP_MULTIPLY = 85,
	KEY_KP_MINUS    = 86,
	KEY_KP_PLUS     = 87,
	KEY_KP_ENTER    = 88,
	KEY_KP_1        = 89,
	KEY_KP_2        = 90,
	KEY_KP_3        = 91,
	KEY_KP_4        = 92,
	KEY_KP_5        = 93,
	KEY_KP_6        = 94,
	KEY_KP_7        = 95,
	KEY_KP_8        = 96,
	KEY_KP_9        = 97,
	KEY_KP_0        = 98,
	KEY_KP_PERIOD   = 99,

	KEY_NONUBACKSLASH = 100,
	KEY_APPLICATION   = 101,
	KEY_POWER         = 102,
	KEY_KP_EQUAL      = 103,

	KEY_F13 = 104,
	KEY_F14 = 105,
	KEY_F15 = 106,
	KEY_F16 = 107,
	KEY_F17 = 108,
	KEY_F18 = 109,
	KEY_F19 = 110,
	KEY_F20 = 111,
	KEY_F21 = 112,
	KEY_F22 = 113,
	KEY_F23 = 114,
	KEY_F24 = 115,

	KEY_EXECUTE    = 116,
	KEY_HELP       = 117,
	KEY_MENU       = 118,
	KEY_SELECT     = 119,
	KEY_STOP       = 120,
	KEY_AGAIN      = 121,
	KEY_UNDO       = 122,
	KEY_CUT        = 123,
	KEY_COPY       = 124,
	KEY_PASTE      = 125,
	KEY_FIND       = 126,
	KEY_MUTE       = 127,
	KEY_VOLUMEUP   = 128,
	KEY_VOLUMEDOWN = 129,

	KEY_LCTRL      = 224,
	KEY_LSHIFT     = 225,
	KEY_LALT       = 226,
	KEY_LGUI       = 227,
	KEY_RCTRL      = 228,
	KEY_RSHIFT     = 229,
	KEY_RALT       = 230,
	KEY_RGUI       = 231,
};

////////////////////////////////////////

/// @brief Keyboard device
/// A keyboard device that can send key press/release events.
class keyboard
{
public:
	/// @brief Constructor
	keyboard( void );

	/// @brief Destructor
	virtual ~keyboard( void );

	/// @brief Set the callback for press events
	/// Set the callback function for key press events.
	/// @param f Function to call
	void when_pressed( std::function<void(scancode)> f ) { _pressed = f; }

	/// @brief Set the callback for release events
	/// Set the callback function for key release events.
	/// @param f Function to call
	void when_released( std::function<void(scancode)> f ) { _released = f; }

	/// @brief Send a key press event
	/// Send a key press event with the given scan code.
	/// @param sc Key which was pressed
	void pressed( scancode sc ) { if ( _pressed ) _pressed( sc ); }

	/// @brief Send a key release event
	/// Send a key release event with the given scan code.
	/// @param sc Key which was release
	void released( scancode sc ) { if ( _released ) _released( sc ); }

private:
	std::function<void(scancode)> _pressed;
	std::function<void(scancode)> _released;
};

////////////////////////////////////////

}

