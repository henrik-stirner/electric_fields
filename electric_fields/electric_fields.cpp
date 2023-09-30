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

	// Create a renderer, render window, and interactor
	vtkNew<vtkRenderer> renderer;
	renderer->SetBackground(colors->GetColor3d("BackgroundColor").GetData());

	vtkNew<vtkRenderWindow> renderWindow;
	renderWindow->AddRenderer(renderer);
	renderWindow->SetWindowName("Electric Fields");

	vtkNew<vtkRenderWindowInteractor> renderWindowInteractor;
	renderWindowInteractor->SetRenderWindow(renderWindow);

	// ==========
	
	// Create a cone
	vtkNew<vtkConeSource> coneSource;
	coneSource->Update();

	// Create a mapper and actor.
	vtkNew<vtkPolyDataMapper> mapper;
	mapper->SetInputConnection(coneSource->GetOutputPort());

	vtkNew<vtkActor> actor;
	actor->SetMapper(mapper);
	actor->GetProperty()->SetDiffuseColor(colors->GetColor3d("Bisque").GetData());

	// Add the actor to the scene
	renderer->AddActor(actor);

	// ==========

	// Render and interact
	renderWindow->Render();
	renderWindowInteractor->Start();

	return 0;
}
