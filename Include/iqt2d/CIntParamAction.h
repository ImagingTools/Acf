#ifndef iqt2d_CIntParamAction_h
#define iqt2d_CIntParamAction_h


// Qt includes
#include <QtWidgets/QAction>
#include <QtWidgets/QWidgetAction>
#include <QtWidgets/QSpinBox>


namespace iqt2d
{


class CIntParamAction: public QAction
{
public:
	CIntParamAction(const QIcon& icon, const QString& text, int minValue, int maxValue, int value, QObject* parentPtr = NULL);
	CIntParamAction(const QString& text, int minValue, int maxValue, int value, QObject* parentPtr = NULL);

	int GetValue() const
	{
		Q_ASSERT(m_spinEditor);

		if (m_spinEditor){
			return m_spinEditor->value();
		}

		return 0;
	}

	QSpinBox* GetEditor() const
	{
		Q_ASSERT(m_spinEditor);

		return m_spinEditor;
	}

	QWidgetAction* GetWidgetAction() const
	{
		Q_ASSERT(m_widgetActionPtr);

		return m_widgetActionPtr;
	}

protected:
	void Init(int minValue, int maxValue, int value);

	QWidgetAction* m_widgetActionPtr;
	QSpinBox* m_spinEditor;
};


}


#endif //!iqt2d_CIntParamAction_h