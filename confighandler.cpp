#include "confighandler.h"
#include "configtypeproxy.h"

ConfigHandler::ConfigHandler(const char *filename)
    : m_filename(filename)
{
    loadDefaults();
    readFile();
}

ConfigHandler::~ConfigHandler()
{
    writeFile();
}

int ConfigHandler::getValueAsInt(const char *key) const
{
    QJsonObject root = m_document.object();

    if (!root.contains(key))
        throw std::invalid_argument("config: key does not exist");

    return root[key].toInt();
}

std::string ConfigHandler::getValueAsStr(const char *key) const
{
    QJsonObject root = m_document.object();

    if (!root.contains(key))
        throw std::invalid_argument("config: key does not exist");

    return root[key].toString().toStdString();
}

double ConfigHandler::getValueAsDouble(const char *key) const
{
    QJsonObject root = m_document.object();

    if (!root.contains(key))
        throw std::invalid_argument("config: key does not exist");

    return root[key].toDouble();
}

bool ConfigHandler::getValueAsBool(const char *key) const
{
    QJsonObject root = m_document.object();

    if (!root.contains(key))
        throw std::invalid_argument("config: key does not exist");

    return root[key].toBool();
}

void ConfigHandler::loadDefaults()
{
    QJsonObject root {
        { "background_color",   "black" }, // background color of the window
        { "foreground_color",   "white" }, // text color
        { "scrollback_lines",      5000 }, // number of lines for scrollback
        { "window_size",      "600x300" }, // window dimensions
        { "functions",     QJsonArray {}}  // function array (variables are x -> y)
    };

    // create the json document
    m_defaultDocument = QJsonDocument(root);
}

void ConfigHandler::readFile()
{
    QFile file(m_filename);

    // file does not exist so load default settings
    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QString config = file.readAll();
        file.close();
        // store the config
        m_document = QJsonDocument::fromJson(config.toUtf8());
    }

    QJsonObject root = m_document.object();
    QJsonObject defRoot = m_defaultDocument.object();

    QStringList keys = defRoot.keys();

    // add any pairs that are missing
    for (auto it = keys.begin(); it != keys.end(); ++it) {
        QString k = *it;
        if (!root.contains(k)) {
            root[k] = defRoot[k];
        }
    }
}

void ConfigHandler::writeFile()
{
    QFile file(m_filename);

    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
        return;

    file.write(m_document.toJson(QJsonDocument::Indented));
    file.close();
}

void ConfigHandler::setValue(const char *key, std::string value)
{
    m_document.object()[key] = QString(value.c_str());
}

void ConfigHandler::setValue(const char *key, int value)
{
    m_document.object()[key] = value;
}

void ConfigHandler::setValue(const char *key, bool value)
{
    m_document.object()[key] = value;
}

void ConfigHandler::setValue(const char *key, double value)
{
    m_document.object()[key] = value;
}

void ConfigHandler::setFunction(std::pair<std::string, std::string> func)
{
    QJsonArray a = m_document.object()["functions"].toArray();

    for (auto it = a.begin(); it != a.end(); ++it) {
        QJsonObject o = (*it).toObject();
        if (o.contains(func.first.c_str())) {
            o[func.first.c_str()] = func.second.c_str();
            return;
        }
    }

    // it doesnt exist so add it
    a.push_back(QJsonObject{
        { QString(func.first.c_str()), QString(func.second.c_str()) }
    });
}

std::vector<std::pair<std::string, std::string> > ConfigHandler::getFunctions() const
{
    std::vector<std::pair<std::string, std::string>> funcs;
    QJsonArray a = m_document.object()["functions"].toArray();

    for (auto it = a.begin(); it != a.end(); ++it) {
        QJsonObject o = (*it).toObject();
        for (QString s : o.keys()) {
            funcs.push_back(std::make_pair(s.toStdString(), o[s].toString().toStdString()));
        }
    }

    return funcs;
}

const ConfigTypeProxy ConfigHandler::getValue(const char *key) const
{
    if (!m_document.object().contains(key))
        throw std::invalid_argument("Config key does not exist");

    return ConfigTypeProxy(this, key);
}
