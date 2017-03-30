#ifndef FILELINEBUTTON_H_INCLUDED
#define FILELINEBUTTON_H_INCLUDED

#include <QLineEdit>
#include <QPushButton>
#include <QHBoxLayout>
#include <QFileDialog>

//An object representing a lineedit with a button "explore" aside
class FileLineButton : public QWidget
{

	Q_OBJECT

	public:
		FileLineButton(QLineEdit* line, QPushButton* button, QFileDialog::FileMode = QFileDialog::AnyFile);
		QString text();

	private slots:
		void updateLine();

	private:
		QLineEdit* _line;
		QPushButton* _button;
		const QFileDialog::FileMode _filemode;
		QHBoxLayout* _layout;
};

#endif // FILELINEBUTTON_H_INCLUDED
