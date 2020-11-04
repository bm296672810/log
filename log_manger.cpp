#include <assert.h>
#include <iostream>
#include <sstream>

#ifdef __WIN32
#else
#include <sys/stat.h>
#include <sys/time.h>
#endif

#include "log_manger.h"

namespace bm {

vector<string> split(const string& str, const string& sp)
{
    vector<string> result;
    string::size_type start = 0;
    string::size_type pos = 0;
    do
    {
        pos = str.find(sp, start);
        if(pos != string::npos)
        {
            result.push_back(str.substr(start, pos - start));
            start = pos + sp.size();
        }
        else
            result.push_back(str.substr(start));
    }
    while(pos != string::npos);

    return result;
}
void mkdirs(const string& path, mode_t mode)
{
    vector<string> paths = split(path, "/");

    string dir;
    for(auto i : paths)
    {
        dir += i + "/";
        mkdir(dir.c_str(), mode);
    }
}

log_manger::log_manger(const string& log_path):
    m_log_path(log_path)
{
    mkdirs(log_path, 0766);

    _open_file();
//    m_fs_error.open(m_log_path + "error" + t + ".log", ios::app | ios::in | ios::out);
}
log_manger::~log_manger()
{
    m_fs_info.flush();
    m_fs_info.close();
}

void log_manger::write(const char* curr_file_name, const char* curr_func_name, int line_num, log_leval leval, const string& content)
{
    _write(curr_file_name, curr_func_name, line_num);
    _write(leval);
    _write(content);
    _endl();
    _flush();
}
void log_manger::_endl()
{
    lock_guard<mutex> lock(m_lock);
    m_fs_info << std::endl;
}
void log_manger::_flush()
{
    lock_guard<mutex> lock(m_lock);
    m_fs_info.flush();
}
void log_manger::_write(log_leval leval)
{
    lock_guard<mutex> lock(m_lock);
    m_fs_info << get_log_leval_string(leval) << ":";
}
void log_manger::_write(const string& content)
{
    lock_guard<mutex> lock(m_lock);
    m_fs_info << content;
}
void log_manger::_write(const char* curr_file_name, const char* curr_func_name, int line_num)
{
    lock_guard<mutex> lock(m_lock);
    m_fs_info << curr_file_name << "##" << curr_func_name << "##" << line_num;
    m_fs_info << get_current_time();
}

void log_manger::_open_file()
{
    lock_guard<mutex> lock(m_lock);
    string t = get_current_date();
    string file_path = m_log_path + "/info_" + t + ".log";
    ifstream f(file_path);
    if(f.good())
        return;
    
    m_fs_info.open(file_path, ios::app | ios::in | ios::out);
}

string log_manger::get_current_date()
{
    char d[50] = {0};

    struct tm *local = NULL;
    time_t t = time(NULL);
    local = localtime(&t);

    sprintf(d, "%d-%d-%d", local->tm_year + 1900, local->tm_mon + 1, local->tm_mday);

    return string(d);
}
string log_manger::get_current_time()
{
    struct tm *local = NULL;
    time_t t = time(NULL);
    local = localtime(&t);

    char d[100] = {0};
    printf(d, "[%d-%d-%d %d:%d:%d]",
           local->tm_year + 1900, local->tm_mon + 1, local->tm_mday,
           local->tm_hour, local->tm_min, local->tm_sec);

    return string(d);
}

string log_manger::get_log_leval_string(log_leval leval)
{
    string result;
    switch(leval)
    {
    case INFO:
        result = "[INFO]";
        break;
    case WARNING:
        result = "[WARNING]";
        break;
    case ERROR:
        result = "[ERROR]";
        break;
    default:
        result = "[NO_LEVAL]";
        break;
    }
    return result;
}

log_manger& log_manger::operator<<(string content)
{
    lock_guard<mutex> lock(m_lock);
    m_fs_info << content;
    // m_fs_info << endl;
    return *this;
}

}
