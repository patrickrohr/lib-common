/**************************************************************
 *                                                            *
 *               Copyright (c) 2018 - Patrick Rohr            *
 *                      All Rights Reserved                   *
 *                                                            *
 *************************************************************/

#include "ArgumentParser.h"

ArgumentParser::ArgumentParser() :
    m_mapArguments()
{}

ArgumentParser::ArgumentParser(std::string strArgs) :
    m_mapArguments()
{
    Parse(std::move(strArgs));
}

ArgumentParser::~ArgumentParser() = default;

#define BUFFER_RESERVE_BYTES 16
void ArgumentParser::Parse(std::string&& strArgs)
{
    bool isArg = false;
    std::string argBuffer;
    std::string paramBuffer;
    argBuffer.reserve(BUFFER_RESERVE_BYTES); // prevent continually allocating memory
    paramBuffer.reserve(BUFFER_RESERVE_BYTES);

    // Appending whitespace will cause this loop to go through one last time after the
    // last arguments or parameters were buffered
    strArgs.append(' ');
    for (auto it = strArgs.cbegin(); it != strArgs.cend(); ++it)
    {
        if  (' ' == *it) // whitespace resets
        {
            if(isArg)
            {
                if (!argBuffer.empty())
                {
                    m_mapArguments[argBuffer];
                }
                isArg = false;
            }
            else
            {
                if (!argBuffer.empty() && !paramBuffer.empty())
                {
                    m_mapArguments[argBuffer] = paramBuffer;
                    argBuffer.reset();
                    paramBuffer.reset();
                }
            }
        }
        else if ('-' == *it) // We either got an argument (-) or a long argument (--)
        {
            argBuffer.reset(); // starting a new argument
            paramBuffer.reset();
            isArg = true;
        }
        else if (isArg)
        {
            argBuffer.append(*it);
        }
        else // parameter
        {
            if (!argBuffer.empty())
            {
                paramBuffer.append(*it);
            }
        }
    }
}

const std::map<std::string, std::string> GetArguments() const
{
    return m_mapArguments;
}


bool ArgumentParser::GetArgument(const std::string& strShortKey, const std::string& strLongKey) const
{
    return GetArgument(strShortKey, strLongKey, std::string());
}

bool ArgumentParser::GetArgument(const std::string& strShortKey, const std::string& strLongKey, std::string& strValue) const
{
    auto it = m_mapArguments.find(strShortKey);
    if (m_mapArguments.end() == it)
    {
        it = m_mapArguments.find(strLongKey);
        if (m_mapArguments.end() == it)
        {
            return false;
        }
    }

    strValue = it->second;
    return true;
}