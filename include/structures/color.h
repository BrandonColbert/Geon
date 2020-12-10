#pragma once

#include <SDL2/SDL.h>

class Color {
	public:
		static Color red, green, blue, white, black;

		float r, g, b, a;

		Color(float r, float g, float b);
		Color(float r, float g, float b, float a);
		Color(int rgba);

		int getRGB() const;
		int getRGBA() const;
		int getARGB() const;

		Color operator+(const Color&);
		Color& operator+=(const Color&);

		Color operator-(const Color&);
		Color& operator-=(const Color&);

		Color operator*(const Color&);
		Color& operator*=(const Color&);

		Color operator/(const Color&);
		Color& operator/=(const Color&);

		Color operator*(const float);
		Color& operator*=(const float);
		friend Color operator*(float, const Color&);

		Color operator/(const float);
		Color& operator/=(const float);

		operator SDL_Color() const;

		static Color lerp(Color a, Color b, float t);
};