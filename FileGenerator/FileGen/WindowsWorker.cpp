
#include "framework.h"
#include <string>
#include <thread>
#include <vector>

#include "File.h"
#include "Worker.h"


#include "WindowsWorker.h"


WindowsWorker::WindowsWorker(HWND WndHandle) : Worker("c:\\log", "log", "txt", 200),
        hWnd{ WndHandle },
        szEmptyLine{ new char[1024] }
{
    std::memset(szEmptyLine, 32, 1024);
    szEmptyLine[1023] = 0;


    LOGFONT		LogFont;

    memset(&LogFont, 0, sizeof(LOGFONT));

    LogFont.lfHeight = 15;
    LogFont.lfEscapement = GM_COMPATIBLE;
    LogFont.lfOrientation = GM_COMPATIBLE;
    LogFont.lfWeight = FW_DONTCARE;
    LogFont.lfCharSet = ANSI_CHARSET;
    LogFont.lfOutPrecision = OUT_OUTLINE_PRECIS;
    LogFont.lfClipPrecision = CLIP_DEFAULT_PRECIS;
    LogFont.lfQuality = DEFAULT_QUALITY;
    LogFont.lfPitchAndFamily = DEFAULT_PITCH;
    LogFont.lfItalic = FALSE;
    LogFont.lfStrikeOut = FALSE;
    LogFont.lfUnderline = FALSE;


    strcpy(LogFont.lfFaceName, "Verdana");

    m_Font = CreateFontIndirect(&LogFont);

    hdc = GetDC(hWnd);
    m_OldFont = (HFONT)SelectObject(hdc, m_Font);
}

WindowsWorker::~WindowsWorker()
{
    delete[] szEmptyLine;
    deleteVector(false);

    SelectObject(hdc, m_OldFont);
    ReleaseDC(hWnd, hdc);
}

void WindowsWorker::fill(RECT& rc, DWORD dwColor)
{

    HBRUSH hbr, hbrOld;

    hbr = ::CreateSolidBrush(dwColor);
    hbrOld = (HBRUSH)::SelectObject(hdc, hbr);

    ::FillRect(hdc, &rc, hbr);

    ::SelectObject(hdc, hbrOld);
    ::DeleteObject(hbr);
}


void WindowsWorker::print(uint16_t line, const char* szMsg)
{

    y = messagesY + (line * 15);
    ::TextOut(hdc, x, y, szEmptyLine, static_cast<int>(std::strlen(szEmptyLine)));
    ::TextOut(hdc, x, y, szMsg, static_cast<int>(std::strlen(szMsg)));
}

