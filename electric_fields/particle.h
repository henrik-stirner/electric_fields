#pragma once

#include <array>

#include <vtkNew.h>
#include <vtkNamedColors.h>

#include <vtkPolyDataMapper.h>

#include <vtkActor.h>
#include <vtkProperty.h>
#include <vtkSphereSource.h>

class Particle {
protected:
	double x;
	double y;
	double z;

	std::array<unsigned char, 4> color{ {255, 255, 255, 255} };

public:
	Particle(double x_, double y_, double z_) {
		x = x_;
		y = y_;
		z = z_;
	}

	void setCoordinates(double x_, double y_, double z_) {
		x = x_;
		y = y_;
		z = z_;
	}

	double getX() {
		return x;
	}

	double getY() {
		return y;
	}

	double getZ() {
		return z;
	}

	std::array<unsigned char, 4> getColor() {
		return color;
	}

	vtkNew<vtkActor> getActor() {
		vtkNew<vtkSphereSource> sphereSource;
		sphereSource->SetCenter(0.0, 0.0, 0.0);
		sphereSource->SetRadius(2.0);
		sphereSource->SetPhiResolution(100);
		sphereSource->SetThetaResolution(100);

		vtkNew<vtkPolyDataMapper> mapper;
		mapper->SetInputConnection(sphereSource->GetOutputPort());

		vtkNew<vtkActor> actor;
		actor->SetMapper(mapper);
		actor->SetPosition(x, y, z);

		vtkNew<vtkNamedColors> colors;
		colors->SetColor("ParticleColor", color.data());
		actor->GetProperty()->SetColor(colors->GetColor3d("ParticleColor").GetData());

		return actor;
	}
};


class ChargedParticle : public Particle {
protected:
	double charge;

	void colorByCharge() {
		std::array<unsigned char, 4> color_negative = { 0, 0, 255, 255 };
		std::array<unsigned char, 4> color_positive = { 255, 0, 0, 255 };

		for (int i = 0; i < 3; ++i) {
			color[i] = color_negative.at(i) * (1 - charge) + color_positive.at(i) * charge;
		}
	}

public:
	ChargedParticle(int x_, int y_, int z_, float charge_)
		: Particle(x_, y_, z_) {
		charge = charge_;

		colorByCharge();
	}

	double getCharge() {
		return charge;
	}


};


class Electron : public ChargedParticle {
public:
	Electron(double x_, double y_, double z_)
		: ChargedParticle(x_, y_, z_, 0) {
	}
};

class Proton : public ChargedParticle {
public:
	Proton(double x_, double y_, double z_)
		: ChargedParticle(x_, y_, z_, 1) {
	}
};
