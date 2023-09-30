#pragma once

#include "particle.h"

#include <array>
#include <vector>


class FieldVector {
private:
	int x1;
	int y1;
	int z1;

	int x2;
	int y2;
	int z2;

public:
	FieldVector(int x1_, int y1_, int z1_) {
		x1 = x1_;
		y1 = y1_;
		z1 = z1_;

		x2 = x1_;
		y2 = y1_;
		z2 = z1_;
	}

	void setOrigin(int x1_, int y1_, int z1_) {
		x1 = x1_;
		y1 = y1_;
		z1 = z1_;
	}

	int getX1() {
		return x1;
	}

	int getY1() {
		return y1;
	}

	int getZ1() {
		return z1;
	}

	void align(int x2_, int y2_, int z2_) {
		x2 = x2_;
		y2 = y2_;
		z2 = z2_;

		// TODO: align vector
	}

	// TODO: create actor; get_actor() function
};


class ElectricFieldVector : public FieldVector {
private:
	float charge = 1.0;
	std::array<int, 3> color = { 255, 255, 255 };

public:
	ElectricFieldVector(int x1_, int y1_, int z1_, std::vector<ChargedParticle> charges)
		: FieldVector(x1_, y1_, z1_) {
		update(charges);
	}

	void update(std::vector<ChargedParticle> charges) {
		// TODO: align vector according to charges
	}
};


class ElectricVectorField {
private:
	int fieldVectorsPerAxis;
	int distanceBetweenFieldVectors;

	std::vector<ElectricFieldVector> fieldVectors;

	void createVectors(std::vector<ChargedParticle> charges) {
		for (int nx; nx < fieldVectorsPerAxis; ++nx) {
			for (int ny; ny < fieldVectorsPerAxis; ++ny) {
				for (int nz; nz < fieldVectorsPerAxis; ++nz) {
					ElectricFieldVector new_vector = ElectricFieldVector(
						nx * distanceBetweenFieldVectors,
						ny * distanceBetweenFieldVectors,
						nz * distanceBetweenFieldVectors,
						charges
					);

					fieldVectors.push_back(new_vector);
				}
			}
		}
	}

public:
	ElectricVectorField(int fieldVectorsPerAxis_, int distanceBetweenFieldVectors_, std::vector<ChargedParticle> charges) {
		fieldVectorsPerAxis = fieldVectorsPerAxis_;
		distanceBetweenFieldVectors = distanceBetweenFieldVectors_;

		createVectors(charges);
	}

	void updateVectors(std::vector<ChargedParticle> charges) {
		for (ElectricFieldVector field_vector : fieldVectors) {
			field_vector.update(charges);
		}
	}
};
