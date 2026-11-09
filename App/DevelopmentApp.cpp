#include "DevelopmentApp.h"

DevelopmentApp::DevelopmentApp()
	: Application()
{
    Settings.AppName = L"Development Application";
    Settings.Viewport.Left = 300;
    Settings.Viewport.Top = 300;
    Settings.Viewport.Width = 800;
    Settings.Viewport.Height = 600;
}

void DevelopmentApp::Initialize()
{

}

void DevelopmentApp::Tick(double inDeltaTime)
{
	inDeltaTime;
}

void DevelopmentApp::Shutdown()
{

}