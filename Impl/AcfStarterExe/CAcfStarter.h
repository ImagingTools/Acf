#ifndef CAcfStarter_included
#define CAcfStarter_included
 

#include "Generated/ui_CAcfStarter.h"


class CAcfStarter: public QWidget, public Ui::CAcfStarter
{
    Q_OBJECT
 
public:
	CAcfStarter(char **argv);
	CAcfStarter();

public slots:
    void GetPath();	
    void RunACF();   
	void RunACF(int msec);
	void GetRegistryPath(QListWidgetItem*);
	
private:
	QString acfDir;
	QString configFilePath;
	QString activeRegistryFilePath;
	QStringList applicationRegistriesList;
	bool InitApplicationRegistriesList();
	void SetProperty();
	QTimer *timer;
	
	
};


#endif // !CAcfStarter_included


