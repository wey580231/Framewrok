/*!
 * @brief     自定义常用宏   
 * @author    wey
 * @version   1.0
 * @date      2021.01.25 14:19:37
 * @warning
 * @copyright NanJing RenGu.
 * @note
 */
#pragma once

#include <QObject>

/*!
 * @brief 友元实现多个枚举值单位逻辑运算
 * @attention 必须已经对当前枚举调用Q_DECLARE_FLAGS()宏
 * @details Qt版的Q_DECLARE_OPERATORS_FOR_FLAGS 宏不适用于类内的枚举对象定义，因为扩展后函数使用2个参数进行输入。在修改的时候
			【使用友元方式，将此宏重写】
 */
#define Q_DECLARE_FRIEND_FLAGS(Flags) \
Q_DECL_CONSTEXPR inline friend QFlags<Flags::enum_type> operator|(Flags::enum_type f1, Flags::enum_type f2) Q_DECL_NOTHROW \
{ return QFlags<Flags::enum_type>(f1) | f2; } 