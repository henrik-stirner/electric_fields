// electric_fields.cpp : Defines the entry point for the application.
//

#include "electric_fields.h"


#define USER_MATRIX


int main()
{
	vtkNew<vtkNamedColors> colors;

	// Set the background color.
	std::array<unsigned char, 4> background_color{ {0, 0, 0, 255} };
	colors->SetColor("BackgroundColor", background_color.data());

	// ====================================================================================================
	// Create a renderer, render window, and interactor
	// ====================================================================================================

	vtkNew<vtkRenderer> renderer;
	renderer->SetBackground(colors->GetColor3d("BackgroundColor").GetData());

	vtkNew<vtkRenderWindow> renderWindow;
	renderWindow->AddRenderer(renderer);
	renderWindow->SetWindowName("Electric Fields");

	vtkNew<vtkRenderWindowInteractor> renderWindowInteractor;
	renderWindowInteractor->SetRenderWindow(renderWindow);

	// ====================================================================================================

	// ====================================================================================================
	// Create actors
	// ====================================================================================================

	// std::array<unsigned char, 4> electron_color{ {0, 0, 255, 255} };
	// colors->SetColor("ElectronColor", electron_color.data());
	// std::array<unsigned char, 4> proton_color{ {255, 0, 0, 255} };
	// colors->SetColor("ProtonColor", proton_color.data());

	Electron electron = Electron(25, 25, 25);
	vtkNew<vtkActor> electronActor = electron.getActor();
	colors->SetColor("ElectronColor", electron.getColor().data());
	electronActor->GetProperty()->SetColor(colors->GetColor3d("ElectronColor").GetData());
	renderer->AddActor(electronActor);

	Proton proton = Proton(75, 75, 75);
	vtkNew<vtkActor> protonActor = proton.getActor();
	colors->SetColor("ProtonColor", proton.getColor().data());
	protonActor->GetProperty()->SetColor(colors->GetColor3d("ProtonColor").GetData());
	renderer->AddActor(protonActor);

	std::vector<ChargedParticle> charges = {  };


	ElectricVectorField electricField = ElectricVectorField(10, 10, charges);
	for (ElectricFieldVector electric_field_vector : electricField.getVectors()) {
	 	renderer->AddActor(electric_field_vector.getActor());
	}
	
	// ====================================================================================================

	// ====================================================================================================
	// Render and interact
	// ====================================================================================================
	
	renderWindow->Render();
	renderWindowInteractor->Start();

	// ====================================================================================================

	return 0;
}
