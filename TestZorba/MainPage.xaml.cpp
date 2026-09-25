//
// MainPage.xaml.cpp
// Implementation of the MainPage class.
//

#include "pch.h"
#include "MainPage.xaml.h"

#include <iostream>
#include <sstream>

using namespace TestZorba;

using namespace Platform;
using namespace Windows::Foundation;
using namespace Windows::Foundation::Collections;
using namespace Windows::UI::Xaml;
using namespace Windows::UI::Xaml::Controls;
using namespace Windows::UI::Xaml::Controls::Primitives;
using namespace Windows::UI::Xaml::Data;
using namespace Windows::UI::Xaml::Input;
using namespace Windows::UI::Xaml::Media;
using namespace Windows::UI::Xaml::Navigation;

// The Blank Page item template is documented at https://go.microsoft.com/fwlink/?LinkId=402352&clcid=0x409

MainPage::MainPage()
{
	InitializeComponent();
	lStore = StoreManager::getStore();
	lZorba = Zorba::getInstance(lStore);
}

MainPage::~MainPage()
{
	lZorba->shutdown();
	StoreManager::shutdownStore(lStore);
}

void TestZorba::MainPage::Button_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{
	// 1. Veiligheid: Controleer of het invoerveld niet leeg is
	if (xquery->Text == nullptr || xquery->Text->IsEmpty())
	{
		result->Text = "Voer aanzien eerst een geldige XQuery in.";
		return;
	}

	try
	{
		// 2. CONVERTEER Platform::String^ (UWP) naar std::string (Zorba)
		// We gebruiken de UTF-8 helper-pijplijn om te zorgen dat speciale karakters behouden blijven
		std::wstring wstr(xquery->Text->Data());
		std::string strQuery(wstr.begin(), wstr.end());

		// 3. Compilereer en voer de XQuery uit in Zorba
		XQuery_t lQuery = lZorba->compileQuery(strQuery);

		// We vangen de output van de query op via een stringstream
		std::stringstream ss;
		ss << lQuery; // Voert de query uit en schrijft het resultaat (meestal XML/JSON of text) naar de stream

		std::string sResult = ss.str();

		// 4. CONVERTEER std::string (Zorba) terug naar Platform::String^ (UWP)
		// We zetten de string om naar een wide-string en stoppen hem in een Platform::String container
		std::wstring wResult(sResult.begin(), sResult.end());
		Platform::String^ pResult = ref new Platform::String(wResult.c_str());

		// 5. Vul het Result TextBox-veld op het scherm van de Lumia!
		result->Text = pResult;
	}
	catch (const std::exception& ex)
	{
		// Vang standaard C++ fouten uit de Zorba engine op (bijv. syntaxfouten in de XQuery)
		std::string sError = "Zorba Fout: " + std::string(ex.what());
		std::wstring wError(sError.begin(), sError.end());
		result->Text = ref new Platform::String(wError.c_str());
	}
	catch (Platform::Exception^ ex)
	{
		// Vang eventuele UWP systeemfouten op
		result->Text = "UWP Systeemfout: " + ex->Message;
	}
}