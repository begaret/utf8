#include "string.h"
    
namespace utf8 {
    string::string() noexcept : m_length(0) 
    {
        m_buffer = new codepoint[1];
        *m_buffer = '\0';
    }

    string::string(codepoint cpt) noexcept : m_length(1)
    {
        m_buffer = new codepoint[2];
        m_buffer[0] = cpt;
        m_buffer[1] = '\0';
    }

    string::string(std::string str) noexcept
    {
        if (is_valid(str)) {
            m_buffer = new codepoint[2];
            m_buffer[0] = str;
            m_buffer[1] = '\0';
            m_length = 1;
        } else {
            int len = str.length();
            m_buffer = new codepoint[len + 1];

            int i = 0;
            for (auto cpt: get_codepoints(str, 0, true)) {
                m_buffer[i++] = cpt;
            } m_length = i;

            m_buffer[len] = '\0';
        }
    }

    string::string(const char *str) noexcept
    {
        if (!str) {
            m_buffer = new codepoint[0];
            m_length = 0;
        }

        else {
            if (is_valid(str)) {
                m_buffer = new codepoint[2];
                m_buffer[0] = str;
                m_buffer[1] = '\0';
                m_length = 1;
            } else {
                int len = strlen(str);
                m_buffer = new codepoint[len + 2];

                int i = 0;
                for (auto cpt: get_codepoints(str, 0, true)) {
                    m_buffer[i++] = cpt;
                } m_length = i;

                m_buffer[len] = '\0';
            }
        }
    }

    string::string(codepoint *cpts) noexcept
    {
        if (!cpts) {
            m_buffer = new codepoint[0];
            m_length = 0;
        }

        else {
            int i = 0;
            while (cpts[i]) i++;
            m_length = i;
            m_buffer = new codepoint[i + 1];

            std::copy(cpts, cpts + m_length, m_buffer);
            m_buffer[m_length] = '\0';
        }
    }

    string::string(const string &other) noexcept
    {
        codepoint* buffer = new codepoint[other.m_length + 1];
        std::copy(other.m_buffer, other.m_buffer + other.m_length, buffer);
        buffer[other.m_length] = '\0';

        m_buffer = buffer;
        m_length = other.m_length;
    }

    string::~string() noexcept
    {
        delete [] m_buffer;
        m_buffer = nullptr;
    }

    codepoint &string::at(size_t index)
    {
        if (index > 0 && index < m_length) {
            return m_buffer[index];
        } else {
            throw out_of_range_exception();
        }
    }

    codepoint string::at(size_t index) const
    {
        if (index > 0 && index < m_length) {
            return m_buffer[index];
        } else {
            throw out_of_range_exception();
        }
    }

    bool string::operator==(const string &other) const
    {
        if (other.m_length != m_length)
            return false;

        for (int i = 0; i < other.m_length; i++)
            if (other.m_buffer[i] != m_buffer[i])
                return false;
        return true;
    }

    string &string::operator+=(const string &other)
    {
        if (this != &other)
        {
            size_t new_length = m_length + other.m_length;
            codepoint* buffer = new codepoint[new_length + 1];

            std::copy(m_buffer, m_buffer + m_length, buffer);
            std::copy(other.m_buffer, other.m_buffer + other.m_length, buffer + m_length);
            buffer[new_length] = '\0';

            std::swap(m_buffer, buffer);
            m_length = new_length;

            delete[] buffer;
        }

        return *this;
    }

    string string::operator+(const string &other) const
    {
        const size_t new_length = m_length + other.m_length;
        codepoint* buffer = new codepoint[new_length + 1];

        std::copy(m_buffer, m_buffer + m_length, buffer);
        std::copy(other.m_buffer, other.m_buffer + other.m_length, buffer + m_length);
        buffer[new_length] = '\0';

        return string(buffer);
    }

    std::ostream &operator<<(std::ostream &os, const string &str)
    {
        int i = 0;
        while (str.m_buffer[i])
            os << str.m_buffer[i++];

        return os;
    }
}