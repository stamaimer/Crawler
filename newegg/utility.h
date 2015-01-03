#ifndef UTILITY_H
#define UTILITY_H

#include <streambuf>
#include <iostream>
#include <string>

#include <QTextEdit>

class Utility : public std::basic_streambuf<char>
{
    std::ostream& out_stream;
    std::streambuf* buffer;
    QTextEdit* console;

public:
    Utility(std::ostream& out_stream, QTextEdit* console)
    {
        this->out_stream = out_stream;
        this->buffer = out_stream.rdbuf();
        out_stream.rdbuf(this);
        this->console = console;
    }

    ~Utility()
    {
        out_stream.rdbuf(buffer);
    }

    static void redirect()
    {
        qInstallMessageHandler();
    }

private:

};

#endif // UTILITY_H
