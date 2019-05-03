#pragma comment(linker,"\"/manifestdependency:type='win32' \
name='Microsoft.Windows.Common-Controls' version='6.0.0.0' \
processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
#include <windows.h>
#include "train.h"
#include <ctime>
#include <iomanip>
#include <sstream>

LONG WINAPI WndProc(HWND, UINT, WPARAM, LPARAM);

bool is_date_in_range(tm from, tm to, tm val)
{
	val.tm_hour = val.tm_min = 0;
	return mktime(&from) <= mktime(&val) && mktime(&val) <= mktime(&to);
}

static HWND SearchControl, OutputControl, FromDateControl, ToDateControl, SearchControl1;
static timetable t;

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)

{
	std::ifstream file("train.csv");
	while (!file.eof())
	{
		file >> t;
	}
	t.sort_by_date();
	HWND hwnd;
	MSG msg;
	WNDCLASS w;
	memset(&w, 0, sizeof(WNDCLASS));
	w.style = CS_HREDRAW | CS_VREDRAW;
	w.lpfnWndProc = WndProc;
	w.hInstance = hInstance;
	w.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	w.lpszClassName = "My Class";
	RegisterClass(&w);
	hwnd = CreateWindow("My Class", "Train Timetable", WS_OVERLAPPEDWINDOW, 300, 200, 415, 380, NULL, NULL, hInstance, NULL);
	FromDateControl = CreateWindowExA(NULL, "SysDateTimePick32", nullptr, WS_BORDER | WS_CHILD | WS_VISIBLE,
		0, 0, 200, 40, hwnd, nullptr, nullptr, nullptr);
	ToDateControl = CreateWindowExA(NULL, "SysDateTimePick32", nullptr, WS_BORDER | WS_CHILD | WS_VISIBLE,
		200, 0, 200, 40, hwnd, nullptr, nullptr, nullptr);
	SearchControl = CreateWindowExA(NULL, "edit", nullptr, WS_CHILD | WS_VISIBLE | WS_BORDER,
		100, 40, 350, 40, hwnd, nullptr, nullptr, nullptr);
	SearchControl1 = CreateWindowExA(NULL, "edit", nullptr, WS_CHILD | WS_VISIBLE | WS_BORDER,
		100, 80, 350, 40, hwnd, nullptr, nullptr, nullptr);
	CreateWindowExA(NULL, "edit", "Departure city:", WS_CHILD | WS_VISIBLE | WS_BORDER | ES_READONLY,
		0, 40, 100, 40, hwnd, nullptr, nullptr, nullptr);
	CreateWindowExA(NULL, "edit", "Arrival city:", WS_CHILD | WS_VISIBLE | WS_BORDER | ES_READONLY,
		0, 80, 100, 40, hwnd, nullptr, nullptr, nullptr);
	OutputControl = CreateWindowExA(NULL, "edit", nullptr, WS_CHILD | WS_VISIBLE | WS_BORDER | WS_EX_CLIENTEDGE | ES_MULTILINE | ES_AUTOVSCROLL | WS_VSCROLL | ES_READONLY,
		0, 120, 400, 180, hwnd, nullptr, nullptr, nullptr);
	CreateWindowExA(NULL, "button", "Search", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		0, 300, 100, 40, hwnd, reinterpret_cast<HMENU>(1), nullptr, nullptr);
	CreateWindowExA(NULL, "button", "Show all", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		300, 300, 100, 40, hwnd, reinterpret_cast<HMENU>(3), nullptr, nullptr);
	ShowWindow(hwnd, nCmdShow);
	UpdateWindow(hwnd);
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return msg.wParam;
}

LONG WINAPI WndProc(HWND hwnd, UINT Message, WPARAM wparam, LPARAM lparam)

{
	switch (Message) {
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	case WM_COMMAND:
		if (LOWORD(wparam) == 1)
		{
			tm date_from, date_to;
			const auto date_from_s = new char[100];
			GetWindowTextA(FromDateControl, date_from_s, 100);
			std::stringstream ss1(date_from_s);
			ss1 >> std::get_time(&(date_from), "%d-%B-%Y");
			const auto date_to_s = new char[100];
			GetWindowTextA(ToDateControl, date_to_s, 100);
			std::stringstream ss2(date_to_s);
			ss2 >> std::get_time(&(date_to), "%d-%B-%Y");
			date_from.tm_hour = date_from.tm_min = date_to.tm_hour = date_to.tm_min = 0;

			const auto text_length = GetWindowTextLengthA(SearchControl) + 1;
			const auto search_text = new char[text_length];
			GetWindowTextA(SearchControl, search_text, text_length);

			const auto text_length1 = GetWindowTextLengthA(SearchControl1) + 1;
			const auto search_text1 = new char[text_length1];
			GetWindowTextA(SearchControl1, search_text1, text_length1);

			std::string output_text;
			char date[20];
			char date1[20];

			t.sort_by_date();
			for (const auto& e : t)
			{
				if ((text_length == 0 || e.departure_city.find(search_text) != std::string::npos) && (text_length1 == 0 || e.arrival_city.find(search_text1) != std::string::npos) && is_date_in_range(date_from, date_to, e.departure_date))
				{
					strftime(date, 20, "%b %d %Y %H:%M", &(e.departure_date));
					strftime(date1, 20, "%b %d %Y %H:%M", &(e.arrival_date));
					output_text += e.departure_city + " " + date + " " + e.arrival_city + " " + date1 + "\r\n" + std::to_string(e.capacity) + " " + std::to_string(e.tickets_left) + "\r\n";
				}
			}
			SetWindowTextA(OutputControl, output_text.c_str());
		}
		if (LOWORD(wparam) == 3)
		{
			tm date_from, date_to;
			const auto date_from_s = new char[100];
			GetWindowTextA(FromDateControl, date_from_s, 100);
			std::stringstream ss1(date_from_s);
			ss1 >> std::get_time(&(date_from), "%d-%B-%Y");
			const auto date_to_s = new char[100];
			GetWindowTextA(ToDateControl, date_to_s, 100);
			std::stringstream ss2(date_to_s);
			ss2 >> std::get_time(&(date_to), "%d-%B-%Y");
			date_from.tm_hour = date_from.tm_min = date_to.tm_hour = date_to.tm_min = 0;

			const auto text_length = GetWindowTextLengthA(SearchControl) + 1;
			const auto search_text = new char[text_length];
			GetWindowTextA(SearchControl, search_text, text_length);

			std::string output_text;
			char date[20];
			char date1[20];

			t.sort_by_date();
			for (const auto& e : t)
			{
				strftime(date, 20, "%b %d %Y %H:%M", &(e.departure_date));
				strftime(date1, 20, "%b %d %Y %H:%M", &(e.arrival_date));
				output_text += e.departure_city + " " + date + " " + e.arrival_city + " " + date1 + " " + std::to_string(e.capacity) + " " + std::to_string(e.tickets_left) + "\r\n";
			}
			SetWindowTextA(OutputControl, output_text.c_str());
		}

	default:
		return DefWindowProc(hwnd, Message, wparam, lparam);
	}
	return 0;
}