#ifndef imebase_CMeContext_included
#define imebase_CMeContext_included


#include <QMutex>

#include "imebase.h"


namespace imebase
{
	/**
		Context for data processing
	*/
	class CMeContext
	{
	public:
		CMeContext();
		~CMeContext();

		/**
			Register this context to MEiDS
		*/
		bool Register(CMeAddr addr, int dir);
		void Unregister(void);

		/**
			Set/Get parameters.
		*/
		CMeAddr& GetAddress(void);
		void SetSamplesContainer(void* container);
		bool SetBufferSize(int size);
		int GetBufferSize(void)  const;
		bool SetId(int Id);
		int GetId(void)  const;

		/**
			Check buffer status.
		*/
		int GetCount(void)  const;
		bool IsDone(void);
		/**
			Wait for task to end. If time out return false.
		*/
		bool Wait(double Timeout /*in secounds*/);

		/**
			Get pointer to data
		*/
		const int* GetBufferPointer(void) const;

		void copyToContainer(void);
		void copyFromContainer(void);

	private:
		static int cbAIFunc(int device, int subdevice, int count, void* context, int error);
		static int cbAOFunc(int device, int subdevice, int count, void* context, int error);

	public:

	private:
		QMutex mutexKeyLock;
		bool keyLock;
		QMutex mutex;
		int taskId;
		int bufferCount;
		int bufferSize;
		int* hwBuffer;

		isig::ISamplesContainer* samplesContainer;
		CMeAddr address;
	};

} // namespace imebase


#endif // !imebase_CMeContext_included
