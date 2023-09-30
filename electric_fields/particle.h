#pragma once

#include <array>


class Particle {
private:
	int x;
	int y;
	int z;

public:
	Particle(int x_, int y_, int z_) {
		x = x_;
		y = y_;
		z = z_;
	}

	void setCoordinates(int x_, int y_, int z_) {
		x = x_;
		y = y_;
		z = z_;
	}

	int getX() {
		return x;
	}

	int getY() {
		return y;
	}

	int getZ() {
		return z;
	}

	// TODO: create actor; getActor() function
};


class ChargedParticle : public Particle {
private:
	float charge;
	std::array<int, 3> color{};

	std::array<int, 3> createColor() {
		std::array<int, 3> color_negative = { 0, 0, 255 };
		std::array<int, 3> color_positive = { 255, 0, 0 };

		for (int i = 0; i < 3; ++i) {
			color[i] = color_negative.at(i) * (1 - charge) + color_positive.at(i) * charge;
		}

		return color;
	}

public:
	ChargedParticle(int x_, int y_, int z_, float charge_)
		: Particle(x_, y_, z_) {
		charge = charge_;
		color = createColor();
	}

	std::array<int, 3> getColor() {
		return color;
	}
};


class Electron : ChargedParticle {
public:
	Electron(int x_, int y_, int z_)
		: ChargedParticle(x_, y_, z_, 0) {
	}
};

class Proton : ChargedParticle {
public:
	Proton(int x_, int y_, int z_)
		: ChargedParticle(x_, y_, z_, 1) {
	}
};
