#ifdef CHANGED
#include "copyright.h"
#include "system.h"
#include "synchconsole.h"
#include "synch.h"
static Semaphore *readAvail;
static Semaphore *writeDone;

static void ReadAvailHandler(void *arg) {
    (void) arg;
    readAvail->V();
}

static void WriteDoneHandler(void *arg) {
    (void) arg;
    writeDone->V();
}

SynchConsole::SynchConsole(const char *in, const char *out) {
    readAvail = new Semaphore("read avail", 0);
    writeDone = new Semaphore("write done", 0);
    console = new Console(in, out, ReadAvailHandler, WriteDoneHandler, 0);
}

SynchConsole::~SynchConsole() {
    delete console;
    delete writeDone;
    delete readAvail;
}

void SynchConsole::SynchPutChar(int ch) {
    console->PutChar(ch);
    writeDone->P();
}

int SynchConsole::SynchGetChar() {
    readAvail->P();
    return console->GetChar();
}

void SynchConsole::SynchPutString(const char s[]) {
    int i = 0;
    while (s[i] != '\0') {
        SynchPutChar(s[i]);
        i++;
    }
}

void SynchConsole::SynchGetString(char *s, int n) {
    int i = 0;
    char ch;
    while (i < n - 1) {
        ch = SynchGetChar();

        if (ch == EOF)
            break;

        s[i] = ch;
        i++;

        if (ch == '\n')
            break;

    }
    s[i] = '\0';
}
#endif // CHANGED