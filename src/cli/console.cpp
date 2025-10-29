#include "console.h"
#include <iostream>

constexpr std::string_view PROMPT = "> ";
constexpr std::string_view HELLO_MSG = R"(Welcome to the cutetrader CLI, enter "help" for help or "quit" to exit
Good luck and happy trading!)";

//TODO: use Qt classes instead of std::string & co.

Console::Console(QObject *parent)
{
    _notifier = new QSocketNotifier(fileno(stdin), QSocketNotifier::Read, this);
    connect(_notifier, &QSocketNotifier::activated, this, &Console::readLine);

    std::cout << HELLO_MSG << std::endl;
    std::cout << PROMPT << std::flush;
}

void Console::print(const QString &txt)
{
    std::cout << txt.toStdString() << std::endl;
    std::cout << PROMPT << std::flush;
}

void Console::readLine()
{
    std::string line;
    std::getline(std::cin, line);
    QString input = QString(line.c_str()).trimmed();
    if (!input.isEmpty()) {
        emit newInput(input);
    } else {
        std::cout << PROMPT << std::flush;
    }
}
