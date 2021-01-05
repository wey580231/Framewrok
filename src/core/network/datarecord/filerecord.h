/*!
 *  @brief     文件记录
 *  @details
 *  @author    wey
 *  @version   1.0
 *  @date      2019.01.12
 *  @warning
 *  @copyright NanJing RenGu.
 *  @note
 */
#ifndef FILERECORD_H
#define FILERECORD_H

#include <core/protocol/datastruct.h>

namespace Core{

class FileRecord
{
public:
    FileRecord();

    virtual void record(Datastruct::ProtocolArray & array) = 0;

};

} //namespace Core

#endif // FILERECORD_H
