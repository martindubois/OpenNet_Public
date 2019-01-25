
// Author   KMS - Martin Dubois, ing.
// Product  OpenNet
// File     Samples/Rada/Main.cpp

// Includes
/////////////////////////////////////////////////////////////////////////////

// ===== C ==================================================================
#include <stdint.h>

// ===== Windows ============================================================
#include <Windows.h>

// ===== Local ==============================================================
#include "Radar_View.h"

// Point d'entree
/////////////////////////////////////////////////////////////////////////////

int APIENTRY WinMain(HINSTANCE aInstance, HINSTANCE aPrevious, LPSTR aCommand, int aCmdShow)
{
    System::Windows::Forms::Application::Run(gcnew Radar::Radar_View());
}
