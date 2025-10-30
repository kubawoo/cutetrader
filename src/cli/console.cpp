#include "console.h"
#include <QFile>
#include <QTextStream>

const QString PROMPT = "> ";
const QString HELLO_MSG = R"(Welcome to the cutetrader CLI, enter "help" for help or "quit" to exit
Good luck and happy trading!
)";

Console::Console(QObject *parent)
    : _inputStream(stdin, QFile::ReadOnly)
    , _outputStream(stdout, QFile::WriteOnly)
    , _notifier(new QSocketNotifier(fileno(stdin), QSocketNotifier::Read, this))
{
    connect(_notifier, &QSocketNotifier::activated, this, &Console::readLine);

    _outputStream << HELLO_MSG;
    printPrompt();
}

void Console::print(const QString &txt)
{
    _outputStream << txt << '\n';
    printPrompt();
}

void Console::readLine()
{
    QString input = QString(_inputStream.readLine()).trimmed();
    if (!input.isEmpty()) {
        emit newInput(input);
    } else {
        printPrompt();
    }
}

void Console::printPrompt()
{
    _outputStream << PROMPT << Qt::flush;
}
