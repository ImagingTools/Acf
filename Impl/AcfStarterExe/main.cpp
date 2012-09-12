#include "Generated/ui_CAcfStarter.h"
#include "CAcfStarter.h"

//QT 
#include <QtCore/QFileInfo>
#include <QtGui/QMessageBox>



int main(int argc, char *argv[])
{		
	QApplication::setStyle("plastique");
	QApplication app(argc, argv);

	if (argc > 1){
		QFileInfo confFile = QString(argv[1]);

		if (!confFile.exists()){
			QMessageBox::warning(0, "ACF Wrapper",
								  "File not exist",
								  QMessageBox::Ok);		
			return 1;
		}

		if (confFile.suffix() == "xpc"){	
			CAcfStarter starter(argv);
			starter.RunACF(1000);
			starter.show();				
			return app.exec();			
		
		}else{			
			QMessageBox::warning(0, "ACF Wrapper",
								  "Invalid file format",
								  QMessageBox::Ok);			
			return 1;
		}	

	} else {
		CAcfStarter starter;

		starter.show();

		return app.exec();
	}


}

