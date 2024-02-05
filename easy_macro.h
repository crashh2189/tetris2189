#pragma once
#pragma warning(disable :4996)
#include <conio.h>
#include <iostream>
#define MAX_LEN 1000
#include <windows.h>

// 윈도우의 크기에 따라 좌표를 변환하는 함수
POINT ConvertCoordinates(HWND hwnd, POINT pt, SIZE originalSize) {
    RECT rc;
    GetWindowRect(hwnd, &rc);

    // 윈도우의 현재 크기를 얻습니다.
    SIZE currentSize = { rc.right - rc.left, rc.bottom - rc.top };

    // 원래 크기에 대한 현재 크기의 비율을 계산합니다.
    float scaleX = (float)currentSize.cx / originalSize.cx;
    float scaleY = (float)currentSize.cy / originalSize.cy;

    // 좌표를 변환합니다.
    POINT convertedPt;
    convertedPt.x = (long)(pt.x * scaleX);
    convertedPt.y = (long)(pt.y * scaleY);

    return convertedPt;
}


void SetClipboardText(const std::string& text) {
    // 클립보드를 엽니다.
    if (!OpenClipboard(NULL)) {
        return;
    }

    // 클립보드를 비웁니다.
    EmptyClipboard();

    // 메모리를 할당합니다.
    HGLOBAL hMem = GlobalAlloc(GMEM_MOVEABLE, text.size() + 1);
    if (!hMem) {
        CloseClipboard();
        return;
    }

    // 문자열을 메모리에 복사합니다.
    char* pMem = (char*)GlobalLock(hMem);
    memcpy(pMem, text.c_str(), text.size() + 1);
    GlobalUnlock(hMem);

    // 클립보드에 문자열을 설정합니다.
    SetClipboardData(CF_TEXT, hMem);

    // 클립보드를 닫습니다.
    CloseClipboard();
}
void typing_str(std::string str)
{
    for (char c : str) {
        Sleep(15);
        if (isupper(c)) {
            keybd_event(VK_SHIFT, 0, 0, 0);
        }
        // 문자를 누르는 이벤트
        INPUT ip = { INPUT_KEYBOARD, {VkKeyScan(c), 0, 0, 0} };
        SendInput(1, &ip, sizeof(INPUT));

        // 문자를 뗴는 이벤트
        ip.ki.dwFlags = KEYEVENTF_KEYUP;
        SendInput(1, &ip, sizeof(INPUT));
        keybd_event(VK_SHIFT, 0, KEYEVENTF_KEYUP, 0);
    }
    return;

}

std::string getClipboardText() {
    std::string clipboardText;
    if (OpenClipboard(NULL)) {
        HANDLE hData = GetClipboardData(CF_TEXT);
        if (hData != NULL) {
            char* pszText = static_cast<char*>(GlobalLock(hData));
            if (pszText != NULL) {
                clipboardText = pszText;
                GlobalUnlock(hData);
            }
        }
        CloseClipboard();
    }
    return clipboardText;
}
void typing_ctrlC()
{
    keybd_event(VK_CONTROL, 0, 0, 0);
    keybd_event('C', 0, 0, 0);
    keybd_event('C', 0, KEYEVENTF_KEYUP, 0);
    keybd_event(VK_CONTROL, 0, KEYEVENTF_KEYUP, 0);
    return;
}

void typing_ctrlV()
{
    keybd_event(VK_CONTROL, 0, 0, 0);
    keybd_event('V', 0, 0, 0);
    keybd_event('V', 0, KEYEVENTF_KEYUP, 0);
    keybd_event(VK_CONTROL, 0, KEYEVENTF_KEYUP, 0);
    return;
}


void typing_selectAll()
{
    keybd_event(VK_LCONTROL, 0, 0, 0);
    keybd_event('A', 0, 0, 0);
    keybd_event('A', 0, KEYEVENTF_KEYUP, 0);
    keybd_event(VK_LCONTROL, 0, KEYEVENTF_KEYUP, 0);
}

void typing_altTab()
{
    keybd_event(VK_MENU, 0, 0, 0);
    keybd_event(VK_TAB, 0, 0, 0);
    keybd_event(VK_TAB, 0, KEYEVENTF_KEYUP, 0);
    keybd_event(VK_MENU, 0, KEYEVENTF_KEYUP, 0);
}

void typing_enter()
{
    keybd_event(VK_RETURN, 0, 0, 0);
    keybd_event(VK_RETURN, 0, KEYEVENTF_KEYUP, 0);
}




std::string typing_toHangul(std::string str) {
    std::string case1, case2; //case1은 현재 언어로 입력하기 , case2는 VK_MENU 눌러서 입력하기
    system("start notepad");
    Sleep(500);
    HWND notepad = FindWindowA(NULL, "제목 없음 - Windows 메모장");
    if (notepad == NULL) {
        std::cerr << "Failed to Find Notepad.";
        return "error";
    }
    SetForegroundWindow(notepad);
    SetWindowLong(notepad, GWL_EXSTYLE, GetWindowLong(notepad, GWL_EXSTYLE) | WS_EX_LAYERED);
    SetLayeredWindowAttributes(notepad, 0, 0, LWA_ALPHA);
    HWND Edit = FindWindowEx(notepad, NULL, TEXT("Edit"), NULL);
    if (Edit == NULL) {
        std::cerr << "Failed to Find Edit Class";
        return "error";
    }
    SetFocus(Edit);


    typing_str(str);
    typing_selectAll();
    typing_ctrlC();
    Sleep(100);
    case1 = getClipboardText();

    Sleep(500);

    keybd_event(VK_HANGUL, 0, 0, 0);
    keybd_event(VK_HANGUL, 0, KEYEVENTF_KEYUP, 0);


    typing_str(str);
    typing_selectAll();
    typing_ctrlC();
    Sleep(100);
    case2 = getClipboardText();

    PostMessage(notepad, WM_CLOSE, NULL, NULL);
    keybd_event('N', 0, 0, 0);
    keybd_event('N', 0, KEYEVENTF_KEYUP, 0);



    //case1이 한글인가
    if (case1 != str) {
        return case1;
    }
    //case2가 한글인가
    else if (case2 != str) {
        return case2;
    }

    return case1;

}

void openConsole()
{
    keybd_event(VK_CONTROL, 0, 0, 0);
    keybd_event(VK_SHIFT, 0, 0, 0);
    keybd_event('P', 0, 0, 0);
    keybd_event('P', 0, KEYEVENTF_KEYUP, 0);
    keybd_event(VK_SHIFT, 0, KEYEVENTF_KEYUP, 0);
    keybd_event(VK_CONTROL, 0, KEYEVENTF_KEYUP, 0);

    Sleep(100);



    std::string str = "show console";
    for (char c : str) {
        Sleep(10);
        if (isupper(c)) {
            keybd_event(VK_SHIFT, 0, 0, 0);
        }

        INPUT ip = { INPUT_KEYBOARD, {VkKeyScan(c), 0, 0, 0} };
        SendInput(1, &ip, sizeof(INPUT));

        ip.ki.dwFlags = KEYEVENTF_KEYUP;
        SendInput(1, &ip, sizeof(INPUT));
        keybd_event(VK_SHIFT, 0, KEYEVENTF_KEYUP, 0);
    }


    keybd_event(VK_RETURN, 0, 0, 0);


    keybd_event(VK_RETURN, 0, KEYEVENTF_KEYUP, 0);

    return;
}


DWORD WINAPI MouseMonitorThread(LPVOID lpParam) {
    // 메모장 윈도우를 찾습니다.
    HWND notepad = FindWindow(NULL, TEXT("친구 - Discord"));
    if (notepad == NULL) {
        std::cerr << "Failed to find Notepad.\n";
        return 1;
    }

    while (true) {
        // 마우스 왼쪽 버튼이 눌렸는지 확인합니다.
        if (GetAsyncKeyState(VK_LBUTTON) & 0x8000) {
            // 마우스 포인터의 현재 위치를 얻습니다.
            POINT point;
            GetCursorPos(&point);

            // 마우스 포인터가 메모장 윈도우 내에 있는지 확인합니다.
            RECT rect;
            GetWindowRect(notepad, &rect);
            if (PtInRect(&rect, point)) {
                // 마우스 포인터의 위치를 메모장 윈도우의 클라이언트 영역 좌표로 변환합니다.
                ScreenToClient(notepad, &point);

                // 현재 위치를 출력합니다.
                std::cout << "Clicked in Notepad at (" << point.x << ", " << point.y << ")\n";
            }

            // 클릭을 감지했으므로 잠시 대기합니다.
            Sleep(100);
        }
    }
    return 0;
}

void openDiscord(int isptb)
{
    if (isptb) {
        system("taskkill /F /IM DiscordPTB.exe"); //close discord ptb.exe


        Sleep(1500);

        system("start %localappdata%\\discordptb\\app-1.0.1054\\DiscordPTB.exe");

        Sleep(5700);

        HWND discord_friend_tab = FindWindow(NULL, TEXT("친구 - Discord"));
        if (discord_friend_tab == NULL) {
            MessageBox(NULL, TEXT("Discord 앱을 켜시고, Friends 탭에서 친구 추가하기 버튼을 눌러주세요. 기본 크기의 창모드로 해주세요."), TEXT("세팅 오류"), 0);
            return;
        }

        SetForegroundWindow(discord_friend_tab);
        SetWindowLong(discord_friend_tab, GWL_EXSTYLE, GetWindowLong(discord_friend_tab, GWL_EXSTYLE) | WS_EX_LAYERED);
        SetLayeredWindowAttributes(discord_friend_tab, 0, 0, LWA_ALPHA);
        
        


        //HWND discord_friend_tab = FindWindow(NULL, TEXT("친구 - Discord"));
        return;
    }
}



void openDeveloperTool()
{
    keybd_event(VK_CONTROL, 0, 0, 0);
    keybd_event(VK_SHIFT, 0, 0, 0);
    keybd_event('I', 0, 0, 0);
    keybd_event('I', 0, KEYEVENTF_KEYUP, 0);
    keybd_event(VK_SHIFT, 0, KEYEVENTF_KEYUP, 0);
    keybd_event(VK_CONTROL, 0, KEYEVENTF_KEYUP, 0);

    return;
}



std::string discord_GetToken()
{
    HWND discord_friend_tab = FindWindow(NULL, TEXT("친구 - Discord"));
    if (discord_friend_tab == NULL) {
        MessageBox(NULL, TEXT("Discord 앱을 켜시고, Friends 탭에서 친구 추가하기 버튼을 눌러주세요. 기본 크기의 창모드로 해주세요."), TEXT("세팅 오류"), 0);
        return "error";
    }
    SetForegroundWindow(discord_friend_tab);
    SetWindowLong(discord_friend_tab, GWL_EXSTYLE, GetWindowLong(discord_friend_tab, GWL_EXSTYLE) | WS_EX_LAYERED);
    SetLayeredWindowAttributes(discord_friend_tab, 0, 0, LWA_ALPHA);

    openDeveloperTool();
    Sleep(500);
    openConsole();
    Sleep(500);

    typing_str("pasting allow");
    typing_enter();

    Sleep(1500);
    FILE* fs;
    fs = fopen("scripts.txt", "r");
    while (feof(fs) == 0) {
        char str[MAX_LEN];
        fgets(str, MAX_LEN, fs);
        SetClipboardText(str);
        typing_ctrlV();
        Sleep(100);
        typing_enter();
        Sleep(100);
    }

    Sleep(500);
    std::string token = getClipboardText();
    Sleep(1000);
    openDeveloperTool();
    SetForegroundWindow(discord_friend_tab);
    SetWindowLong(discord_friend_tab, GWL_EXSTYLE, GetWindowLong(discord_friend_tab, GWL_EXSTYLE) | WS_EX_LAYERED);
    SetLayeredWindowAttributes(discord_friend_tab, 0, 255, LWA_ALPHA);
    return token;
}

void discord_sendFriendRequest(std::string username)
{
    //디코 친추 하기

    HWND discord_friend_tab = FindWindow(NULL, TEXT("친구 - Discord"));
    if (discord_friend_tab == NULL) {
        MessageBox(NULL, TEXT("Discord 앱을 켜시고, Friends 탭에서 친구 추가하기 버튼을 눌러주세요. 기본 크기의 창모드로 해주세요."), TEXT("세팅 오류"), 0);
        return;
    }

    Sleep(1000);

    //디코 창 숨기기
    SetForegroundWindow(discord_friend_tab);
    SetWindowLong(discord_friend_tab, GWL_EXSTYLE, GetWindowLong(discord_friend_tab, GWL_EXSTYLE) | WS_EX_LAYERED);
    SetLayeredWindowAttributes(discord_friend_tab, 0, 0, LWA_ALPHA);

    Sleep(1500);

    //디스코드 사용자명 입력하는 박스 가운데 클릭
    SendMessage(discord_friend_tab, WM_LBUTTONDOWN, 0, (LPARAM)(783, 43));
    SendMessage(discord_friend_tab, WM_LBUTTONUP, 0, (LPARAM)(783, 43));

    SetForegroundWindow(discord_friend_tab);

    Sleep(1000);

    SetForegroundWindow(discord_friend_tab);


    SendMessage(discord_friend_tab, WM_LBUTTONDOWN, 0, (LPARAM)MAKELPARAM(562, 181));
    SendMessage(discord_friend_tab, WM_LBUTTONUP, 0, (LPARAM)MAKELPARAM(562, 181));

    SetForegroundWindow(discord_friend_tab);
    typing_str(username);
    SetForegroundWindow(discord_friend_tab);

    Sleep(500);

    SendMessage(discord_friend_tab, WM_LBUTTONDOWN, 0, (LPARAM)MAKELPARAM(636, 279));
    SendMessage(discord_friend_tab, WM_LBUTTONUP, 0, (LPARAM)MAKELPARAM(636, 279));

    SetForegroundWindow(discord_friend_tab);

    Sleep(500);
    SetForegroundWindow(discord_friend_tab);
    SendMessage(discord_friend_tab, WM_LBUTTONDOWN, 0, (LPARAM)MAKELPARAM(857, 183));
    SetForegroundWindow(discord_friend_tab);
    Sleep(500);
    SetForegroundWindow(discord_friend_tab);
    SendMessage(discord_friend_tab, WM_LBUTTONUP, 0, (LPARAM)MAKELPARAM(857, 183));
    SetForegroundWindow(discord_friend_tab);

    Sleep(500);
    SetForegroundWindow(discord_friend_tab);
    SendMessage(discord_friend_tab, WM_LBUTTONDOWN, 0, (LPARAM)MAKELPARAM(519, 464));
    SetForegroundWindow(discord_friend_tab);
    SendMessage(discord_friend_tab, WM_LBUTTONUP, 0, (LPARAM)MAKELPARAM(519, 464));
    Sleep(500);
    SetForegroundWindow(discord_friend_tab);

    SendMessage(discord_friend_tab, WM_LBUTTONDOWN, 0, (LPARAM)MAKELPARAM(519, 464));
    SendMessage(discord_friend_tab, WM_LBUTTONUP, 0, (LPARAM)MAKELPARAM(519, 464));
    SetForegroundWindow(discord_friend_tab);

    Sleep(1000);
    SendMessage(discord_friend_tab, WM_LBUTTONDOWN, 0, (LPARAM)MAKELPARAM(143, 743));
    SetForegroundWindow(discord_friend_tab);
    SendMessage(discord_friend_tab, WM_LBUTTONUP, 0, (LPARAM)MAKELPARAM(143, 743));
    SetForegroundWindow(discord_friend_tab);
    Sleep(500);
    SendMessage(discord_friend_tab, WM_LBUTTONDOWN, 0, (LPARAM)MAKELPARAM(194, 638));
    SetForegroundWindow(discord_friend_tab);
    SendMessage(discord_friend_tab, WM_LBUTTONUP, 0, (LPARAM)MAKELPARAM(194, 638));
    Sleep(500);
    SetForegroundWindow(discord_friend_tab);
    SendMessage(discord_friend_tab, WM_LBUTTONDOWN, 0, (LPARAM)MAKELPARAM(726, 441));
    SendMessage(discord_friend_tab, WM_LBUTTONUP, 0, (LPARAM)MAKELPARAM(726, 441));
    SetForegroundWindow(discord_friend_tab);
    Sleep(500);
    SetForegroundWindow(discord_friend_tab);
    SetLayeredWindowAttributes(discord_friend_tab, 0, 255, LWA_ALPHA);

    return;
}

