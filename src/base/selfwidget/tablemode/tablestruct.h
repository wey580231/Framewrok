/*!
 * @brief     表格通用数据结构
 * @author    wey
 * @version   1.0
 * @date      2021.01.09 16:32:24
 * @warning
 * @copyright NanJing RenGu.
 * @note
 */
#pragma once

#include <QString>

namespace Base {

	/*!
	 * @brief 表格列尺寸更新模式
	 */
	enum ColumnResizeMode {
		Interactive,		/*!< 提供用户交互方式，即可拖拽列 */
		Fixed,				/*!< 列固定模式，可resizeSection改变大小 */
		Strech,				/*!< 列自动调整，不可编码改变大小 */
		ResizeToContents	/*!< 自动改变至内容宽度，不可编码或用户拖拽改变大小 */
	};

	/*!
	* @brief 自定义数据显示列
	* @details
	*/
	struct ColumnItem
	{
		ColumnItem() :ColumnItem(0, "", true, Qt::AlignCenter, Interactive, 120) {}
		ColumnItem(int id, QString name, bool visible = true, Qt::Alignment align = Qt::AlignCenter, ColumnResizeMode  model = Interactive, int width = 120) :m_id(id), m_name(name), m_visible(visible), m_align(align), m_columnWidth(width){}
		ColumnItem(int id, QString name, int width):ColumnItem(id,name,true,Qt::AlignCenter,Interactive,width){}

		int m_id;			/*!< 列唯一标识 */
		QString m_name;		/*!< 列名称 */
		bool m_visible;		/*!< 列是否显示 */
		Qt::Alignment m_align;	/*!< 列对齐方式 */

		ColumnResizeMode m_resizeModel;		/*!< 表格列改变模式 */
		int m_columnWidth;					/*!< 表格列宽度 */
	};

	typedef QList<ColumnItem> ColumnItems;

} //namespace Base 