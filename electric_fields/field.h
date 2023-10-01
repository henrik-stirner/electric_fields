#pragma once

#include "util.h"
#include "particle.h"

#include <array>
#include <vector>

#include <cmath>

#include <vtkNew.h>
#include <vtkNamedColors.h>

#include <vtkPolyDataMapper.h>

#include <vtkActor.h>
#include <vtkProperty.h>
#include <vtkArrowSource.h>

#include <vtkMath.h>
#include <vtkMinimalStandardRandomSequence.h>

#include <vtkTransform.h>
#include <vtkTransformPolyDataFilter.h>


class FieldVector {
protected:
	double x1;
	double y1;
	double z1;

	double x2;
	double y2;
	double z2;

	double rot_x;
	double rot_y;

	std::array<unsigned char, 4> color{ {255, 255, 255, 255} };

public:
	FieldVector(double x1_, double y1_, double z1_) {
		x1 = x1_;
		y1 = y1_;
		z1 = z1_;

		x2 = x1_ + 5;
		y2 = y1_;
		z2 = z1_;

		rot_x = 0;
		rot_y = 0;
	}

	void setOrigin(double x1_, double y1_, double z1_) {
		x1 = x1_;
		y1 = y1_;
		z1 = z1_;
	}

	double getX1() {
		return x1;
	}

	double getY1() {
		return y1;
	}

	double getZ1() {
		return z1;
	}

	void align(double x2_, double y2_, double z2_) {
		x2 = x2_;
		y2 = y2_;
		z2 = z2_;
	}

	double getX2() {
		return x2;
	}

	double getY2() {
		return y2;
	}

	double getZ2() {
		return z2;
	}

	void rotate(double rot_x_, double rot_y_) {
		rot_x = rot_x_;
		rot_y = rot_y_;
	}

	double getRotX() {
		return rot_x;
	}

	double getRotY() {
		return rot_y;
	}

	std::array<unsigned char, 4> getColor() {
		return color;
	}

	vtkNew<vtkActor> getActor() {
		vtkNew<vtkArrowSource> arrowSource;
		arrowSource->SetTipResolution(4);
		arrowSource->SetShaftResolution(4);
		arrowSource->Update();

		vtkNew<vtkPolyDataMapper> mapper;
		mapper->SetInputConnection(arrowSource->GetOutputPort());

		// ====================================================================================================
		// align
		// ====================================================================================================

		vtkNew<vtkMinimalStandardRandomSequence> rng;

		double startPoint[3] = {x1, y1, z1};
		double endPoint[3] = {x2, y2, z2};

		// Compute a basis
		double normalizedX[3];
		double normalizedY[3];
		double normalizedZ[3];

		// The X axis is a vector from start to end
		vtkMath::Subtract(endPoint, startPoint, normalizedX);
		double length = vtkMath::Norm(normalizedX);
		vtkMath::Normalize(normalizedX);

		// The Z axis is an arbitrary vector cross X
		double arbitrary[3];
		for (auto i = 0; i < 3; ++i)
		{
			rng->Next();
			arbitrary[i] = rng->GetRangeValue(-10, 10);
		}
		vtkMath::Cross(normalizedX, arbitrary, normalizedZ);
		vtkMath::Normalize(normalizedZ);

		// The Y axis is Z cross X
		vtkMath::Cross(normalizedZ, normalizedX, normalizedY);
		vtkNew<vtkMatrix4x4> matrix;

		// Create the direction cosine matrix
		matrix->Identity();
		for (auto i = 0; i < 3; i++)
		{
			matrix->SetElement(i, 0, normalizedX[i]);
			matrix->SetElement(i, 1, normalizedY[i]);
			matrix->SetElement(i, 2, normalizedZ[i]);
		}

		// Apply the transforms
		vtkNew<vtkTransform> transform;
		transform->Translate(startPoint);
		transform->Concatenate(matrix);
		transform->Scale(length, length, length);

		// Transform the polydata
		vtkNew<vtkTransformPolyDataFilter> transformPolyDataFilter;
		transformPolyDataFilter->SetTransform(transform);
		transformPolyDataFilter->SetInputConnection(arrowSource->GetOutputPort());

#ifdef USER_MATRIX
		mapper->SetInputConnection(arrowSource->GetOutputPort());
		actor->SetUserMatrix(transform->GetMatrix());
#else
		mapper->SetInputConnection(transformPolyDataFilter->GetOutputPort());
#endif

		// ====================================================================================================

		vtkNew<vtkActor> actor;
		actor->SetMapper(mapper);

		// Rotate
		actor->RotateX(rot_x);
		actor->RotateY(rot_y);

		vtkNew<vtkNamedColors> colors;
		colors->SetColor("FieldVectorColor", color.data());
		actor->GetProperty()->SetColor(colors->GetColor3d("FieldVectorColor").GetData());

		return actor;
	}
};


class ElectricFieldVector : public FieldVector {
protected:
	double charge = 1.0;
	double forceOfAttraction[3];

public:
	ElectricFieldVector(double x1_, double y1_, double z1_, std::vector<ChargedParticle> charges)
		: FieldVector(x1_, y1_, z1_) {
		color = { 255, 255, 255, 255 };

		align(charges);
	}


	void align(std::vector<ChargedParticle> charges) {
		double start_point[3] = { x1, y1, z1 };
		for (ChargedParticle charged_particle : charges) {
			double end_point[3] = { charged_particle.getX(), charged_particle.getY(), charged_particle.getZ() };

			// Radius
			double r[3]; 
			vtkMath::Subtract(end_point, start_point, r);

			// Attraction to charge
			double dot[3];
			f_el(charge, charged_particle.getCharge(), r, dot);

			// Add up all forces
			vtkMath::Add(forceOfAttraction, dot, forceOfAttraction);
		}

		std::cout << 
			forceOfAttraction[0] << " | " <<
			forceOfAttraction[1] << " | " <<
			forceOfAttraction[2] << " | " <<
			std::endl;


		double length_start_point = sqrt(pow(start_point[0], 2) + pow(start_point[1], 2) + pow(start_point[2], 2));
		double rot_x_start_point = acos(start_point[0] / length_start_point);
		double rot_y_start_point = acos(start_point[1] / length_start_point);
		
		double length_forceOfAttraction = sqrt(pow(forceOfAttraction[0], 2) + pow(forceOfAttraction[1], 2) + pow(forceOfAttraction[2], 2));
		double rot_x_forceOfAttraction = acos(forceOfAttraction[0] / length_forceOfAttraction);
		double rot_y_forceOfAttraction = acos(forceOfAttraction[1] / length_forceOfAttraction);
		
		double rot_x = rot_x_forceOfAttraction - rot_x_start_point;
		double rot_y = rot_y_forceOfAttraction - rot_y_start_point;

		std::cout <<
			rot_x << " | " <<
			rot_y << " | " <<
			std::endl;

		FieldVector::rotate(rot_x, rot_y);
	}
};


class ElectricVectorField {
protected:
	int fieldVectorsPerAxis;
	int distanceBetweenFieldVectors;

	std::vector<ElectricFieldVector> fieldVectors;

	void createVectors(std::vector<ChargedParticle> charges) {
		for (int nx = 0; nx < fieldVectorsPerAxis; ++nx) {
			for (int ny = 0; ny < fieldVectorsPerAxis; ++ny) {
				for (int nz = 0; nz < fieldVectorsPerAxis; ++nz) {
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

	std::vector<ElectricFieldVector> getVectors() {
		return fieldVectors;
	}

	void alignVectors(std::vector<ChargedParticle> charges) {
		for (ElectricFieldVector field_vector : fieldVectors) {
			field_vector.align(charges);
		}
	}
};
