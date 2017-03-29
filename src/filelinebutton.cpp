#include "filelinebutton.h"

FileLineButton::FileLineButton(QLineEdit* line, QPushButton* button, QFileDialog::FileMode mode) :
	_line(line),
	_button(button),
	_filemode(mode),
	_layout(new QHBoxLayout)
{
	_layout->addWidget(_line);
	_layout->addWidget(_button);
	this->setLayout(_layout);
	connect(_button, SIGNAL(clicked()), this, SLOT(updateLine()));
}

void FileLineButton::updateLine()
{

	QFileDialog dialog;
	dialog.setFileMode(_filemode);
	dialog.setDirectory("/home");
	dialog.setOptions(QFileDialog::DontUseNativeDialog);

	if(dialog.exec())
	{
		_line->setText(dialog.selectedFiles()[0]);
	}
}

QString FileLineButton::text()
{
	return _line->text();
}

