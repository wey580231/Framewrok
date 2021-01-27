/*!
 *  @brief     带图标的按钮
 *  @file      1.可选择的设置按钮图标
			   2.可设置图标和文字的排列方向
 *  @author    wey
 *  @version   1.0
 *  @date      2020.10.14
 *  @warning
 *  @copyright NanJing RenGu.
 *  @note
 */
#ifndef ICONBUTTON_H
#define ICONBUTTON_H

#include <QAbstractButton>

#include "../base_global.h"
#include "../macros.h"

namespace Base {

	class BASESHARED_EXPORT RIconButton : public QAbstractButton
	{
		Q_OBJECT
	public:
		RIconButton(QWidget * parent = nullptr);

		/*!
		 *  @brief 图标尺寸
		 */
		enum IconSize {
			ICON_16,
			ICON_24,
			ICON_32,
			ICON_48,
			ICON_64,
			ICON_custom
		};

		enum ColorChoose {
			Color_NormalBackGround = 0x0001,
			Color_CheckedBackGround = 0x0002,
			Color_HoverBackground = 0x0004,
			Color_NormalBorder = 0x0008,
			Color_CheckedBorder = 0x0010,
			Color_HoverBorder = 0x0020,
			Color_NormalText = 0x0040,
			Color_CheckedText = 0x0080,
			Color_HoverText = 0x0100,

			Color_All = Color_NormalBackGround | Color_CheckedBackGround | Color_HoverBackground | Color_NormalBorder | Color_CheckedBorder |
			Color_HoverBorder | Color_NormalText | Color_CheckedText | Color_HoverText
		};

		Q_DECLARE_FLAGS(ColorChooses, ColorChoose)
		Q_DECLARE_FRIEND_FLAGS(ColorChooses)

		void enableColor(ColorChoose choose, QColor color = QColor(255, 0, 0, 0));
		void enableColors(ColorChooses chooses, QColor color = QColor(255, 0, 0, 0));
		void disableColor(ColorChoose choose);
		void disableColors(ColorChooses chooses);

		void setCheckedIcon(const QIcon & icon);
		void setIconSize(IconSize type, QSize size = QSize());
		void setIconTextSpacing(int spacing = 5) { m_spacing = spacing; }

		void setTextFont(QFont & font);
		void setTextVisible(bool visible);

		void setBorderRadius(int radiusX = 0,int radiusY = 0);
		QPair<int, int> borderRadius() const { return m_borderRadius; }

		/*!
		 * @brief 设置图标和文本的位置关系
		 * @param iconLeftToText true:图标在左，文字在右；false:图标在右，文字在左
		 */
		void setIconTextDirection(bool iconLeftToText = true);

		QSize sizeHint() const;
		QSize minimumSizeHint() const;

	protected:
		void paintEvent(QPaintEvent * event);
		void mousePressEvent(QMouseEvent * event);
		void mouseReleaseEvent(QMouseEvent * event);
		void enterEvent(QEvent * event);
		void leaveEvent(QEvent * event);

	private:
		QSize calcMiniumSize() const;
		QSize calcMiniumContentSize() const;

		void updateColor(ColorChoose choose, QColor color);

		struct ColorCollect {
			QColor m_normalBackGroundColor;
			QColor m_checkedBackGroundColor;
			QColor m_hoverBackGroundColor;

			QColor m_normalBorderColor;
			QColor m_checkedBorderColor;
			QColor m_hoverBorderColor;

			QColor m_normalTextColor;
			QColor m_checkedTextColor;
			QColor m_hoverTextColor;
		};

	protected:
		QSize m_iconSize;               /*!< 图标和文字之间的间隔 */
		int m_spacing;

		QIcon m_checkedIcon;            /*!< 选中状态下图标 */
		QFont m_textFont;				/*!< 文字字体 */
		bool m_textVixible;				/*!< 文字是否可见 */
		bool m_iconLeftToText;			/*!< 图标在左、文字在右 */
		QPair<int, int> m_borderRadius;	/*!< 边框圆角，first:x方向，second:y方向 */

		bool m_mouseEnter;
		ColorChooses m_colorChoose;      /*!< 各个颜色开关 */
		QColor m_disableColor;           /*!< 无效的颜色：默认为透明色 */

		ColorCollect m_colorCollect;

		bool m_mousePressed;			 /*!< 鼠标是否按下，按下后可将图标和文字向右下方偏移一定像素距离 */
	};

} //namespace Base

#endif // ICONBUTTON_H
