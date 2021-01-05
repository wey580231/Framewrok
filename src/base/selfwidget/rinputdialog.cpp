#include "rinputdialog.h"

#include <QSpinBox>
#include <QDoubleSpinBox>
#include <QComboBox>
#include <QPlainTextEdit>
#include <QListView>
#include <QLabel>
#include <QVBoxLayout>
#include <QDebug>

namespace Base {

	class RInputDialogPrivate
	{
		Q_DECLARE_PUBLIC(RInputDialog)
	public:
		RInputDialogPrivate(RInputDialog * q) :q_ptr(q), label(nullptr), lineEdit(nullptr), plainTextEdit(nullptr), intSpinBox(nullptr)
			, doubleSpinBox(nullptr), comboBox(nullptr)
			, listView(nullptr), inputWidget(nullptr), mainLayout(nullptr) {
		}
		void ensureLayout();
		void ensureLineEdit();
		void ensurePlainTextEdit();
		void ensureIntSpinBox();
		void ensureDoubleSpinBox();
		void ensureComboBox();
		void ensureListView();
		void setInputWidget(QWidget *widget);
		void ensureLayout() const { const_cast<RInputDialogPrivate *>(this)->ensureLayout(); }
		void chooseRightTextInputWidget();
		bool useComboBoxOrListView() const { return comboBox && comboBox->count() > 0; }

		void setComboBoxText(const QString &text);
		void setListViewText(const QString &text);
		QString listViewText()  const;

		QLabel *label;
		QLineEdit *lineEdit;
		QPlainTextEdit *plainTextEdit;
		QSpinBox *intSpinBox;
		QDoubleSpinBox *doubleSpinBox;
		QComboBox *comboBox;
		QListView *listView;

		QWidget *inputWidget;
		QVBoxLayout *mainLayout;

		QString textValue;

		RInputDialog::InputDialogOptions opts;

		RInputDialog * q_ptr;
	};

	void RInputDialogPrivate::ensureLayout()
	{
		if (mainLayout)
			return;

		if (!inputWidget) {
			ensureLineEdit();
			inputWidget = lineEdit;
		}

		if (!label)
			label = new QLabel(RInputDialog::tr("Enter a value:"), q_ptr);
#ifndef QT_NO_SHORTCUT
		label->setBuddy(inputWidget);
#endif
		label->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Fixed);

		mainLayout = new QVBoxLayout();
		mainLayout->setSizeConstraint(QLayout::SetMinAndMaxSize);
		mainLayout->addWidget(label);
		mainLayout->addWidget(inputWidget);
		inputWidget->setVisible(true);

		QWidget * t_mainWidget = new QWidget();
		t_mainWidget->setLayout(mainLayout);

		q_ptr->setContentWidget(t_mainWidget);
	}

	void RInputDialogPrivate::ensureLineEdit()
	{
		if (!lineEdit) {
			lineEdit = new QLineEdit(q_ptr);
			lineEdit->hide();
		}
	}

	void RInputDialogPrivate::ensurePlainTextEdit()
	{
		if (!plainTextEdit) {
			plainTextEdit = new QPlainTextEdit(q_ptr);
			plainTextEdit->setLineWrapMode(QPlainTextEdit::NoWrap);
			plainTextEdit->hide();
		}
	}

	void RInputDialogPrivate::ensureIntSpinBox()
	{
		if (!intSpinBox) {
			intSpinBox = new QSpinBox(q_ptr);
			intSpinBox->hide();
		}
	}

	void RInputDialogPrivate::ensureDoubleSpinBox()
	{
		if (!doubleSpinBox) {
			doubleSpinBox = new QDoubleSpinBox(q_ptr);
			doubleSpinBox->hide();
		}
	}

	void RInputDialogPrivate::ensureComboBox()
	{
		if (!comboBox) {
			comboBox = new QComboBox(q_ptr);
		}
	}

	void RInputDialogPrivate::ensureListView()
	{
		if (!listView) {
			ensureComboBox();

			listView = new QListView(q_ptr);
			listView->hide();
			listView->setEditTriggers(QAbstractItemView::NoEditTriggers);
			listView->setSelectionMode(QAbstractItemView::SingleSelection);
			listView->setModel(comboBox->model());
			listView->setCurrentIndex(QModelIndex()); // ###
		}
	}

	void RInputDialogPrivate::setInputWidget(QWidget *widget)
	{
		Q_ASSERT(widget);
		if (inputWidget == widget)
			return;

		if (mainLayout) {
			Q_ASSERT(inputWidget);
			mainLayout->removeWidget(inputWidget);
			inputWidget->hide();
			mainLayout->insertWidget(1, widget);
			widget->show();
		}

		inputWidget = widget;

		if (widget == lineEdit) {
			lineEdit->setText(textValue);
		}
		else if (widget == plainTextEdit) {
			plainTextEdit->setPlainText(textValue);
		}
		else if (widget == comboBox) {
			setComboBoxText(textValue);
		}
		else if (widget == listView) {
			setListViewText(textValue);
			ensureLayout();
		}
	}

	void RInputDialogPrivate::chooseRightTextInputWidget()
	{
		QWidget *widget;

		if (useComboBoxOrListView()) {
			if ((opts & RInputDialog::UseListViewForComboBoxItems) && !comboBox->isEditable()) {
				ensureListView();
				widget = listView;
			}
			else {
				widget = comboBox;
			}
		}
		else if (opts & RInputDialog::UsePlainTextEditForTextInput) {
			ensurePlainTextEdit();
			widget = plainTextEdit;
		}
		else {
			ensureLineEdit();
			widget = lineEdit;
		}

		setInputWidget(widget);
	}

	void RInputDialogPrivate::setComboBoxText(const QString &text)
	{
		int index = comboBox->findText(text);
		if (index != -1) {
			comboBox->setCurrentIndex(index);
		}
		else if (comboBox->isEditable()) {
			comboBox->setEditText(text);
		}
	}

	void RInputDialogPrivate::setListViewText(const QString &text)
	{
		int row = comboBox->findText(text);
		if (row != -1) {
			QModelIndex index(comboBox->model()->index(row, 0));
			listView->selectionModel()->setCurrentIndex(index, QItemSelectionModel::Clear
				| QItemSelectionModel::SelectCurrent);
		}
	}

	QString RInputDialogPrivate::listViewText() const
	{
		if (listView->selectionModel()->hasSelection()) {
			int row = listView->selectionModel()->selectedRows().value(0).row();
			return comboBox->itemText(row);
		}
		else {
			return QString();
		}
	}


	RInputDialog::RInputDialog(QWidget *parent) :DialogProxy(parent), d_ptr(new RInputDialogPrivate(this))
	{
		setButton(DialogProxy::Ok, this, SLOT(accept()));
		setButton(DialogProxy::Cancel, this, SLOT(reject()));
	}

	RInputDialog::~RInputDialog()
	{

	}

	void RInputDialog::setOptions(InputDialogOptions options)
	{
		Q_D(RInputDialog);

		InputDialogOptions changed = (options ^ d->opts);
		if (!changed)
			return;

		d->opts = options;
		d->ensureLayout();

		if ((changed & UseListViewForComboBoxItems) && inputMode() == TextInput)
			d->chooseRightTextInputWidget();
		if ((changed & UsePlainTextEditForTextInput) && inputMode() == TextInput)
			d->chooseRightTextInputWidget();
	}

	void RInputDialog::setInputMode(RInputDialog::InputMode mode)
	{
		Q_D(RInputDialog);

		QWidget *widget;

		switch (mode) {
		case IntInput:
			d->ensureIntSpinBox();
			widget = d->intSpinBox;
			break;
		case DoubleInput:
			d->ensureDoubleSpinBox();
			widget = d->doubleSpinBox;
			break;

		default:
			return;
		}

		d->setInputWidget(widget);
	}

	RInputDialog::InputMode RInputDialog::inputMode() const
	{
		Q_D(const RInputDialog);

		if (d->inputWidget) {
			if (d->inputWidget == d->intSpinBox) {
				return IntInput;
			}
			else if (d->inputWidget == d->doubleSpinBox) {
				return DoubleInput;
			}
		}

		return TextInput;
	}

	void RInputDialog::setLabelText(const QString &text)
	{
		Q_D(RInputDialog);
		if (!d->label) {
			d->label = new QLabel(text, this);
		}
		else {
			d->label->setText(text);
		}
	}

	QString RInputDialog::labelText() const
	{
		Q_D(const RInputDialog);
		d->ensureLayout();
		return d->label->text();
	}

	void RInputDialog::setTextValue(const QString &text)
	{
		Q_D(RInputDialog);

		setInputMode(TextInput);
		if (d->inputWidget == d->lineEdit) {
			d->ensureLineEdit();
			d->lineEdit->setText(text);
		}
		else if (d->inputWidget == d->plainTextEdit) {
			d->plainTextEdit->setPlainText(text);
		}
		else if (d->inputWidget == d->comboBox) {
			d->setComboBoxText(text);
		}
		else {
			d->setListViewText(text);
		}
	}

	QString RInputDialog::textValue() const
	{
		Q_D(const RInputDialog);

		if (d->inputWidget == d->lineEdit) {
			return d->lineEdit->text();
		}
		else if (d->inputWidget == d->plainTextEdit) {
			return d->plainTextEdit->toPlainText();
		}
		else if (d->inputWidget == d->comboBox) {
			return d->comboBox->currentText();
		}
		return d->textValue;
	}

	void RInputDialog::setTextEchoMode(QLineEdit::EchoMode mode)
	{
		Q_D(RInputDialog);
		d->ensureLineEdit();
		d->lineEdit->setEchoMode(mode);
	}

	QLineEdit::EchoMode RInputDialog::textEchoMode() const
	{
		Q_D(const RInputDialog);
		if (d->lineEdit) {
			return d->lineEdit->echoMode();
		}
		else {
			return QLineEdit::Normal;
		}
	}

	void RInputDialog::setComboBoxEditable(bool editable)
	{
		Q_D(RInputDialog);
		d->ensureComboBox();
		d->comboBox->setEditable(editable);
		if (inputMode() == TextInput)
			d->chooseRightTextInputWidget();
	}

	bool RInputDialog::isComboBoxEditable() const
	{
		Q_D(const RInputDialog);
		if (d->comboBox) {
			return d->comboBox->isEditable();
		}
		else {
			return false;
		}
	}

	void RInputDialog::setComboBoxItems(const QStringList &items)
	{
		Q_D(RInputDialog);

		d->ensureComboBox();
		{
			const QSignalBlocker blocker(d->comboBox);
			d->comboBox->clear();
			d->comboBox->addItems(items);
		}

		if (inputMode() == TextInput)
			d->chooseRightTextInputWidget();
	}

	QStringList RInputDialog::comboBoxItems() const
	{
		Q_D(const RInputDialog);
		QStringList result;
		if (d->comboBox) {
			const int count = d->comboBox->count();
			result.reserve(count);
			for (int i = 0; i < count; ++i)
				result.append(d->comboBox->itemText(i));
		}
		return result;
	}

	void RInputDialog::setIntValue(int value)
	{
		Q_D(RInputDialog);
		setInputMode(IntInput);
		d->intSpinBox->setValue(value);
	}

	int RInputDialog::intValue() const
	{
		Q_D(const RInputDialog);
		if (d->intSpinBox) {
			return d->intSpinBox->value();
		}
		else {
			return 0;
		}
	}

	void RInputDialog::setIntMinimum(int min)
	{
		Q_D(RInputDialog);
		d->ensureIntSpinBox();
		d->intSpinBox->setMinimum(min);
	}

	int RInputDialog::intMinimum() const
	{
		Q_D(const RInputDialog);
		if (d->intSpinBox) {
			return d->intSpinBox->minimum();
		}
		else {
			return 0;
		}
	}

	void RInputDialog::setIntMaximum(int max)
	{
		Q_D(RInputDialog);
		d->ensureIntSpinBox();
		d->intSpinBox->setMaximum(max);
	}

	int RInputDialog::intMaximum() const
	{
		Q_D(const RInputDialog);
		if (d->intSpinBox) {
			return d->intSpinBox->maximum();
		}
		else {
			return 99;
		}
	}

	void RInputDialog::setIntRange(int min, int max)
	{
		Q_D(RInputDialog);
		d->ensureIntSpinBox();
		d->intSpinBox->setRange(min, max);
	}

	void RInputDialog::setIntStep(int step)
	{
		Q_D(RInputDialog);
		d->ensureIntSpinBox();
		d->intSpinBox->setSingleStep(step);
	}

	int RInputDialog::intStep() const
	{
		Q_D(const RInputDialog);
		if (d->intSpinBox) {
			return d->intSpinBox->singleStep();
		}
		else {
			return 1;
		}
	}

	void RInputDialog::setDoubleValue(double value)
	{
		Q_D(RInputDialog);
		setInputMode(DoubleInput);
		d->doubleSpinBox->setValue(value);
	}

	double RInputDialog::doubleValue() const
	{
		Q_D(const RInputDialog);
		if (d->doubleSpinBox) {
			return d->doubleSpinBox->value();
		}
		else {
			return 0.0;
		}
	}

	void RInputDialog::setDoubleMinimum(double min)
	{
		Q_D(RInputDialog);
		d->ensureDoubleSpinBox();
		d->doubleSpinBox->setMinimum(min);
	}

	double RInputDialog::doubleMinimum() const
	{
		Q_D(const RInputDialog);
		if (d->doubleSpinBox) {
			return d->doubleSpinBox->minimum();
		}
		else {
			return 0.0;
		}
	}

	void RInputDialog::setDoubleMaximum(double max)
	{
		Q_D(RInputDialog);
		d->ensureDoubleSpinBox();
		d->doubleSpinBox->setMaximum(max);
	}

	double RInputDialog::doubleMaximum() const
	{
		Q_D(const RInputDialog);
		if (d->doubleSpinBox) {
			return d->doubleSpinBox->maximum();
		}
		else {
			return 99.99;
		}
	}

	void RInputDialog::setDoubleRange(double min, double max)
	{
		Q_D(RInputDialog);
		d->ensureDoubleSpinBox();
		d->doubleSpinBox->setRange(min, max);
	}

	void RInputDialog::setDoubleDecimals(int decimals)
	{
		Q_D(RInputDialog);
		d->ensureDoubleSpinBox();
		d->doubleSpinBox->setDecimals(decimals);
	}

	int RInputDialog::doubleDecimals() const
	{
		Q_D(const RInputDialog);
		if (d->doubleSpinBox) {
			return d->doubleSpinBox->decimals();
		}
		else {
			return 2;
		}
	}

	QString RInputDialog::getText(QWidget *parent, const QString &title, const QString &label, QLineEdit::EchoMode echo, const QString &text, bool *ok)
	{
		RInputDialog dialog(parent);
		dialog.setTitle(title);
		dialog.setLabelText(label);
		dialog.setTextValue(text);
		dialog.setTextEchoMode(echo);

		int ret = dialog.exec();
		if (ok)
			*ok = ret;

		if (ret) {
			return dialog.textValue();
		}
		else {
			return QString();
		}
	}

	int RInputDialog::getInt(QWidget *parent, const QString &title, const QString &label, int value, int minValue, int maxValue, int step, bool *ok)
	{
		RInputDialog dialog(parent);
		dialog.setTitle(title);
		dialog.setLabelText(label);
		dialog.setIntRange(minValue, maxValue);
		dialog.setIntValue(value);
		dialog.setIntStep(step);
		dialog.setInputMode(RInputDialog::IntInput);

		int ret = dialog.exec();
		if (ok)
			*ok = ret;

		if (ret) {
			return dialog.intValue();
		}
		else {
			return value;
		}
	}

	double RInputDialog::getDouble(QWidget *parent, const QString &title, const QString &label, double value, double minValue, double maxValue, int decimals, bool *ok)
	{
		RInputDialog dialog(parent);
		dialog.setTitle(title);
		dialog.setLabelText(label);
		dialog.setDoubleDecimals(decimals);
		dialog.setDoubleRange(minValue, maxValue);
		dialog.setDoubleValue(value);

		int ret = dialog.exec();
		if (ok)
			*ok = ret;

		if (ret) {
			return dialog.doubleValue();
		}
		else {
			return value;
		}
	}

	QSize RInputDialog::sizeHint() const
	{
		Q_D(const RInputDialog);
		d->ensureLayout();
		return QSize(200, 100);
	}

	void RInputDialog::respOk()
	{
		Q_D(RInputDialog);
		respCancel();
	}

	void RInputDialog::respCancel()
	{
		close();
	}

} //namespace Base 