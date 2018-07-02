#pragma once
#include "confighandler.h"

class ConfigTypeProxy {
private:
    const ConfigHandler *m_handler;
    const char *m_key;

public:
    ConfigTypeProxy(const ConfigHandler *handler, const char *key)
        : m_handler(handler), m_key(key)
    {}

    operator int() const {
        return m_handler->getValueAsInt(m_key);
    }

    operator double() const {
        return m_handler->getValueAsDouble(m_key);
    }

    operator std::string() const {
        return m_handler->getValueAsStr(m_key);
    }

    operator bool() const {
        return m_handler->getValueAsBool(m_key);
    }
};
