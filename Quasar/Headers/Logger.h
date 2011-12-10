#pragma once

class Logger : public std::streambuf {
public:
    Logger() : output() {
    }

    void SetOutput(std::ostream& output) {
        this->output = &output;
    }

    void SetOutput(std::ostream* output) {
        this->output = output;
    }

    void AddInput(std::ostream& input) {
        input.rdbuf(this);
    }

    template <typename T>
    void Write(const T& data) {
        if (output) {
            *output << data;
        }
    }

    template <typename T>
    void WriteLine(const T& data) {
        if (output) {
            *output << data << std::endl;
        }
    }

protected:
    std::ostream* output;
};

class FileLogger : public Logger {
public:
    FileLogger() : Logger(), file(), append() {
    }

    ~FileLogger() {
        delete file;
    }

    void SetPath(const string& path) {
        this->path = path;
    }

    void EnableAppend(bool append = true) {
        this->append = append;
    }

    void Open() {
        if (file) {
            delete file;
            file = nullptr;
        }
        if (path.length() == 0) return;

        file = new std::ofstream(path, (append ? std::ios::app : std::ios::out));
        SetOutput(file);
    }

protected:
    virtual int overflow(int c) {
        if (!file) Open();
        if (!output) SetOutput(file);

        output->put(c);
        return c;
    }

    std::ofstream* file;

    string path;
    bool append;
};