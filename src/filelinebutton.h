/* Author Lawrencio Rasamoelison, Demailly Benjamin
 * this class inherits QWidget but is basically a wrapper
 * of a QLineEdit and QPushButton
 * It display a QLineEdit and QPushButton aside.
 * Its main use is to provide a widget on which you can see
 * and edit a path to a file or directory by typing or by clicking
 * on an "explore" button (which provided an file explorer dialog)
 * (Also it may takes a QFileDialog option (enum), specified in Qt.
 * if QFileDialog is set to QFileDialog::Directory, then the file explorer
 * dialog will only allow selection of directories and a trailing "/" will be
 * Internally added at the end of the path if not already present)
 * This class is instanciated in mainwindow.cpp
 * This class use Qt
 */


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
