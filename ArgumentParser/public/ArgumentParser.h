/**************************************************************
 *                                                            *
 *               Copyright (c) 2018 - Patrick Rohr            *
 *                      All Rights Reserved                   *
 *                                                            *
 *************************************************************/

#include <string>
#include <map>


class ArgumentParser
{
public:
    ArgumentParser();
    ArgumentParser(const std::string& strArgs);
    ~ArgumentParser();

    void Parse(const std::string& strArgs);
    const std::map<std::string, std::string>& GetArguments() const;

    bool GetArgument(const std::string& strShortKey, const std::string& strLongKey) const;
    bool GetArgument(const std::string& strShortKey, const std::string& strLongKey, std::string& strValue) const;

    ArgumentParser(const ArgumentParser&) = delete;
    ArgumentParser& operator=(const ArgumentParser&) = delete;

private:
    // Map of index and value, if there is one
    std::map<std::string, std::string> mapArguments;
};