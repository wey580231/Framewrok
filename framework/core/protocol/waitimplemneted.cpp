#include "waitimplemneted.h"

namespace Core{


BaseProtocol::BaseProtocol(DataParser *parser):m_parser(parser),type(0),startCode(0),startLen(0),endLen(0),endCode(0){

}

DataParser* BaseProtocol::getParser(){
    m_parser->setBaseProtocol(this);
    return m_parser;
}

DataParser::DataParser()
{
   m_baseProtocol = nullptr;
}

void DataParser::setBaseProtocol(BaseProtocol *baseProtocol)
{
    m_baseProtocol = baseProtocol;
}


}//namespace Core
