#pragma once
#include "Worker.h"

class WindowsWorker :
    public Worker
{
public:
    static constexpr int messagesX{ 5 };
    static constexpr int messagesY{ 5 };

private:
    HDC             hdc{ nullptr };
    const HWND      hWnd;
    HFONT		    m_Font{ nullptr };
    HFONT           m_OldFont{ nullptr };
    int             x{ messagesX };
    int             y{ messagesY };
    char            *szEmptyLine;

public:
    WindowsWorker(HWND WndHandle);

    ~WindowsWorker();

    void fill(RECT& rc, DWORD dwColor = RGB(255, 255, 255));
    void print(uint16_t line, const char* szMsg);
};

