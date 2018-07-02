#pragma once
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonParseError>
#include <QFile>

#include "common.h"

class ConfigTypeProxy;

class ConfigHandler
{
    friend class ConfigTypeProxy;
private:
    const char *m_filename;
    QJsonDocument m_document;
    QJsonDocument m_defaultDocument;

    int getValueAsInt(const char *key) const;
    std::string getValueAsStr(const char *key) const;
    double getValueAsDouble(const char *key) const;
    bool getValueAsBool(const char *key) const;
    void loadDefaults();

public:
    ConfigHandler(const char *filename);
    ~ConfigHandler();

    void readFile();
    void writeFile();

    void setValue(const char *key, std::string value);
    void setValue(const char *key, int value);
    void setValue(const char *key, bool value);
    void setValue(const char *key, double value);

    void setFunction(std::pair<std::string, std::string> func);

    std::vector<std::pair<std::string, std::string>> getFunctions() const;

    const ConfigTypeProxy getValue(const char * key) const;
};
