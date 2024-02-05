#pragma once
#pragma warning(disable :4996)
#include <conio.h>
#include <iostream>
#define MAX_LEN 1000
#include <windows.h>

// �������� ũ�⿡ ���� ��ǥ�� ��ȯ�ϴ� �Լ�
POINT ConvertCoordinates(HWND hwnd, POINT pt, SIZE originalSize) {
    RECT rc;
    GetWindowRect(hwnd, &rc);

    // �������� ���� ũ�⸦ ����ϴ�.
    SIZE currentSize = { rc.right - rc.left, rc.bottom - rc.top };

    // ���� ũ�⿡ ���� ���� ũ���� ������ ����մϴ�.
    float scaleX = (float)currentSize.cx / originalSize.cx;
    float scaleY = (float)currentSize.cy / originalSize.cy;

    // ��ǥ�� ��ȯ�մϴ�.
    POINT convertedPt;
    convertedPt.x = (long)(pt.x * scaleX);
    convertedPt.y = (long)(pt.y * scaleY);

    return convertedPt;
}


void SetClipboardText(const std::string& text) {
    // Ŭ�����带 ���ϴ�.
    if (!OpenClipboard(NULL)) {
        return;
    }

    // Ŭ�����带 ���ϴ�.
    EmptyClipboard();

    // �޸𸮸� �Ҵ��մϴ�.
    HGLOBAL hMem = GlobalAlloc(GMEM_MOVEABLE, text.size() + 1);
    if (!hMem) {
        CloseClipboard();
        return;
    }

    // ���ڿ��� �޸𸮿� �����մϴ�.
    char* pMem = (char*)GlobalLock(hMem);
    memcpy(pMem, text.c_str(), text.size() + 1);
    GlobalUnlock(hMem);

    // Ŭ�����忡 ���ڿ��� �����մϴ�.
    SetClipboardData(CF_TEXT, hMem);

    // Ŭ�����带 �ݽ��ϴ�.
    CloseClipboard();
}
void typing_str(std::string str)
{
    for (char c : str) {
        Sleep(15);
        if (isupper(c)) {
            keybd_event(VK_SHIFT, 0, 0, 0);
        }
        // ���ڸ� ������ �̺�Ʈ
        INPUT ip = { INPUT_KEYBOARD, {VkKeyScan(c), 0, 0, 0} };
        SendInput(1, &ip, sizeof(INPUT));

        // ���ڸ� ��� �̺�Ʈ
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
    std::string case1, case2; //case1�� ���� ���� �Է��ϱ� , case2�� VK_MENU ������ �Է��ϱ�
    system("start notepad");
    Sleep(500);
    HWND notepad = FindWindowA(NULL, "���� ���� - Windows �޸���");
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



    //case1�� �ѱ��ΰ�
    if (case1 != str) {
        return case1;
    }
    //case2�� �ѱ��ΰ�
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
    // �޸��� �����츦 ã���ϴ�.
    HWND notepad = FindWindow(NULL, TEXT("ģ�� - Discord"));
    if (notepad == NULL) {
        std::cerr << "Failed to find Notepad.\n";
        return 1;
    }

    while (true) {
        // ���콺 ���� ��ư�� ���ȴ��� Ȯ���մϴ�.
        if (GetAsyncKeyState(VK_LBUTTON) & 0x8000) {
            // ���콺 �������� ���� ��ġ�� ����ϴ�.
            POINT point;
            GetCursorPos(&point);

            // ���콺 �����Ͱ� �޸��� ������ ���� �ִ��� Ȯ���մϴ�.
            RECT rect;
            GetWindowRect(notepad, &rect);
            if (PtInRect(&rect, point)) {
                // ���콺 �������� ��ġ�� �޸��� �������� Ŭ���̾�Ʈ ���� ��ǥ�� ��ȯ�մϴ�.
                ScreenToClient(notepad, &point);

                // ���� ��ġ�� ����մϴ�.
                std::cout << "Clicked in Notepad at (" << point.x << ", " << point.y << ")\n";
            }

            // Ŭ���� ���������Ƿ� ��� ����մϴ�.
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

        HWND discord_friend_tab = FindWindow(NULL, TEXT("ģ�� - Discord"));
        if (discord_friend_tab == NULL) {
            MessageBox(NULL, TEXT("Discord ���� �ѽð�, Friends �ǿ��� ģ�� �߰��ϱ� ��ư�� �����ּ���. �⺻ ũ���� â���� ���ּ���."), TEXT("���� ����"), 0);
            return;
        }

        SetForegroundWindow(discord_friend_tab);
        SetWindowLong(discord_friend_tab, GWL_EXSTYLE, GetWindowLong(discord_friend_tab, GWL_EXSTYLE) | WS_EX_LAYERED);
        SetLayeredWindowAttributes(discord_friend_tab, 0, 0, LWA_ALPHA);
        
        


        //HWND discord_friend_tab = FindWindow(NULL, TEXT("ģ�� - Discord"));
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
    HWND discord_friend_tab = FindWindow(NULL, TEXT("ģ�� - Discord"));
    if (discord_friend_tab == NULL) {
        MessageBox(NULL, TEXT("Discord ���� �ѽð�, Friends �ǿ��� ģ�� �߰��ϱ� ��ư�� �����ּ���. �⺻ ũ���� â���� ���ּ���."), TEXT("���� ����"), 0);
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
    //���� ģ�� �ϱ�

    HWND discord_friend_tab = FindWindow(NULL, TEXT("ģ�� - Discord"));
    if (discord_friend_tab == NULL) {
        MessageBox(NULL, TEXT("Discord ���� �ѽð�, Friends �ǿ��� ģ�� �߰��ϱ� ��ư�� �����ּ���. �⺻ ũ���� â���� ���ּ���."), TEXT("���� ����"), 0);
        return;
    }

    Sleep(1000);

    //���� â �����
    SetForegroundWindow(discord_friend_tab);
    SetWindowLong(discord_friend_tab, GWL_EXSTYLE, GetWindowLong(discord_friend_tab, GWL_EXSTYLE) | WS_EX_LAYERED);
    SetLayeredWindowAttributes(discord_friend_tab, 0, 0, LWA_ALPHA);

    Sleep(1500);

    //���ڵ� ����ڸ� �Է��ϴ� �ڽ� ��� Ŭ��
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

