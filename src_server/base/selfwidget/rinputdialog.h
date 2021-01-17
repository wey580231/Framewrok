/*!
 *  @brief     便捷输入框
 *  @details   提供int、double、combox、list等快捷获取窗口
 *  @author
 *  @version   1.0
 *  @date      2019.04.04
 *  @warning
 *  @copyright NanJing RenGu.
 *  @note
 */
#ifndef RINPUTDIALOG_H
#define RINPUTDIALOG_H

#include <QLineEdit>
#include "dialogproxy.h"
#include "../base_global.h"

namespace Base {

	class RInputDialogPrivate;

	class BASESHARED_EXPORT RInputDialog : public DialogProxy
	{
		Q_OBJECT
			Q_DECLARE_PRIVATE(RInputDialog)
	public:
		explicit RInputDialog(QWidget *parent = 0);
		~RInputDialog();

		enum InputDialogOption {
			NoButtons = 0x00000001,
			UseListViewForComboBoxItems = 0x00000002,
			UsePlainTextEditForTextInput = 0x00000004
		};

		Q_DECLARE_FLAGS(InputDialogOptions, InputDialogOption)

			enum InputMode {
			TextInput,
			IntInput,
			DoubleInput
		};

		void setOptions(InputDialogOptions options);

		void setInputMode(InputMode mode);
		InputMode inputMode() const;

		void setLabelText(const QString &text);
		QString labelText() const;

		void setTextValue(const QString &text);
		QString textValue() const;

		void setTextEchoMode(QLineEdit::EchoMode mode);
		QLineEdit::EchoMode textEchoMode() const;

		void setComboBoxEditable(bool editable);
		bool isComboBoxEditable() const;

		void setComboBoxItems(const QStringList &items);
		QStringList comboBoxItems() const;

		void setIntValue(int value);
		int intValue() const;

		void setIntMinimum(int min);
		int intMinimum() const;

		void setIntMaximum(int max);
		int intMaximum() const;

		void setIntRange(int min, int max);

		void setIntStep(int step);
		int intStep() const;

		void setDoubleValue(double value);
		double doubleValue() const;

		void setDoubleMinimum(double min);
		double doubleMinimum() const;

		void setDoubleMaximum(double max);
		double doubleMaximum() const;

		void setDoubleRange(double min, double max);

		void setDoubleDecimals(int decimals);
		int doubleDecimals() const;

		static QString getText(QWidget *parent, const QString &title, const QString &label,
			QLineEdit::EchoMode echo = QLineEdit::Normal,
			const QString &text = QString(), bool *ok = Q_NULLPTR);

		static int getInt(QWidget *parent, const QString &title, const QString &label, int value = 0,
			int minValue = -2147483647, int maxValue = 2147483647,
			int step = 1, bool *ok = Q_NULLPTR);

		static double getDouble(QWidget *parent, const QString &title, const QString &label, double value = 0,
			double minValue = -2147483647, double maxValue = 2147483647,
			int decimals = 1, bool *ok = Q_NULLPTR);


	protected:
		QSize sizeHint() const override;

	private slots:
		void respOk();
		void respCancel();

	private:
		QSharedPointer<RInputDialogPrivate> d_ptr;

	};

} //namespace Base 

#endif // RINPUTDIALOG_H
